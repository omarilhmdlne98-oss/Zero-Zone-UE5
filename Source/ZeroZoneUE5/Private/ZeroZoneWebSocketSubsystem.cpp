// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneWebSocketSubsystem.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "ZeroZoneUE5.h"

void UZeroZoneWebSocketSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!FModuleManager::Get().IsModuleLoaded(TEXT("WebSockets")))
	{
		FModuleManager::Get().LoadModule(TEXT("WebSockets"));
	}
}

void UZeroZoneWebSocketSubsystem::Deinitialize()
{
	Disconnect();
	Super::Deinitialize();
}

void UZeroZoneWebSocketSubsystem::Connect(const FString& ServerUrl, const FString& AuthToken)
{
	if (IsConnected())
	{
		Disconnect();
	}

	TMap<FString, FString> Headers;
	if (!AuthToken.IsEmpty())
	{
		Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AuthToken));
	}

	const FString Protocol = ServerUrl.StartsWith(TEXT("wss://")) ? TEXT("wss") : TEXT("ws");
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerUrl, Protocol, Headers);

	if (!WebSocket.IsValid())
	{
		UE_LOG(LogZeroZone, Error, TEXT("Failed to create WebSocket for URL: %s"), *ServerUrl);
		OnConnectionError.Broadcast(TEXT("Failed to create WebSocket instance"));
		return;
	}

	WebSocket->OnConnected().AddUObject(this, &UZeroZoneWebSocketSubsystem::HandleConnected);
	WebSocket->OnConnectionError().AddUObject(this, &UZeroZoneWebSocketSubsystem::HandleConnectionError);
	WebSocket->OnClosed().AddUObject(this, &UZeroZoneWebSocketSubsystem::HandleClosed);
	WebSocket->OnMessage().AddUObject(this, &UZeroZoneWebSocketSubsystem::HandleMessage);

	UE_LOG(LogZeroZone, Log, TEXT("Connecting WebSocket to %s"), *ServerUrl);
	WebSocket->Connect();
}

void UZeroZoneWebSocketSubsystem::Disconnect()
{
	if (WebSocket.IsValid())
	{
		if (WebSocket->IsConnected())
		{
			WebSocket->Close();
		}
		WebSocket.Reset();
	}
}

bool UZeroZoneWebSocketSubsystem::IsConnected() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void UZeroZoneWebSocketSubsystem::SendPlayerState(const FZeroZoneNetworkPlayerState& PlayerState)
{
	if (!IsConnected())
	{
		return;
	}

	TSharedRef<FJsonObject> RootObj = MakeShared<FJsonObject>();
	RootObj->SetStringField(TEXT("type"), TEXT("player_state"));
	RootObj->SetStringField(TEXT("playerId"), PlayerState.PlayerId);
	RootObj->SetStringField(TEXT("operatorClass"), OperatorClassToString(PlayerState.OperatorClass));
	RootObj->SetNumberField(TEXT("x"), PlayerState.Location.X);
	RootObj->SetNumberField(TEXT("y"), PlayerState.Location.Y);
	RootObj->SetNumberField(TEXT("z"), PlayerState.Location.Z);
	RootObj->SetNumberField(TEXT("pitch"), PlayerState.Rotation.Pitch);
	RootObj->SetNumberField(TEXT("yaw"), PlayerState.Rotation.Yaw);
	RootObj->SetNumberField(TEXT("roll"), PlayerState.Rotation.Roll);
	RootObj->SetNumberField(TEXT("health"), PlayerState.Health);
	RootObj->SetNumberField(TEXT("shield"), PlayerState.Shield);
	RootObj->SetBoolField(TEXT("isFiring"), PlayerState.bIsFiring);
	RootObj->SetBoolField(TEXT("isAbilityActive"), PlayerState.bIsAbilityActive);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(RootObj, Writer))
	{
		WebSocket->Send(OutputString);
	}
}

void UZeroZoneWebSocketSubsystem::SendCombatEvent(const FString& EventType, const FString& PlayerId, const FString& TargetId, float Value)
{
	if (!IsConnected())
	{
		return;
	}

	TSharedRef<FJsonObject> RootObj = MakeShared<FJsonObject>();
	RootObj->SetStringField(TEXT("type"), TEXT("combat_event"));
	RootObj->SetStringField(TEXT("event"), EventType);
	RootObj->SetStringField(TEXT("playerId"), PlayerId);
	RootObj->SetStringField(TEXT("targetId"), TargetId);
	RootObj->SetNumberField(TEXT("value"), Value);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(RootObj, Writer))
	{
		WebSocket->Send(OutputString);
	}
}

void UZeroZoneWebSocketSubsystem::SendOperatorSelection(const FString& PlayerId, EZeroZoneOperatorClass OperatorClass)
{
	if (!IsConnected())
	{
		return;
	}

	TSharedRef<FJsonObject> RootObj = MakeShared<FJsonObject>();
	RootObj->SetStringField(TEXT("type"), TEXT("operator_select"));
	RootObj->SetStringField(TEXT("playerId"), PlayerId);
	RootObj->SetStringField(TEXT("operatorClass"), OperatorClassToString(OperatorClass));

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(RootObj, Writer))
	{
		WebSocket->Send(OutputString);
	}
}

void UZeroZoneWebSocketSubsystem::SendRawJson(const FString& JsonPayload)
{
	if (IsConnected())
	{
		WebSocket->Send(JsonPayload);
	}
}

void UZeroZoneWebSocketSubsystem::HandleConnected()
{
	UE_LOG(LogZeroZone, Log, TEXT("WebSocket connected."));
	OnConnected.Broadcast();
}

void UZeroZoneWebSocketSubsystem::HandleConnectionError(const FString& Error)
{
	UE_LOG(LogZeroZone, Error, TEXT("WebSocket connection error: %s"), *Error);
	OnConnectionError.Broadcast(Error);
}

void UZeroZoneWebSocketSubsystem::HandleClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogZeroZone, Warning, TEXT("WebSocket closed (%d): %s [Clean=%s]"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
	OnClosed.Broadcast(StatusCode, Reason, bWasClean);
}

void UZeroZoneWebSocketSubsystem::HandleMessage(const FString& Message)
{
	OnRawMessageReceived.Broadcast(Message);

	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);
	if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
	{
		return;
	}

	const FString MessageType = JsonObj->GetStringField(TEXT("type"));
	if (MessageType == TEXT("player_state"))
	{
		FZeroZoneNetworkPlayerState State;
		State.PlayerId = JsonObj->GetStringField(TEXT("playerId"));
		State.OperatorClass = StringToOperatorClass(JsonObj->GetStringField(TEXT("operatorClass")));
		State.Location = FVector(
			JsonObj->GetNumberField(TEXT("x")),
			JsonObj->GetNumberField(TEXT("y")),
			JsonObj->GetNumberField(TEXT("z"))
		);
		State.Rotation = FRotator(
			JsonObj->GetNumberField(TEXT("pitch")),
			JsonObj->GetNumberField(TEXT("yaw")),
			JsonObj->GetNumberField(TEXT("roll"))
		);
		State.Health = static_cast<float>(JsonObj->GetNumberField(TEXT("health")));
		State.Shield = static_cast<float>(JsonObj->GetNumberField(TEXT("shield")));
		State.bIsFiring = JsonObj->GetBoolField(TEXT("isFiring"));
		State.bIsAbilityActive = JsonObj->GetBoolField(TEXT("isAbilityActive"));

		OnRemotePlayerStateUpdated.Broadcast(State);
	}
	else if (MessageType == TEXT("combat_event"))
	{
		const FString EventName = JsonObj->GetStringField(TEXT("event"));
		const FString SourcePlayerId = JsonObj->GetStringField(TEXT("playerId"));
		const float Value = static_cast<float>(JsonObj->GetNumberField(TEXT("value")));

		OnRemoteCombatEvent.Broadcast(EventName, SourcePlayerId, Value);
	}
}

FString UZeroZoneWebSocketSubsystem::OperatorClassToString(EZeroZoneOperatorClass OperatorClass)
{
	switch (OperatorClass)
	{
	case EZeroZoneOperatorClass::Vanguard:
		return TEXT("Vanguard");
	case EZeroZoneOperatorClass::Spectre:
		return TEXT("Spectre");
	case EZeroZoneOperatorClass::Juggernaut:
		return TEXT("Juggernaut");
	default:
		return TEXT("Vanguard");
	}
}

EZeroZoneOperatorClass UZeroZoneWebSocketSubsystem::StringToOperatorClass(const FString& ClassStr)
{
	if (ClassStr.Equals(TEXT("Spectre"), ESearchCase::IgnoreCase))
	{
		return EZeroZoneOperatorClass::Spectre;
	}
	if (ClassStr.Equals(TEXT("Juggernaut"), ESearchCase::IgnoreCase))
	{
		return EZeroZoneOperatorClass::Juggernaut;
	}
	return EZeroZoneOperatorClass::Vanguard;
}

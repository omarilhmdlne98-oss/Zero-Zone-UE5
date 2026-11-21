// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneMatchmakingSubsystem.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Engine/GameInstance.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "ZeroZoneWebSocketSubsystem.h"
#include "ZeroZoneUE5.h"

void UZeroZoneMatchmakingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(UZeroZoneWebSocketSubsystem::StaticClass());
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			WS->OnRawMessageReceived.AddDynamic(this, &UZeroZoneMatchmakingSubsystem::HandleWebSocketMessage);
		}
	}
}

void UZeroZoneMatchmakingSubsystem::Deinitialize()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			WS->OnRawMessageReceived.RemoveDynamic(this, &UZeroZoneMatchmakingSubsystem::HandleWebSocketMessage);
		}
	}
	Super::Deinitialize();
}

void UZeroZoneMatchmakingSubsystem::CreateRoom(const FString& RoomName, const FString& GameMode, int32 MaxPlayers)
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (!WS || !WS->IsConnected())
	{
		UE_LOG(LogZeroZone, Warning, TEXT("Cannot CreateRoom: WebSocket is not connected."));
		return;
	}

	TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
	Payload->SetStringField(TEXT("type"), TEXT("mm_create_room"));
	Payload->SetStringField(TEXT("roomName"), RoomName);
	Payload->SetStringField(TEXT("gameMode"), GameMode);
	Payload->SetNumberField(TEXT("maxPlayers"), FMath::Max(2, MaxPlayers));

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(Payload, Writer))
	{
		WS->SendRawJson(OutputString);
	}
}

void UZeroZoneMatchmakingSubsystem::JoinRoom(const FString& RoomId)
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (!WS || !WS->IsConnected())
	{
		return;
	}

	SetMatchmakingState(EZeroZoneMatchmakingState::Connecting);

	TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
	Payload->SetStringField(TEXT("type"), TEXT("mm_join_room"));
	Payload->SetStringField(TEXT("roomId"), RoomId);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(Payload, Writer))
	{
		WS->SendRawJson(OutputString);
	}
}

void UZeroZoneMatchmakingSubsystem::LeaveRoom()
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (WS && WS->IsConnected() && !CurrentRoom.RoomId.IsEmpty())
	{
		TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
		Payload->SetStringField(TEXT("type"), TEXT("mm_leave_room"));
		Payload->SetStringField(TEXT("roomId"), CurrentRoom.RoomId);

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		if (FJsonSerializer::Serialize(Payload, Writer))
		{
			WS->SendRawJson(OutputString);
		}
	}

	CurrentRoom = FZeroZoneRoomInfo();
	SetMatchmakingState(EZeroZoneMatchmakingState::Idle);
}

void UZeroZoneMatchmakingSubsystem::StartMatchmaking(EZeroZoneOperatorClass PreferredOperator)
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (!WS || !WS->IsConnected())
	{
		UE_LOG(LogZeroZone, Warning, TEXT("Cannot StartMatchmaking: WebSocket is not connected."));
		return;
	}

	SetMatchmakingState(EZeroZoneMatchmakingState::Searching);

	FString OperatorStr = TEXT("Vanguard");
	if (PreferredOperator == EZeroZoneOperatorClass::Spectre)
	{
		OperatorStr = TEXT("Spectre");
	}
	else if (PreferredOperator == EZeroZoneOperatorClass::Juggernaut)
	{
		OperatorStr = TEXT("Juggernaut");
	}

	TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
	Payload->SetStringField(TEXT("type"), TEXT("mm_queue_join"));
	Payload->SetStringField(TEXT("preferredOperator"), OperatorStr);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(Payload, Writer))
	{
		WS->SendRawJson(OutputString);
	}
}

void UZeroZoneMatchmakingSubsystem::CancelMatchmaking()
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (WS && WS->IsConnected())
	{
		TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
		Payload->SetStringField(TEXT("type"), TEXT("mm_queue_cancel"));

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		if (FJsonSerializer::Serialize(Payload, Writer))
		{
			WS->SendRawJson(OutputString);
		}
	}

	SetMatchmakingState(EZeroZoneMatchmakingState::Idle);
}

void UZeroZoneMatchmakingSubsystem::RequestRoomList()
{
	UGameInstance* GI = GetGameInstance();
	UZeroZoneWebSocketSubsystem* WS = GI ? GI->GetSubsystem<UZeroZoneWebSocketSubsystem>() : nullptr;
	if (!WS || !WS->IsConnected())
	{
		return;
	}

	TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
	Payload->SetStringField(TEXT("type"), TEXT("mm_list_rooms"));

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(Payload, Writer))
	{
		WS->SendRawJson(OutputString);
	}
}

void UZeroZoneMatchmakingSubsystem::HandleWebSocketMessage(const FString& RawMessage)
{
	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RawMessage);
	if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
	{
		return;
	}

	const FString MsgType = JsonObj->GetStringField(TEXT("type"));
	if (MsgType == TEXT("mm_room_list"))
	{
		TArray<FZeroZoneRoomInfo> Rooms;
		const TArray<TSharedPtr<FJsonValue>>* RoomsArray = nullptr;
		if (JsonObj->TryGetArrayField(TEXT("rooms"), RoomsArray) && RoomsArray)
		{
			for (const TSharedPtr<FJsonValue>& Val : *RoomsArray)
			{
				if (Val.IsValid() && Val->Type == EJson::Object)
				{
					Rooms.Add(ParseRoomInfo(Val->AsObject()));
				}
			}
		}
		OnRoomListReceived.Broadcast(Rooms);
	}
	else if (MsgType == TEXT("mm_room_joined"))
	{
		const TSharedPtr<FJsonObject>* RoomObj = nullptr;
		if (JsonObj->TryGetObjectField(TEXT("room"), RoomObj) && RoomObj)
		{
			CurrentRoom = ParseRoomInfo(*RoomObj);
			SetMatchmakingState(EZeroZoneMatchmakingState::InLobby);
			OnRoomJoined.Broadcast(CurrentRoom);
		}
	}
	else if (MsgType == TEXT("mm_match_found"))
	{
		const TSharedPtr<FJsonObject>* RoomObj = nullptr;
		if (JsonObj->TryGetObjectField(TEXT("room"), RoomObj) && RoomObj)
		{
			CurrentRoom = ParseRoomInfo(*RoomObj);
			SetMatchmakingState(EZeroZoneMatchmakingState::MatchFound);
			OnMatchFound.Broadcast(CurrentRoom);
		}
	}
}

void UZeroZoneMatchmakingSubsystem::SetMatchmakingState(EZeroZoneMatchmakingState NewState)
{
	if (MatchmakingState != NewState)
	{
		MatchmakingState = NewState;
		OnMatchmakingStateChanged.Broadcast(MatchmakingState);
	}
}

FZeroZoneRoomInfo UZeroZoneMatchmakingSubsystem::ParseRoomInfo(const TSharedPtr<FJsonObject>& RoomObj) const
{
	FZeroZoneRoomInfo Info;
	if (!RoomObj.IsValid())
	{
		return Info;
	}

	Info.RoomId = RoomObj->GetStringField(TEXT("roomId"));
	Info.RoomName = RoomObj->GetStringField(TEXT("roomName"));
	Info.GameMode = RoomObj->GetStringField(TEXT("gameMode"));
	Info.CurrentPlayers = static_cast<int32>(RoomObj->GetNumberField(TEXT("currentPlayers")));
	Info.MaxPlayers = static_cast<int32>(RoomObj->GetNumberField(TEXT("maxPlayers")));
	Info.ServerAddress = RoomObj->GetStringField(TEXT("serverAddress"));
	return Info;
}

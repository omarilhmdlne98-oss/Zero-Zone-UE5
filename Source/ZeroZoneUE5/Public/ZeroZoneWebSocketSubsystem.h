// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneWebSocketSubsystem.generated.h"

class IWebSocket;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWebSocketConnectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebSocketErrorSignature, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWebSocketClosedSignature, int32, StatusCode, const FString&, Reason, bool, bWasClean);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebSocketMessageSignature, const FString&, RawMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemotePlayerStateUpdatedSignature, const FZeroZoneNetworkPlayerState&, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoteCombatEventSignature, const FString&, EventType, const FString&, SourcePlayerId, float, Value);

UCLASS()
class ZEROZONEUE5_API UZeroZoneWebSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void Connect(const FString& ServerUrl, const FString& AuthToken = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void Disconnect();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Networking")
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void SendPlayerState(const FZeroZoneNetworkPlayerState& PlayerState);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void SendCombatEvent(const FString& EventType, const FString& PlayerId, const FString& TargetId, float Value, const FString& WeaponId = TEXT("rifle"), const FString& HitZone = TEXT("torso"));

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void SendOperatorSelection(const FString& PlayerId, EZeroZoneOperatorClass OperatorClass);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void SendRawJson(const FString& JsonPayload);

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnWebSocketConnectedSignature OnConnected;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnWebSocketErrorSignature OnConnectionError;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnWebSocketClosedSignature OnClosed;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnWebSocketMessageSignature OnRawMessageReceived;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnRemotePlayerStateUpdatedSignature OnRemotePlayerStateUpdated;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Networking|Events")
	FOnRemoteCombatEventSignature OnRemoteCombatEvent;

protected:
	void HandleConnected();
	void HandleConnectionError(const FString& Error);
	void HandleClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void HandleMessage(const FString& Message);

	static FString OperatorClassToString(EZeroZoneOperatorClass OperatorClass);
	static EZeroZoneOperatorClass StringToOperatorClass(const FString& ClassStr);

private:
	TSharedPtr<IWebSocket> WebSocket;
};

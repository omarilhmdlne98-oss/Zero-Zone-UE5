// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneMatchmakingSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchmakingStateChangedSignature, EZeroZoneMatchmakingState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomListReceivedSignature, const TArray<FZeroZoneRoomInfo>&, Rooms);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomJoinedSignature, const FZeroZoneRoomInfo&, RoomInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchFoundSignature, const FZeroZoneRoomInfo&, MatchRoom);

UCLASS()
class ZEROZONEUE5_API UZeroZoneMatchmakingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void CreateRoom(const FString& RoomName, const FString& GameMode = TEXT("Arena"), int32 MaxPlayers = 10);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void JoinRoom(const FString& RoomId);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void LeaveRoom();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void StartMatchmaking(EZeroZoneOperatorClass PreferredOperator = EZeroZoneOperatorClass::Vanguard);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void CancelMatchmaking();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Matchmaking")
	void RequestRoomList();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Matchmaking")
	EZeroZoneMatchmakingState GetMatchmakingState() const { return MatchmakingState; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Matchmaking")
	const FZeroZoneRoomInfo& GetCurrentRoom() const { return CurrentRoom; }

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Matchmaking|Events")
	FOnMatchmakingStateChangedSignature OnMatchmakingStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Matchmaking|Events")
	FOnRoomListReceivedSignature OnRoomListReceived;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Matchmaking|Events")
	FOnRoomJoinedSignature OnRoomJoined;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Matchmaking|Events")
	FOnMatchFoundSignature OnMatchFound;

protected:
	UFUNCTION()
	void HandleWebSocketMessage(const FString& RawMessage);

	void SetMatchmakingState(EZeroZoneMatchmakingState NewState);
	FZeroZoneRoomInfo ParseRoomInfo(const TSharedPtr<class FJsonObject>& RoomObj) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ZeroZone|Matchmaking")
	EZeroZoneMatchmakingState MatchmakingState = EZeroZoneMatchmakingState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "ZeroZone|Matchmaking")
	FZeroZoneRoomInfo CurrentRoom;
};

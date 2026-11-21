// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneArenaGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchTimerUpdatedSignature, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoresUpdatedSignature, int32, TeamAScore, int32, TeamBScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchPhaseChangedSignature, EZeroZoneMatchPhase, NewPhase);

UCLASS()
class ZEROZONEUE5_API AZeroZoneArenaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AZeroZoneArenaGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "ZeroZone|Arena")
	void SetMatchTimeRemaining(float NewTimeRemaining);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "ZeroZone|Arena")
	void SetMatchPhase(EZeroZoneMatchPhase NewPhase);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "ZeroZone|Arena")
	void AddTeamScore(int32 TeamIndex, int32 Points);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "ZeroZone|Arena")
	void MulticastBroadcastKillFeed(const FString& KillerName, const FString& VictimName, const FString& WeaponName, bool bIsHeadshot);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Arena")
	float GetMatchTimeRemaining() const { return MatchTimeRemaining; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Arena")
	int32 GetTeamAScore() const { return TeamAScore; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Arena")
	int32 GetTeamBScore() const { return TeamBScore; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Arena")
	EZeroZoneMatchPhase GetMatchPhase() const { return MatchPhase; }

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Arena|Events")
	FOnMatchTimerUpdatedSignature OnMatchTimerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Arena|Events")
	FOnTeamScoresUpdatedSignature OnTeamScoresUpdated;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Arena|Events")
	FOnMatchPhaseChangedSignature OnMatchPhaseChanged;

protected:
	UFUNCTION()
	void OnRep_MatchTimeRemaining();

	UFUNCTION()
	void OnRep_TeamScores();

	UFUNCTION()
	void OnRep_MatchPhase();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MatchTimeRemaining, BlueprintReadOnly, Category = "ZeroZone|Arena")
	float MatchTimeRemaining = 300.f;

	UPROPERTY(ReplicatedUsing = OnRep_TeamScores, BlueprintReadOnly, Category = "ZeroZone|Arena")
	int32 TeamAScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_TeamScores, BlueprintReadOnly, Category = "ZeroZone|Arena")
	int32 TeamBScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchPhase, BlueprintReadOnly, Category = "ZeroZone|Arena")
	EZeroZoneMatchPhase MatchPhase = EZeroZoneMatchPhase::WaitingForPlayers;
};

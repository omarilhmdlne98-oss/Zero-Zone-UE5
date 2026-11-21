// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneGameMode.generated.h"

UCLASS()
class ZEROZONEUE5_API AZeroZoneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZeroZoneGameMode();

	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|GameMode")
	void StartMatch();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|GameMode")
	void EndMatch();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|GameMode")
	void RecordElimination(AController* KillerController, AController* VictimController, const FString& WeaponName, int32 KillerTeamIndex, bool bIsHeadshot = false);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|GameMode")
	void RespawnPlayer(AController* Controller, EZeroZoneOperatorClass OperatorClass);

protected:
	void TickMatchTimer();

	UFUNCTION()
	void HandleMatchFound(const FZeroZoneRoomInfo& MatchRoom);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Match")
	float DefaultMatchDuration = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Match")
	int32 PointsPerElimination = 100;

	FTimerHandle MatchTimerHandle;
};

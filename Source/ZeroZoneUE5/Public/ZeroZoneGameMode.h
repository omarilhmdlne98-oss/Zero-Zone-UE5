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

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|GameMode")
	void RespawnPlayer(AController* Controller, EZeroZoneOperatorClass OperatorClass);
};

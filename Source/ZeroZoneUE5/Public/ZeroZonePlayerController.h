// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroZoneTypes.h"
#include "ZeroZonePlayerController.generated.h"

UCLASS()
class ZEROZONEUE5_API AZeroZonePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Operator")
	void RequestSelectOperatorClass(EZeroZoneOperatorClass NewOperatorClass);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Networking")
	void ConnectToMultiplayerServer(const FString& WebSocketUrl, const FString& AuthToken = TEXT(""));
};

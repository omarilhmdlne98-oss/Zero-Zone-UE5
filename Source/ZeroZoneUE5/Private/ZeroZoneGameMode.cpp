// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneGameMode.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZonePlayerController.h"

AZeroZoneGameMode::AZeroZoneGameMode()
{
	DefaultPawnClass = AZeroZoneCharacter::StaticClass();
	PlayerControllerClass = AZeroZonePlayerController::StaticClass();
}

void AZeroZoneGameMode::RespawnPlayer(AController* Controller, EZeroZoneOperatorClass OperatorClass)
{
	if (!Controller)
	{
		return;
	}

	if (APawn* ExistingPawn = Controller->GetPawn())
	{
		ExistingPawn->Destroy();
	}

	RestartPlayer(Controller);

	if (AZeroZoneCharacter* NewChar = Cast<AZeroZoneCharacter>(Controller->GetPawn()))
	{
		NewChar->SetOperatorClass(OperatorClass);
	}
}

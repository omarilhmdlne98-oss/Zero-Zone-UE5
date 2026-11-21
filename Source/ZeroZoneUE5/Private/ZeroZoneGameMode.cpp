// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneGameMode.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "ZeroZoneArenaGameState.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneHUD.h"
#include "ZeroZonePlayerController.h"

AZeroZoneGameMode::AZeroZoneGameMode()
{
	DefaultPawnClass = AZeroZoneCharacter::StaticClass();
	PlayerControllerClass = AZeroZonePlayerController::StaticClass();
	GameStateClass = AZeroZoneArenaGameState::StaticClass();
	HUDClass = AZeroZoneHUD::StaticClass();
}

void AZeroZoneGameMode::StartPlay()
{
	Super::StartPlay();
	StartMatch();
}

void AZeroZoneGameMode::StartMatch()
{
	if (AZeroZoneArenaGameState* GS = GetGameState<AZeroZoneArenaGameState>())
	{
		GS->SetMatchTimeRemaining(DefaultMatchDuration);
		GS->SetMatchPhase(EZeroZoneMatchPhase::InProgress);
	}

	GetWorldTimerManager().SetTimer(
		MatchTimerHandle,
		this,
		&AZeroZoneGameMode::TickMatchTimer,
		1.0f,
		true
	);
}

void AZeroZoneGameMode::EndMatch()
{
	GetWorldTimerManager().ClearTimer(MatchTimerHandle);

	if (AZeroZoneArenaGameState* GS = GetGameState<AZeroZoneArenaGameState>())
	{
		GS->SetMatchPhase(EZeroZoneMatchPhase::PostMatch);
	}
}

void AZeroZoneGameMode::TickMatchTimer()
{
	AZeroZoneArenaGameState* GS = GetGameState<AZeroZoneArenaGameState>();
	if (!GS || GS->GetMatchPhase() != EZeroZoneMatchPhase::InProgress)
	{
		return;
	}

	const float NewTime = FMath::Max(0.f, GS->GetMatchTimeRemaining() - 1.f);
	GS->SetMatchTimeRemaining(NewTime);

	if (NewTime <= 0.f)
	{
		EndMatch();
	}
}

void AZeroZoneGameMode::RecordElimination(AController* KillerController, AController* VictimController, const FString& WeaponName, int32 KillerTeamIndex, bool bIsHeadshot)
{
	AZeroZoneArenaGameState* GS = GetGameState<AZeroZoneArenaGameState>();
	if (!GS)
	{
		return;
	}

	const FString KillerName = (KillerController && KillerController->PlayerState) ? KillerController->PlayerState->GetPlayerName() : TEXT("Unknown");
	const FString VictimName = (VictimController && VictimController->PlayerState) ? VictimController->PlayerState->GetPlayerName() : TEXT("Unknown");

	GS->AddTeamScore(KillerTeamIndex, PointsPerElimination);
	GS->MulticastBroadcastKillFeed(KillerName, VictimName, WeaponName, bIsHeadshot);
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

// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneGameMode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "ZeroZoneArenaGameState.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneHUD.h"
#include "ZeroZoneMatchmakingSubsystem.h"
#include "ZeroZonePlayerController.h"
#include "ZeroZoneWebSocketSubsystem.h"

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

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneMatchmakingSubsystem* MM = GI->GetSubsystem<UZeroZoneMatchmakingSubsystem>())
		{
			MM->OnMatchFound.AddUniqueDynamic(this, &AZeroZoneGameMode::HandleMatchFound);
		}
	}

	StartMatch();
}

void AZeroZoneGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(MatchTimerHandle);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneMatchmakingSubsystem* MM = GI->GetSubsystem<UZeroZoneMatchmakingSubsystem>())
		{
			MM->OnMatchFound.RemoveDynamic(this, &AZeroZoneGameMode::HandleMatchFound);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AZeroZoneGameMode::HandleMatchFound(const FZeroZoneRoomInfo& MatchRoom)
{
	StartMatch();
}

void AZeroZoneGameMode::StartMatch()
{
	if (AZeroZoneArenaGameState* GS = GetGameState<AZeroZoneArenaGameState>())
	{
		GS->SetMatchTimeRemaining(DefaultMatchDuration);
		GS->SetMatchPhase(EZeroZoneMatchPhase::InProgress);
	}

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			if (WS->IsConnected())
			{
				const UZeroZoneMatchmakingSubsystem* MM = GI->GetSubsystem<UZeroZoneMatchmakingSubsystem>();
				const FString RoomId = MM ? MM->GetCurrentRoom().RoomId : TEXT("alpha");
				WS->SendCombatEvent(TEXT("match_start"), TEXT("server"), RoomId, DefaultMatchDuration);
			}
		}
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

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			if (WS->IsConnected())
			{
				WS->SendCombatEvent(TEXT("match_over"), TEXT("server"), TEXT(""), 0.f);
			}
		}
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

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			if (WS->IsConnected())
			{
				WS->SendCombatEvent(TEXT("hit_player"), KillerName, VictimName, static_cast<float>(PointsPerElimination));
			}
		}
	}
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

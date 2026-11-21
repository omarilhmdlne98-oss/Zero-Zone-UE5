// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneArenaGameState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ZeroZoneHUD.h"

AZeroZoneArenaGameState::AZeroZoneArenaGameState()
{
	bReplicates = true;
}

void AZeroZoneArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroZoneArenaGameState, MatchTimeRemaining);
	DOREPLIFETIME(AZeroZoneArenaGameState, TeamAScore);
	DOREPLIFETIME(AZeroZoneArenaGameState, TeamBScore);
	DOREPLIFETIME(AZeroZoneArenaGameState, MatchPhase);
}

void AZeroZoneArenaGameState::SetMatchTimeRemaining(float NewTimeRemaining)
{
	if (HasAuthority())
	{
		MatchTimeRemaining = FMath::Max(0.f, NewTimeRemaining);
		OnMatchTimerUpdated.Broadcast(MatchTimeRemaining);
	}
}

void AZeroZoneArenaGameState::SetMatchPhase(EZeroZoneMatchPhase NewPhase)
{
	if (HasAuthority() && MatchPhase != NewPhase)
	{
		MatchPhase = NewPhase;
		OnMatchPhaseChanged.Broadcast(MatchPhase);
	}
}

void AZeroZoneArenaGameState::AddTeamScore(int32 TeamIndex, int32 Points)
{
	if (!HasAuthority() || Points == 0)
	{
		return;
	}

	if (TeamIndex == 0)
	{
		TeamAScore = FMath::Max(0, TeamAScore + Points);
	}
	else if (TeamIndex == 1)
	{
		TeamBScore = FMath::Max(0, TeamBScore + Points);
	}

	OnTeamScoresUpdated.Broadcast(TeamAScore, TeamBScore);
}

void AZeroZoneArenaGameState::MulticastBroadcastKillFeed_Implementation(const FString& KillerName, const FString& VictimName, const FString& WeaponName, bool bIsHeadshot)
{
	if (!GetWorld())
	{
		return;
	}

	if (APlayerController* LocalPC = GetWorld()->GetFirstPlayerController())
	{
		if (AZeroZoneHUD* HUD = Cast<AZeroZoneHUD>(LocalPC->GetHUD()))
		{
			HUD->AddKillFeedEntry(KillerName, VictimName, WeaponName, bIsHeadshot);
		}
	}
}

void AZeroZoneArenaGameState::OnRep_MatchTimeRemaining()
{
	OnMatchTimerUpdated.Broadcast(MatchTimeRemaining);
}

void AZeroZoneArenaGameState::OnRep_TeamScores()
{
	OnTeamScoresUpdated.Broadcast(TeamAScore, TeamBScore);
}

void AZeroZoneArenaGameState::OnRep_MatchPhase()
{
	OnMatchPhaseChanged.Broadcast(MatchPhase);
}

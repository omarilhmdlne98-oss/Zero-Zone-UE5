// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneArenaGameState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ZeroZoneHUD.h"

AZeroZoneArenaGameState::AZeroZoneArenaGameState()
{
	bReplicates = true;

	// Default PBR surface catalog for tactical multi-tier arena structures
	FZeroZoneArenaSurfaceConfig Concrete;
	Concrete.SurfaceName = TEXT("Weathered Concrete");
	Concrete.Roughness = 0.85f;
	Concrete.Metallic = 0.05f;
	Concrete.bDestructible = false;
	ArenaSurfaceConfigs.Add(Concrete);

	FZeroZoneArenaSurfaceConfig BrushedSteel;
	BrushedSteel.SurfaceName = TEXT("Brushed Ballistic Steel");
	BrushedSteel.Roughness = 0.34f;
	BrushedSteel.Metallic = 0.85f;
	BrushedSteel.bDestructible = false;
	ArenaSurfaceConfigs.Add(BrushedSteel);

	FZeroZoneArenaSurfaceConfig SupplyCrate;
	SupplyCrate.SurfaceName = TEXT("Reinforced Tactical Crate");
	SupplyCrate.Roughness = 0.72f;
	SupplyCrate.Metallic = 0.28f;
	SupplyCrate.bDestructible = true;
	SupplyCrate.MaxDurability = 100.f;
	ArenaSurfaceConfigs.Add(SupplyCrate);
}

void AZeroZoneArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroZoneArenaGameState, MatchTimeRemaining);
	DOREPLIFETIME(AZeroZoneArenaGameState, TeamAScore);
	DOREPLIFETIME(AZeroZoneArenaGameState, TeamBScore);
	DOREPLIFETIME(AZeroZoneArenaGameState, MatchPhase);
	DOREPLIFETIME(AZeroZoneArenaGameState, LightingPreset);
	DOREPLIFETIME(AZeroZoneArenaGameState, TimeOfDayHours);
	DOREPLIFETIME(AZeroZoneArenaGameState, VaultBreachProgress);
	DOREPLIFETIME(AZeroZoneArenaGameState, BreachingPlayerId);
	DOREPLIFETIME(AZeroZoneArenaGameState, DestroyedCoverIds);
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

void AZeroZoneArenaGameState::SetLightingPreset(EZeroZoneLightingPreset NewPreset, float NewTimeOfDayHours)
{
	if (HasAuthority())
	{
		LightingPreset = NewPreset;
		TimeOfDayHours = FMath::Clamp(NewTimeOfDayHours, 0.f, 24.f);
		OnLightingPresetChanged.Broadcast(LightingPreset, TimeOfDayHours);
	}
}

void AZeroZoneArenaGameState::SetVaultBreachState(float NewProgress, const FString& NewBreachingPlayerId)
{
	if (HasAuthority())
	{
		VaultBreachProgress = FMath::Clamp(NewProgress, 0.f, 20.f);
		BreachingPlayerId = NewBreachingPlayerId;
		OnVaultBreachUpdated.Broadcast(VaultBreachProgress, BreachingPlayerId);
	}
}

void AZeroZoneArenaGameState::RegisterCoverDestroyed(const FString& CoverId)
{
	if (HasAuthority() && !CoverId.IsEmpty())
	{
		DestroyedCoverIds.AddUnique(CoverId);
	}
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

void AZeroZoneArenaGameState::OnRep_LightingPreset()
{
	OnLightingPresetChanged.Broadcast(LightingPreset, TimeOfDayHours);
}

void AZeroZoneArenaGameState::OnRep_VaultBreachProgress()
{
	OnVaultBreachUpdated.Broadcast(VaultBreachProgress, BreachingPlayerId);
}

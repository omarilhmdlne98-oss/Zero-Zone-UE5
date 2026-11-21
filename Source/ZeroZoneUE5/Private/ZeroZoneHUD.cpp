// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneHUD.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "ZeroZoneArenaGameState.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneCombatComponent.h"
#include "ZeroZoneWeapon.h"

AZeroZoneHUD::AZeroZoneHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZeroZoneHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AZeroZoneHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Canvas)
	{
		DrawCrosshair();
		if (HitmarkerRemainingTime > 0.f)
		{
			DrawHitmarker();
			HitmarkerRemainingTime = FMath::Max(0.f, HitmarkerRemainingTime - GetWorld()->GetDeltaSeconds());
		}
	}

	PruneExpiredKillFeedEntries();
	OnHUDDataUpdated.Broadcast(GetHUDData());
}

void AZeroZoneHUD::DrawCrosshair()
{
	const float CenterX = Canvas->ClipX * 0.5f;
	const float CenterY = Canvas->ClipY * 0.5f;
	const float Gap = BaseCrosshairGap + CurrentCrosshairSpread;

	// Top
	DrawLine(CenterX, CenterY - Gap - CrosshairLineLength, CenterX, CenterY - Gap, CrosshairColor, CrosshairLineThickness);
	// Bottom
	DrawLine(CenterX, CenterY + Gap, CenterX, CenterY + Gap + CrosshairLineLength, CrosshairColor, CrosshairLineThickness);
	// Left
	DrawLine(CenterX - Gap - CrosshairLineLength, CenterY, CenterX - Gap, CenterY, CrosshairColor, CrosshairLineThickness);
	// Right
	DrawLine(CenterX + Gap, CenterY, CenterX + Gap + CrosshairLineLength, CenterY, CrosshairColor, CrosshairLineThickness);
}

void AZeroZoneHUD::DrawHitmarker()
{
	const float CenterX = Canvas->ClipX * 0.5f;
	const float CenterY = Canvas->ClipY * 0.5f;
	const FLinearColor Color = bLastHitWasHeadshot ? FLinearColor::Red : FLinearColor::White;
	const float Inner = 5.f;
	const float Outer = 12.f;

	DrawLine(CenterX - Outer, CenterY - Outer, CenterX - Inner, CenterY - Inner, Color, 2.f);
	DrawLine(CenterX + Inner, CenterY - Inner, CenterX + Outer, CenterY - Outer, Color, 2.f);
	DrawLine(CenterX - Outer, CenterY + Outer, CenterX - Inner, CenterY + Inner, Color, 2.f);
	DrawLine(CenterX + Inner, CenterY + Inner, CenterX + Outer, CenterY + Outer, Color, 2.f);
}

void AZeroZoneHUD::AddKillFeedEntry(const FString& KillerName, const FString& VictimName, const FString& WeaponName, bool bIsHeadshot)
{
	FZeroZoneKillFeedEntry Entry;
	Entry.KillerName = KillerName;
	Entry.VictimName = VictimName;
	Entry.WeaponName = WeaponName;
	Entry.bIsHeadshot = bIsHeadshot;
	Entry.Timestamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	KillFeedEntries.Insert(Entry, 0);
	if (KillFeedEntries.Num() > MaxKillFeedEntries)
	{
		KillFeedEntries.SetNum(MaxKillFeedEntries);
	}

	OnKillFeedUpdated.Broadcast(KillFeedEntries);
}

void AZeroZoneHUD::PruneExpiredKillFeedEntries()
{
	if (!GetWorld() || KillFeedEntries.IsEmpty())
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const int32 PreviousCount = KillFeedEntries.Num();

	KillFeedEntries.RemoveAll([CurrentTime, this](const FZeroZoneKillFeedEntry& Entry)
	{
		return (CurrentTime - Entry.Timestamp) > KillFeedEntryLifetime;
	});

	if (KillFeedEntries.Num() != PreviousCount)
	{
		OnKillFeedUpdated.Broadcast(KillFeedEntries);
	}
}

void AZeroZoneHUD::SetCrosshairSpread(float NewSpread)
{
	CurrentCrosshairSpread = FMath::Max(0.f, NewSpread);
}

void AZeroZoneHUD::TriggerHitmarker(bool bIsHeadshot)
{
	bLastHitWasHeadshot = bIsHeadshot;
	HitmarkerRemainingTime = 0.2f;
}

FZeroZoneHUDData AZeroZoneHUD::GetHUDData() const
{
	FZeroZoneHUDData Data;

	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (AZeroZoneCharacter* Char = Cast<AZeroZoneCharacter>(PC->GetPawn()))
		{
			Data.Health = Char->GetHealth();
			Data.MaxHealth = Char->GetMaxHealth();
			Data.Shield = Char->GetShield();
			Data.MaxShield = Char->GetMaxShield();
			Data.OperatorClass = Char->GetOperatorClass();

			if (UZeroZoneCombatComponent* Combat = Char->GetCombatComponent())
			{
				if (AZeroZoneWeapon* Weapon = Combat->GetEquippedWeapon())
				{
					Data.CurrentAmmo = Weapon->GetCurrentAmmo();
					Data.ReserveAmmo = Weapon->GetReserveAmmo();
				}
			}
		}
	}

	if (GetWorld())
	{
		if (AZeroZoneArenaGameState* GS = GetWorld()->GetGameState<AZeroZoneArenaGameState>())
		{
			Data.MatchTimeRemaining = GS->GetMatchTimeRemaining();
			Data.TeamAScore = GS->GetTeamAScore();
			Data.TeamBScore = GS->GetTeamBScore();
			Data.MatchPhase = GS->GetMatchPhase();
		}
	}

	return Data;
}

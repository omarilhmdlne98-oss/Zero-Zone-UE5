// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneBattlefieldAssetLibrary.h"

TArray<FZeroZoneBattlefieldPBRPreset> UZeroZoneBattlefieldAssetLibrary::GetBattlefieldPBRPresets()
{
	TArray<FZeroZoneBattlefieldPBRPreset> Presets;

	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("weathered_concrete");
		P.DisplayName = TEXT("Weathered Tactical Concrete");
		P.BaseColor = FLinearColor(0.43f, 0.47f, 0.51f, 1.f);
		P.Roughness = 0.86f;
		P.Metallic = 0.06f;
		P.RimStrength = 0.14f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("brushed_steel");
		P.DisplayName = TEXT("Brushed Ballistic Steel");
		P.BaseColor = FLinearColor(0.30f, 0.35f, 0.41f, 1.f);
		P.Roughness = 0.34f;
		P.Metallic = 0.84f;
		P.RimStrength = 0.32f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("overwatch_alloy");
		P.DisplayName = TEXT("Overwatch Tower Alloy");
		P.BaseColor = FLinearColor(0.21f, 0.25f, 0.31f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.22f, 0.74f, 0.97f, 1.f);
		P.Roughness = 0.38f;
		P.Metallic = 0.78f;
		P.RimStrength = 0.38f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("ballistic_bunker");
		P.DisplayName = TEXT("Reinforced Ballistic Bunker");
		P.BaseColor = FLinearColor(0.35f, 0.38f, 0.41f, 1.f);
		P.Roughness = 0.76f;
		P.Metallic = 0.24f;
		P.RimStrength = 0.18f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("vault_core");
		P.DisplayName = TEXT("Armored Vault Core");
		P.BaseColor = FLinearColor(0.17f, 0.21f, 0.27f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.18f, 0.83f, 0.75f, 1.f);
		P.Roughness = 0.28f;
		P.Metallic = 0.88f;
		P.RimStrength = 0.45f;
		Presets.Add(P);
	}

	return Presets;
}

TArray<FZeroZoneTacticalStructureSpec> UZeroZoneBattlefieldAssetLibrary::GetTacticalStructureSpecs()
{
	TArray<FZeroZoneTacticalStructureSpec> Specs;

	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("overwatch_tower");
		S.DisplayName = TEXT("Armored Overwatch Guard Tower");
		S.DimensionsCm = FVector(360.f, 360.f, 580.f);
		S.MaterialPresetId = TEXT("overwatch_alloy");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("ballistic_bunker");
		S.DisplayName = TEXT("Reinforced Ballistic Shield Bunker");
		S.DimensionsCm = FVector(460.f, 220.f, 185.f);
		S.MaterialPresetId = TEXT("ballistic_bunker");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("steel_gantry");
		S.DisplayName = TEXT("Overhead Industrial Steel Gantry");
		S.DimensionsCm = FVector(2200.f, 300.f, 45.f);
		S.MaterialPresetId = TEXT("brushed_steel");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("sniper_catwalk");
		S.DisplayName = TEXT("Elevated Sniper Perch Deck");
		S.DimensionsCm = FVector(1800.f, 420.f, 310.f);
		S.MaterialPresetId = TEXT("brushed_steel");
		Specs.Add(S);
	}

	return Specs;
}

FZeroZoneWeaponInspectionSpec UZeroZoneBattlefieldAssetLibrary::GetWeaponInspectionSpec(EZeroZoneWeaponType WeaponType)
{
	FZeroZoneWeaponInspectionSpec Spec;
	switch (WeaponType)
	{
	case EZeroZoneWeaponType::SniperRifle:
		Spec.WeaponId = TEXT("railgun");
		Spec.DisplayName = TEXT("XR-9 Railgun");
		Spec.MuzzleVelocityCmPerSec = 26000.f;
		Spec.BaseDamage = 68.f;
		Spec.HeadshotDamage = 68.f * 2.2f;
		break;
	case EZeroZoneWeaponType::Shotgun:
		Spec.WeaponId = TEXT("shotgun");
		Spec.DisplayName = TEXT("M-12 Heavy Shotgun");
		Spec.MuzzleVelocityCmPerSec = 9500.f;
		Spec.BaseDamage = 14.f;
		Spec.HeadshotDamage = 14.f * 2.2f;
		break;
	case EZeroZoneWeaponType::AssaultRifle:
	default:
		Spec.WeaponId = TEXT("rifle");
		Spec.DisplayName = TEXT("VX-47 Assault Rifle");
		Spec.MuzzleVelocityCmPerSec = 12000.f;
		Spec.BaseDamage = 24.f;
		Spec.HeadshotDamage = 24.f * 2.2f;
		break;
	}
	return Spec;
}

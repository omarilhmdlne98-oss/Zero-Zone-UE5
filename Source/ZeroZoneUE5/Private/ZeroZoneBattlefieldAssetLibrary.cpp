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
		P.PresetId = TEXT("catwalk_grate");
		P.DisplayName = TEXT("Perforated Steel Catwalk Grate");
		P.BaseColor = FLinearColor(0.24f, 0.28f, 0.34f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.22f, 0.74f, 0.97f, 1.f);
		P.Roughness = 0.40f;
		P.Metallic = 0.86f;
		P.RimStrength = 0.36f;
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
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("sinai_sand");
		P.DisplayName = TEXT("Sinai Wind-Rippled Desert Sand");
		P.BaseColor = FLinearColor(0.85f, 0.73f, 0.52f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.96f, 0.87f, 0.70f, 1.f);
		P.Roughness = 0.92f;
		P.Metallic = 0.02f;
		P.RimStrength = 0.12f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("berm_earth");
		P.DisplayName = TEXT("Compacted East Bank Berm Earth");
		P.BaseColor = FLinearColor(0.76f, 0.61f, 0.38f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.91f, 0.77f, 0.53f, 1.f);
		P.Roughness = 0.94f;
		P.Metallic = 0.02f;
		P.RimStrength = 0.10f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("desert_concrete");
		P.DisplayName = TEXT("Bar-Lev Reinforced Desert Concrete");
		P.BaseColor = FLinearColor(0.72f, 0.65f, 0.55f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.94f, 0.87f, 0.75f, 1.f);
		P.Roughness = 0.86f;
		P.Metallic = 0.08f;
		P.RimStrength = 0.15f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("pontoon_steel");
		P.DisplayName = TEXT("Olive Tactical Pontoon Steel");
		P.BaseColor = FLinearColor(0.37f, 0.41f, 0.32f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.89f, 0.78f, 0.58f, 1.f);
		P.Roughness = 0.46f;
		P.Metallic = 0.68f;
		P.RimStrength = 0.24f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("suez_water");
		P.DisplayName = TEXT("Suez Canal Flowing Water");
		P.BaseColor = FLinearColor(0.11f, 0.39f, 0.44f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.49f, 0.83f, 0.99f, 1.f);
		P.Roughness = 0.14f;
		P.Metallic = 0.25f;
		P.RimStrength = 0.42f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("sinai_limestone");
		P.DisplayName = TEXT("Weathered Sinai Limestone Rock");
		P.BaseColor = FLinearColor(0.66f, 0.56f, 0.42f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.91f, 0.81f, 0.64f, 1.f);
		P.Roughness = 0.88f;
		P.Metallic = 0.04f;
		P.RimStrength = 0.16f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("oasis_palm_foliage");
		P.DisplayName = TEXT("West Bank Oasis Date Palm Foliage");
		P.BaseColor = FLinearColor(0.30f, 0.42f, 0.24f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.64f, 0.90f, 0.21f, 1.f);
		P.Roughness = 0.78f;
		P.Metallic = 0.02f;
		P.RimStrength = 0.18f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("desert_camo_net");
		P.DisplayName = TEXT("Bar-Lev Desert Camouflage Netting");
		P.BaseColor = FLinearColor(0.61f, 0.52f, 0.35f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.84f, 0.75f, 0.57f, 1.f);
		P.Roughness = 0.90f;
		P.Metallic = 0.02f;
		P.RimStrength = 0.12f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("charred_tank_armor");
		P.DisplayName = TEXT("Charred 1973 Sinai Tank Hull Steel");
		P.BaseColor = FLinearColor(0.24f, 0.22f, 0.19f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.98f, 0.45f, 0.09f, 1.f);
		P.Roughness = 0.64f;
		P.Metallic = 0.72f;
		P.RimStrength = 0.28f;
		Presets.Add(P);
	}
	{
		FZeroZoneBattlefieldPBRPreset P;
		P.PresetId = TEXT("field_tent_canvas");
		P.DisplayName = TEXT("Desert Olive Field Tent Canvas");
		P.BaseColor = FLinearColor(0.54f, 0.47f, 0.33f, 1.f);
		P.RimEmissiveColor = FLinearColor(0.90f, 0.81f, 0.64f, 1.f);
		P.Roughness = 0.92f;
		P.Metallic = 0.02f;
		P.RimStrength = 0.14f;
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
		S.DisplayName = TEXT("Armored Overwatch Guard Tower (x4)");
		S.DimensionsCm = FVector(400.f, 400.f, 420.f);
		S.MaterialPresetId = TEXT("overwatch_alloy");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("perimeter_catwalk");
		S.DisplayName = TEXT("Perimeter Steel Catwalk Loop (x4)");
		S.DimensionsCm = FVector(3500.f, 320.f, 28.f);
		S.MaterialPresetId = TEXT("catwalk_grate");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("central_cross_bridge");
		S.DisplayName = TEXT("Vault Central Cross-Bridge (x2)");
		S.DimensionsCm = FVector(1310.f, 300.f, 26.f);
		S.MaterialPresetId = TEXT("catwalk_grate");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("vault_overpass");
		S.DisplayName = TEXT("Armored Vault Overpass Ring");
		S.DimensionsCm = FVector(920.f, 920.f, 28.f);
		S.MaterialPresetId = TEXT("brushed_steel");
		Specs.Add(S);
	}
	{
		FZeroZoneTacticalStructureSpec S;
		S.StructureId = TEXT("inclined_ramp");
		S.DisplayName = TEXT("Smooth 21.6-deg Steel Access Ramp (x4)");
		S.DimensionsCm = FVector(280.f, 1140.f, 24.f);
		S.MaterialPresetId = TEXT("catwalk_grate");
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

	return Specs;
}

TArray<FZeroZoneBridgeNetworkSegment> UZeroZoneBattlefieldAssetLibrary::GetUpperCombatNetworkSpecs()
{
	TArray<FZeroZoneBridgeNetworkSegment> Segments;
	constexpr float UpperDeckZ = 420.f;
	constexpr float TowerCoord = 1950.f;
	constexpr float RampPitchDeg = 21.61f;
	constexpr float RampSlopeLen = 1140.2f;

	auto AddSeg = [&Segments](const TCHAR* Id, EZeroZoneBridgeSegmentKind Kind, FVector Loc, FVector Dim, FRotator Rot, const TCHAR* MatId)
	{
		FZeroZoneBridgeNetworkSegment Seg;
		Seg.SegmentId = Id;
		Seg.Kind = Kind;
		Seg.LocationCm = Loc;
		Seg.DimensionsCm = Dim;
		Seg.RotationDeg = Rot;
		Seg.MaterialPresetId = MatId;
		Segments.Add(Seg);
	};

	// 4 Corner Overwatch Guard Towers (y/z mapped to UE5 cm coordinates)
	AddSeg(TEXT("overwatch_tower_nw"), EZeroZoneBridgeSegmentKind::OverwatchTower, FVector(-TowerCoord, -TowerCoord, UpperDeckZ), FVector(400.f, 400.f, 32.f), FRotator::ZeroRotator, TEXT("overwatch_alloy"));
	AddSeg(TEXT("overwatch_tower_ne"), EZeroZoneBridgeSegmentKind::OverwatchTower, FVector(TowerCoord, -TowerCoord, UpperDeckZ), FVector(400.f, 400.f, 32.f), FRotator::ZeroRotator, TEXT("overwatch_alloy"));
	AddSeg(TEXT("overwatch_tower_sw"), EZeroZoneBridgeSegmentKind::OverwatchTower, FVector(-TowerCoord, TowerCoord, UpperDeckZ), FVector(400.f, 400.f, 32.f), FRotator::ZeroRotator, TEXT("overwatch_alloy"));
	AddSeg(TEXT("overwatch_tower_se"), EZeroZoneBridgeSegmentKind::OverwatchTower, FVector(TowerCoord, TowerCoord, UpperDeckZ), FVector(400.f, 400.f, 32.f), FRotator::ZeroRotator, TEXT("overwatch_alloy"));

	// 4 Perimeter Steel Catwalks linking all 4 Overwatch Guard Towers
	AddSeg(TEXT("catwalk_north"), EZeroZoneBridgeSegmentKind::PerimeterCatwalk, FVector(0.f, -TowerCoord, UpperDeckZ), FVector(3500.f, 320.f, 28.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("catwalk_south"), EZeroZoneBridgeSegmentKind::PerimeterCatwalk, FVector(0.f, TowerCoord, UpperDeckZ), FVector(3500.f, 320.f, 28.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("catwalk_west"), EZeroZoneBridgeSegmentKind::PerimeterCatwalk, FVector(-TowerCoord, 0.f, UpperDeckZ), FVector(320.f, 3500.f, 28.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("catwalk_east"), EZeroZoneBridgeSegmentKind::PerimeterCatwalk, FVector(TowerCoord, 0.f, UpperDeckZ), FVector(320.f, 3500.f, 28.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));

	// Central Vault Overpass Ring
	AddSeg(TEXT("vault_overpass_n"), EZeroZoneBridgeSegmentKind::VaultOverpass, FVector(0.f, -350.f, UpperDeckZ), FVector(920.f, 260.f, 28.f), FRotator::ZeroRotator, TEXT("brushed_steel"));
	AddSeg(TEXT("vault_overpass_s"), EZeroZoneBridgeSegmentKind::VaultOverpass, FVector(0.f, 350.f, UpperDeckZ), FVector(920.f, 260.f, 28.f), FRotator::ZeroRotator, TEXT("brushed_steel"));
	AddSeg(TEXT("vault_overpass_w"), EZeroZoneBridgeSegmentKind::VaultOverpass, FVector(-350.f, 0.f, UpperDeckZ), FVector(260.f, 440.f, 28.f), FRotator::ZeroRotator, TEXT("brushed_steel"));
	AddSeg(TEXT("vault_overpass_e"), EZeroZoneBridgeSegmentKind::VaultOverpass, FVector(350.f, 0.f, UpperDeckZ), FVector(260.f, 440.f, 28.f), FRotator::ZeroRotator, TEXT("brushed_steel"));

	// 2 Central Cross-Bridges (North-South & East-West spans connecting perimeter catwalks to Vault Overpass)
	AddSeg(TEXT("cross_bridge_n"), EZeroZoneBridgeSegmentKind::CentralCrossBridge, FVector(0.f, -1135.f, UpperDeckZ), FVector(300.f, 1310.f, 26.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("cross_bridge_s"), EZeroZoneBridgeSegmentKind::CentralCrossBridge, FVector(0.f, 1135.f, UpperDeckZ), FVector(300.f, 1310.f, 26.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("cross_bridge_w"), EZeroZoneBridgeSegmentKind::CentralCrossBridge, FVector(-1135.f, 0.f, UpperDeckZ), FVector(1310.f, 300.f, 26.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));
	AddSeg(TEXT("cross_bridge_e"), EZeroZoneBridgeSegmentKind::CentralCrossBridge, FVector(1135.f, 0.f, UpperDeckZ), FVector(1310.f, 300.f, 26.f), FRotator::ZeroRotator, TEXT("catwalk_grate"));

	// 4 Smooth Inclined Access Ramps (21.61 deg incline from ground to UpperDeckZ)
	AddSeg(TEXT("access_ramp_nw"), EZeroZoneBridgeSegmentKind::InclinedRamp, FVector(-1180.f, -1260.f, UpperDeckZ * 0.5f), FVector(280.f, RampSlopeLen, 24.f), FRotator(RampPitchDeg, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSeg(TEXT("access_ramp_ne"), EZeroZoneBridgeSegmentKind::InclinedRamp, FVector(1180.f, -1260.f, UpperDeckZ * 0.5f), FVector(280.f, RampSlopeLen, 24.f), FRotator(RampPitchDeg, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSeg(TEXT("access_ramp_sw"), EZeroZoneBridgeSegmentKind::InclinedRamp, FVector(-1180.f, 1260.f, UpperDeckZ * 0.5f), FVector(280.f, RampSlopeLen, 24.f), FRotator(-RampPitchDeg, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSeg(TEXT("access_ramp_se"), EZeroZoneBridgeSegmentKind::InclinedRamp, FVector(1180.f, 1260.f, UpperDeckZ * 0.5f), FVector(280.f, RampSlopeLen, 24.f), FRotator(-RampPitchDeg, 0.f, 0.f), TEXT("catwalk_grate"));

	return Segments;
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

FZeroZoneWeaponAttachmentSpec UZeroZoneBattlefieldAssetLibrary::GetWeaponAttachmentSpecForType(EZeroZoneWeaponType WeaponType)
{
	FZeroZoneWeaponAttachmentSpec Spec;
	switch (WeaponType)
	{
	case EZeroZoneWeaponType::SniperRifle:
		Spec.WeaponId = TEXT("railgun");
		Spec.AttachmentName = TEXT("XR-9 Digital Rail Scope");
		Spec.AttachmentType = TEXT("DigitalRailScope");
		Spec.ReticleColor = FLinearColor(0.75f, 0.52f, 0.99f, 1.f);
		Spec.ADSFOV = 32.f;
		Spec.bHasLaserBeam = false;
		break;
	case EZeroZoneWeaponType::Shotgun:
		Spec.WeaponId = TEXT("shotgun");
		Spec.AttachmentName = TEXT("M-12 Green Tactical Laser");
		Spec.AttachmentType = TEXT("GreenTacticalLaser");
		Spec.ReticleColor = FLinearColor(0.13f, 1.0f, 0.40f, 1.f);
		Spec.ADSFOV = 56.f;
		Spec.bHasLaserBeam = true;
		break;
	case EZeroZoneWeaponType::AssaultRifle:
	default:
		Spec.WeaponId = TEXT("rifle");
		Spec.AttachmentName = TEXT("VX-47 Holographic Red-Dot Sight");
		Spec.AttachmentType = TEXT("HoloRedDot");
		Spec.ReticleColor = FLinearColor(1.0f, 0.16f, 0.16f, 1.f);
		Spec.ADSFOV = 50.f;
		Spec.bHasLaserBeam = false;
		break;
	}
	return Spec;
}

TArray<FZeroZoneWeaponAttachmentSpec> UZeroZoneBattlefieldAssetLibrary::GetWeaponAttachmentSpecs()
{
	TArray<FZeroZoneWeaponAttachmentSpec> Specs;
	Specs.Add(GetWeaponAttachmentSpecForType(EZeroZoneWeaponType::AssaultRifle));
	Specs.Add(GetWeaponAttachmentSpecForType(EZeroZoneWeaponType::SniperRifle));
	Specs.Add(GetWeaponAttachmentSpecForType(EZeroZoneWeaponType::Shotgun));
	return Specs;
}

TArray<FZeroZoneExplosiveBarrelSpec> UZeroZoneBattlefieldAssetLibrary::GetExplosiveBarrelSpecs()
{
	TArray<FZeroZoneExplosiveBarrelSpec> Barrels;
	constexpr float UpperDeckZ = 450.f;
	constexpr float PontoonZ = 45.f;

	auto AddBarrel = [&Barrels](const TCHAR* Id, float X, float Y, float Z)
	{
		FZeroZoneExplosiveBarrelSpec Spec;
		Spec.BarrelId = Id;
		Spec.LocationCm = FVector(X, Y, Z);
		Spec.MaxHealth = 40.f;
		Spec.BlastRadiusCm = 650.f;
		Spec.MaxSplashDamage = 120.f;
		Spec.RespawnDelaySeconds = 25.f;
		Barrels.Add(Spec);
	};

	// 3 Suez Canal Pontoon Bridge choke-point barrels (x = -2800, 0, +2800 cm)
	AddBarrel(TEXT("barrel_pontoon_w"), -2800.f, 0.f, PontoonZ);
	AddBarrel(TEXT("barrel_pontoon_c"), 0.f, 0.f, PontoonZ);
	AddBarrel(TEXT("barrel_pontoon_e"), 2800.f, 0.f, PontoonZ);

	// 2 East Bank Sand Berm Breach assault corridor barrels
	AddBarrel(TEXT("barrel_breach_w"), -2800.f, -1800.f, 0.f);
	AddBarrel(TEXT("barrel_breach_e"), 2800.f, -1800.f, 0.f);

	// 3 Upper Sand Berm Crest & Strategic Bunker Hill barrels
	AddBarrel(TEXT("barrel_berm_nw"), -1400.f, -2200.f, UpperDeckZ);
	AddBarrel(TEXT("barrel_berm_ne"), 1400.f, -2200.f, UpperDeckZ);
	AddBarrel(TEXT("barrel_hill_cmd"), 0.f, -6150.f, 380.f);

	return Barrels;
}

TArray<FZeroZoneSinaiBattlefieldSpec> UZeroZoneBattlefieldAssetLibrary::GetSinaiBattlefieldSpecs()
{
	TArray<FZeroZoneSinaiBattlefieldSpec> Specs;
	constexpr float BermTopZ = 450.f;
	constexpr float PontoonZ = 45.f;

	auto AddSpec = [&Specs](const TCHAR* Id, const TCHAR* Name, EZeroZoneSinaiZoneKind Kind, FVector Loc, FVector Dim, FRotator Rot, const TCHAR* MatId)
	{
		FZeroZoneSinaiBattlefieldSpec S;
		S.SegmentId = Id;
		S.DisplayName = Name;
		S.ZoneKind = Kind;
		S.LocationCm = Loc;
		S.DimensionsCm = Dim;
		S.RotationDeg = Rot;
		S.MaterialPresetId = MatId;
		Specs.Add(S);
	};

	// 400m x 400m Open-World Sinai Desert Heightfield & 16m-Wide Animated Suez Canal Water Channel (40000cm x 1640cm)
	AddSpec(TEXT("sinai_open_world_terrain"), TEXT("Sinai 400m x 400m Slope-Blended Wind-Rippled Terrain"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(0.f, 0.f, 0.f), FVector(40000.f, 40000.f, 1800.f), FRotator::ZeroRotator, TEXT("sinai_sand"));
	AddSpec(TEXT("suez_canal_channel"), TEXT("Animated Dual-Normal Suez Canal Water Channel (16m)"), EZeroZoneSinaiZoneKind::SuezCanalChannel,
		FVector(0.f, 0.f, -35.f), FVector(40000.f, 1640.f, 100.f), FRotator::ZeroRotator, TEXT("suez_water"));
	AddSpec(TEXT("east_bank_command_vault"), TEXT("East Bank Command Relay Vault Plaza"), EZeroZoneSinaiZoneKind::CommandRelay,
		FVector(0.f, -1450.f, 0.f), FVector(460.f, 460.f, 452.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));

	// 3 Wide Tactical Pontoon Bridges (West x=-28m, Center x=0m, East x=+28m)
	AddSpec(TEXT("pontoon_bridge_west"), TEXT("West Tactical Pontoon Bridge"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(-2800.f, 0.f, PontoonZ), FVector(460.f, 1760.f, 26.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));
	AddSpec(TEXT("pontoon_bridge_center"), TEXT("Center Tactical Pontoon Bridge"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(0.f, 0.f, PontoonZ), FVector(520.f, 1760.f, 26.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));
	AddSpec(TEXT("pontoon_bridge_east"), TEXT("East Tactical Pontoon Bridge"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(2800.f, 0.f, PontoonZ), FVector(460.f, 1760.f, 26.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));

	// East Bank Sand Berm Rampart (4 compacted earth spans across 116m leaving 3 hydro-breached assault corridors)
	AddSpec(TEXT("berm_crest_far_w"), TEXT("East Bank Sand Berm (Far West Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(-4600.f, -2200.f, BermTopZ), FVector(2400.f, 380.f, 450.f), FRotator::ZeroRotator, TEXT("berm_earth"));
	AddSpec(TEXT("berm_crest_mid_w"), TEXT("East Bank Sand Berm (Mid West Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(-1400.f, -2200.f, BermTopZ), FVector(1650.f, 380.f, 450.f), FRotator::ZeroRotator, TEXT("berm_earth"));
	AddSpec(TEXT("berm_crest_mid_e"), TEXT("East Bank Sand Berm (Mid East Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(1400.f, -2200.f, BermTopZ), FVector(1650.f, 380.f, 450.f), FRotator::ZeroRotator, TEXT("berm_earth"));
	AddSpec(TEXT("berm_crest_far_e"), TEXT("East Bank Sand Berm (Far East Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(4600.f, -2200.f, BermTopZ), FVector(2400.f, 380.f, 450.f), FRotator::ZeroRotator, TEXT("berm_earth"));

	// 4 Smooth Walkable 21.61-deg Berm Assault Ramps
	AddSpec(TEXT("berm_ramp_inner_w"), TEXT("East Berm Inner Assault Ramp (West)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(-1400.f, -2920.f, BermTopZ * 0.5f), FVector(320.f, 1220.f, 28.f), FRotator(-21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("berm_ramp_inner_e"), TEXT("East Berm Inner Assault Ramp (East)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(1400.f, -2920.f, BermTopZ * 0.5f), FVector(320.f, 1220.f, 28.f), FRotator(-21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("berm_ramp_outer_w"), TEXT("East Berm Outer Flank Ramp (West)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(-5800.f, -1480.f, BermTopZ * 0.5f), FVector(320.f, 1220.f, 28.f), FRotator(21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("berm_ramp_outer_e"), TEXT("East Berm Outer Flank Ramp (East)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(5800.f, -1480.f, BermTopZ * 0.5f), FVector(320.f, 1220.f, 28.f), FRotator(21.61f, 0.f, 0.f), TEXT("catwalk_grate"));

	// 6 Reinforced Bar-Lev Desert Concrete Bunkers on Strategic Open-World Hills (+-90m combat zone)
	AddSpec(TEXT("bunker_eb_west"), TEXT("Bar-Lev Hill Bunker (East Bank West)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(-4200.f, -3600.f, 320.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_eb_east"), TEXT("Bar-Lev Hill Bunker (East Bank East)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(4200.f, -3600.f, 320.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_eb_cmd"), TEXT("Bar-Lev Command Hill Bunker (East Bank North)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(0.f, -6400.f, 380.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_wb_west"), TEXT("Desert Hill Bunker (West Bank West)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(-4200.f, 3600.f, 280.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_wb_east"), TEXT("Desert Hill Bunker (West Bank East)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(4200.f, 3600.f, 280.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_wb_stage"), TEXT("Desert Staging Hill Bunker (West Bank South)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(0.f, 6600.f, 340.f), FVector(520.f, 380.f, 240.f), FRotator::ZeroRotator, TEXT("desert_concrete"));

	// Instanced Sinai Battlefield Realism Batches (Date Palm Oasis, Czech Hedgehogs, Camo Canopies, Limestone Outcrops)
	AddSpec(TEXT("oasis_palm_west_bank"), TEXT("West Bank Canal Oasis Date Palm Line (x14 Instanced)"), EZeroZoneSinaiZoneKind::OasisPalmCluster,
		FVector(0.f, 1500.f, 0.f), FVector(12400.f, 3200.f, 520.f), FRotator::ZeroRotator, TEXT("oasis_palm_foliage"));
	AddSpec(TEXT("hedgehog_antitank_line"), TEXT("East Bank Berm Czech Hedgehog Anti-Tank Line (x14 Instanced)"), EZeroZoneSinaiZoneKind::AntiTankHedgehogLine,
		FVector(0.f, -1500.f, 54.f), FVector(7600.f, 800.f, 155.f), FRotator::ZeroRotator, TEXT("brushed_steel"));
	AddSpec(TEXT("bunker_camo_canopies"), TEXT("Bar-Lev Bunker Desert Camo Netting Canopies (x6 Instanced)"), EZeroZoneSinaiZoneKind::BunkerCamoCanopy,
		FVector(0.f, -3600.f, 625.f), FVector(620.f, 480.f, 8.f), FRotator::ZeroRotator, TEXT("desert_camo_net"));
	AddSpec(TEXT("sinai_limestone_outcrops"), TEXT("Weathered Sinai Limestone Rocky Outcrops (x16 Instanced)"), EZeroZoneSinaiZoneKind::LimestoneOutcrop,
		FVector(0.f, 0.f, 180.f), FVector(17600.f, 15000.f, 360.f), FRotator::ZeroRotator, TEXT("sinai_limestone"));

	// 6 Disabled / Burnt 1973 Tank Hull Cover Wrecks (with 3 Rising Battlefield Smoke Plumes)
	AddSpec(TEXT("tank_wreck_eb_breach_w"), TEXT("Charred Tank Hull Wreck (West Berm Breach + Smoke Plume)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(-2450.f, -2650.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, 21.8f, 0.f), TEXT("charred_tank_armor"));
	AddSpec(TEXT("tank_wreck_eb_breach_c"), TEXT("Charred Tank Hull Wreck (Center Berm Breach + Smoke Plume)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(480.f, -2800.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, -25.2f, 0.f), TEXT("charred_tank_armor"));
	AddSpec(TEXT("tank_wreck_eb_breach_e"), TEXT("Charred Tank Hull Wreck (East Berm Breach + Smoke Plume)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(2450.f, -2650.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, -18.3f, 0.f), TEXT("charred_tank_armor"));
	AddSpec(TEXT("tank_wreck_eb_flank_w"), TEXT("Charred Tank Hull Wreck (East Bank West Flank)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(-3600.f, -1550.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, 35.5f, 0.f), TEXT("charred_tank_armor"));
	AddSpec(TEXT("tank_wreck_eb_flank_e"), TEXT("Charred Tank Hull Wreck (East Bank East Flank)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(3600.f, -1550.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, -33.2f, 0.f), TEXT("charred_tank_armor"));
	AddSpec(TEXT("tank_wreck_wb_center"), TEXT("Charred Tank Hull Wreck (West Bank Canal Approach)"), EZeroZoneSinaiZoneKind::CharredTankWreck,
		FVector(-650.f, 1650.f, 95.f), FVector(320.f, 540.f, 195.f), FRotator(0.f, 12.6f, 0.f), TEXT("charred_tank_armor"));

	// Reinforced Concrete Dragon's Teeth Anti-Tank Pyramid Lines & 4 Field Command Supply Depots
	AddSpec(TEXT("dragons_teeth_flanks"), TEXT("Reinforced Concrete Dragons Teeth Tank Traps (x36 Instanced)"), EZeroZoneSinaiZoneKind::DragonsTeethLine,
		FVector(0.f, -1835.f, 52.f), FVector(10800.f, 240.f, 115.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("depot_eb_north_w"), TEXT("East Bank Field Command Tent & Supply Depot (West)"), EZeroZoneSinaiZoneKind::FieldSupplyDepot,
		FVector(-2200.f, -5200.f, 115.f), FVector(540.f, 380.f, 230.f), FRotator(0.f, 4.6f, 0.f), TEXT("field_tent_canvas"));
	AddSpec(TEXT("depot_eb_north_e"), TEXT("East Bank Field Command Tent & Supply Depot (East)"), EZeroZoneSinaiZoneKind::FieldSupplyDepot,
		FVector(2200.f, -5200.f, 115.f), FVector(540.f, 380.f, 230.f), FRotator(0.f, -4.6f, 0.f), TEXT("field_tent_canvas"));
	AddSpec(TEXT("depot_wb_south_w"), TEXT("West Bank Field Command Tent & Supply Depot (West)"), EZeroZoneSinaiZoneKind::FieldSupplyDepot,
		FVector(-2200.f, 4800.f, 115.f), FVector(540.f, 380.f, 230.f), FRotator(0.f, -3.4f, 0.f), TEXT("field_tent_canvas"));
	AddSpec(TEXT("depot_wb_south_e"), TEXT("West Bank Field Command Tent & Supply Depot (East)"), EZeroZoneSinaiZoneKind::FieldSupplyDepot,
		FVector(2200.f, 4800.f, 115.f), FVector(540.f, 380.f, 230.f), FRotator(0.f, 3.4f, 0.f), TEXT("field_tent_canvas"));
	AddSpec(TEXT("sinai_shell_craters"), TEXT("Scorched Artillery Shell Craters (x8 Carved Terrain Bowls)"), EZeroZoneSinaiZoneKind::ArtilleryShellCrater,
		FVector(0.f, -1800.f, -75.f), FVector(6800.f, 8000.f, 88.f), FRotator::ZeroRotator, TEXT("sinai_sand"));

	return Specs;
}

TArray<FZeroZoneTacticalKillstreakSpec> UZeroZoneBattlefieldAssetLibrary::GetTacticalKillstreakSpecs()
{
	TArray<FZeroZoneTacticalKillstreakSpec> Streaks;
	Streaks.Reserve(2);

	{
		FZeroZoneTacticalKillstreakSpec Uav;
		Uav.KillstreakId = TEXT("sinai_uav_recon");
		Uav.DisplayName = TEXT("UAV Recon Radar Pulse");
		Uav.Kind = EZeroZoneKillstreakKind::UavReconPulse;
		Uav.HotkeyLabel = TEXT("[4]");
		Uav.CooldownSeconds = 22.f;
		Uav.ActiveDurationSeconds = 12.f;
		Uav.BlastRadiusCm = 0.f;
		Uav.MaxSplashDamage = 0.f;
		Uav.ShellCount = 0;
		Uav.KoCooldownRefundSeconds = 6.f;
		Streaks.Add(Uav);
	}
	{
		FZeroZoneTacticalKillstreakSpec Arty;
		Arty.KillstreakId = TEXT("sinai_artillery_155");
		Arty.DisplayName = TEXT("155mm Sinai Artillery Barrage");
		Arty.Kind = EZeroZoneKillstreakKind::ArtilleryBarrage155;
		Arty.HotkeyLabel = TEXT("[5]");
		Arty.CooldownSeconds = 28.f;
		Arty.ActiveDurationSeconds = 4.5f;
		Arty.BlastRadiusCm = 850.f;
		Arty.MaxSplashDamage = 115.f;
		Arty.ShellCount = 5;
		Arty.KoCooldownRefundSeconds = 6.f;
		Streaks.Add(Arty);
	}

	return Streaks;
}

FZeroZoneMinimapRadarSpec UZeroZoneBattlefieldAssetLibrary::GetMinimapRadarSpec()
{
	FZeroZoneMinimapRadarSpec Spec;
	Spec.RadarRangeCm = 7400.f;
	Spec.PassiveDetectRangeCm = 3400.f;
	Spec.CanalHalfWidthCm = 800.f;
	Spec.EastBermOffsetYCm = -2200.f;
	Spec.bShowExplosiveBarrels = true;
	return Spec;
}

TArray<FZeroZoneSinaiAtmospherePreset> UZeroZoneBattlefieldAssetLibrary::GetSinaiAtmospherePresets()
{
	TArray<FZeroZoneSinaiAtmospherePreset> Presets;
	Presets.Reserve(4);

	auto AddAtmo = [&Presets](EZeroZoneSinaiTimeOfDay Tod, const TCHAR* Label, FVector Dir, FLinearColor SunCol, float Intensity, FLinearColor Zenith, FLinearColor Horizon, float Fog)
	{
		FZeroZoneSinaiAtmospherePreset P;
		P.TimeOfDay = Tod;
		P.PresetLabel = Label;
		P.SunDirection = Dir;
		P.SunColor = SunCol;
		P.SunIntensity = Intensity;
		P.ZenithColor = Zenith;
		P.HorizonFogColor = Horizon;
		P.FogDensity = Fog;
		Presets.Add(P);
	};

	AddAtmo(EZeroZoneSinaiTimeOfDay::Dawn, TEXT("DAWN // 06:15"), FVector(64.f, 22.f, 36.f),
		FLinearColor(1.0f, 0.73f, 0.51f, 1.f), 1.55f, FLinearColor(0.17f, 0.34f, 0.55f, 1.f), FLinearColor(0.86f, 0.64f, 0.48f, 1.f), 0.0046f);
	AddAtmo(EZeroZoneSinaiTimeOfDay::Noon, TEXT("NOON // 12:30"), FVector(24.f, 92.f, 18.f),
		FLinearColor(1.0f, 0.97f, 0.91f, 1.f), 2.35f, FLinearColor(0.16f, 0.45f, 0.77f, 1.f), FLinearColor(0.89f, 0.78f, 0.60f, 1.f), 0.0034f);
	AddAtmo(EZeroZoneSinaiTimeOfDay::GoldenDusk, TEXT("DUSK // 17:30"), FVector(42.f, 34.f, 32.f),
		FLinearColor(1.0f, 0.81f, 0.59f, 1.f), 1.95f, FLinearColor(0.18f, 0.35f, 0.57f, 1.f), FLinearColor(0.85f, 0.71f, 0.50f, 1.f), 0.0042f);
	AddAtmo(EZeroZoneSinaiTimeOfDay::DesertNight, TEXT("NIGHT // 22:30"), FVector(-38.f, 48.f, -28.f),
		FLinearColor(0.61f, 0.72f, 0.91f, 1.f), 0.72f, FLinearColor(0.04f, 0.07f, 0.15f, 1.f), FLinearColor(0.11f, 0.18f, 0.27f, 1.f), 0.0052f);

	return Presets;
}

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
	constexpr float UpperDeckZ = 420.f;

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

	// 4 Suez Canal Pontoon Bridge & East Bank Berm Breach choke-point barrels (cm)
	AddBarrel(TEXT("barrel_pontoon_w"), -950.f, 0.f, 42.f);
	AddBarrel(TEXT("barrel_pontoon_e"), 950.f, 0.f, 42.f);
	AddBarrel(TEXT("barrel_breach_w"), -950.f, -1050.f, 0.f);
	AddBarrel(TEXT("barrel_breach_e"), 950.f, -1050.f, 0.f);

	// 4 Upper Sand Berm Crest & West Overlook barrels (Z = 420cm)
	AddBarrel(TEXT("barrel_berm_nw"), -1680.f, -1350.f, UpperDeckZ);
	AddBarrel(TEXT("barrel_berm_ne"), 1680.f, -1350.f, UpperDeckZ);
	AddBarrel(TEXT("barrel_berm_c"), 0.f, -1350.f, UpperDeckZ);
	AddBarrel(TEXT("barrel_west_overlook"), 0.f, 1950.f, UpperDeckZ);

	return Barrels;
}

TArray<FZeroZoneSinaiBattlefieldSpec> UZeroZoneBattlefieldAssetLibrary::GetSinaiBattlefieldSpecs()
{
	TArray<FZeroZoneSinaiBattlefieldSpec> Specs;
	constexpr float BermTopZ = 420.f;
	constexpr float PontoonZ = 42.f;

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

	// Central Suez Canal Water Channel & Command Relay Causeway Island
	AddSpec(TEXT("suez_canal_channel"), TEXT("Suez Canal Water Channel"), EZeroZoneSinaiZoneKind::SuezCanalChannel,
		FVector(0.f, 0.f, -52.f), FVector(4800.f, 900.f, 52.f), FRotator::ZeroRotator, TEXT("sinai_sand"));
	AddSpec(TEXT("canal_command_relay"), TEXT("Central Canal Command Relay Vault Platform"), EZeroZoneSinaiZoneKind::CommandRelay,
		FVector(0.f, 0.f, PontoonZ), FVector(560.f, 560.f, 42.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));

	// 2 Tactical Pontoon Bridges + Causeway Links
	AddSpec(TEXT("pontoon_bridge_w"), TEXT("West Tactical Pontoon Bridge"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(-950.f, 0.f, PontoonZ), FVector(420.f, 1020.f, 24.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));
	AddSpec(TEXT("pontoon_bridge_e"), TEXT("East Tactical Pontoon Bridge"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(950.f, 0.f, PontoonZ), FVector(420.f, 1020.f, 24.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));
	AddSpec(TEXT("canal_causeway_w"), TEXT("West Relay Causeway"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(-510.f, 0.f, PontoonZ), FVector(480.f, 260.f, 24.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));
	AddSpec(TEXT("canal_causeway_e"), TEXT("East Relay Causeway"), EZeroZoneSinaiZoneKind::PontoonBridge,
		FVector(510.f, 0.f, PontoonZ), FVector(480.f, 260.f, 24.f), FRotator::ZeroRotator, TEXT("pontoon_steel"));

	// East Bank Sand Berm Rampart (3 compacted earth spans leaving 2 breached assault corridors)
	AddSpec(TEXT("berm_crest_w"), TEXT("East Bank Sand Berm (West Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(-1680.f, -1350.f, BermTopZ), FVector(880.f, 320.f, 420.f), FRotator::ZeroRotator, TEXT("berm_earth"));
	AddSpec(TEXT("berm_crest_c"), TEXT("East Bank Sand Berm (Center Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(0.f, -1350.f, BermTopZ), FVector(1360.f, 320.f, 420.f), FRotator::ZeroRotator, TEXT("berm_earth"));
	AddSpec(TEXT("berm_crest_e"), TEXT("East Bank Sand Berm (East Span)"), EZeroZoneSinaiZoneKind::SandBermRampart,
		FVector(1680.f, -1350.f, BermTopZ), FVector(880.f, 320.f, 420.f), FRotator::ZeroRotator, TEXT("berm_earth"));

	// 4 Smooth Walkable 21.61-deg Access Ramps (2 East Berm flanks + 2 West Bank overlook flanks)
	AddSpec(TEXT("east_berm_ramp_w"), TEXT("East Berm Flank Assault Ramp (West)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(-1950.f, -820.f, BermTopZ * 0.5f), FVector(280.f, 1140.2f, 26.f), FRotator(21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("east_berm_ramp_e"), TEXT("East Berm Flank Assault Ramp (East)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(1950.f, -820.f, BermTopZ * 0.5f), FVector(280.f, 1140.2f, 26.f), FRotator(21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("west_bank_ramp_w"), TEXT("West Bank Overlook Ramp (West)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(-1950.f, 1220.f, BermTopZ * 0.5f), FVector(280.f, 1140.2f, 26.f), FRotator(-21.61f, 0.f, 0.f), TEXT("catwalk_grate"));
	AddSpec(TEXT("west_bank_ramp_e"), TEXT("West Bank Overlook Ramp (East)"), EZeroZoneSinaiZoneKind::SandBermRamp,
		FVector(1950.f, 1220.f, BermTopZ * 0.5f), FVector(280.f, 1140.2f, 26.f), FRotator(-21.61f, 0.f, 0.f), TEXT("catwalk_grate"));

	// 4 Reinforced Bar-Lev Desert Concrete Bunkers with Firing Slits
	AddSpec(TEXT("bunker_east_w"), TEXT("Bar-Lev Concrete Bunker (East Bank West)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(-1480.f, -900.f, 0.f), FVector(480.f, 340.f, 236.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_east_e"), TEXT("Bar-Lev Concrete Bunker (East Bank East)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(1480.f, -900.f, 0.f), FVector(480.f, 340.f, 236.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_west_w"), TEXT("Desert Concrete Bunker (West Bank West)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(-1480.f, 1020.f, 0.f), FVector(480.f, 340.f, 236.f), FRotator::ZeroRotator, TEXT("desert_concrete"));
	AddSpec(TEXT("bunker_west_e"), TEXT("Desert Concrete Bunker (West Bank East)"), EZeroZoneSinaiZoneKind::BarLevBunker,
		FVector(1480.f, 1020.f, 0.f), FVector(480.f, 340.f, 236.f), FRotator::ZeroRotator, TEXT("desert_concrete"));

	return Specs;
}

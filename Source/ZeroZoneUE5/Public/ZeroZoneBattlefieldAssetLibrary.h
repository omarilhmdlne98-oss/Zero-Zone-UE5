// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneBattlefieldAssetLibrary.generated.h"

UENUM(BlueprintType)
enum class EZeroZoneBridgeSegmentKind : uint8
{
	OverwatchTower	UMETA(DisplayName = "Overwatch Tower"),
	PerimeterCatwalk UMETA(DisplayName = "Perimeter Catwalk"),
	CentralCrossBridge UMETA(DisplayName = "Central Cross-Bridge"),
	VaultOverpass	UMETA(DisplayName = "Vault Overpass"),
	InclinedRamp	UMETA(DisplayName = "Inclined Access Ramp")
};

USTRUCT(BlueprintType)
struct FZeroZoneBattlefieldPBRPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	FString PresetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	FLinearColor BaseColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	FLinearColor RimEmissiveColor = FLinearColor(0.22f, 0.74f, 0.97f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	float Roughness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	float Metallic = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|PBR")
	float RimStrength = 0.25f;
};

USTRUCT(BlueprintType)
struct FZeroZoneTacticalStructureSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Structures")
	FString StructureId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Structures")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Structures")
	FVector DimensionsCm = FVector(400.f, 400.f, 200.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Structures")
	FString MaterialPresetId = TEXT("brushed_steel");
};

USTRUCT(BlueprintType)
struct FZeroZoneBridgeNetworkSegment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	FString SegmentId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	EZeroZoneBridgeSegmentKind Kind = EZeroZoneBridgeSegmentKind::PerimeterCatwalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	FVector LocationCm = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	FVector DimensionsCm = FVector(300.f, 300.f, 28.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	FRotator RotationDeg = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|BridgeNetwork")
	FString MaterialPresetId = TEXT("catwalk_grate");
};

USTRUCT(BlueprintType)
struct FZeroZoneWeaponInspectionSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	FString WeaponId = TEXT("rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	FString DisplayName = TEXT("VX-47 Assault Rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	float MuzzleVelocityCmPerSec = 12000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	float BaseDamage = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	float HeadshotDamage = 52.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Inspection")
	FString ShaderProfile = TEXT("Vulkan_GLSL_450_PBR_Fresnel");
};

USTRUCT(BlueprintType)
struct FZeroZoneWeaponAttachmentSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	FString WeaponId = TEXT("rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	FString AttachmentName = TEXT("Holographic Red-Dot Sight");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	FString AttachmentType = TEXT("HoloRedDot");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	FLinearColor ReticleColor = FLinearColor(1.0f, 0.16f, 0.16f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	float ADSFOV = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Attachments")
	bool bHasLaserBeam = false;
};

USTRUCT(BlueprintType)
struct FZeroZoneExplosiveBarrelSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	FString BarrelId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	FVector LocationCm = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	float MaxHealth = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	float BlastRadiusCm = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	float MaxSplashDamage = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Explosives")
	float RespawnDelaySeconds = 25.f;
};

UENUM(BlueprintType)
enum class EZeroZoneSinaiZoneKind : uint8
{
	SuezCanalChannel	UMETA(DisplayName = "Suez Canal Water Channel"),
	PontoonBridge		UMETA(DisplayName = "Tactical Pontoon Bridge"),
	SandBermRampart		UMETA(DisplayName = "East Bank Sand Berm Rampart"),
	SandBermRamp		UMETA(DisplayName = "Sand Berm Assault Ramp"),
	BarLevBunker		UMETA(DisplayName = "Desert Concrete Bunker"),
	CommandRelay		UMETA(DisplayName = "Central Canal Command Relay"),
	TrenchBarricade		UMETA(DisplayName = "Trench & Sandbag Barricade"),
	OasisPalmCluster	UMETA(DisplayName = "West Bank Oasis Date Palms"),
	AntiTankHedgehogLine UMETA(DisplayName = "Czech Hedgehog Anti-Tank Line"),
	BunkerCamoCanopy	UMETA(DisplayName = "Bar-Lev Camo Netting Canopy"),
	LimestoneOutcrop	UMETA(DisplayName = "Sinai Limestone Rocky Outcrop"),
	CharredTankWreck	UMETA(DisplayName = "Charred 1973 Tank Hull Wreck"),
	DragonsTeethLine	UMETA(DisplayName = "Concrete Dragons Teeth Tank Traps"),
	FieldSupplyDepot	UMETA(DisplayName = "Military Field Tent & Supply Depot"),
	ArtilleryShellCrater UMETA(DisplayName = "Scorched Artillery Shell Crater")
};

USTRUCT(BlueprintType)
struct FZeroZoneSinaiBattlefieldSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FString SegmentId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	EZeroZoneSinaiZoneKind ZoneKind = EZeroZoneSinaiZoneKind::PontoonBridge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FVector LocationCm = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FVector DimensionsCm = FVector(420.f, 1020.f, 24.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FRotator RotationDeg = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FString MaterialPresetId = TEXT("pontoon_steel");
};

UENUM(BlueprintType)
enum class EZeroZoneKillstreakKind : uint8
{
	UavReconPulse		UMETA(DisplayName = "UAV Recon Radar Pulse"),
	ArtilleryBarrage155	UMETA(DisplayName = "155mm Sinai Artillery Barrage")
};

USTRUCT(BlueprintType)
struct FZeroZoneTacticalKillstreakSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	FString KillstreakId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	EZeroZoneKillstreakKind Kind = EZeroZoneKillstreakKind::UavReconPulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	FString HotkeyLabel = TEXT("[4]");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	float CooldownSeconds = 22.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	float ActiveDurationSeconds = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	float BlastRadiusCm = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	float MaxSplashDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	int32 ShellCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Killstreaks")
	float KoCooldownRefundSeconds = 6.f;
};

USTRUCT(BlueprintType)
struct FZeroZoneMinimapRadarSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Radar")
	float RadarRangeCm = 7400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Radar")
	float PassiveDetectRangeCm = 3400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Radar")
	float CanalHalfWidthCm = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Radar")
	float EastBermOffsetYCm = -2200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Radar")
	bool bShowExplosiveBarrels = true;
};

UENUM(BlueprintType)
enum class EZeroZoneSinaiTimeOfDay : uint8
{
	Dawn		UMETA(DisplayName = "Sinai Dawn (06:15)"),
	Noon		UMETA(DisplayName = "Sinai High Noon (12:30)"),
	GoldenDusk	UMETA(DisplayName = "Sinai Golden Dusk (17:30)"),
	DesertNight	UMETA(DisplayName = "Sinai Desert Night (22:30)")
};

USTRUCT(BlueprintType)
struct FZeroZoneSinaiAtmospherePreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	EZeroZoneSinaiTimeOfDay TimeOfDay = EZeroZoneSinaiTimeOfDay::GoldenDusk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FString PresetLabel = TEXT("DUSK // 17:30");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FVector SunDirection = FVector(42.f, 34.f, 32.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FLinearColor SunColor = FLinearColor(1.0f, 0.81f, 0.59f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	float SunIntensity = 1.95f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FLinearColor ZenithColor = FLinearColor(0.18f, 0.35f, 0.57f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	FLinearColor HorizonFogColor = FLinearColor(0.85f, 0.71f, 0.50f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZeroZone|Sinai1973")
	float FogDensity = 0.0042f;
};

UCLASS()
class ZEROZONEUE5_API UZeroZoneBattlefieldAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static TArray<FZeroZoneBattlefieldPBRPreset> GetBattlefieldPBRPresets();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static TArray<FZeroZoneTacticalStructureSpec> GetTacticalStructureSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static TArray<FZeroZoneBridgeNetworkSegment> GetUpperCombatNetworkSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static FZeroZoneWeaponInspectionSpec GetWeaponInspectionSpec(EZeroZoneWeaponType WeaponType);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static TArray<FZeroZoneWeaponAttachmentSpec> GetWeaponAttachmentSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static FZeroZoneWeaponAttachmentSpec GetWeaponAttachmentSpecForType(EZeroZoneWeaponType WeaponType);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|BattlefieldAssets")
	static TArray<FZeroZoneExplosiveBarrelSpec> GetExplosiveBarrelSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Sinai1973")
	static TArray<FZeroZoneSinaiBattlefieldSpec> GetSinaiBattlefieldSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Sinai1973")
	static TArray<FZeroZoneTacticalKillstreakSpec> GetTacticalKillstreakSpecs();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Sinai1973")
	static FZeroZoneMinimapRadarSpec GetMinimapRadarSpec();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Sinai1973")
	static TArray<FZeroZoneSinaiAtmospherePreset> GetSinaiAtmospherePresets();
};

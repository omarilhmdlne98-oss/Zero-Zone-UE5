// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZeroZoneTypes.generated.h"

UENUM(BlueprintType)
enum class EZeroZoneOperatorClass : uint8
{
	Vanguard	UMETA(DisplayName = "Vanguard"),
	Spectre		UMETA(DisplayName = "Spectre"),
	Juggernaut	UMETA(DisplayName = "Juggernaut")
};

UENUM(BlueprintType)
enum class EZeroZoneWeaponType : uint8
{
	AssaultRifle	UMETA(DisplayName = "Assault Rifle"),
	SMG				UMETA(DisplayName = "Submachine Gun"),
	Shotgun			UMETA(DisplayName = "Shotgun"),
	SniperRifle		UMETA(DisplayName = "Sniper Rifle"),
	HeavyCannon		UMETA(DisplayName = "Heavy Cannon"),
	Sidearm			UMETA(DisplayName = "Sidearm")
};

UENUM(BlueprintType)
enum class EZeroZoneLocomotionState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walk			UMETA(DisplayName = "Walk"),
	TacticalSprint	UMETA(DisplayName = "Tactical Sprint"),
	Crouch			UMETA(DisplayName = "Crouch"),
	Slide			UMETA(DisplayName = "Slide"),
	Mantle			UMETA(DisplayName = "Mantle / Vault"),
	ADS				UMETA(DisplayName = "Aim Down Sights")
};

UENUM(BlueprintType)
enum class EZeroZoneHitZone : uint8
{
	Head		UMETA(DisplayName = "Head (2.2x)"),
	UpperTorso	UMETA(DisplayName = "Upper Torso (1.0x)"),
	Limb		UMETA(DisplayName = "Limb (0.75x)")
};

UENUM(BlueprintType)
enum class EZeroZoneLightingPreset : uint8
{
	DawnOps			UMETA(DisplayName = "Dawn Ops"),
	NoonTactical	UMETA(DisplayName = "Noon Tactical"),
	DuskAssault		UMETA(DisplayName = "Dusk Assault"),
	NightStealth	UMETA(DisplayName = "Night Stealth")
};

USTRUCT(BlueprintType)
struct FZeroZoneTacticalLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FString RigProfileName = TEXT("Recon Tactical Rig");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FName HelmetSocketName = TEXT("head");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FName PlateCarrierSocketName = TEXT("spine_03");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FName SidearmSocketName = TEXT("thigh_r");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FName PrimaryWeaponSocketName = TEXT("WeaponSocket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EZeroZoneWeaponType PreferredPrimaryWeapon = EZeroZoneWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	bool bHasTacticalHelmet = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	bool bHasPlateCarrier = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	bool bHasKneePads = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	bool bHasHolsteredSidearm = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	bool bSilentFootsteps = false;
};

USTRUCT(BlueprintType)
struct FZeroZoneIKAnimationData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|IK")
	float LeftFootIKOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|IK")
	float RightFootIKOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|IK")
	float PelvisZOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|AimOffset")
	float AimPitch = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|AimOffset")
	float AimYaw = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Tactical")
	float LeanAlpha = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Tactical")
	float ADSAlpha = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Tactical")
	float CrouchAlpha = 0.f;
};

USTRUCT(BlueprintType)
struct FZeroZoneArenaSurfaceConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|PBR")
	FString SurfaceName = TEXT("Weathered Concrete");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|PBR", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Roughness = 0.82f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|PBR", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Metallic = 0.08f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|PBR")
	bool bDestructible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|PBR")
	float MaxDurability = 100.f;
};

USTRUCT(BlueprintType)
struct FZeroZoneOperatorStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float MaxShield = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float WalkSpeed = 440.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float CrouchSpeed = 240.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float ADSSpeed = 280.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float BaseDamageReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float AbilityCooldown = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float AbilityDuration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	FString AbilityName = TEXT("Tactical Ability");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	bool bSilentFootsteps = false;
};

USTRUCT(BlueprintType)
struct FZeroZoneWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponId = TEXT("rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName = TEXT("VX-47 Assault Rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EZeroZoneWeaponType WeaponType = EZeroZoneWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BaseDamage = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.1"))
	float FireRate = 9.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ballistics", meta = (ClampMin = "1000.0"))
	float MuzzleVelocity = 12000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ballistics", meta = (ClampMin = "0.0"))
	float BulletDropGravity = 980.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ballistics", meta = (ClampMin = "1"))
	int32 PelletCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ballistics")
	float HeadshotMultiplier = 2.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ballistics")
	float LimbMultiplier = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Handling")
	float RecoilPitch = 0.55f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Handling")
	float RecoilYaw = 0.22f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Handling")
	float RecoilRecoveryRate = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Handling", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float ADSSpreadMultiplier = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1"))
	int32 MaxMagazineAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0"))
	int32 MaxReserveAmmo = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.1"))
	float ReloadDuration = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
	float BaseSpreadDegrees = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsAutomatic = true;
};

USTRUCT(BlueprintType)
struct FZeroZoneNetworkPlayerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	EZeroZoneOperatorClass OperatorClass = EZeroZoneOperatorClass::Vanguard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	float Shield = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	bool bIsAbilityActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	FString WeaponId = TEXT("rifle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	bool bIsADS = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
	float LeanAngle = 0.f;
};

UENUM(BlueprintType)
enum class EZeroZoneMatchPhase : uint8
{
	WaitingForPlayers	UMETA(DisplayName = "Waiting For Players"),
	Warmup				UMETA(DisplayName = "Warmup"),
	InProgress			UMETA(DisplayName = "In Progress"),
	PostMatch			UMETA(DisplayName = "Post Match")
};

UENUM(BlueprintType)
enum class EZeroZoneMatchmakingState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Searching	UMETA(DisplayName = "Searching"),
	InLobby		UMETA(DisplayName = "In Lobby"),
	MatchFound	UMETA(DisplayName = "Match Found"),
	Connecting	UMETA(DisplayName = "Connecting")
};

USTRUCT(BlueprintType)
struct FZeroZoneKillFeedEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|KillFeed")
	FString KillerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|KillFeed")
	FString VictimName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|KillFeed")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|KillFeed")
	bool bIsHeadshot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|KillFeed")
	float Timestamp = 0.f;
};

USTRUCT(BlueprintType)
struct FZeroZoneHUDData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Shield = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float MaxShield = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 ReserveAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	EZeroZoneOperatorClass OperatorClass = EZeroZoneOperatorClass::Vanguard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float MatchTimeRemaining = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 TeamAScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 TeamBScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	EZeroZoneMatchPhase MatchPhase = EZeroZoneMatchPhase::WaitingForPlayers;
};

USTRUCT(BlueprintType)
struct FZeroZoneRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	FString RoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	FString GameMode = TEXT("Arena");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	int32 CurrentPlayers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	int32 MaxPlayers = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matchmaking")
	FString ServerAddress;
};


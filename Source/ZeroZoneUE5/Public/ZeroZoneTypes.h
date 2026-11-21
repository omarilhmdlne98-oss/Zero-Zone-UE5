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

USTRUCT(BlueprintType)
struct FZeroZoneOperatorStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float MaxShield = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float SprintSpeed = 850.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float BaseDamageReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float AbilityCooldown = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	float AbilityDuration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operator")
	FString AbilityName = TEXT("Tactical Ability");
};

USTRUCT(BlueprintType)
struct FZeroZoneWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName = TEXT("VX-47 Carbine");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EZeroZoneWeaponType WeaponType = EZeroZoneWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BaseDamage = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.1"))
	float FireRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 15000.f;

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


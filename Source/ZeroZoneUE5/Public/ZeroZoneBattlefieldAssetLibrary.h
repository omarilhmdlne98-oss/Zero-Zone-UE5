// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneBattlefieldAssetLibrary.generated.h"

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
	static FZeroZoneWeaponInspectionSpec GetWeaponInspectionSpec(EZeroZoneWeaponType WeaponType);
};

#pragma once
#include "CoreMinimal.h"
#include "ZeroZoneBattlefieldAssetLibrary.h"
#include "Engine/DataAsset.h"
#include "ZeroZoneTheaterLibrary.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct ZEROZONEUE5_API FZeroZoneTheaterSector : public FZeroZoneGpsSectorMetadata
{
    GENERATED_BODY()
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Theater") FName MapPackage;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Theater") bool bGeoreferenced = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Theater") float SampleSpacingM = 4.f;
};

UCLASS(BlueprintType)
class ZEROZONEUE5_API UZeroZoneTheaterPalette : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Surfaces") TObjectPtr<UMaterialInterface> WetCanalSilt;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Surfaces") TObjectPtr<UMaterialInterface> WindRippledSand;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Surfaces") TObjectPtr<UMaterialInterface> FracturedLimestone;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Surfaces") TObjectPtr<UMaterialInterface> WeatheredConcrete;
};

UCLASS()
class ZEROZONEUE5_API UZeroZoneTheaterLibrary : public UZeroZoneBattlefieldAssetLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="ZeroZone|Theater") static FZeroZoneTheaterSector DescribeTheater(bool bHighlands);
    // Editor-only operation; returns false in packaged games or outside a new WP level.
    UFUNCTION(BlueprintCallable, Category="ZeroZone|Theater") static bool BakeTheaterLandscape(bool bHighlands, UZeroZoneTheaterPalette* Palette);
    UFUNCTION(BlueprintPure, Category="ZeroZone|Theater") static float TheaterHeightCm(bool bHighlands, float X, float Y);
};

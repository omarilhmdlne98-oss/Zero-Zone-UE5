// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDDataUpdatedSignature, const FZeroZoneHUDData&, HUDData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillFeedUpdatedSignature, const TArray<FZeroZoneKillFeedEntry>&, KillFeed);

UCLASS()
class ZEROZONEUE5_API AZeroZoneHUD : public AHUD
{
	GENERATED_BODY()

public:
	AZeroZoneHUD();

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|HUD")
	void AddKillFeedEntry(const FString& KillerName, const FString& VictimName, const FString& WeaponName, bool bIsHeadshot);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|HUD")
	void SetCrosshairSpread(float NewSpread);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|HUD")
	void TriggerHitmarker(bool bIsHeadshot = false);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|HUD")
	FZeroZoneHUDData GetHUDData() const;

	UFUNCTION(BlueprintPure, Category = "ZeroZone|HUD")
	const TArray<FZeroZoneKillFeedEntry>& GetKillFeed() const { return KillFeedEntries; }

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|HUD|Events")
	FOnHUDDataUpdatedSignature OnHUDDataUpdated;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|HUD|Events")
	FOnKillFeedUpdatedSignature OnKillFeedUpdated;

protected:
	virtual void BeginPlay() override;

	void DrawCrosshair();
	void DrawHitmarker();
	void PruneExpiredKillFeedEntries();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|Crosshair")
	FLinearColor CrosshairColor = FLinearColor(0.9f, 0.95f, 1.0f, 0.85f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|Crosshair")
	float CrosshairLineLength = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|Crosshair")
	float CrosshairLineThickness = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|Crosshair")
	float BaseCrosshairGap = 6.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|KillFeed")
	int32 MaxKillFeedEntries = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|HUD|KillFeed")
	float KillFeedEntryLifetime = 6.f;

	UPROPERTY(BlueprintReadOnly, Category = "ZeroZone|HUD|KillFeed")
	TArray<FZeroZoneKillFeedEntry> KillFeedEntries;

	float CurrentCrosshairSpread = 0.f;
	float HitmarkerRemainingTime = 0.f;
	bool bLastHitWasHeadshot = false;
};

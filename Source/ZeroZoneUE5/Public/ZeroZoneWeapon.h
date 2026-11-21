// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32, CurrentAmmo, int32, ReserveAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFiredSignature, FVector, ImpactPoint);

UCLASS()
class ZEROZONEUE5_API AZeroZoneWeapon : public AActor
{
	GENERATED_BODY()

public:
	AZeroZoneWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanReload() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetReserveAmmo() const { return ReserveAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	const FZeroZoneWeaponStats& GetWeaponStats() const { return WeaponStats; }

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnAmmoChangedSignature OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponFiredSignature OnWeaponFired;

protected:
	virtual void BeginPlay() override;

	void HandleFireShot();
	void ExecuteHitscan(const FVector& TraceStart, const FVector& TraceEnd);
	void FinishReload();

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector& TraceStart, const FVector& TraceEnd);

	UFUNCTION(Server, Reliable)
	void ServerStartReload();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayFireEffects(const FVector& ImpactPoint, bool bHitSomething);

	UFUNCTION()
	void OnRep_Ammo();

	UFUNCTION()
	void OnRep_IsReloading();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FZeroZoneWeaponStats WeaponStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = TEXT("MuzzleFlash");

	UPROPERTY(ReplicatedUsing = OnRep_Ammo, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo = 30;

	UPROPERTY(ReplicatedUsing = OnRep_Ammo, BlueprintReadOnly, Category = "Weapon")
	int32 ReserveAmmo = 150;

	UPROPERTY(ReplicatedUsing = OnRep_IsReloading, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading = false;

	FTimerHandle FireRateTimerHandle;
	FTimerHandle ReloadTimerHandle;
	float LastFireTime = -100.f;
	bool bIsFiring = false;
};

// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZeroZoneCombatComponent.generated.h"

class AZeroZoneWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedSignature, AZeroZoneWeapon*, NewWeapon);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZEROZONEUE5_API UZeroZoneCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroZoneCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SpawnDefaultWeapons();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(AZeroZoneWeapon* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SwitchWeapon(int32 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetADS(bool bNewADS);

	UFUNCTION(BlueprintPure, Category = "Combat")
	AZeroZoneWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsFiring() const { return bIsFiring; }

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnEquippedWeaponChangedSignature OnEquippedWeaponChanged;

protected:
	virtual void BeginPlay() override;

	void AttachWeaponToOwner(AZeroZoneWeapon* Weapon);

	UFUNCTION(Server, Reliable)
	void ServerSwitchWeapon(int32 WeaponIndex);

	UFUNCTION()
	void OnRep_EquippedWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<TSubclassOf<AZeroZoneWeapon>> DefaultWeaponClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName WeaponAttachSocket = TEXT("WeaponSocket");

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	TArray<TObjectPtr<AZeroZoneWeapon>> Weapons;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AZeroZoneWeapon> EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsFiring = false;
};

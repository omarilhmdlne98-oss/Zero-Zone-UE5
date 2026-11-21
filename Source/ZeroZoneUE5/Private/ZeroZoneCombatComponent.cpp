// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneCombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "ZeroZoneWeapon.h"
#include "ZeroZoneUE5.h"

UZeroZoneCombatComponent::UZeroZoneCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UZeroZoneCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZeroZoneCombatComponent, Weapons);
	DOREPLIFETIME(UZeroZoneCombatComponent, EquippedWeapon);
}

void UZeroZoneCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		SpawnDefaultWeapons();
	}
}

void UZeroZoneCombatComponent::SpawnDefaultWeapons()
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !GetWorld())
	{
		return;
	}

	if (DefaultWeaponClasses.IsEmpty())
	{
		DefaultWeaponClasses.Add(AZeroZoneWeapon::StaticClass());
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (const TSubclassOf<AZeroZoneWeapon>& WeaponClass : DefaultWeaponClasses)
	{
		if (!WeaponClass)
		{
			continue;
		}

		AZeroZoneWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AZeroZoneWeapon>(
			WeaponClass,
			GetOwner()->GetActorTransform(),
			SpawnParams
		);

		if (SpawnedWeapon)
		{
			SpawnedWeapon->SetActorHiddenInGame(true);
			Weapons.Add(SpawnedWeapon);
		}
	}

	if (Weapons.Num() > 0)
	{
		EquipWeapon(Weapons[0]);
	}
}

void UZeroZoneCombatComponent::EquipWeapon(AZeroZoneWeapon* WeaponToEquip)
{
	if (!WeaponToEquip || !GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (EquippedWeapon)
	{
		EquippedWeapon->StopFire();
		EquippedWeapon->SetActorHiddenInGame(true);
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(GetOwner());
	EquippedWeapon->SetActorHiddenInGame(false);
	AttachWeaponToOwner(EquippedWeapon);
	OnEquippedWeaponChanged.Broadcast(EquippedWeapon);
}

void UZeroZoneCombatComponent::SwitchWeapon(int32 WeaponIndex)
{
	if (!Weapons.IsValidIndex(WeaponIndex))
	{
		return;
	}

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		EquipWeapon(Weapons[WeaponIndex]);
	}
	else
	{
		ServerSwitchWeapon(WeaponIndex);
	}
}

void UZeroZoneCombatComponent::ServerSwitchWeapon_Implementation(int32 WeaponIndex)
{
	SwitchWeapon(WeaponIndex);
}

void UZeroZoneCombatComponent::StartFire()
{
	if (!EquippedWeapon)
	{
		return;
	}

	bIsFiring = true;
	EquippedWeapon->StartFire();
}

void UZeroZoneCombatComponent::StopFire()
{
	bIsFiring = false;
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFire();
	}
}

void UZeroZoneCombatComponent::Reload()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Reload();
	}
}

void UZeroZoneCombatComponent::AttachWeaponToOwner(AZeroZoneWeapon* Weapon)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!Weapon || !OwnerCharacter || !OwnerCharacter->GetMesh())
	{
		return;
	}

	Weapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		WeaponAttachSocket
	);
}

void UZeroZoneCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		AttachWeaponToOwner(EquippedWeapon);
	}
	OnEquippedWeaponChanged.Broadcast(EquippedWeapon);
}

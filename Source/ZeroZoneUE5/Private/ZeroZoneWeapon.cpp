// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "ZeroZoneUE5.h"

AZeroZoneWeapon::AZeroZoneWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZeroZoneWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroZoneWeapon, CurrentAmmo);
	DOREPLIFETIME(AZeroZoneWeapon, ReserveAmmo);
	DOREPLIFETIME(AZeroZoneWeapon, bIsReloading);
}

void AZeroZoneWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = WeaponStats.MaxMagazineAmmo;
	ReserveAmmo = WeaponStats.MaxReserveAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
}

bool AZeroZoneWeapon::CanFire() const
{
	return !bIsReloading && CurrentAmmo > 0;
}

bool AZeroZoneWeapon::CanReload() const
{
	return !bIsReloading && CurrentAmmo < WeaponStats.MaxMagazineAmmo && ReserveAmmo > 0;
}

void AZeroZoneWeapon::StartFire()
{
	if (bIsFiring || !CanFire())
	{
		if (!bIsReloading && CurrentAmmo <= 0 && CanReload())
		{
			Reload();
		}
		return;
	}

	bIsFiring = true;

	const float TimeBetweenShots = 1.f / FMath::Max(WeaponStats.FireRate, 0.1f);
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - CurrentTime, 0.f);

	if (WeaponStats.bIsAutomatic)
	{
		GetWorldTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&AZeroZoneWeapon::HandleFireShot,
			TimeBetweenShots,
			true,
			FirstDelay
		);
	}
	else if (FirstDelay <= KINDA_SMALL_NUMBER)
	{
		HandleFireShot();
	}
}

void AZeroZoneWeapon::StopFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
}

void AZeroZoneWeapon::HandleFireShot()
{
	if (!CanFire())
	{
		StopFire();
		if (CurrentAmmo <= 0 && CanReload())
		{
			Reload();
		}
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	FVector ViewLocation;
	FRotator ViewRotation;

	if (OwnerController)
	{
		OwnerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetActorLocation();
		ViewRotation = GetActorRotation();
	}

	const float SpreadRad = FMath::DegreesToRadians(WeaponStats.BaseSpreadDegrees);
	const FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), SpreadRad);
	const FVector TraceEnd = ViewLocation + (ShotDirection * WeaponStats.Range);

	LastFireTime = GetWorld()->GetTimeSeconds();

	if (HasAuthority())
	{
		ExecuteHitscan(ViewLocation, TraceEnd);
	}
	else
	{
		ServerFire(ViewLocation, TraceEnd);
	}
}

void AZeroZoneWeapon::ServerFire_Implementation(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!CanFire())
	{
		return;
	}
	ExecuteHitscan(TraceStart, TraceEnd);
}

void AZeroZoneWeapon::ExecuteHitscan(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!HasAuthority() || !CanFire())
	{
		return;
	}

	CurrentAmmo = FMath::Max(0, CurrentAmmo - 1);
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (AActor* MyOwner = GetOwner())
	{
		QueryParams.AddIgnoredActor(MyOwner);
	}
	QueryParams.bTraceComplex = true;

	FHitResult Hit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	const FVector ImpactPoint = bHit ? Hit.ImpactPoint : TraceEnd;

	if (bHit && Hit.GetActor())
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		AController* InstigatorController = OwnerPawn ? OwnerPawn->GetController() : nullptr;
		const FVector ShotDir = (TraceEnd - TraceStart).GetSafeNormal();

		UGameplayStatics::ApplyPointDamage(
			Hit.GetActor(),
			WeaponStats.BaseDamage,
			ShotDir,
			Hit,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
	}

	MulticastPlayFireEffects(ImpactPoint, bHit);
}

void AZeroZoneWeapon::MulticastPlayFireEffects_Implementation(const FVector& ImpactPoint, bool bHitSomething)
{
	OnWeaponFired.Broadcast(ImpactPoint);
}

void AZeroZoneWeapon::Reload()
{
	if (!CanReload())
	{
		return;
	}

	StopFire();

	if (HasAuthority())
	{
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(
			ReloadTimerHandle,
			this,
			&AZeroZoneWeapon::FinishReload,
			WeaponStats.ReloadDuration,
			false
		);
	}
	else
	{
		ServerStartReload();
	}
}

void AZeroZoneWeapon::ServerStartReload_Implementation()
{
	Reload();
}

void AZeroZoneWeapon::FinishReload()
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 NeededAmmo = WeaponStats.MaxMagazineAmmo - CurrentAmmo;
	const int32 AmmoToLoad = FMath::Min(NeededAmmo, ReserveAmmo);

	CurrentAmmo += AmmoToLoad;
	ReserveAmmo -= AmmoToLoad;
	bIsReloading = false;

	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
}

void AZeroZoneWeapon::OnRep_Ammo()
{
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
}

void AZeroZoneWeapon::OnRep_IsReloading()
{
	UE_LOG(LogZeroZone, Verbose, TEXT("Weapon %s reload state: %s"), *WeaponStats.WeaponName, bIsReloading ? TEXT("Reloading") : TEXT("Ready"));
}

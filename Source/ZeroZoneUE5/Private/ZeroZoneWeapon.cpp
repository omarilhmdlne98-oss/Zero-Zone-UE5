// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneCombatComponent.h"
#include "ZeroZoneHUD.h"
#include "ZeroZoneWebSocketSubsystem.h"
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

void AZeroZoneWeapon::SetADS(bool bNewADS)
{
	bIsADS = bNewADS;
}

void AZeroZoneWeapon::ConfigureWeaponPreset(EZeroZoneWeaponType PresetType)
{
	WeaponStats.WeaponType = PresetType;

	switch (PresetType)
	{
	case EZeroZoneWeaponType::SniperRifle:
		WeaponStats.WeaponId = TEXT("railgun");
		WeaponStats.WeaponName = TEXT("XR-9 Railgun");
		WeaponStats.BaseDamage = 68.f;
		WeaponStats.FireRate = 1.2f;
		WeaponStats.Range = 25000.f;
		WeaponStats.MuzzleVelocity = 26000.f;
		WeaponStats.BulletDropGravity = 320.f;
		WeaponStats.PelletCount = 1;
		WeaponStats.RecoilPitch = 1.85f;
		WeaponStats.RecoilYaw = 0.35f;
		WeaponStats.BaseSpreadDegrees = 0.45f;
		WeaponStats.ADSSpreadMultiplier = 0.10f;
		WeaponStats.MaxMagazineAmmo = 6;
		WeaponStats.MaxReserveAmmo = 30;
		WeaponStats.ReloadDuration = 2.3f;
		WeaponStats.bIsAutomatic = false;
		break;

	case EZeroZoneWeaponType::Shotgun:
		WeaponStats.WeaponId = TEXT("shotgun");
		WeaponStats.WeaponName = TEXT("M-12 Heavy Shotgun");
		WeaponStats.BaseDamage = 14.f;
		WeaponStats.FireRate = 1.6f;
		WeaponStats.Range = 6500.f;
		WeaponStats.MuzzleVelocity = 9500.f;
		WeaponStats.BulletDropGravity = 1100.f;
		WeaponStats.PelletCount = 6;
		WeaponStats.RecoilPitch = 1.65f;
		WeaponStats.RecoilYaw = 0.55f;
		WeaponStats.BaseSpreadDegrees = 4.5f;
		WeaponStats.ADSSpreadMultiplier = 0.65f;
		WeaponStats.MaxMagazineAmmo = 8;
		WeaponStats.MaxReserveAmmo = 40;
		WeaponStats.ReloadDuration = 2.1f;
		WeaponStats.bIsAutomatic = false;
		break;

	case EZeroZoneWeaponType::AssaultRifle:
	default:
		WeaponStats.WeaponId = TEXT("rifle");
		WeaponStats.WeaponName = TEXT("VX-47 Assault Rifle");
		WeaponStats.BaseDamage = 24.f;
		WeaponStats.FireRate = 9.0f;
		WeaponStats.Range = 15000.f;
		WeaponStats.MuzzleVelocity = 12000.f;
		WeaponStats.BulletDropGravity = 980.f;
		WeaponStats.PelletCount = 1;
		WeaponStats.RecoilPitch = 0.55f;
		WeaponStats.RecoilYaw = 0.22f;
		WeaponStats.BaseSpreadDegrees = 1.2f;
		WeaponStats.ADSSpreadMultiplier = 0.35f;
		WeaponStats.MaxMagazineAmmo = 30;
		WeaponStats.MaxReserveAmmo = 150;
		WeaponStats.ReloadDuration = 1.8f;
		WeaponStats.bIsAutomatic = true;
		break;
	}

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

void AZeroZoneWeapon::ApplyRecoilToOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled())
	{
		return;
	}

	const float RecoilScale = bIsADS ? 0.65f : 1.0f;
	const float PitchKick = -WeaponStats.RecoilPitch * RecoilScale;
	const float YawKick = FMath::FRandRange(-WeaponStats.RecoilYaw, WeaponStats.RecoilYaw) * RecoilScale;

	OwnerPawn->AddControllerPitchInput(PitchKick);
	OwnerPawn->AddControllerYawInput(YawKick);
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

	if (!GetOwner() || !GetWorld())
	{
		return;
	}

	if (!HasAuthority())
	{
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
	ApplyRecoilToOwner();

	if (HasAuthority())
	{
		ServerFire_Implementation(FVector::ZeroVector, FVector::ZeroVector);
	}
	else
	{
		ServerFire(FVector::ZeroVector, FVector::ZeroVector);
	}
}

void AZeroZoneWeapon::ServerFire_Implementation(const FVector&, const FVector&)
{
	if (!HasAuthority() || !GetWorld() || !CanFire())
	{
		return;
	}

	AZeroZoneCharacter* OwnerCharacter = Cast<AZeroZoneCharacter>(GetOwner());
	if (!OwnerCharacter || OwnerCharacter->IsDead())
	{
		return;
	}

	UZeroZoneCombatComponent* Combat = OwnerCharacter->GetCombatComponent();
	if (!Combat || Combat->GetEquippedWeapon() != this)
	{
		return;
	}

	AController* OwnerController = OwnerCharacter->GetController();
	if (!OwnerController)
	{
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	OwnerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector AimDirection = ViewRotation.Vector().GetSafeNormal();
	if (AimDirection.IsNearlyZero())
	{
		return;
	}

	const float EffectiveSpreadDeg = WeaponStats.BaseSpreadDegrees * (bIsADS ? WeaponStats.ADSSpreadMultiplier : 1.0f);
	const float SpreadRad = FMath::DegreesToRadians(EffectiveSpreadDeg);
	const FVector ShotDirection = FMath::VRandCone(AimDirection, SpreadRad);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeBetweenShots = 1.f / FMath::Max(WeaponStats.FireRate, 0.1f);
	// Tolerate network jitter: accept shots arriving at >=85% of the nominal interval
	const auto MinAllowedInterval = TimeBetweenShots * 0.85f;
	if (CurrentTime - LastFireTime + KINDA_SMALL_NUMBER < MinAllowedInterval)
	{
		return;
	}

	LastFireTime = CurrentTime;
	const FVector TraceEnd = ViewLocation + (ShotDirection * WeaponStats.Range);
	ExecuteHitscan(ViewLocation, TraceEnd);
}

void AZeroZoneWeapon::ExecuteBallisticTrace(const FVector& StartLocation, const FVector& ShotDirection, FHitResult& OutHit, FVector& OutImpactPoint) const
{
	OutImpactPoint = StartLocation + (ShotDirection * WeaponStats.Range);
	if (!GetWorld())
	{
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (AActor* MyOwner = GetOwner())
	{
		QueryParams.AddIgnoredActor(MyOwner);
	}
	QueryParams.bTraceComplex = true;

	// Segmented ballistic arc simulation with muzzle velocity and gravity bullet drop
	const int32 NumSegments = 6;
	const float TotalFlightTime = WeaponStats.Range / FMath::Max(WeaponStats.MuzzleVelocity, 1000.f);
	const float StepTime = TotalFlightTime / static_cast<float>(NumSegments);
	const FVector GravityVec(0.f, 0.f, -WeaponStats.BulletDropGravity);

	FVector CurrentPos = StartLocation;
	FVector CurrentVel = ShotDirection.GetSafeNormal() * WeaponStats.MuzzleVelocity;

	for (int32 Step = 0; Step < NumSegments; ++Step)
	{
		const FVector NextPos = CurrentPos + (CurrentVel * StepTime) + (0.5f * GravityVec * StepTime * StepTime);
		CurrentVel += GravityVec * StepTime;

		if (GetWorld()->LineTraceSingleByChannel(OutHit, CurrentPos, NextPos, ECC_Visibility, QueryParams))
		{
			OutImpactPoint = OutHit.ImpactPoint;
			return;
		}
		CurrentPos = NextPos;
	}

	OutImpactPoint = CurrentPos;
}

void AZeroZoneWeapon::ExecuteHitscan(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!HasAuthority() || !CanFire())
	{
		return;
	}

	CurrentAmmo = FMath::Max(0, CurrentAmmo - 1);
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);

	const FVector BaseDir = (TraceEnd - TraceStart).GetSafeNormal();
	const int32 NumPellets = FMath::Max(1, WeaponStats.PelletCount);
	const float EffectiveSpreadDeg = WeaponStats.BaseSpreadDegrees * (bIsADS ? WeaponStats.ADSSpreadMultiplier : 1.0f);
	const float PelletSpreadRad = FMath::DegreesToRadians(EffectiveSpreadDeg);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AController* InstigatorController = OwnerPawn ? OwnerPawn->GetController() : nullptr;
	FVector LastImpactPoint = TraceEnd;
	bool bAnyHit = false;

	for (int32 Pellet = 0; Pellet < NumPellets; ++Pellet)
	{
		const FVector PelletDir = (Pellet == 0) ? BaseDir : FMath::VRandCone(BaseDir, PelletSpreadRad);
		FHitResult Hit;
		FVector ImpactPoint = TraceEnd;
		ExecuteBallisticTrace(TraceStart, PelletDir, Hit, ImpactPoint);
		LastImpactPoint = ImpactPoint;

		if (Hit.bBlockingHit && Hit.GetActor())
		{
			bAnyHit = true;
			const EZeroZoneHitZone HitZone = AZeroZoneCharacter::ResolveHitZoneFromBone(Hit.BoneName);
			const bool bIsHeadshot = (HitZone == EZeroZoneHitZone::Head);

			UGameplayStatics::ApplyPointDamage(
				Hit.GetActor(),
				WeaponStats.BaseDamage,
				PelletDir,
				Hit,
				InstigatorController,
				this,
				UDamageType::StaticClass()
			);

			if (APlayerController* PC = Cast<APlayerController>(InstigatorController))
			{
				if (AZeroZoneHUD* HUD = Cast<AZeroZoneHUD>(PC->GetHUD()))
				{
					HUD->TriggerHitmarker(bIsHeadshot);
				}
			}

			if (UGameInstance* GI = GetGameInstance())
			{
				if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
				{
					if (WS->IsConnected())
					{
						const FString AttackerId = (OwnerPawn && OwnerPawn->GetPlayerState()) ? OwnerPawn->GetPlayerState()->GetPlayerName() : GetName();
						const FString TargetId = Hit.GetActor()->GetName();
						const FString ZoneStr = bIsHeadshot ? TEXT("head") : (HitZone == EZeroZoneHitZone::Limb ? TEXT("limb") : TEXT("torso"));
						WS->SendCombatEvent(TEXT("hit_player"), AttackerId, TargetId, WeaponStats.BaseDamage, WeaponStats.WeaponId, ZoneStr);
					}
				}
			}
		}
	}

	MulticastPlayFireEffects(LastImpactPoint, bAnyHit);
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

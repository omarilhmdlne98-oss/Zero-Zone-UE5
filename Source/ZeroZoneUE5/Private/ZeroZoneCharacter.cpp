// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "ZeroZoneCombatComponent.h"
#include "ZeroZoneWeapon.h"
#include "ZeroZoneWebSocketSubsystem.h"
#include "ZeroZoneUE5.h"

AZeroZoneCharacter::AZeroZoneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Realistic 180cm tall human tactical operator capsule (half-height 90cm, radius 34cm)
	GetCapsuleComponent()->InitCapsuleSize(34.f, 90.f);

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 520.f;
	GetCharacterMovement()->AirControl = 0.30f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(58.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 220.f;
	CameraBoom->SocketOffset = FVector(0.f, 48.f, 68.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetFieldOfView(DefaultFOV);

	// Tactical equipment attachment hierarchy (attached to skeletal sockets in BeginPlay / ApplyOperatorLoadout)
	TacticalHelmetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TacticalHelmetMesh"));
	TacticalHelmetMesh->SetupAttachment(GetMesh(), TEXT("head"));
	TacticalHelmetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlateCarrierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateCarrierMesh"));
	PlateCarrierMesh->SetupAttachment(GetMesh(), TEXT("spine_03"));
	PlateCarrierMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HolsteredSidearmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HolsteredSidearmMesh"));
	HolsteredSidearmMesh->SetupAttachment(GetMesh(), TEXT("thigh_r"));
	HolsteredSidearmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CombatComponent = CreateDefaultSubobject<UZeroZoneCombatComponent>(TEXT("CombatComponent"));
}

void AZeroZoneCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroZoneCharacter, OperatorClass);
	DOREPLIFETIME(AZeroZoneCharacter, Health);
	DOREPLIFETIME(AZeroZoneCharacter, MaxHealth);
	DOREPLIFETIME(AZeroZoneCharacter, Shield);
	DOREPLIFETIME(AZeroZoneCharacter, MaxShield);
	DOREPLIFETIME(AZeroZoneCharacter, bIsSprinting);
	DOREPLIFETIME(AZeroZoneCharacter, bIsADS);
	DOREPLIFETIME(AZeroZoneCharacter, bIsSliding);
	DOREPLIFETIME(AZeroZoneCharacter, TargetLeanAngle);
	DOREPLIFETIME(AZeroZoneCharacter, bIsAbilityActive);
	DOREPLIFETIME(AZeroZoneCharacter, bIsDead);
}

void AZeroZoneCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyOperatorStats(true);
	ApplyOperatorLoadout();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AZeroZoneCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SlideTimerHandle);
	GetWorldTimerManager().ClearTimer(AbilityDurationTimerHandle);
	GetWorldTimerManager().ClearTimer(AbilityCooldownTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AZeroZoneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraAndLean(DeltaTime);
	UpdateProceduralIK(DeltaTime);
	UpdateLocomotionState();

	if (IsLocallyControlled() && !bIsDead)
	{
		WebSocketSyncAccumulator += DeltaTime;
		if (WebSocketSyncAccumulator >= WebSocketSyncInterval)
		{
			WebSocketSyncAccumulator = 0.f;
			SyncStateToWebSocket();
		}
	}
}

void AZeroZoneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroZoneCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroZoneCharacter::Look);
		}
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::StartSprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AZeroZoneCharacter::StopSprint);
		}
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::ToggleTacticalCrouch);
		}
		if (ADSAction)
		{
			EnhancedInput->BindAction(ADSAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::StartADS);
			EnhancedInput->BindAction(ADSAction, ETriggerEvent::Completed, this, &AZeroZoneCharacter::StopADS);
		}
		if (LeanLeftAction)
		{
			EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::StartLeanLeft);
			EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Completed, this, &AZeroZoneCharacter::StopLeanLeft);
		}
		if (LeanRightAction)
		{
			EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::StartLeanRight);
			EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Completed, this, &AZeroZoneCharacter::StopLeanRight);
		}
		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::StartFire);
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &AZeroZoneCharacter::StopFire);
		}
		if (ReloadAction)
		{
			EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::ReloadWeapon);
		}
		if (TacticalAbilityAction)
		{
			EnhancedInput->BindAction(TacticalAbilityAction, ETriggerEvent::Started, this, &AZeroZoneCharacter::ActivateTacticalAbility);
		}
	}
}

void AZeroZoneCharacter::Move(const FInputActionValue& Value)
{
	if (bIsDead || !Controller)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AZeroZoneCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	const float SensScale = bIsADS ? 0.65f : 1.0f;
	AddControllerYawInput(LookAxisVector.X * SensScale);
	AddControllerPitchInput(LookAxisVector.Y * SensScale);
}

void AZeroZoneCharacter::StartSprint()
{
	if (bIsDead)
	{
		return;
	}

	if (bIsCrouched)
	{
		UnCrouch();
	}
	if (bIsADS)
	{
		StopADS();
	}

	bIsSprinting = true;
	UpdateMovementSpeed();
	if (!HasAuthority())
	{
		ServerSetSprinting(true);
	}
}

void AZeroZoneCharacter::StopSprint()
{
	bIsSprinting = false;
	UpdateMovementSpeed();
	if (!HasAuthority())
	{
		ServerSetSprinting(false);
	}
}

void AZeroZoneCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	bIsSprinting = bNewSprinting;
	UpdateMovementSpeed();
}

void AZeroZoneCharacter::StartADS()
{
	if (bIsDead)
	{
		return;
	}

	bIsADS = true;
	bIsSprinting = false;
	if (CombatComponent)
	{
		CombatComponent->SetADS(true);
	}
	UpdateMovementSpeed();
	if (!HasAuthority())
	{
		ServerSetADS(true);
	}
}

void AZeroZoneCharacter::StopADS()
{
	bIsADS = false;
	if (CombatComponent)
	{
		CombatComponent->SetADS(false);
	}
	UpdateMovementSpeed();
	if (!HasAuthority())
	{
		ServerSetADS(false);
	}
}

void AZeroZoneCharacter::ServerSetADS_Implementation(bool bNewADS)
{
	bIsADS = bNewADS;
	if (CombatComponent)
	{
		CombatComponent->SetADS(bNewADS);
	}
	UpdateMovementSpeed();
}

void AZeroZoneCharacter::ToggleTacticalCrouch()
{
	if (bIsDead)
	{
		return;
	}

	if (bIsSprinting && GetVelocity().Size2D() > CurrentOperatorStats.WalkSpeed * 0.9f && !GetCharacterMovement()->IsFalling())
	{
		StartSlide();
		return;
	}

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
	UpdateMovementSpeed();
}

void AZeroZoneCharacter::StartSlide()
{
	if (bIsDead || bIsSliding)
	{
		return;
	}

	bIsSliding = true;
	bIsSprinting = false;
	Crouch();

	const FVector SlideDir = GetVelocity().GetSafeNormal2D().IsNearlyZero() ? GetActorForwardVector() : GetVelocity().GetSafeNormal2D();
	LaunchCharacter(SlideDir * (CurrentOperatorStats.SprintSpeed * 1.15f), true, false);

	GetWorldTimerManager().SetTimer(
		SlideTimerHandle,
		this,
		&AZeroZoneCharacter::EndSlide,
		0.65f,
		false
	);
}

void AZeroZoneCharacter::EndSlide()
{
	bIsSliding = false;
	UpdateMovementSpeed();
}

bool AZeroZoneCharacter::TryMantleVault()
{
	if (bIsDead || !GetWorld())
	{
		return false;
	}

	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 45.f);
	const FVector End = Start + GetActorForwardVector() * 110.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult WallHit;
	if (GetWorld()->LineTraceSingleByChannel(WallHit, Start, End, ECC_Visibility, Params))
	{
		const FVector LedgeTopStart = WallHit.ImpactPoint + GetActorForwardVector() * 35.f + FVector(0.f, 0.f, 130.f);
		const FVector LedgeTopEnd = LedgeTopStart - FVector(0.f, 0.f, 140.f);
		FHitResult LedgeHit;
		if (GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeTopStart, LedgeTopEnd, ECC_Visibility, Params))
		{
			const FVector MantleDest = LedgeHit.ImpactPoint + FVector(0.f, 0.f, 92.f);
			SetActorLocation(MantleDest, true);
			LocomotionState = EZeroZoneLocomotionState::Mantle;
			OnLocomotionStateChanged.Broadcast(LocomotionState);
			return true;
		}
	}

	return false;
}

void AZeroZoneCharacter::StartLeanLeft()
{
	bLeanLeftHeld = true;
	SetLeanInput(bLeanRightHeld ? 0.f : -1.f);
}

void AZeroZoneCharacter::StopLeanLeft()
{
	bLeanLeftHeld = false;
	SetLeanInput(bLeanRightHeld ? 1.f : 0.f);
}

void AZeroZoneCharacter::StartLeanRight()
{
	bLeanRightHeld = true;
	SetLeanInput(bLeanLeftHeld ? 0.f : 1.f);
}

void AZeroZoneCharacter::StopLeanRight()
{
	bLeanRightHeld = false;
	SetLeanInput(bLeanLeftHeld ? -1.f : 0.f);
}

void AZeroZoneCharacter::SetLeanInput(float LeanDirection)
{
	TargetLeanAngle = FMath::Clamp(LeanDirection, -1.f, 1.f) * MaxLeanDegrees;
	if (!HasAuthority())
	{
		ServerSetLeanAngle(TargetLeanAngle);
	}
}

void AZeroZoneCharacter::ServerSetLeanAngle_Implementation(float NewTargetLean)
{
	TargetLeanAngle = FMath::Clamp(NewTargetLean, -MaxLeanDegrees, MaxLeanDegrees);
}

void AZeroZoneCharacter::StartFire()
{
	if (!bIsDead && CombatComponent)
	{
		CombatComponent->StartFire();
	}
}

void AZeroZoneCharacter::StopFire()
{
	if (CombatComponent)
	{
		CombatComponent->StopFire();
	}
}

void AZeroZoneCharacter::ReloadWeapon()
{
	if (!bIsDead && CombatComponent)
	{
		CombatComponent->Reload();
	}
}

FZeroZoneOperatorStats AZeroZoneCharacter::GetOperatorStatsForClass(EZeroZoneOperatorClass ClassType)
{
	FZeroZoneOperatorStats Stats;
	switch (ClassType)
	{
	case EZeroZoneOperatorClass::Vanguard:
		Stats.MaxHealth = 100.f;
		Stats.MaxShield = 50.f;
		Stats.WalkSpeed = 440.f;
		Stats.SprintSpeed = 650.f;
		Stats.CrouchSpeed = 240.f;
		Stats.ADSSpeed = 290.f;
		Stats.BaseDamageReduction = 0.0f;
		Stats.AbilityCooldown = 8.f;
		Stats.AbilityDuration = 2.5f;
		Stats.AbilityName = TEXT("Tactical Dash");
		Stats.bSilentFootsteps = false;
		break;

	case EZeroZoneOperatorClass::Spectre:
		Stats.MaxHealth = 90.f;
		Stats.MaxShield = 40.f;
		Stats.WalkSpeed = 460.f;
		Stats.SprintSpeed = 620.f;
		Stats.CrouchSpeed = 270.f;
		Stats.ADSSpeed = 300.f;
		Stats.BaseDamageReduction = 0.0f;
		Stats.AbilityCooldown = 12.f;
		Stats.AbilityDuration = 4.0f;
		Stats.AbilityName = TEXT("Optical Cloak / Thermal");
		Stats.bSilentFootsteps = true;
		break;

	case EZeroZoneOperatorClass::Juggernaut:
		Stats.MaxHealth = 150.f;
		Stats.MaxShield = 100.f;
		Stats.WalkSpeed = 340.f;
		Stats.SprintSpeed = 460.f;
		Stats.CrouchSpeed = 190.f;
		Stats.ADSSpeed = 210.f;
		Stats.BaseDamageReduction = 0.20f;
		Stats.AbilityCooldown = 14.f;
		Stats.AbilityDuration = 5.0f;
		Stats.AbilityName = TEXT("Kinetic Riot Barrier");
		Stats.bSilentFootsteps = false;
		break;
	}
	return Stats;
}

FZeroZoneTacticalLoadout AZeroZoneCharacter::GetTacticalLoadoutForClass(EZeroZoneOperatorClass ClassType)
{
	FZeroZoneTacticalLoadout Loadout;
	switch (ClassType)
	{
	case EZeroZoneOperatorClass::Vanguard:
		Loadout.RigProfileName = TEXT("Vanguard Recon Plate Carrier");
		Loadout.PreferredPrimaryWeapon = EZeroZoneWeaponType::AssaultRifle;
		Loadout.bSilentFootsteps = false;
		break;

	case EZeroZoneOperatorClass::Spectre:
		Loadout.RigProfileName = TEXT("Spectre Ghillie Exosuit");
		Loadout.PreferredPrimaryWeapon = EZeroZoneWeaponType::SniperRifle;
		Loadout.bSilentFootsteps = true;
		break;

	case EZeroZoneOperatorClass::Juggernaut:
		Loadout.RigProfileName = TEXT("Juggernaut Heavy EOD Ballistic Rig");
		Loadout.PreferredPrimaryWeapon = EZeroZoneWeaponType::Shotgun;
		Loadout.bSilentFootsteps = false;
		break;
	}
	return Loadout;
}

EZeroZoneHitZone AZeroZoneCharacter::ResolveHitZoneFromBone(FName BoneName)
{
	const FString NameLower = BoneName.ToString().ToLower();
	if (NameLower.Contains(TEXT("head")) || NameLower.Contains(TEXT("neck")))
	{
		return EZeroZoneHitZone::Head;
	}
	if (NameLower.Contains(TEXT("arm")) || NameLower.Contains(TEXT("hand")) ||
		NameLower.Contains(TEXT("leg")) || NameLower.Contains(TEXT("thigh")) ||
		NameLower.Contains(TEXT("calf")) || NameLower.Contains(TEXT("foot")))
	{
		return EZeroZoneHitZone::Limb;
	}
	return EZeroZoneHitZone::UpperTorso;
}

float AZeroZoneCharacter::GetDamageMultiplierForHitZone(EZeroZoneHitZone HitZone)
{
	switch (HitZone)
	{
	case EZeroZoneHitZone::Head:
		return 2.2f;
	case EZeroZoneHitZone::Limb:
		return 0.75f;
	case EZeroZoneHitZone::UpperTorso:
	default:
		return 1.0f;
	}
}

void AZeroZoneCharacter::SetOperatorClass(EZeroZoneOperatorClass NewOperatorClass)
{
	if (HasAuthority())
	{
		OperatorClass = NewOperatorClass;
		ApplyOperatorStats(true);
		ApplyOperatorLoadout();
		OnOperatorClassChanged.Broadcast(OperatorClass);
	}
	else
	{
		ServerSetOperatorClass(NewOperatorClass);
	}
}

void AZeroZoneCharacter::ServerSetOperatorClass_Implementation(EZeroZoneOperatorClass NewOperatorClass)
{
	SetOperatorClass(NewOperatorClass);
}

void AZeroZoneCharacter::ApplyOperatorStats(bool bResetVitals)
{
	CurrentOperatorStats = GetOperatorStatsForClass(OperatorClass);
	MaxHealth = CurrentOperatorStats.MaxHealth;
	MaxShield = CurrentOperatorStats.MaxShield;

	if (bResetVitals)
	{
		Health = MaxHealth;
		Shield = MaxShield;
		OnHealthChanged.Broadcast(Health, MaxHealth);
		OnShieldChanged.Broadcast(Shield, MaxShield);
	}

	UpdateMovementSpeed();
}

void AZeroZoneCharacter::ApplyOperatorLoadout()
{
	CurrentLoadout = GetTacticalLoadoutForClass(OperatorClass);

	if (TacticalHelmetMesh && GetMesh())
	{
		TacticalHelmetMesh->SetVisibility(CurrentLoadout.bHasTacticalHelmet);
		TacticalHelmetMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentLoadout.HelmetSocketName);
	}
	if (PlateCarrierMesh && GetMesh())
	{
		PlateCarrierMesh->SetVisibility(CurrentLoadout.bHasPlateCarrier);
		PlateCarrierMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentLoadout.PlateCarrierSocketName);
	}
	if (HolsteredSidearmMesh && GetMesh())
	{
		HolsteredSidearmMesh->SetVisibility(CurrentLoadout.bHasHolsteredSidearm);
		HolsteredSidearmMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentLoadout.SidearmSocketName);
	}
}

void AZeroZoneCharacter::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp)
	{
		return;
	}

	if (bIsADS)
	{
		MoveComp->MaxWalkSpeed = CurrentOperatorStats.ADSSpeed;
	}
	else if (bIsCrouched)
	{
		MoveComp->MaxWalkSpeed = CurrentOperatorStats.CrouchSpeed;
		MoveComp->MaxWalkSpeedCrouched = CurrentOperatorStats.CrouchSpeed;
	}
	else if (bIsSprinting)
	{
		MoveComp->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed;
	}
	else
	{
		MoveComp->MaxWalkSpeed = CurrentOperatorStats.WalkSpeed;
	}
}

void AZeroZoneCharacter::UpdateLocomotionState()
{
	EZeroZoneLocomotionState NewState = EZeroZoneLocomotionState::Idle;
	const float Speed2D = GetVelocity().Size2D();

	if (bIsSliding)
	{
		NewState = EZeroZoneLocomotionState::Slide;
	}
	else if (bIsADS)
	{
		NewState = EZeroZoneLocomotionState::ADS;
	}
	else if (bIsCrouched)
	{
		NewState = EZeroZoneLocomotionState::Crouch;
	}
	else if (bIsSprinting && Speed2D > 10.f)
	{
		NewState = EZeroZoneLocomotionState::TacticalSprint;
	}
	else if (Speed2D > 10.f)
	{
		NewState = EZeroZoneLocomotionState::Walk;
	}

	if (NewState != LocomotionState)
	{
		LocomotionState = NewState;
		OnLocomotionStateChanged.Broadcast(LocomotionState);
	}
}

void AZeroZoneCharacter::UpdateCameraAndLean(float DeltaTime)
{
	CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaTime, 12.f);
	const float LeanAlpha = (MaxLeanDegrees > KINDA_SMALL_NUMBER) ? (CurrentLeanAngle / MaxLeanDegrees) : 0.f;

	if (FollowCamera)
	{
		const float TargetFOV = bIsADS ? ADSFOV : DefaultFOV;
		FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, 14.f));
		FollowCamera->SetRelativeRotation(FRotator(0.f, 0.f, CurrentLeanAngle * 0.55f));
	}

	if (CameraBoom)
	{
		const float BaseZ = bIsCrouched ? 44.f : 68.f;
		const float TargetY = (bIsADS ? 32.f : 48.f) + (LeanAlpha * LeanCameraOffset);
		const FVector TargetOffset(0.f, TargetY, BaseZ);
		CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 12.f);
		const float TargetArm = bIsADS ? 120.f : 220.f;
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArm, DeltaTime, 12.f);
	}
}

void AZeroZoneCharacter::UpdateProceduralIK(float DeltaTime)
{
	IKAnimData.LeanAlpha = (MaxLeanDegrees > KINDA_SMALL_NUMBER) ? (CurrentLeanAngle / MaxLeanDegrees) : 0.f;
	IKAnimData.ADSAlpha = FMath::FInterpTo(IKAnimData.ADSAlpha, bIsADS ? 1.f : 0.f, DeltaTime, 14.f);
	IKAnimData.CrouchAlpha = FMath::FInterpTo(IKAnimData.CrouchAlpha, bIsCrouched ? 1.f : 0.f, DeltaTime, 12.f);

	const FRotator ControlRot = GetControlRotation();
	const FRotator DeltaRot = (ControlRot - GetActorRotation()).GetNormalized();
	IKAnimData.AimPitch = FMath::Clamp(DeltaRot.Pitch, -85.f, 85.f);
	IKAnimData.AimYaw = FMath::Clamp(DeltaRot.Yaw, -90.f, 90.f);

	const float LeftTarget = TraceFootIKOffset(TEXT("foot_l"));
	const float RightTarget = TraceFootIKOffset(TEXT("foot_r"));
	IKAnimData.LeftFootIKOffset = FMath::FInterpTo(IKAnimData.LeftFootIKOffset, LeftTarget, DeltaTime, 15.f);
	IKAnimData.RightFootIKOffset = FMath::FInterpTo(IKAnimData.RightFootIKOffset, RightTarget, DeltaTime, 15.f);
	IKAnimData.PelvisZOffset = FMath::Min(IKAnimData.LeftFootIKOffset, IKAnimData.RightFootIKOffset);
}

float AZeroZoneCharacter::TraceFootIKOffset(FName FootSocketName) const
{
	if (!GetWorld() || !GetMesh() || !GetMesh()->DoesSocketExist(FootSocketName))
	{
		return 0.f;
	}

	const FVector FootLoc = GetMesh()->GetSocketLocation(FootSocketName);
	const FVector Start(FootLoc.X, FootLoc.Y, GetActorLocation().Z);
	const FVector End = Start - FVector(0.f, 0.f, 130.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		const float FootGroundZ = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		return FMath::Clamp(Hit.ImpactPoint.Z - FootGroundZ, -35.f, 35.f);
	}
	return 0.f;
}

void AZeroZoneCharacter::ActivateTacticalAbility()
{
	if (bIsDead || bIsAbilityActive || bIsAbilityOnCooldown)
	{
		return;
	}

	if (!HasAuthority())
	{
		ServerActivateTacticalAbility();
		return;
	}

	bIsAbilityActive = true;
	bIsAbilityOnCooldown = true;

	switch (OperatorClass)
	{
	case EZeroZoneOperatorClass::Vanguard:
	{
		const FVector SurgeDir = GetActorForwardVector();
		LaunchCharacter(SurgeDir * 950.f + FVector(0.f, 0.f, 110.f), true, true);
		GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed * 1.25f;
		break;
	}
	case EZeroZoneOperatorClass::Spectre:
	{
		bThermalScannerActive = true;
		if (GetMesh())
		{
			GetMesh()->SetVisibility(false, true);
		}
		GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed * 1.15f;
		break;
	}
	case EZeroZoneOperatorClass::Juggernaut:
	{
		bRiotBarrierActive = true;
		ActiveDamageReductionBonus = 0.55f;
		Shield = FMath::Min(MaxShield, Shield + 45.f);
		OnShieldChanged.Broadcast(Shield, MaxShield);
		break;
	}
	}

	GetWorldTimerManager().SetTimer(
		AbilityDurationTimerHandle,
		this,
		&AZeroZoneCharacter::EndTacticalAbility,
		CurrentOperatorStats.AbilityDuration,
		false
	);

	GetWorldTimerManager().SetTimer(
		AbilityCooldownTimerHandle,
		this,
		&AZeroZoneCharacter::ResetAbilityCooldown,
		CurrentOperatorStats.AbilityCooldown,
		false
	);
}

void AZeroZoneCharacter::ServerActivateTacticalAbility_Implementation()
{
	ActivateTacticalAbility();
}

void AZeroZoneCharacter::EndTacticalAbility()
{
	bIsAbilityActive = false;
	bRiotBarrierActive = false;
	bThermalScannerActive = false;
	ActiveDamageReductionBonus = 0.f;

	if (OperatorClass == EZeroZoneOperatorClass::Spectre && GetMesh())
	{
		GetMesh()->SetVisibility(true, true);
	}

	UpdateMovementSpeed();
}

void AZeroZoneCharacter::ResetAbilityCooldown()
{
	bIsAbilityOnCooldown = false;
}

float AZeroZoneCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority() || bIsDead || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	float HitZoneMultiplier = 1.0f;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (PointEvent)
		{
			const EZeroZoneHitZone Zone = ResolveHitZoneFromBone(PointEvent->HitInfo.BoneName);
			HitZoneMultiplier = GetDamageMultiplierForHitZone(Zone);
		}
	}

	const float TotalReduction = FMath::Clamp(CurrentOperatorStats.BaseDamageReduction + ActiveDamageReductionBonus, 0.f, 0.85f);
	float EffectiveDamage = (DamageAmount * HitZoneMultiplier) * (1.f - TotalReduction);

	if (Shield > 0.f)
	{
		const float ShieldAbsorbed = FMath::Min(Shield, EffectiveDamage);
		Shield -= ShieldAbsorbed;
		EffectiveDamage -= ShieldAbsorbed;
		OnShieldChanged.Broadcast(Shield, MaxShield);
	}

	if (EffectiveDamage > 0.f)
	{
		Health = FMath::Max(0.f, Health - EffectiveDamage);
		OnHealthChanged.Broadcast(Health, MaxHealth);
	}

	if (Health <= 0.f && !bIsDead)
	{
		bIsDead = true;
		StopFire();
		StopADS();
		EndTacticalAbility();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		OnCharacterDied.Broadcast(this);
	}

	return DamageAmount * HitZoneMultiplier;
}

void AZeroZoneCharacter::SyncStateToWebSocket()
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>();
	if (!WS || !WS->IsConnected())
	{
		return;
	}

	FZeroZoneNetworkPlayerState State;
	State.PlayerId = GetPlayerState() ? GetPlayerState()->GetPlayerName() : GetName();
	State.OperatorClass = OperatorClass;
	State.Location = GetActorLocation();
	State.Rotation = GetActorRotation();
	State.Health = Health;
	State.Shield = Shield;
	State.bIsFiring = CombatComponent ? CombatComponent->IsFiring() : false;
	State.bIsAbilityActive = bIsAbilityActive;
	State.WeaponId = (CombatComponent && CombatComponent->GetEquippedWeapon())
		? CombatComponent->GetEquippedWeapon()->GetWeaponStats().WeaponId
		: TEXT("rifle");
	State.bIsADS = bIsADS;
	State.bIsCrouching = bIsCrouched;
	State.LeanAngle = CurrentLeanAngle;

	WS->SendPlayerState(State);
}

void AZeroZoneCharacter::OnRep_OperatorClass()
{
	ApplyOperatorStats(false);
	ApplyOperatorLoadout();
	OnOperatorClassChanged.Broadcast(OperatorClass);
}

void AZeroZoneCharacter::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void AZeroZoneCharacter::OnRep_Shield()
{
	OnShieldChanged.Broadcast(Shield, MaxShield);
}

void AZeroZoneCharacter::OnRep_IsAbilityActive()
{
	if (OperatorClass == EZeroZoneOperatorClass::Spectre && GetMesh())
	{
		GetMesh()->SetVisibility(!bIsAbilityActive, true);
	}
}

void AZeroZoneCharacter::OnRep_IsDead()
{
	if (bIsDead)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		OnCharacterDied.Broadcast(this);
	}
}

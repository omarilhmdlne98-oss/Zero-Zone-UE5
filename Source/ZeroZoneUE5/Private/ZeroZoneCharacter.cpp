// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "ZeroZoneCombatComponent.h"
#include "ZeroZoneWebSocketSubsystem.h"
#include "ZeroZoneUE5.h"

AZeroZoneCharacter::AZeroZoneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 620.f;
	GetCharacterMovement()->AirControl = 0.35f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 260.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

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
	DOREPLIFETIME(AZeroZoneCharacter, bIsAbilityActive);
	DOREPLIFETIME(AZeroZoneCharacter, bIsDead);
}

void AZeroZoneCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyOperatorStats(true);

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

void AZeroZoneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AZeroZoneCharacter::StartSprint()
{
	if (bIsDead)
	{
		return;
	}

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed;
	if (!HasAuthority())
	{
		ServerSetSprinting(true);
	}
}

void AZeroZoneCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.WalkSpeed;
	if (!HasAuthority())
	{
		ServerSetSprinting(false);
	}
}

void AZeroZoneCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	bIsSprinting = bNewSprinting;
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? CurrentOperatorStats.SprintSpeed : CurrentOperatorStats.WalkSpeed;
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
		Stats.MaxHealth = 120.f;
		Stats.MaxShield = 50.f;
		Stats.WalkSpeed = 600.f;
		Stats.SprintSpeed = 860.f;
		Stats.BaseDamageReduction = 0.0f;
		Stats.AbilityCooldown = 8.f;
		Stats.AbilityDuration = 2.5f;
		Stats.AbilityName = TEXT("Phase Surge");
		break;

	case EZeroZoneOperatorClass::Spectre:
		Stats.MaxHealth = 95.f;
		Stats.MaxShield = 40.f;
		Stats.WalkSpeed = 650.f;
		Stats.SprintSpeed = 950.f;
		Stats.BaseDamageReduction = 0.0f;
		Stats.AbilityCooldown = 12.f;
		Stats.AbilityDuration = 4.0f;
		Stats.AbilityName = TEXT("Active Camo");
		break;

	case EZeroZoneOperatorClass::Juggernaut:
		Stats.MaxHealth = 180.f;
		Stats.MaxShield = 120.f;
		Stats.WalkSpeed = 480.f;
		Stats.SprintSpeed = 660.f;
		Stats.BaseDamageReduction = 0.15f;
		Stats.AbilityCooldown = 14.f;
		Stats.AbilityDuration = 5.0f;
		Stats.AbilityName = TEXT("Aegis Fortify");
		break;
	}
	return Stats;
}

void AZeroZoneCharacter::SetOperatorClass(EZeroZoneOperatorClass NewOperatorClass)
{
	if (HasAuthority())
	{
		OperatorClass = NewOperatorClass;
		ApplyOperatorStats(true);
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

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? CurrentOperatorStats.SprintSpeed : CurrentOperatorStats.WalkSpeed;
	}
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
		// Phase Surge: forward impulse + temporary sprint boost
		const FVector SurgeDir = GetActorForwardVector();
		LaunchCharacter(SurgeDir * 950.f + FVector(0.f, 0.f, 120.f), true, true);
		GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed * 1.25f;
		break;
	}
	case EZeroZoneOperatorClass::Spectre:
	{
		// Active Camo: reduce visibility & boost movement speed
		if (GetMesh())
		{
			GetMesh()->SetVisibility(false, true);
		}
		GetCharacterMovement()->MaxWalkSpeed = CurrentOperatorStats.SprintSpeed * 1.15f;
		break;
	}
	case EZeroZoneOperatorClass::Juggernaut:
	{
		// Aegis Fortify: 50% extra damage reduction + partial shield restore
		ActiveDamageReductionBonus = 0.50f;
		Shield = FMath::Min(MaxShield, Shield + 40.f);
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
	ActiveDamageReductionBonus = 0.f;

	if (OperatorClass == EZeroZoneOperatorClass::Spectre && GetMesh())
	{
		GetMesh()->SetVisibility(true, true);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? CurrentOperatorStats.SprintSpeed : CurrentOperatorStats.WalkSpeed;
	}
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

	const float TotalReduction = FMath::Clamp(CurrentOperatorStats.BaseDamageReduction + ActiveDamageReductionBonus, 0.f, 0.85f);
	float EffectiveDamage = DamageAmount * (1.f - TotalReduction);

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
		EndTacticalAbility();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		OnCharacterDied.Broadcast(this);
	}

	return DamageAmount;
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

	WS->SendPlayerState(State);
}

void AZeroZoneCharacter::OnRep_OperatorClass()
{
	ApplyOperatorStats(false);
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

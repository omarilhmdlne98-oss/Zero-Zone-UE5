// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class UZeroZoneCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChangedSignature, float, CurrentShield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOperatorClassChangedSignature, EZeroZoneOperatorClass, NewOperatorClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDiedSignature, AZeroZoneCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocomotionStateChangedSignature, EZeroZoneLocomotionState, NewLocomotionState);

UCLASS()
class ZEROZONEUE5_API AZeroZoneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZeroZoneCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Operator")
	void SetOperatorClass(EZeroZoneOperatorClass NewOperatorClass);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Operator")
	void ActivateTacticalAbility();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	void StartADS();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	void StopADS();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	void ToggleTacticalCrouch();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	void StartSlide();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	bool TryMantleVault();

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Movement")
	void SetLeanInput(float LeanDirection);

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|Combat")
	void ToggleWeaponInspect();

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Combat")
	bool IsInspectingWeapon() const { return bIsInspectingWeapon; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	static FZeroZoneOperatorStats GetOperatorStatsForClass(EZeroZoneOperatorClass ClassType);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	static FZeroZoneTacticalLoadout GetTacticalLoadoutForClass(EZeroZoneOperatorClass ClassType);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Combat")
	static EZeroZoneHitZone ResolveHitZoneFromBone(FName BoneName);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Combat")
	static float GetDamageMultiplierForHitZone(EZeroZoneHitZone HitZone);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	EZeroZoneOperatorClass GetOperatorClass() const { return OperatorClass; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	const FZeroZoneTacticalLoadout& GetTacticalLoadout() const { return CurrentLoadout; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Animation")
	const FZeroZoneIKAnimationData& GetIKAnimationData() const { return IKAnimData; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Movement")
	EZeroZoneLocomotionState GetLocomotionState() const { return LocomotionState; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Movement")
	bool IsADS() const { return bIsADS; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Movement")
	bool IsSliding() const { return bIsSliding; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Movement")
	float GetCurrentLeanAngle() const { return CurrentLeanAngle; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Stats")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Stats")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Stats")
	float GetShield() const { return Shield; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Stats")
	float GetMaxShield() const { return MaxShield; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Stats")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	bool IsAbilityActive() const { return bIsAbilityActive; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Combat")
	UZeroZoneCombatComponent* GetCombatComponent() const { return CombatComponent; }

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Events")
	FOnShieldChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Events")
	FOnOperatorClassChangedSignature OnOperatorClassChanged;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Events")
	FOnCharacterDiedSignature OnCharacterDied;

	UPROPERTY(BlueprintAssignable, Category = "ZeroZone|Events")
	FOnLocomotionStateChangedSignature OnLocomotionStateChanged;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void StartFire();
	void StopFire();
	void ReloadWeapon();
	void StartLeanLeft();
	void StopLeanLeft();
	void StartLeanRight();
	void StopLeanRight();

	void ApplyOperatorStats(bool bResetVitals);
	void ApplyOperatorLoadout();
	void UpdateMovementSpeed();
	void UpdateLocomotionState();
	void UpdateCameraAndLean(float DeltaTime);
	void UpdateProceduralIK(float DeltaTime);
	float TraceFootIKOffset(FName FootSocketName) const;
	void EndSlide();
	void EndTacticalAbility();
	void ResetAbilityCooldown();
	void SyncStateToWebSocket();

	UFUNCTION(Server, Reliable)
	void ServerSetOperatorClass(EZeroZoneOperatorClass NewOperatorClass);

	UFUNCTION(Server, Reliable)
	void ServerActivateTacticalAbility();

	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(bool bNewSprinting);

	UFUNCTION(Server, Reliable)
	void ServerSetADS(bool bNewADS);

	UFUNCTION(Server, Reliable)
	void ServerSetLeanAngle(float NewTargetLean);

	UFUNCTION()
	void OnRep_OperatorClass();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Shield();

	UFUNCTION()
	void OnRep_IsAbilityActive();

	UFUNCTION()
	void OnRep_IsDead();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UZeroZoneCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Equipment")
	TObjectPtr<UStaticMeshComponent> TacticalHelmetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Equipment")
	TObjectPtr<UStaticMeshComponent> PlateCarrierMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Equipment")
	TObjectPtr<UStaticMeshComponent> HolsteredSidearmMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ADSAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LeanLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LeanRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TacticalAbilityAction;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_OperatorClass, BlueprintReadOnly, Category = "ZeroZone|Operator")
	EZeroZoneOperatorClass OperatorClass = EZeroZoneOperatorClass::Vanguard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|Operator")
	FZeroZoneOperatorStats CurrentOperatorStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|Operator")
	FZeroZoneTacticalLoadout CurrentLoadout;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|Animation")
	FZeroZoneIKAnimationData IKAnimData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|Movement")
	EZeroZoneLocomotionState LocomotionState = EZeroZoneLocomotionState::Idle;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float Health = 100.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float Shield = 50.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float MaxShield = 50.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Movement")
	bool bIsSprinting = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Movement")
	bool bIsADS = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Movement")
	bool bIsSliding = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Movement")
	float TargetLeanAngle = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ZeroZone|Movement")
	float CurrentLeanAngle = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_IsAbilityActive, BlueprintReadOnly, Category = "ZeroZone|Operator")
	bool bIsAbilityActive = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "ZeroZone|Stats")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Camera")
	float DefaultFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Camera")
	float ADSFOV = 62.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Camera")
	float MaxLeanDegrees = 16.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Camera")
	float LeanCameraOffset = 28.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Networking")
	float WebSocketSyncInterval = 0.05f;

	bool bIsAbilityOnCooldown = false;
	bool bRiotBarrierActive = false;
	bool bThermalScannerActive = false;
	bool bIsInspectingWeapon = false;
	bool bLeanLeftHeld = false;
	bool bLeanRightHeld = false;
	float ActiveDamageReductionBonus = 0.f;
	float WebSocketSyncAccumulator = 0.f;

	FTimerHandle SlideTimerHandle;
	FTimerHandle AbilityDurationTimerHandle;
	FTimerHandle AbilityCooldownTimerHandle;
};

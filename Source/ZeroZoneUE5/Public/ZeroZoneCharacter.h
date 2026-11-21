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
class UZeroZoneCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChangedSignature, float, CurrentShield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOperatorClassChangedSignature, EZeroZoneOperatorClass, NewOperatorClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDiedSignature, AZeroZoneCharacter*, DeadCharacter);

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

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	static FZeroZoneOperatorStats GetOperatorStatsForClass(EZeroZoneOperatorClass ClassType);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|Operator")
	EZeroZoneOperatorClass GetOperatorClass() const { return OperatorClass; }

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

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void StartFire();
	void StopFire();
	void ReloadWeapon();

	void ApplyOperatorStats(bool bResetVitals);
	void EndTacticalAbility();
	void ResetAbilityCooldown();
	void SyncStateToWebSocket();

	UFUNCTION(Server, Reliable)
	void ServerSetOperatorClass(EZeroZoneOperatorClass NewOperatorClass);

	UFUNCTION(Server, Reliable)
	void ServerActivateTacticalAbility();

	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(bool bNewSprinting);

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
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TacticalAbilityAction;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_OperatorClass, BlueprintReadOnly, Category = "ZeroZone|Operator")
	EZeroZoneOperatorClass OperatorClass = EZeroZoneOperatorClass::Vanguard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|Operator")
	FZeroZoneOperatorStats CurrentOperatorStats;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float Health = 120.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float MaxHealth = 120.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float Shield = 50.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Stats")
	float MaxShield = 50.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ZeroZone|Movement")
	bool bIsSprinting = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsAbilityActive, BlueprintReadOnly, Category = "ZeroZone|Operator")
	bool bIsAbilityActive = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "ZeroZone|Stats")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|Networking")
	float WebSocketSyncInterval = 0.05f;

	bool bIsAbilityOnCooldown = false;
	float ActiveDamageReductionBonus = 0.f;
	float WebSocketSyncAccumulator = 0.f;

	FTimerHandle AbilityDurationTimerHandle;
	FTimerHandle AbilityCooldownTimerHandle;
};

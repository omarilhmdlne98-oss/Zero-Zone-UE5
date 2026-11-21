// Copyright Zero-Zone. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZeroZoneTypes.h"
#include "ZeroZoneAIController.generated.h"

class AZeroZoneCharacter;

UENUM(BlueprintType)
enum class EZeroZoneAITacticalState : uint8
{
	Patrol			UMETA(DisplayName = "Patrol"),
	CombatStrafe	UMETA(DisplayName = "Combat Strafe"),
	TakeCover		UMETA(DisplayName = "Take Cover"),
	Flank			UMETA(DisplayName = "Flank")
};

UCLASS()
class ZEROZONEUE5_API AZeroZoneAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZeroZoneAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable, Category = "ZeroZone|AI")
	void NotifyGunshotHeard(const FVector& SoundLocation, AActor* SoundInstigator);

	UFUNCTION(BlueprintPure, Category = "ZeroZone|AI")
	EZeroZoneAITacticalState GetTacticalState() const { return TacticalState; }

	UFUNCTION(BlueprintPure, Category = "ZeroZone|AI")
	bool HasLineOfSightToTarget(const AActor* TargetActor) const;

protected:
	void EvaluateTacticalAI();
	AZeroZoneCharacter* AcquireBestTarget() const;
	FVector ComputeCoverLocation(const AActor* ThreatActor) const;
	FVector ComputeFlankLocation(const AActor* ThreatActor) const;
	void UpdateCombatBehavior(AZeroZoneCharacter* ControlledChar, AZeroZoneCharacter* TargetChar);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|AI|Perception")
	float SightRange = 3200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|AI|Perception")
	float SightHalfAngleDegrees = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|AI|Perception")
	float HearingRadius = 3600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|AI|Tactics")
	float CoverHealthThresholdRatio = 0.40f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZeroZone|AI|Tactics")
	float DecisionInterval = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ZeroZone|AI")
	EZeroZoneAITacticalState TacticalState = EZeroZoneAITacticalState::Patrol;

	UPROPERTY()
	TWeakObjectPtr<AZeroZoneCharacter> CurrentTarget;

	FVector LastHeardGunshotLocation = FVector::ZeroVector;
	bool bHasInvestigatePoint = false;
	float StrafeDirection = 1.f;
	FTimerHandle DecisionTimerHandle;
};

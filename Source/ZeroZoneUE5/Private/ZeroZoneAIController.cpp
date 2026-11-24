// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZoneAIController.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneCombatComponent.h"
#include "ZeroZoneWeapon.h"

AZeroZoneAIController::AZeroZoneAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AZeroZoneAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StrafeDirection = FMath::RandBool() ? 1.f : -1.f;
	GetWorldTimerManager().SetTimer(
		DecisionTimerHandle,
		this,
		&AZeroZoneAIController::EvaluateTacticalAI,
		DecisionInterval,
		true
	);
}

void AZeroZoneAIController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(DecisionTimerHandle);
	CurrentTarget.Reset();
	Super::OnUnPossess();
}

void AZeroZoneAIController::NotifyGunshotHeard(const FVector& SoundLocation, AActor* SoundInstigator)
{
	APawn* Controlled = GetPawn();
	if (!Controlled || SoundInstigator == Controlled)
	{
		return;
	}

	const float Dist = FVector::Dist(Controlled->GetActorLocation(), SoundLocation);
	if (Dist <= HearingRadius)
	{
		LastHeardGunshotLocation = SoundLocation;
		bHasInvestigatePoint = true;
		if (AZeroZoneCharacter* Shooter = Cast<AZeroZoneCharacter>(SoundInstigator))
		{
			if (!Shooter->IsDead())
			{
				CurrentTarget = Shooter;
			}
		}
	}
}

bool AZeroZoneAIController::HasLineOfSightToTarget(const AActor* TargetActor) const
{
	const APawn* Controlled = GetPawn();
	if (!Controlled || !TargetActor || !GetWorld())
	{
		return false;
	}

	const FVector EyeStart = Controlled->GetActorLocation() + FVector(0.f, 0.f, 65.f);
	const FVector TargetLoc = TargetActor->GetActorLocation() + FVector(0.f, 0.f, 55.f);
	const FVector ToTarget = TargetLoc - EyeStart;
	const float Distance = ToTarget.Size();
	if (Distance > SightRange)
	{
		return false;
	}

	const FVector Forward = Controlled->GetActorForwardVector();
	const float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(SightHalfAngleDegrees));
	if (FVector::DotProduct(Forward, ToTarget.GetSafeNormal()) < CosHalfAngle)
	{
		return false;
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Controlled);
	Params.AddIgnoredActor(TargetActor);

	FHitResult Hit;
	return !GetWorld()->LineTraceSingleByChannel(Hit, EyeStart, TargetLoc, ECC_Visibility, Params);
}

AZeroZoneCharacter* AZeroZoneAIController::AcquireBestTarget() const
{
	const APawn* Controlled = GetPawn();
	if (!Controlled || !GetWorld())
	{
		return nullptr;
	}

	AZeroZoneCharacter* BestTarget = nullptr;
	float BestDist = SightRange;

	for (TActorIterator<AZeroZoneCharacter> It(GetWorld()); It; ++It)
	{
		AZeroZoneCharacter* Candidate = *It;
		if (!Candidate || Candidate == Controlled || Candidate->IsDead())
		{
			continue;
		}
		if (Candidate->GetOperatorClass() == EZeroZoneOperatorClass::Spectre && Candidate->IsAbilityActive())
		{
			continue;
		}

		const float Dist = FVector::Dist(Controlled->GetActorLocation(), Candidate->GetActorLocation());
		if (Dist < BestDist && HasLineOfSightToTarget(Candidate))
		{
			BestDist = Dist;
			BestTarget = Candidate;
		}
	}

	return BestTarget;
}

FVector AZeroZoneAIController::ComputeCoverLocation(const AActor* ThreatActor) const
{
	const APawn* Controlled = GetPawn();
	if (!Controlled || !ThreatActor)
	{
		return FVector::ZeroVector;
	}

	const FVector AwayDir = (Controlled->GetActorLocation() - ThreatActor->GetActorLocation()).GetSafeNormal2D();
	const FVector LateralDir = FVector(-AwayDir.Y, AwayDir.X, 0.f) * StrafeDirection;
	return Controlled->GetActorLocation() + (AwayDir * 420.f) + (LateralDir * 240.f);
}

FVector AZeroZoneAIController::ComputeFlankLocation(const AActor* ThreatActor) const
{
	const APawn* Controlled = GetPawn();
	if (!Controlled || !ThreatActor)
	{
		return FVector::ZeroVector;
	}

	const FVector ToThreat = (ThreatActor->GetActorLocation() - Controlled->GetActorLocation()).GetSafeNormal2D();
	const FVector SideDir = FVector(-ToThreat.Y, ToThreat.X, 0.f) * StrafeDirection;
	return ThreatActor->GetActorLocation() + (SideDir * 520.f) - (ToThreat * 260.f);
}

void AZeroZoneAIController::EvaluateTacticalAI()
{
	AZeroZoneCharacter* ControlledChar = Cast<AZeroZoneCharacter>(GetPawn());
	if (!ControlledChar || ControlledChar->IsDead())
	{
		return;
	}

	AZeroZoneCharacter* TargetChar = CurrentTarget.Get();
	if (!TargetChar || TargetChar->IsDead() || !HasLineOfSightToTarget(TargetChar))
	{
		TargetChar = AcquireBestTarget();
		CurrentTarget = TargetChar;
	}

	if (TargetChar)
	{
		bHasInvestigatePoint = false;
		UpdateCombatBehavior(ControlledChar, TargetChar);
		return;
	}

	if (UZeroZoneCombatComponent* Combat = ControlledChar->GetCombatComponent())
	{
		Combat->StopFire();
	}
	ControlledChar->StopADS();
	TacticalState = EZeroZoneAITacticalState::Patrol;

	if (bHasInvestigatePoint)
	{
		MoveToLocation(LastHeardGunshotLocation, 120.f);
		if (FVector::Dist2D(ControlledChar->GetActorLocation(), LastHeardGunshotLocation) < 160.f)
		{
			bHasInvestigatePoint = false;
		}
	}
}

void AZeroZoneAIController::UpdateCombatBehavior(AZeroZoneCharacter* ControlledChar, AZeroZoneCharacter* TargetChar)
{
	const float HealthRatio = (ControlledChar->GetMaxHealth() > 0.f)
		? (ControlledChar->GetHealth() / ControlledChar->GetMaxHealth())
		: 1.f;
	const bool bLowVitals = (ControlledChar->GetShield() <= 0.f) || (HealthRatio < CoverHealthThresholdRatio);

	SetFocus(TargetChar);

	if (bLowVitals)
	{
		TacticalState = EZeroZoneAITacticalState::TakeCover;
		ControlledChar->ActivateTacticalAbility();
		if (!ControlledChar->bIsCrouched)
		{
			ControlledChar->ToggleTacticalCrouch();
		}
		MoveToLocation(ComputeCoverLocation(TargetChar), 60.f);
	}
	else
	{
		const float Dist = FVector::Dist(ControlledChar->GetActorLocation(), TargetChar->GetActorLocation());
		if (Dist > 1600.f)
		{
			TacticalState = EZeroZoneAITacticalState::Flank;
			MoveToLocation(ComputeFlankLocation(TargetChar), 90.f);
		}
		else
		{
			TacticalState = EZeroZoneAITacticalState::CombatStrafe;
			if (FMath::FRand() < 0.2f)
			{
				StrafeDirection *= -1.f;
			}
			const FVector StrafeDest = ControlledChar->GetActorLocation() + ControlledChar->GetActorRightVector() * (StrafeDirection * 240.f);
			MoveToLocation(StrafeDest, 45.f);
		}

		if (FMath::FRand() < 0.15f)
		{
			ControlledChar->ActivateTacticalAbility();
		}
	}

	if (UZeroZoneCombatComponent* Combat = ControlledChar->GetCombatComponent())
	{
		ControlledChar->StartADS();
		AZeroZoneWeapon* EquippedWeapon = Combat->GetEquippedWeapon();
		if (EquippedWeapon && !EquippedWeapon->GetWeaponStats().bIsAutomatic)
		{
			Combat->StopFire();
		}
		Combat->StartFire();
	}
}

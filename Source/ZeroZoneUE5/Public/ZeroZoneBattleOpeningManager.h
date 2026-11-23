#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroZoneBattleOpeningManager.generated.h"

class UNiagaraSystem;
class UStaticMeshComponent;
class UPointLightComponent;
class USoundBase;
class UAudioComponent;
UCLASS(Blueprintable)
class ZEROZONEUE5_API AZeroZoneBattleOpeningManager : public AActor
{
    GENERATED_BODY()
public:
    AZeroZoneBattleOpeningManager();
    virtual void Tick(float DeltaSeconds) override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Opening") TObjectPtr<UStaticMeshComponent> Aircraft;
    UPROPERTY(VisibleAnywhere, Category="Opening") TObjectPtr<UStaticMeshComponent> Wings;
    UPROPERTY(VisibleAnywhere, Category="Opening") TObjectPtr<UPointLightComponent> HorizonFlash;
    UPROPERTY(EditAnywhere, Category="Opening") TObjectPtr<UNiagaraSystem> SmokeTrail;
    UPROPERTY(EditAnywhere, Category="Opening") TObjectPtr<UNiagaraSystem> HorizonBurst;
    UPROPERTY(EditAnywhere, Category="Opening") TObjectPtr<USoundBase> Siren;
    UPROPERTY(EditAnywhere, Category="Opening") TObjectPtr<USoundBase> SonicBoom;
private:
    void PlayCue(bool bSiren);
    UPROPERTY(Transient) TObjectPtr<UAudioComponent> Voice;
    float Clock = 0.f;
    int32 Beat = 0;
    bool bInMatch = false;
};

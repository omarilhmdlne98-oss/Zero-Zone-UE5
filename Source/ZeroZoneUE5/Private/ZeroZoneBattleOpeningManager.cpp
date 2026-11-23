#include "ZeroZoneBattleOpeningManager.h"
#include "ZeroZoneArenaGameState.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundWaveProcedural.h"
#include "UObject/ConstructorHelpers.h"

AZeroZoneBattleOpeningManager::AZeroZoneBattleOpeningManager()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Stage")));
    Aircraft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aircraft"));
    Wings = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wings"));
    Aircraft->SetupAttachment(RootComponent); Wings->SetupAttachment(Aircraft);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Shape(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    for (UStaticMeshComponent* M : { Aircraft.Get(), Wings.Get() })
    {
        M->SetStaticMesh(Shape.Object); M->SetCollisionEnabled(ECollisionEnabled::NoCollision); M->SetGenerateOverlapEvents(false);
    }
    Aircraft->SetRelativeScale3D(FVector(9,1,1)); Wings->SetRelativeScale3D(FVector(.18f,9,.35f));
    Aircraft->SetVisibility(false,true);
    HorizonFlash = CreateDefaultSubobject<UPointLightComponent>(TEXT("HorizonGlow"));
    HorizonFlash->SetupAttachment(RootComponent); HorizonFlash->SetRelativeLocation(FVector(14000,8500,1500));
    HorizonFlash->SetLightColor(FLinearColor(1,.46f,.13f)); HorizonFlash->SetAttenuationRadius(9000); HorizonFlash->SetIntensity(0);
}
void AZeroZoneBattleOpeningManager::PlayCue(bool bSiren)
{
    if (Voice) Voice->Stop();
    USoundBase* Cue = bSiren ? Siren.Get() : SonicBoom.Get();
    if (!Cue)
    {
        constexpr int32 Rate = 22050; const int32 Count = bSiren ? 3*Rate : Rate;
        auto* Wave = NewObject<USoundWaveProcedural>(this); Wave->SetSampleRate(Rate); Wave->NumChannels=1;
        TArray<int16> Samples; Samples.SetNumUninitialized(Count); float Phase=0; FRandomStream Noise(101);
        for (int32 I=0; I<Count; ++I)
        {
            float T=float(I)/Rate;
            Phase += 2.f*PI*(bSiren ? 420.f+115.f*FMath::Sin(3.f*T) : 62.f)/Rate;
            float Envelope=bSiren ? FMath::Sin(PI*I/Count) : FMath::Exp(-6.f*T);
            Samples[I]=int16(5000*Envelope*(bSiren ? FMath::Sin(Phase) : .6f*FMath::Sin(Phase)+.4f*Noise.FRandRange(-1,1)));
        }
        Wave->QueueAudio(reinterpret_cast<const uint8*>(Samples.GetData()),Count*sizeof(int16)); Cue=Wave;
    }
    Voice=UGameplayStatics::SpawnSound2D(this,Cue,.45f);
}
void AZeroZoneBattleOpeningManager::Tick(float Dt)
{
    Super::Tick(Dt);
    auto* GS=GetWorld()->GetGameState<AZeroZoneArenaGameState>();
    if (!GS || GS->GetMatchPhase()!=EZeroZoneMatchPhase::InProgress)
    {
        if (bInMatch) { Aircraft->SetVisibility(false,true); HorizonFlash->SetIntensity(0); if (Voice) Voice->Stop(); }
        bInMatch=false; return;
    }
    if (!bInMatch) { bInMatch=true; Clock=0; Beat=0; PlayCue(true); }
    if (Clock>=9.f) return;
    const float Previous=Clock; Clock+=Dt;
    if (Beat<3 && Clock>=.6f+Beat*.9f)
    {
        HorizonFlash->SetIntensity(1500000.f);
        if (HorizonBurst) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,HorizonBurst,HorizonFlash->GetComponentLocation());
        ++Beat;
    }
    else HorizonFlash->SetIntensity(HorizonFlash->Intensity*FMath::Exp(-10.f*Dt));
    if (Clock>=3.f && Previous<3.f)
    {
        Aircraft->SetRelativeLocation(FVector(-18000,4500,3400)); Aircraft->SetVisibility(true,true);
        if (SmokeTrail) UNiagaraFunctionLibrary::SpawnSystemAttached(SmokeTrail,Aircraft,NAME_None,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true);
    }
    if (Clock>=3.f) Aircraft->SetRelativeLocation(FVector(FMath::Lerp(-18000.f,18000.f,FMath::Clamp((Clock-3.f)/5.f,0.f,1.f)),4500,3400));
    if (Clock>=5.6f && Previous<5.6f) PlayCue(false);
    if (Clock>=8.f && Previous<8.f) { Aircraft->SetVisibility(false,true); if (Voice) Voice->Stop(); HorizonFlash->SetIntensity(0); }
}
void AZeroZoneBattleOpeningManager::EndPlay(const EEndPlayReason::Type Reason)
{
    if (Voice) Voice->Stop(); Super::EndPlay(Reason);
}

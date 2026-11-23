#include "ZeroZoneTheaterLibrary.h"
#include "Materials/MaterialInterface.h"
#if WITH_EDITOR
#include "Editor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Landscape.h"
#include "LandscapeInfo.h"
#endif

namespace { constexpr int32 Quads = 1008; constexpr float Pitch = 400.f; }
FZeroZoneTheaterSector UZeroZoneTheaterLibrary::DescribeTheater(bool bHighlands)
{
    FZeroZoneTheaterSector S;
    S.SectorName = bHighlands ? TEXT("Chalk Reach") : TEXT("Amber Canal");
    S.MapPackage = bHighlands ? FName(TEXT("/Game/Theaters/ChalkReach")) : FName(TEXT("/Game/Theaters/AmberCanal"));
    S.LatitudeDeg = S.LongitudeDeg = 0.; // Not geographic coordinates; see bGeoreferenced.
    S.DemGridSize = Quads + 1; S.DataSource = TEXT("Procedural studio landscape v1; not surveyed DEM");
    S.MinElevationM = bHighlands ? 0.f : -4.f; S.MaxElevationM = bHighlands ? 165.f : 18.f;
    return S;
}

float UZeroZoneTheaterLibrary::TheaterHeightCm(bool bHighlands, float X, float Y)
{
    const float N = FMath::PerlinNoise2D(FVector2D(X,Y) / 41000.f);
    if (bHighlands) return 7800.f + 5000.f*FMath::Sin(X/47000.f + 1.8f*N) + 2300.f*N + 800.f*FMath::Cos(Y/9000.f);
    const float Edge = FMath::SmoothStep(3500.f, 9000.f, FMath::Abs(X));
    return FMath::Lerp(-400.f, 850.f + 490.f*N + 380.f*FMath::Sin((X+Y*.32f)/3300.f), Edge);
}
bool UZeroZoneTheaterLibrary::BakeTheaterLandscape(bool bHighlands, UZeroZoneTheaterPalette* Palette)
{
#if WITH_EDITOR
    UWorld* W = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!W || !W->GetWorldPartition() || W->WorldType != EWorldType::Editor) return false;
    for (TActorIterator<ALandscape> It(W); It; ++It) return false;
    const FVector Origin(-Quads*Pitch*.5f,-Quads*Pitch*.5f,0);
    ALandscape* L = W->SpawnActor<ALandscape>(Origin, FRotator::ZeroRotator);
    if (!L) return false;
    L->SetActorScale3D(FVector(Pitch,Pitch,100));
    if (Palette) L->LandscapeMaterial = bHighlands ? Palette->FracturedLimestone : Palette->WindRippledSand;
    TMap<FGuid,TArray<uint16>> Heights;
    TArray<uint16>& H = Heights.Add(FGuid()); H.SetNumUninitialized((Quads+1)*(Quads+1));
    for (int32 Y=0; Y<=Quads; ++Y) for (int32 X=0; X<=Quads; ++X)
    {
        H[Y*(Quads+1)+X] = uint16(FMath::Clamp(FMath::RoundToInt(32768.f + TheaterHeightCm(bHighlands,Origin.X+X*Pitch,Origin.Y+Y*Pitch)*1.28f),0,65535));
    }
    TMap<FGuid,TArray<FLandscapeImportLayerInfo>> Layers; Layers.Add(FGuid());
    L->Import(FGuid::NewGuid(),0,0,Quads,Quads,1,63,Heights,nullptr,Layers,ELandscapeImportAlphamapType::Additive);
    L->RegisterAllComponents(); L->PostEditChange(); L->MarkPackageDirty();
    return L->GetLandscapeInfo() != nullptr && L->LandscapeComponents.Num() == 256;
#else
    return false;
#endif
}

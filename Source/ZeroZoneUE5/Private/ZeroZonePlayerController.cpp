// Copyright Zero-Zone. All Rights Reserved.

#include "ZeroZonePlayerController.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerState.h"
#include "ZeroZoneCharacter.h"
#include "ZeroZoneWebSocketSubsystem.h"

void AZeroZonePlayerController::RequestSelectOperatorClass(EZeroZoneOperatorClass NewOperatorClass)
{
	if (AZeroZoneCharacter* ZeroZoneChar = Cast<AZeroZoneCharacter>(GetPawn()))
	{
		ZeroZoneChar->SetOperatorClass(NewOperatorClass);
	}

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			const FString PlayerId = PlayerState ? PlayerState->GetPlayerName() : GetName();
			WS->SendOperatorSelection(PlayerId, NewOperatorClass);
		}
	}
}

void AZeroZonePlayerController::ConnectToMultiplayerServer(const FString& WebSocketUrl, const FString& AuthToken)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UZeroZoneWebSocketSubsystem* WS = GI->GetSubsystem<UZeroZoneWebSocketSubsystem>())
		{
			WS->Connect(WebSocketUrl, AuthToken);
		}
	}
}

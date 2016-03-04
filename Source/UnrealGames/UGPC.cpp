// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGPC.h"
#include "UGGM.h"

void AUGPC::InitPlayerState()
{
	if (GetNetMode() != NM_Client)
	{
		UWorld* const World = GetWorld();
		AUGGM* const GameMode = World ? Cast<AUGGM>(World->GetAuthGameMode()) : NULL;
		if (GameMode != NULL)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.Instigator = Instigator;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnInfo.ObjectFlags |= RF_Transient;	// We never want player states to save into a map
			PlayerState = World->SpawnActor<APlayerState>(GameMode->GetPlayerStateClass(), SpawnInfo);

			// force a default player name if necessary
			if (PlayerState && PlayerState->PlayerName.IsEmpty())
			{
				// don't call SetPlayerName() as that will broadcast entry messages but the GameMode hasn't had a chance
				// to potentially apply a player/bot name yet
				PlayerState->PlayerName = GameMode->DefaultPlayerName.ToString();
			}
		}
		else
		{
			Super::InitPlayerState();
		}
	}
}

void AUGPC::NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest)
{
	Super::NotifyLoadedWorld(WorldPackageName, bFinalDest);

	if (bFinalDest)
	{
		OnEndSeamlessTravel();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGGM.h"
#include "UGGame.h"
#include "UGGI.h"
#include "UGPS.h"


AUGGM::AUGGM(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	auto World = GetWorld();

	if (IsValid(World))
	{
		UE_LOG(LogTemp, Log, TEXT("AUGGM spawned!"));
		auto GI = Cast<UUGGI>(World->GetGameInstance());

		if (IsValid(GI))
		{
			UE_LOG(LogTemp, Log, TEXT("AUGGM has GameInstance!"));

			if (IsValid(GI->Game))
			{
				UE_LOG(LogTemp, Log, TEXT("AUGGM has game!"));
				SetGame(GI->Game);
				UE_LOG(LogTemp, Log, TEXT("    - %s"), *Game->GameName.ToString());
			}
		}
	}
}

void AUGGM::GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToEntry, ActorList);
	ActorList.Add(Game);
}

AUGGame* AUGGM::GetGame()
{
	/* // Redundant
	if (!IsValid(Game))
	{
		auto GI = Cast<UUGGI>(GetGameInstance());

		if (IsValid(GI)) Game = GI->Game;
	}
	*/
	return Game;
}

void AUGGM::SetGame(class AUGGame* _Game)
{
	Game = _Game;

	if (IsValid(Game))
		PlayerStateClass = Game->PlayerStateClass;
}
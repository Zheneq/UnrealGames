// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGGM.h"
#include "UGGame.h"
#include "UGGI.h"
#include "UGPS.h"
#include "UGGS.h"
#include "UGPC.h"
#include "EngineUtils.h"


AUGGM::AUGGM(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	GameStateClass = AUGGS::StaticClass();
	PlayerControllerClass = AUGPC::StaticClass();
	PlayerStateClass = AUGPS::StaticClass();


	auto World = GetWorld();

	// Not looking for the game if we are in the editor
	if (!IsValid(World) || !(World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE))
		return;

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

void AUGGM::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();
	
	if (!IsValid(Game) && World)
	{
		for (TActorIterator<AUGGame> GameIt(World); GameIt; ++GameIt)
		{
			SetGame(*GameIt);
			UE_LOG(LogTemp, Log, TEXT("AUGGM found game in the world!"));
			break;
		}
	}

	SetGame(Game);
}

TSubclassOf<APlayerState> AUGGM::GetPlayerStateClass()
{
	if (IsValid(Game))
		return Game->PlayerStateClass;

	return PlayerStateClass;
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
	{
		PlayerStateClass = Game->PlayerStateClass;
	}

	UE_LOG(LogTemp, Log, TEXT("AUGGM::SetGame: New game is %s."), IsValid(Game) ? *Game->GameName.ToString() : TEXT("None"));
}

bool AUGGM::HasEverybodyConnected()
{
	if (!IsValid(Game)) return false;

	UE_LOG(LogTemp, Log, TEXT("UGGM::HasEverybodyConnected: %d/%d"), NumPlayers, Game->Players.Num());

	return NumPlayers >= Game->Players.Num();
}

void AUGGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUGPS* PS = Cast<AUGPS>(NewPlayer->PlayerState);

	if (PS && IsValid(Game))
	{
		Game->NewPlayer(PS);
	}
}

void AUGGM::AddBoundActor(AActor* Actor)
{
	if(IsValid(Actor))
		BoundActors.AddUnique(Actor);
}

void AUGGM::AddBoundActors(TArray<AActor*> Actors)
{
	for (auto a : Actors)
		AddBoundActor(a);
}
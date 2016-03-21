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
			UE_LOG(LogTemp, Log, TEXT("    - %s"), *GetGame()->GameName.ToString());
		}
	}
}

void AUGGM::BeginPlay()
{
	Super::BeginPlay();

	FindGame();
}

void AUGGM::FindGame()
{
	auto World = GetWorld();

	if (!IsValid(GetGame()) && World)
	{
		for (TActorIterator<AUGGame> GameIt(World); GameIt; ++GameIt)
		{
			SetGame(*GameIt);
			UE_LOG(LogTemp, Log, TEXT("AUGGM found game in the world!"));
			break;
		}
	}

	SetGame(GetGame()); //??!
}

TSubclassOf<APlayerState> AUGGM::GetPlayerStateClass()
{
	if (!IsValid(GetGame()))
		FindGame();

	//if (IsValid(GetGame()))
	//	return GetGame()->PlayerStateClass;

	return PlayerStateClass;
}

void AUGGM::GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToEntry, ActorList);
	ActorList.Add(GetGame());
}

AUGGame* AUGGM::GetGame()
{
	auto gs = Cast<AUGGS>(GameState);
	if (IsValid(gs))
	{
		return gs->GetGame();
	}
	return nullptr;
}

void AUGGM::SetGame(class AUGGame* _Game)
{
	auto gs = Cast<AUGGS>(GameState);
	if (IsValid(gs))
	{
		gs->SetGame(_Game);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGGM::Cannot set Game: GameState is invalid!"));
	}
}

bool AUGGM::HasEverybodyConnected()
{
	if (!IsValid(GetGame())) return false;
	/*
	UE_LOG(LogTemp, Log, TEXT("UGGM::HasEverybodyConnected: %d/%d"), NumPlayers, Game->Players.Num());

	return NumPlayers >= Game->Players.Num();
	*/

	UE_LOG(LogTemp, Log, TEXT("UGGM::HasEverybodyConnected: %d more player(s) to arrive."), NumTravellingPlayers);
	return !NumTravellingPlayers;
}

void AUGGM::PreLogin(const FString & Options, const FString & Address, const TSharedPtr< const FUniqueNetId > & UniqueId, FString & ErrorMessage)
{
	auto Game = GetGame();
	// Reject if game is already up and running
	if (IsValid(Game) && Game->bIsInGame)
	{
		ErrorMessage = TEXT("The game has already started.");
	}
}

void AUGGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUGPS* PS = Cast<AUGPS>(NewPlayer->PlayerState);

	auto Game = GetGame();
	if (IsValid(PS) && IsValid(Game))
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
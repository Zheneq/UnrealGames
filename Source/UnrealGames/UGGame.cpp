// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGGame.h"
#include "UGPS.h"
#include "UGGS.h"
#include "UGGM.h"
#include "UGGI.h"
#include "UGSettingsComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AUGGame::AUGGame():
	Super()
{
 	// Set this actor not to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CurPlayerIndex = 0;
	Lap = 0;
	bRoundOver = false;
	bIsInGame = false;
	bGameHasStarted = false;
	//Settings = CreateDefaultSubobject<UUGSettingsComponent>(TEXT("SettingsComp"));
	Settings = nullptr;

	GameName = FText::FromString(TEXT("Unknown Game"));
	MaxPlayers = 2;
	PlayerStateClass = AUGPS::StaticClass();
}

void AUGGame::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGGame, Lap);
	DOREPLIFETIME_CONDITION(AUGGame, GameName, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AUGGame, MaxPlayers, COND_InitialOnly);
	DOREPLIFETIME(AUGGame, bIsInGame);
	DOREPLIFETIME(AUGGame, bRoundOver);

}

void AUGGame::OnConstruction(const FTransform& Transform)
{
	Settings = FindComponentByClass<UUGSettingsComponent>();

	auto GI = GetWorld() ? Cast<UUGGI>(GetWorld()->GetGameInstance()) : nullptr;
	if (GI && !GI->Game) GI->StoreUGGame(this);

	Super::OnConstruction(Transform);
}

void AUGGame::BeginPlay()
{
	Super::BeginPlay();

	// Collect all players that are already in game
	auto GS = GetWorld() ? GetWorld()->GetGameState() : nullptr;
	if (GS)
	{
		for (auto PS : GS->PlayerArray)
		{
			auto player = Cast<AUGPS>(PS);
			if (player)
			{
				Players.AddUnique(player);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UGGame::BeginPlay: Wrong player state (%s)"), PS ? *PS->GetClass()->GetName() : TEXT(""));
			}
		}
	}


}

TArray<AActor*> AUGGame::GetBoundActorsByTag(FName Tag)
{
	auto GM = GetWorld() ? Cast<AUGGM>(GetWorld()->GetAuthGameMode()) : nullptr;
	TArray<AActor*> res;

	if (GM)
	{
		for (auto a : GM->BoundActors)
		{
			if (a->GetComponentsByTag(UActorComponent::StaticClass(), Tag).Num() > 0)
			{
				res.Add(a);
			}
		}
	}

	return res;
}

AActor* AUGGame::GetBoundActorByTag(FName Tag)
{
	TArray<AActor*> res = GetBoundActorsByTag(Tag);
	return res.Num() ? res[0] : nullptr;
}

AUGPS* AUGGame::GetCurPlayer()
{
	return Players[CurPlayerIndex];
}

int32 AUGGame::GetInGamePlayersCount()
{
	int32 count = 0;

	for (auto p : Players)
	{
		if (p->bIsInGame) count++;
	}

	return count;
}

void AUGGame::IncrementPlayerIndex()
{
	CurPlayerIndex = (CurPlayerIndex + 1) % Players.Num();

	if (GetInGamePlayersCount())
	{
		while (!GetCurPlayer()->bIsInGame)
			CurPlayerIndex = (CurPlayerIndex + 1) % Players.Num();
	}

	UE_LOG(LogTemp, Log, TEXT("UGGame::IncrementPlayerIndex"));
}


void AUGGame::StartGame()
{
	if (bGameHasStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGGame::StartGame: Game has already started!"));
		return;
	}


	// Checking players array for corrupted data
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		if (!IsValid(Players[i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("UGGame::StartGame: Invalid player in players array (%s)."), *GetNameSafe(Players[i]));
			Players.RemoveAt(i);

			--i;
		}
	}
	
	if (Players.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGGame::StartGame: No players! Aborting."));
		return;
	}

	bGameHasStarted = true;

	SortPlayers();

	// Set up player chain
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		Players[i]->Score = 0;
		Players[i]->Next = Players[(i + 1) % Players.Num()];
	}

	UE_LOG(LogTemp, Log, TEXT("UGGame::StartGame: Initialized %d players."), Players.Num());

	InitGame();

	bIsInGame = true;

	ResetRound();
}

void AUGGame::ResetRound()
{
	bRoundOver = false;

	InitRound();
	UE_LOG(LogTemp, Log, TEXT("UGGame::ResetRound: %d players in game."), Players.Num());
	for (auto player : Players)
	{
		player->bIsInGame = true;
		InitPlayer(player);
	}

	auto GS = Cast<AUGGS>(GetWorld()->GetGameState());
	if(GS)
		GS->bIsInGame = true;

	StartTurn();
}

void AUGGame::StartTurn()
{
	// Triggering New Lap event in case the game needs to handle it
	if (CurPlayerIndex == 0)
	{
		Lap++;
		NewLap();
	}

	GetCurPlayer()->bMyTurn = true;

	Turn();
}

void AUGGame::EndTurn()
{
	GetCurPlayer()->bMyTurn = false;
	IncrementPlayerIndex();

	// if game doess not end, start new round
	if (!CheckEndRound())
	{
		StartTurn();
	}
}

void AUGGame::EndRound(TArray<AUGPS*> Winners)
{
	if (Winners.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("UGGame::EndRound: Round ends, we have %d winner(s)!"), Winners.Num());
		for (auto w : Winners)
		{
			w->ScoreObjective();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UGGame::EndRound: Round ends, we have no winners!"));
	}

	if (!CheckEndGame())
	{
		FTimerHandle timer;
		GetWorldTimerManager().SetTimer(timer, this, &AUGGame::ResetRound, 3.0f, false);
	}
	else
	{
		bIsInGame = false;
		GameOver();
	}

}

void AUGGame::LoseRound(AUGPS* Loser)
{
	if (!IsValid(Loser)) return;

	if (Loser->bIsInGame && Players.Find(Loser) != INDEX_NONE)
	{
		Loser->bIsInGame = false;
		Lost(Loser);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGGame::LoseRound: %s is not in game!"), *Loser->GetPlayerName());
	}
}

bool AUGGame::KickPlayer(AUGPS* Player)
{
	if (!IsValid(Player)) return false;

	int32 Index = Players.Find(Player);
	LoseRound(Player);

	if (Index != INDEX_NONE)
	{
		AUGPS* CurPlayer = GetCurPlayer();
		if (Index == CurPlayerIndex)
		{
			IncrementPlayerIndex();
		}

		// Fixing player chain
		Players[(Index + Players.Num() - 1) % Players.Num()]->Next = Players[(Index + 1) % Players.Num()];

		Players.RemoveAt(Index);
		CurPlayerIndex = Players.Find(CurPlayer);

		UE_LOG(LogTemp, Log, TEXT("UGGame::KickPlayer: Kicked %s."), *Player->GetPlayerName());
		return CheckEndRound();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UGGame::KickPlayer: %s is not even playing!"), *Player->GetPlayerName());
	return false;
}

TArray<AUGPS*> AUGGame::GetWinners_Implementation()
{
	TArray<AUGPS*> winners;

	for (auto p : Players)
	{
		if (p->bIsInGame)
		{
			winners.Add(p);
		}
	}

	return winners;
}

bool AUGGame::CheckEndRound_Implementation()
{
	int32 PlayerCount = GetInGamePlayersCount();

	if (bRoundOver)
	{
		UE_LOG(LogTemp, Log, TEXT("UGGame::CheckEndRound: Round is over."));

		EndRound(GetWinners());

		return true;
	}

	if (PlayerCount == 1)
	{
		UE_LOG(LogTemp, Log, TEXT("UGGame::CheckEndRound: Only one player in game."));

		EndRound(GetWinners());

		return true;
	}
	if (PlayerCount == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("UGGame::CheckEndRound: No players in game."));

		EndRound(GetWinners());

		return true;
	}
	return false;
}

bool AUGGame::CheckEndGame_Implementation()
{
	int32 WinScore;
	if (Settings && Settings->GetIntParam(FName(TEXT("WinScore")), WinScore))
	{
		for (auto p : Players)
		{
			if (p->Score >= WinScore)
				return true;
		}
	}

	return false;
}

void AUGGame::NewPlayer_Implementation(AUGPS* Player)
{
	if (!bIsInGame)
	{
		Players.AddUnique(Player);
	}
}

void AUGGame::UpdatePlayerState(AUGPS* OldPS, AUGPS* NewPS)
{
	if (!IsValid(OldPS) || !IsValid(NewPS))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGGame::UpdatePlayerState: Invalid argument."));
		return;
	}

	int32 i = Players.Find(OldPS);
	if (i != INDEX_NONE)
	{
		Players.Remove(OldPS);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGGame::UpdatePlayerState: OldPS (%s) is not in game."), *GetNameSafe(OldPS));
		return;
	}

	Players.Add(NewPS);
	UE_LOG(LogTemp, Log, TEXT("UGGame::UpdatePlayerState: Switched: %s -> %s."), *GetNameSafe(OldPS), *GetNameSafe(NewPS));
}

void AUGGame::Log(FText Message)
{
	UE_LOG(UGGameLog, Log, TEXT("%s"), *Message.ToString());

	AGameStateBase* GS = GetWorld() ? GetWorld()->GetAuthGameMode() ? GetWorld()->GetAuthGameMode()->GameState : nullptr : nullptr;

	for (auto p : GS->PlayerArray)
	{
		auto PS = Cast<AUGPS>(p);
		if (PS)
		{
			PS->ClientGameLog(Message);
		}
	}
}

// In case we want to add some default functionality
void AUGGame::InitGame_Implementation() {}
void AUGGame::SortPlayers_Implementation() {}
void AUGGame::InitRound_Implementation() {}
void AUGGame::InitPlayer_Implementation(class AUGPS* Player) {}
void AUGGame::NewLap_Implementation() {}
void AUGGame::Leaved_Implementation(AUGPS *Player) {}

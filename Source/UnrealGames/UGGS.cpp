// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGGS.h"
#include "UGGI.h"
#include "UGGM.h"
#include "UGPS.h"
#include "UGGame.h"
#include "Net/UnrealNetwork.h"


AUGGame* AUGGS::GetGame()
{

	if (!IsValid(Game) && Role == ROLE_Authority)
	{
		auto GI = Cast<UUGGI>(GetGameInstance());

		if (IsValid(GI)) Game = GI->Game;
	}

	return Game;
}

void AUGGS::SetGame(class AUGGame* _Game)
{
	Game = _Game;

	if (IsValid(Game))
	{
		auto gm = GetWorld() ? Cast<AUGGM>(GetWorld()->GetAuthGameMode()) : nullptr;
		if (gm)
		{
			gm->PlayerStateClass = Game->PlayerStateClass;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("AUGGS::SetGame: New game is %s."), IsValid(Game) ? *Game->GameName.ToString() : TEXT("None"));
}

void AUGGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGGS, Game);
}
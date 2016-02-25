// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGGI.h"
#include "UGGame.h"
#include "UGGM.h"


void UUGGI::StoreUGGame(class AUGGame* _Game)
{
	Game = _Game;

	auto World = GetWorld();
	if (!IsValid(World)) return;
	auto GM = Cast<AUGGM>(World->GetAuthGameMode());
	if (!IsValid(GM)) return;
	GM->SetGame(Game);
}
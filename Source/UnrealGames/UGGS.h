// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "UGGS.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API AUGGS : public AGameState
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_Game)
		class AUGGame* Game;

public:
	UPROPERTY(BlueprintReadOnly)
		bool bIsInGame;

	UFUNCTION(BlueprintImplementableEvent, Category = "UnrealGames")
		void OnRep_Game();

	void SetGame(class AUGGame* Game);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		class AUGGame* GetGame();
	
};

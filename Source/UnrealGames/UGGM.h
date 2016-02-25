// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "UGGM.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API AUGGM : public AGameMode
{
	GENERATED_BODY()

	class AUGGame* Game;

public:
	AUGGM(const FObjectInitializer& ObjectInitializer);

	void GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList) override;

	void SetGame(class AUGGame* Game);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		class AUGGame* GetGame();
	
	
};

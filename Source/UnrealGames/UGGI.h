// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "UGGI.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API UUGGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	class AUGGame* Game;

	// Used to store the unreal game reference during travel
	UFUNCTION(BlueprintCallable, Category="UnrealGames")
		void StoreUGGame(class AUGGame* Game);
	
};

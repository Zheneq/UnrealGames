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

public:
	UPROPERTY(BlueprintReadOnly)
		bool bIsInGame;
	
	
};

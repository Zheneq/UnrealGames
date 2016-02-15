// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "UGPS.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API AUGPS : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		bool bMyTurn;
	
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		bool bIsInGame;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		int32 UGScore;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		int32 PlayerIndex;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		FLinearColor PlayerColor;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		AUGPS* Next;
};

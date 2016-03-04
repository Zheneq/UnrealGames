// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UGPC.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API AUGPC : public APlayerController
{
	GENERATED_BODY()

public:

	// BEGIN AController Interface
	void InitPlayerState() override;
	// END AController Interface

	// BEGIN APlayerController Interface
	void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;
	// END APlayerController Interface

	UFUNCTION(BlueprintImplementableEvent, Category = "UnrealGames")
		void OnEndSeamlessTravel();
};

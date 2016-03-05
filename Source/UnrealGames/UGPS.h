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

	virtual void BeginPlay() override;
	// BEGIN PlayerState intreface
	virtual void CopyProperties(APlayerState* PlayerState) override;
	// END

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		bool bMyTurn;
	
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		bool bIsInGame;

	// Deprecated
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		int32 UGScore;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		int32 PlayerIndex;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		FLinearColor PlayerColor;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		AUGPS* Next;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		int32 Team;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		class AUGSettings* Settings;


	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		void ScoreObjective(int32 Points = 1);

	/**
	* Retreives player avatar from Steam.
	* @return Returns None if the avatar is unavailable
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		UTexture2D* GetSteamAvatar(int32 &width, int32 &height);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		class AUGPC* GetOwningPlayerController();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UnrealGames")
		void ClientGameLog(const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "UnrealGames")
		void ShowGameLogMessageOnScreen(const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void OnScore(int32 Points);
};

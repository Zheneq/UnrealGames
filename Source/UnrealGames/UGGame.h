// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGGame.generated.h"


UCLASS()
class UNREALGAMES_API AUGGame : public AActor
{
	GENERATED_BODY()
private:
	bool bGameHasStarted;

public:
	// Sets default values for this actor's properties
	AUGGame();

	void BeginPlay() override;

	// Construction Script
	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		virtual class AUGPS* GetCurPlayer();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		virtual int32 GetInGamePlayersCount();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		virtual TArray<AActor*> GetBoundActorsByTag(FName Tag);

	/**
	* Same as GetBoundActorsByTag, but returns the first available (or None)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		virtual AActor* GetBoundActorByTag(FName Tag);


	// BEGIN Unreal Games Game Interface 
	/**
	* Starts new game.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual void StartGame();

	/**
	* Call it when player is done with their turn
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual void EndTurn();

	/**
	* Call it when round ends
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual void EndRound(TArray<AUGPS*> Winners);

	/**
	* Call it when player loses and is out of game till next round
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual void LoseRound(AUGPS* Loser);

	/**
	* Kicks a player from the game.
	* @return Returns true if this action caused round end.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual bool KickPlayer(AUGPS* Player);

	/**
	* Writes the message both to on-screen log and internal one.
	* 
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "UnrealGames")
		virtual void Log(FText Message);
	// END Unreal Games Game Interface


	/**
	* Resets all needed values to start a new round.
	* Calls InitRound event to tell the game to do the same
	*/
	virtual void ResetRound();

	/**
	* Prepares for turn and calls Turn
	*/
	virtual void StartTurn();

	/**
	* Set CurPlayerIndex to the index of the next in-game player
	*/
	virtual void IncrementPlayerIndex();

	/**
	* Called during Seamless Travel when playerstates are recreated.
	*/
	virtual void UpdatePlayerState(AUGPS* OldPS, AUGPS* NewPS);


	// BEGIN Unreal Games Game-Specific Events

	/**
	* Called before the game begins.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void InitGame();

	/**
	* If you want to arrange players in some particular order, do it here by rearranging the elements of Players array.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void SortPlayers();

	/**
	* Called before a round begins.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void InitRound();

	/**
	* Called for every player before a round begins.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void InitPlayer(class AUGPS* Player);

	/**
	* 
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void Turn();

	/**
	* Called when Player lost and out of the game till next round.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void Lost(AUGPS *Player);

	/**
	* Called when Player leaves the game for some reason.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void Leaved(AUGPS *Player);

	/**
	* Called after every turn to check if the round is over.
	* By default checks if bRoundOver flag is set and if there are more than one player in game.
	* @return True if it's time to end the round.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		bool CheckEndRound();

	/**
	* Called when round is over.
	* By default returns all players in game.
	* @return List of round winners.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		TArray<AUGPS*> GetWinners();

	/**
	* Called after every round to check if the game is over.
	* By default ends game after a single round.
	* @return True if it's time to end the game.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		bool CheckEndGame();

	/**
	* Called when it's the first player's turn again.
	* Some games need to handle this.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void NewLap();

	/**
	* @return true if current settings are appropriate.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		bool CheckSettings();

	/**
	* Called when new player connected.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void NewPlayer(AUGPS* Player);

	/**
	* Called when game is over.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "UnrealGames")
		void GameOver();

	// END Unreal Games Game-Specific Events
	
	// Clients who are actually in game (not spectators or whatever)
	UPROPERTY(BlueprintReadOnly)
		TArray<class AUGPS*> Players;

	// Holds the index of current player in Players array. Modify it only when you need to override the natural turn.
	UPROPERTY(BlueprintReadWrite)
		int32 CurPlayerIndex;

	// Current lap
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		int32 Lap;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "UnrealGames")
		FText GameName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "UnrealGames")
		int32 MaxPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames")
		TSubclassOf<AUGPS> PlayerStateClass;

	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames")
		class UUGSettingsComponent* Settings;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		bool bIsInGame;

	// If set, ends the round after current turn
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UnrealGames")
		bool bRoundOver;
};

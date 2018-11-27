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

	void FindGame();

public:
	AUGGM(const FObjectInitializer& ObjectInitializer);
	void BeginPlay() override;
	virtual void PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;

	TSubclassOf<APlayerState> GetPlayerStateClass();

	void PostLogin(APlayerController* NewPlayer) override;

	void GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList) override;

	void SetGame(class AUGGame* Game);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		class AUGGame* GetGame();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		bool HasEverybodyConnected();

	UPROPERTY()
		TArray<AActor*> BoundActors;

	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		void AddBoundActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		void AddBoundActors(TArray<AActor*> Actors);


};

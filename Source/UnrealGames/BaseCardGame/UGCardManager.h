// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGCardManager.generated.h"


/**
* Unreal Games Card Manager provides means to track and operate cards in a card game.
*/
UCLASS()
class UNREALGAMES_API AUGCardManager : public AActor
{
	GENERATED_BODY()
	
private:
	TArray<AUGCardGroup*> CardGroups;
	TArray<AUGCard*> RegisteredCards;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "UnrealGames")
		bool bDestroyCardsOnDestroy;

public:	
	// Sets default values for this actor's properties
	AUGCardManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Destroyed() override;
	virtual void Reset() override;

	UFUNCTION(BlueprintPure, Category = "UnrealGames|CardGame")
		TArray<AUGCardGroup*> GetGroups(AUGPS* Player);

	UFUNCTION(BlueprintPure, Category = "UnrealGames|CardGame")
		AUGCardGroup* GetGroup(AUGPS* Player, FName Group);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		void CreateGroup(AUGPS* Player, FName Name, EUGCardGroupPublicityEnum Publicity, AActor* Visualizer = NULL);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		void RegisterCard(AUGCard* Card, AUGPS* Player, FName Group);

	UFUNCTION()
		void ResetCard(AUGCard* Card);

	UFUNCTION(BlueprintPure, Category = "UnrealGames|CardGame")
		bool IsValidCard(AUGCard* Card);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool TransferCard(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup, int32 InsertAt);

	UFUNCTION()
		bool Pop(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer = NULL, bool bBottom = false);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool PopTop(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer = NULL);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool PopBottom(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer = NULL);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool PushTop(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool PushBottom(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		void Shuffle(AUGPS* Player, FName Group);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		void Restart();

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool Save();

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		bool Load();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGCard.generated.h"

// Forward decl
class AUGCardGroup;
class AUGCardManager;
/**
* Unreal Games Card.
* Base card class used by Card Manager.
*/
UCLASS()
class UNREALGAMES_API AUGCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUGCard();

	// Visible name for a card
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "UnrealGames")
		FText CardName;

	// Group the card is currently in
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		AUGCardGroup *Group;

	// Group the card was created in (used for resetting)
	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames")
		AUGCardGroup *InitialGroup;

	// Card Manager that tracks the card
	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames")
		AUGCardManager *CardManager;
	
};

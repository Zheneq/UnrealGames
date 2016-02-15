// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGCardGroupInfo.generated.h"

class AUGCardGroup;
class UUGCardGroupVizComponent;

UCLASS()
class UNREALGAMES_API AUGCardGroupInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUGCardGroupInfo();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnRep_CardCount();

	UFUNCTION()
		virtual void Update();

	// AskingController defaults to first local player.
	// Specify it if it's bot or in local multiplayer.
	UFUNCTION(BlueprintPure, Category = "UnrealGames|CardGame")
		AUGCardGroup* GetGroup(AController *AskingController = NULL);

	// Replicated properties

	// Card Group
	UPROPERTY(Replicated)
		AUGCardGroup* Group;

	// Card Group Name
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		FText GroupName;

	// Visualizer
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		UUGCardGroupVizComponent* Visualizer;

	// Card Count
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CardCount, Category = "UnrealGames")
		int32 CardCount;

};

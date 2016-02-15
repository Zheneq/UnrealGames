// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "UGCardGroupVizComponent.generated.h"

class AUGCardGroupInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUGCardGroupEventDelegate);

/**
* Unreal Games Card Group Visualization Component.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALGAMES_API UUGCardGroupVizComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
		AUGCardGroup* Group;

public:	
	// Sets default values for this component's properties
	UUGCardGroupVizComponent();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// AskingController defaults to first local player.
	// Specify it if it's bot or in local multiplayer.
	UFUNCTION(BlueprintPure, Category = "UnrealGames|CardGame")
		AUGCardGroup* GetGroup(AController *AskingController = NULL);

	UFUNCTION()
		void Update();

	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames|CardGame")
		AUGCardGroupInfo* Info;

	UFUNCTION(BlueprintCallable, Category = "UnrealGames|CardGame")
		void AssignCardGroup(AUGCardGroup* _Group);

	// Fired when data gets out of date
	UPROPERTY(BlueprintAssignable, Category = "UnrealGames")
		FUGCardGroupEventDelegate UpdateVisualizer;
	
};

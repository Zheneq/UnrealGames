// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGCardGroup.generated.h"

// Forward decl
class AUGCard;
class UUGCardGroupVizComponent;
class AUGCardGroupInfo;

UENUM(BlueprintType)
enum class EUGCardGroupPublicityEnum : uint8
{
	CGP_None 	UMETA(DisplayName = "None"),
	CGP_Public	UMETA(DisplayName = "Public"),
	CGP_Private UMETA(DisplayName = "Private"),
	CGP_Secret 	UMETA(DisplayName = "Secret"),
};

/**
* Unreal Games Card Group is used by Card Manager.
* Each card must be assigned to a single group.
* A group may represent deck, cards a player has on his/her hands, etc.
*/
UCLASS()
class UNREALGAMES_API AUGCardGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUGCardGroup();

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		virtual void OnRep_Cards();

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames", Meta = (ExposeOnSpawn = true))
		FName CardGroupName;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		FString FullName;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Cards, Category = "UnrealGames")
		TArray<AUGCard*> Cards;

	bool bIsOutOfDate;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		AUGCardGroupInfo* Info;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames", Meta = (ExposeOnSpawn = true))
		EUGCardGroupPublicityEnum Publicity;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		APlayerState* OwningPlayerState;

};

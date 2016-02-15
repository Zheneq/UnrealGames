// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGCard.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AUGCard::AUGCard()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	CardName = FText::FromString("Unnamed Card");
	Group = NULL;
	InitialGroup = NULL;

}

void AUGCard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGCard, CardName);
	DOREPLIFETIME(AUGCard, Group);

}
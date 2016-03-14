// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UUGSettingsComponent::UUGSettingsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;

	// ...
}


// Called when the game starts
void UUGSettingsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUGSettingsComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UUGSettingsComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUGSettingsComponent, IntParameters);

}
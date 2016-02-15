// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGCardGroup.h"
#include "UGCardGroupVizComponent.h"
#include "UGCardGroupInfo.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UUGCardGroupVizComponent::UUGCardGroupVizComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetActive(false);

	Group = NULL;
	Info = NULL;
}

// Called every frame
void UUGCardGroupVizComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (Group && !Info)
	{
		if (IsValid(Group))
		{
			Info = Group->Info;
			if (Info)
			{
				Info->Visualizer = this;
				Update();
			}
		}
	}
}

AUGCardGroup* UUGCardGroupVizComponent::GetGroup(AController *AskingController)
{
	if (IsValid(Info))
	{
		return Info->GetGroup(AskingController);
	}
	return NULL;
}

void UUGCardGroupVizComponent::AssignCardGroup(AUGCardGroup* _Group)
{
	Group = _Group;
	// GetOwner()->SetOwner(Group->GetOwner());
	SetActive(true);
}

void UUGCardGroupVizComponent::Update()
{
	UpdateVisualizer.Broadcast();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsComponent.h"
#include "Settings/UGSettingsParamWrapperInt.h"
#include "Settings/UGSettingsParamWrapperList.h"
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

	if (GetOwner()->Role == ROLE_Authority)
	{
		for (auto& p : IntParameters)
		{
			auto w = NewObject<UUGSettingsParamWrapperInt>(GetOwner());
			if (!w)
			{
				UE_LOG(LogTemp, Error, TEXT("UUGSettingsComponent::BeginPlay: Could not create wrapper for %s."), *p.DisplayName.ToString());
			}
			else
			{
				w->RegisterComponent();
				w->Init(p);
				w->bEditableOnClient = false;
				IntWrappers.Add(w);
			}
		}

		for (auto& p : ListParameters)
		{
			auto w = NewObject<UUGSettingsParamWrapperList>(GetOwner());
			if (!w)
			{
				UE_LOG(LogTemp, Error, TEXT("UUGSettingsComponent::BeginPlay: Could not create wrapper for %s."), *p.DisplayName.ToString());
			}
			else
			{
				w->RegisterComponent();
				w->Init(p);
				w->bEditableOnClient = false;
				ListWrappers.Add(w);
			}
		}
	}	
}

void UUGSettingsComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUGSettingsComponent, IntWrappers);
	DOREPLIFETIME(UUGSettingsComponent, ListWrappers);

}

bool UUGSettingsComponent::GetIntParam(FName Name, int32& outValue)
{
	for (const auto& w : IntWrappers)
	{
		if (w->IntParam.Name == Name)
		{
			outValue = w->IntParam.Value;
			return true;
		}
	}

	return false;
}

void UUGSettingsComponent::OnParamRep()
{
	OnParamReplication.Broadcast();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsComponent.h"
#include "UGGame.h"
#include "UGPS.h"
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

template<class WrapperClass, class StructClass>
WrapperClass* CreateWrapper(const StructClass& p, AActor* Owner)
{
	auto w = NewObject<WrapperClass>(Owner);
	if (!w)
	{
		UE_LOG(LogTemp, Error, TEXT("UUGSettingsComponent::CreateWrapper: Could not create wrapper for %s."), *p.DisplayName.ToString());
	}
	else
	{
		w->RegisterComponent();
		w->Init(p);
		if (Owner->HasActorBegunPlay())
		{
			w->BeginPlay(); // As owner actor has already begun play, the component's BeginPlay won't be called automatically
		}
		return w;
	}
	return nullptr;

}

// Called when the game starts
void UUGSettingsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->Role == ROLE_Authority)
	{
		Init(IntParameters, ListParameters);

		auto g = Cast<AUGGame>(GetOwner());
		if (g)
		{
			UE_LOG(LogTemp, Log, TEXT("UUGSettingsComponent::BeginPlay: Adding parameters to players."));
			auto GS = GetWorld() ? GetWorld()->GetGameState() : nullptr;
			if (GS)
			{
				for (auto PS : GS->PlayerArray)
				{
					auto p = Cast<AUGPS>(PS);
					if (!p) continue;

					UE_LOG(LogTemp, Log, TEXT("UUGSettingsComponent::BeginPlay: Adding parameters to %s."), *p->PlayerName);
					if (p->Settings)
					{
						p->Settings->Reset();
						p->Settings->Init(PlayerIntParameters, PlayerListParameters);
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("UUGSettingsComponent::BeginPlay: Failed. Settings are invalid."));
					}
				}
			}
		}
	}	
}

void UUGSettingsComponent::Init(const TArray<FIntParam>& _IntParameters, const TArray<FListParam>& _ListParameters)
{
	for (auto& p : _IntParameters)
	{
		IntWrappers.Add(CreateWrapper<UUGSettingsParamWrapperInt>(p, GetOwner()));
	}
	IntWrappers.Remove(nullptr);

	for (auto& p : _ListParameters)
	{
		ListWrappers.Add(CreateWrapper<UUGSettingsParamWrapperList>(p, GetOwner()));
	}
	ListWrappers.Remove(nullptr);
	OnParamRep();
}

void UUGSettingsComponent::Reset()
{
	IntWrappers.Empty();
	ListWrappers.Empty();
	OnParamRep();
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
	for (auto& w : IntWrappers)
	{
		if (w && !w->HasBegunPlay())
			w->BeginPlay();
	}
	for (auto& w : ListWrappers)
	{
		if (w && !w->HasBegunPlay())
			w->BeginPlay();
	}
	OnParamReplication.Broadcast();
}
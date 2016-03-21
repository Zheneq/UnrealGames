// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsParamWrapperBase.h"
#include "UGPS.h"
#include "UGPC.h"
#include "UGGame.h"
#include "Net/UnrealNetwork.h"

UUGSettingsParamWrapperBase::UUGSettingsParamWrapperBase() : UActorComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;

	bEditableHere = false;
}

void UUGSettingsParamWrapperBase::BeginPlay()
{
	Super::BeginPlay();

	auto OwnerRef = GetOwner();
	if (OwnerRef)
	{
		if (Cast<AUGGame>(OwnerRef))
		{
			bEditableHere = OwnerRef->Role == ROLE_Authority;
		}
		else if (Cast<AUGPS>(OwnerRef))
		{
			auto PC = Cast<AUGPS>(OwnerRef)->GetOwningPlayerController();
			bEditableHere = IsValid(PC) && PC->IsLocalPlayerController();
		}
	}
}
/*
bool UUGSettingsParamWrapperBase::IsNameStableForNetworking() const
{
	return false;
}

bool UUGSettingsParamWrapperBase::IsSupportedForNetworking() const
{
	return true;
}
*/
FText UUGSettingsParamWrapperBase::GetParamDisplayName()
{
	return GetBaseStruct()->DisplayName;
}

FName UUGSettingsParamWrapperBase::GetParamName()
{
	return GetBaseStruct()->Name;
}

const FBaseParam* UUGSettingsParamWrapperBase::GetBaseStruct()
{
	return nullptr;
}

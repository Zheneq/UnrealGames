// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsParamWrapperBase.h"
#include "Net/UnrealNetwork.h"

UUGSettingsParamWrapperBase::UUGSettingsParamWrapperBase() : UActorComponent()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;
}

void UUGSettingsParamWrapperBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UUGSettingsParamWrapperBase, bEditableOnClient, COND_InitialOrOwner);
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

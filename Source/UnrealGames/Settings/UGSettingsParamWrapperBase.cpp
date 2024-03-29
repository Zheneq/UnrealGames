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
	bRemotelyEditable = false;
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

			//UE_LOG(LogTemp, Log, TEXT("UGSettingsParamWrapperBase::BeginPlay: I am %s."), GetWorld()->IsServer() ? TEXT("server") : TEXT("client"));
			//UE_LOG(LogTemp, Log, TEXT("UGSettingsParamWrapperBase::BeginPlay: My owner is player %s."), *Cast<AUGPS>(OwnerRef)->PlayerName);
			//UE_LOG(LogTemp, Log, TEXT("UGSettingsParamWrapperBase::BeginPlay: I %shave a PC."), IsValid(PC) ? TEXT("") : TEXT("do not "));
		}
		bRemotelyEditable = !bEditableHere;
	}
}

bool UUGSettingsParamWrapperBase::Validate()
{
	auto w = GetWorld();
	if (!w) return false;

	if (!bEditableHere)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUGSettingsParamWrapperBase::Validate: Attempted to edit %s when prohibited."), *GetParamDisplayName().ToString());
		return false;
	}

	return true;
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
	return GetBaseStruct() ? GetBaseStruct()->DisplayName : FText();
}

FName UUGSettingsParamWrapperBase::GetParamName()
{
	return GetBaseStruct() ? GetBaseStruct()->Name : TEXT("");
}

const FBaseParam* UUGSettingsParamWrapperBase::GetBaseStruct()
{
	return nullptr;
}

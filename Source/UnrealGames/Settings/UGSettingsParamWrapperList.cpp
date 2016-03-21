// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsParamWrapperList.h"
#include "Net/UnrealNetwork.h"


void UUGSettingsParamWrapperList::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUGSettingsParamWrapperList, ListParam);

}


void UUGSettingsParamWrapperList::Init(const FListParam& ParamStruct)
{
	ListParam = ParamStruct;
}

bool UUGSettingsParamWrapperList::Select(int32 Index)
{
	if (!Validate()) return false;

	if (!GetWorld()->IsServer())
	{
		ServerSelect(Index);
	}

	return ListParam.Select(Index);
}

void UUGSettingsParamWrapperList::ServerSelect_Implementation(int32 Index)
{
	ListParam.Select(Index);
}

bool UUGSettingsParamWrapperList::ServerSelect_Validate(int32 Index)
{
	return bRemotelyEditable;
}

const FBaseParam* UUGSettingsParamWrapperList::GetBaseStruct()
{
	return &ListParam;
}



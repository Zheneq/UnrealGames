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
	return ListParam.Select(Index);
}

const FBaseParam* UUGSettingsParamWrapperList::GetBaseStruct()
{
	return &ListParam;
}



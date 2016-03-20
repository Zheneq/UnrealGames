// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsParamWrapperBase.h"

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

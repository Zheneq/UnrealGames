// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettingsParamWrapperInt.h"
#include "Net/UnrealNetwork.h"


void UUGSettingsParamWrapperInt::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUGSettingsParamWrapperInt, IntParam);

}


void UUGSettingsParamWrapperInt::Init(const FIntParam& ParamStruct)
{
	IntParam = ParamStruct;
	IntParam.Value = IntParam.DefaultValue;
}

bool UUGSettingsParamWrapperInt::SetValue(int32 NewValue)
{
	if (NewValue > IntParam.MaxValue || NewValue < IntParam.MinValue)
		return false;

	IntParam.Value = NewValue;
	return true;
}

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
	if (IntParam.MaxValue < IntParam.MinValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUGSettingsParamWrapperInt::Init: Wrong MinValue/MaxValue for %s (%d/%d)."), *IntParam.DisplayName.ToString(), IntParam.MinValue, IntParam.MaxValue);
		IntParam.MaxValue = IntParam.MinValue;
	}
	SetValue(IntParam.DefaultValue);
}

bool UUGSettingsParamWrapperInt::SetValue(int32 NewValue)
{
	return IntParam.SetValue(NewValue);
}

const FBaseParam* UUGSettingsParamWrapperInt::GetBaseStruct()
{
	return &IntParam;
}

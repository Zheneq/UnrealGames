// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UGSettingsComponent.h"
#include "UGSettingsParamWrapperBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALGAMES_API UUGSettingsParamWrapperBase : public UObject
{
	GENERATED_BODY()

public:

	virtual const FBaseParam* GetBaseStruct();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FText GetParamDisplayName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FName GetParamName();
	
	
};

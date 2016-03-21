// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UGSettingsComponent.h"
#include "UGSettingsParamWrapperBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALGAMES_API UUGSettingsParamWrapperBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UUGSettingsParamWrapperBase();

	virtual const FBaseParam* GetBaseStruct();

	//bool IsNameStableForNetworking() const override;
	//bool IsSupportedForNetworking() const override;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		bool bEditableOnClient;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FText GetParamDisplayName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FName GetParamName();
	
	
};

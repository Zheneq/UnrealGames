// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Settings/UGSettingsParamWrapperBase.h"
#include "UGSettingsParamWrapperInt.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API UUGSettingsParamWrapperInt : public UUGSettingsParamWrapperBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		FIntParam IntParam;

	virtual const FBaseParam* GetBaseStruct() override;

	void Init(const FIntParam& ParamStruct);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		bool SetValue(int32 NewValue);

	UFUNCTION(Server, Reliable, WithValidation, Category = "UnrealGames")
		void ServerSetValue(int32 NewValue);
	
	
};

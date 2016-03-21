// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Settings/UGSettingsParamWrapperBase.h"
#include "UGSettingsParamWrapperList.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API UUGSettingsParamWrapperList : public UUGSettingsParamWrapperBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames")
		FListParam ListParam;

	virtual const FBaseParam* GetBaseStruct() override;

	void Init(const FListParam& ParamStruct);

	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		bool Select(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation, Category = "UnrealGames")
		void ServerSelect(int32 Index);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "UGWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup=UnrealGames)
class UNREALGAMES_API UUGWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = UserInterface)
		void SetPivot(FVector2D NewPivot);

	UFUNCTION(BlueprintCallable, Category = UserInterface)
		void SetSpace(EWidgetSpace NewSpace);
	
	
};

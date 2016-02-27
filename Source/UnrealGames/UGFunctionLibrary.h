// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UGFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMES_API UUGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "UnrealGames")
		static float Infinity();

	UFUNCTION(BlueprintPure, Category = "UnrealGames")
		static bool IsPreviewActor(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "UnrealGames", meta = (WorldContext = "WorldContextObject"))
		static class AUGGame* GetGame(UObject* WorldContextObject);

	
	
};

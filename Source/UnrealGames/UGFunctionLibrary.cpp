// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGFunctionLibrary.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "UGGM.h"

float UUGFunctionLibrary::Infinity()
{
	return INFINITY;
}

bool UUGFunctionLibrary::IsPreviewActor(AActor* Actor)
{
	return Actor->HasAnyFlags(RF_Transient);
}

class AUGGame* UUGFunctionLibrary::GetGame(UObject* WorldContextObject)
{
	auto World = WorldContextObject->GetWorld();
	if (World)
	{
		auto GM = Cast<AUGGM>(World->GetAuthGameMode());
		if (GM)
		{
			return GM->GetGame();
		}
	}

	return nullptr;
}
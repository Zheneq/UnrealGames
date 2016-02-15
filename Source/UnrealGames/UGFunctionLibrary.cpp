// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGFunctionLibrary.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

float UUGFunctionLibrary::Infinity()
{
	return INFINITY;
}

bool UUGFunctionLibrary::IsPreviewActor(AActor* Actor)
{
	return Actor->HasAnyFlags(RF_Transient);
}
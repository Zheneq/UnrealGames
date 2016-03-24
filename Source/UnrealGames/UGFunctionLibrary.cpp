// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGFunctionLibrary.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "UGGM.h"
#include "UGGS.h"

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
	auto GS = WorldContextObject->GetWorld() ? Cast<AUGGS>(WorldContextObject->GetWorld()->GetGameState()) : nullptr;
	if (GS)
	{
		return GS->GetGame();
	}

	return nullptr;
}

FVector UUGFunctionLibrary::ProjectPointOnToPlaneParallerToDirection(FVector Point, FVector PlaneBase, FVector PlaneNormal, FVector Direction)
{
	return Point - Direction * FVector::PointPlaneDist(Point, PlaneBase, PlaneNormal) / (Direction | PlaneNormal);
}
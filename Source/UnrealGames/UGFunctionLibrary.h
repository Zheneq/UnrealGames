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

	/**
	* Projects a point onto a plane defined by a point on the plane and a plane normal parallel to a direction.
	*
	* @param  Point Point to project onto the plane.
	* @param  PlaneBase A point on the plane.
	* @param  PlaneNormal Normal of the plane.
	* @param  Direction Direction parallel to which the point is projected.
	* @return Point projected onto the plane.
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (Keywords = "ProjectOnTo"))
		static FVector ProjectPointOnToPlaneParallerToDirection(FVector Point, FVector PlaneBase, FVector PlaneNormal, FVector Direction);

	
};

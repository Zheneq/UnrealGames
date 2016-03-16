// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "UGSettingsComponent.generated.h"

USTRUCT()
struct FBaseParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		FText DisplayName;
};

USTRUCT(BlueprintType)
struct FIntParam : public FBaseParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames")
		int32 Value;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		int32 DefaultValue;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		int32 MinValue;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		int32 MaxValue;

	FIntParam() : FBaseParam()
	{
		Value = 0;
		MinValue = MaxValue = 0;
		DefaultValue = 0;
	}

	bool SetValue(int32 NewValue)
	{
		if (NewValue < MinValue || NewValue > MaxValue)
			return false;
		
		Value = NewValue;
		return true;
	}
};


UCLASS( ClassGroup=(UnrealGames), meta=(BlueprintSpawnableComponent) )
class UNREALGAMES_API UUGSettingsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUGSettingsComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames|Settings")
		TArray<FIntParam> IntParameters;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UnrealGames", meta = (DisplayName = "Int Parameters"))
		TArray<class UUGSettingsParamWrapperInt*> IntWrappers;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		bool GetIntParam(FName Name, int32& outValue);
};

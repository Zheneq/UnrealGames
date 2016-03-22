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

	FBaseParam()
	{

	}

	FBaseParam(const FName& _Name, const FText& _DisplayName):
		Name(_Name), DisplayName(_DisplayName)
	{

	}
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
		Value = FMath::Clamp(NewValue, MinValue, MaxValue);
		return true;
	}
};

USTRUCT(BlueprintType)
struct FListParam : public FBaseParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UnrealGames")
		int32 Selected;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		TArray<FText> List;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UnrealGames")
		bool bDoNotLoop;

	FListParam() : FBaseParam()
	{
		Selected = 0;
		bDoNotLoop = false;
	}

	FListParam(const FName& _Name, const FText& _DisplayName, const TArray<FText>& _List) :
		FBaseParam(_Name, _DisplayName), Selected(0), bDoNotLoop(false)
	{
		List = _List;
	}

	bool Select(int32 Index)
	{
		if (!List.IsValidIndex(Index))
		{
			if (bDoNotLoop)
			{
				return false;
			}
			else
			{
				Index = Index % List.Num();
				if (Index < 0)
				{
					Index += List.Num();
				}
			}
		}

		Selected = Index;
		return true;
	}
};

UCLASS( ClassGroup=(UnrealGames), meta=(BlueprintSpawnableComponent) )
class UNREALGAMES_API UUGSettingsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEvent);

	// Sets default values for this component's properties
	UUGSettingsComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Reset();
	virtual void Init(const TArray<FIntParam>& _IntParameters, const TArray<FListParam>& _ListParameters);


	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames|GameSettings")
		TArray<FIntParam> IntParameters;

	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames|GameSettings")
		TArray<FListParam> ListParameters;

	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames|PlayerSettings")
		TArray<FIntParam> PlayerIntParameters;

	UPROPERTY(EditDefaultsOnly, Category = "UnrealGames|PlayerSettings")
		TArray<FListParam> PlayerListParameters;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnParamRep, Category = "UnrealGames", meta = (DisplayName = "Int Parameters"))
		TArray<class UUGSettingsParamWrapperInt*> IntWrappers;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnParamRep, Category = "UnrealGames", meta = (DisplayName = "List Parameters"))
		TArray<class UUGSettingsParamWrapperList*> ListWrappers;

	UPROPERTY(BlueprintAssignable, Category = "UnrealGames")
		FSimpleEvent OnParamReplication;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		bool GetIntParam(FName Name, int32& outValue);

	UFUNCTION()
		void OnParamRep();

	/**
	* Adds new list parameter.
	* Callable ONLY inside CONSTRUCTION SCRIPT.
	* @return true if successfully added.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealGames")
		bool AddListParam(FName Name, FText DisplayName, TArray<FText> List, bool bPlayerParam = false);
};

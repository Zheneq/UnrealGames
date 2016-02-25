// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UGSettings.generated.h"

DECLARE_DYNAMIC_DELEGATE(FRepEvent);

UCLASS()
class UNREALGAMES_API AUGSettings : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
		virtual void OnRep();

	UPROPERTY(ReplicatedUsing=OnRep)
		TMap<FName, FString> Strings;
	UPROPERTY(ReplicatedUsing = OnRep)
		TMap<FName, int32> Ints;
	UPROPERTY(ReplicatedUsing = OnRep)
		TMap<FName, FVector> Vectors;
	
public:	
	AUGSettings();


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "UnrealGames")
		void AddString(FName Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "UnrealGames")
		void AddInt(FName Key, int32 Value);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "UnrealGames")
		void AddVector(FName Key, FVector Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FString GetString(FName Key);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		int32 GetInt(FName Key);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		FVector GetVector(FName Key);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		int32 GetStringKeys(TArray<FName>& Keys);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		int32 GetIntKeys(TArray<FName>& Keys);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UnrealGames")
		int32 GetVectorKeys(TArray<FName>& Keys);

	UPROPERTY(BlueprintReadWrite, Category = "UnrealGames")
		FRepEvent OnRepDelegate;

};

// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGSettings.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AUGSettings::AUGSettings()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AUGSettings::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGSettings, Strings);
	DOREPLIFETIME(AUGSettings, Ints);
	DOREPLIFETIME(AUGSettings, Vectors);

}

void AUGSettings::OnRep()
{
	OnRepDelegate.ExecuteIfBound();
}

void AUGSettings::AddString_Implementation(FName Key, const FString& Value)
{
	Strings.Add(Key, Value);
}
bool AUGSettings::AddString_Validate(FName Key, const FString& Value)
{
	return true;
}

void AUGSettings::AddInt_Implementation(FName Key, int32 Value)
{
	Ints.Add(Key, Value);
}
bool AUGSettings::AddInt_Validate(FName Key, int32 Value)
{
	return true;
}

void AUGSettings::AddVector_Implementation(FName Key, FVector Value)
{
	Vectors.Add(Key, Value);
}
bool AUGSettings::AddVector_Validate(FName Key, FVector Value)
{
	return true;
}

FString AUGSettings::GetString(FName Key)
{
	return Strings.FindRef(Key);
}

int32 AUGSettings::GetInt(FName Key)
{
	return Ints.FindRef(Key);
}

FVector AUGSettings::GetVector(FName Key)
{
	return Vectors.FindRef(Key);
}

int32 AUGSettings::GetStringKeys(TArray<FName>& Keys)
{
	return Strings.GetKeys(Keys);
}

int32 AUGSettings::GetIntKeys(TArray<FName>& Keys)
{
	return Ints.GetKeys(Keys);
}

int32 AUGSettings::GetVectorKeys(TArray<FName>& Keys)
{
	return Vectors.GetKeys(Keys);
}
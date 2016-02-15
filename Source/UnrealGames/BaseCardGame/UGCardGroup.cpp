// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGCard.h"
#include "UGCardGroup.h"
#include "UGCardGroupInfo.h"
#include "UGCardGroupVizComponent.h"
#include "UGPS.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AUGCardGroup::AUGCardGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	OwningPlayerState = NULL;
	Info = NULL;
	bIsOutOfDate = false;
	Publicity = EUGCardGroupPublicityEnum::CGP_None;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
}

bool AUGCardGroup::IsNetRelevantFor(const AActor * RealViewer, const AActor * ViewTarget, const FVector & SrcLocation) const
{
	bool Res = false;
	
	switch (Publicity)
	{
	case EUGCardGroupPublicityEnum::CGP_Public:
		Res = true;
		break;
	case EUGCardGroupPublicityEnum::CGP_Private:
		Res = RealViewer && IsOwnedBy(RealViewer);
		break;
	case EUGCardGroupPublicityEnum::CGP_Secret:
	default:
		Res = false;
		break;
	}

	/*
	UE_LOG(LogTemp, Log, TEXT("Checking %s's %s group relevancy (publ = %d, %srelevant to %s)."),
		Cast<AUGPS>(GetOwner()) ? *Cast<AUGPS>(GetOwner())->PlayerName : TEXT("The game"),
		*CardGroupName.ToString(),
		(int)Publicity,
		Res ? TEXT("") : TEXT("not "),
		Cast<AController>(RealViewer) ? *Cast<AController>(RealViewer)->PlayerState->PlayerName : _TEXT("None")
		);
	*/
	return Res;
}

// Called when the game starts or when spawned
void AUGCardGroup::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<AController>(GetOwner()))
	{
		OwningPlayerState = Cast<AController>(GetOwner())->PlayerState;
	}
	if (Cast<AUGPS>(GetOwner()))
	{
		OwningPlayerState = Cast<AUGPS>(GetOwner());
	}

	// Generate Display Name
	FullName = FString::Printf(_TEXT("%s's %s"), OwningPlayerState ? *OwningPlayerState->PlayerName : _TEXT("the game"), *CardGroupName.ToString());

	// Spawn Info
	Info = GetWorld()->SpawnActorDeferred<AUGCardGroupInfo>(AUGCardGroupInfo::StaticClass(), FTransform(), GetOwner());
	if (Info)
	{
		Info->Group = this;
		Info->GroupName = FText::FromName(CardGroupName);
		UGameplayStatics::FinishSpawningActor(Info, FTransform());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not spawn info actor for %s."), *CardGroupName.ToString());
	}
	
	if(IsValid(Info))
	{
		Info->Update();
	}
}

// Called every frame
void AUGCardGroup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsOutOfDate)
	{
		if (IsValid(Info))
		{
			Info->Update();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempted to update %s group with no info."), *CardGroupName.ToString());
		}

		bIsOutOfDate = false;
	}

	/*
	if (!GetWorld()->IsServer())
	{
		UE_LOG(LogTemp, Log, TEXT("Client: %s's %s card group is HERE, %d cards, rendered by %s."),
			Cast<AUGPS>(GetOwner()) ? *Cast<AUGPS>(GetOwner())->PlayerName : TEXT("The game"),
			*CardGroupName.ToString(),
			Cards.Num(),
			Visualizer ? *Visualizer->GetOwner()->GetName() : _TEXT("None")
			);
	}
	*/
}

void AUGCardGroup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUGCardGroup, CardGroupName);
	DOREPLIFETIME(AUGCardGroup, Cards);
	DOREPLIFETIME(AUGCardGroup, Publicity);
	DOREPLIFETIME(AUGCardGroup, OwningPlayerState);
	DOREPLIFETIME(AUGCardGroup, Info);
}

void AUGCardGroup::OnRep_Cards()
{
	bIsOutOfDate = true;
}
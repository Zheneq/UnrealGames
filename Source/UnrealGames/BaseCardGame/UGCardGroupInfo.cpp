// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGCardGroupInfo.h"
#include "UGCardGroup.h"
#include "UGCardGroupVizComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AUGCardGroupInfo::AUGCardGroupInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	Visualizer = NULL;
	Group = NULL;
	CardCount = 0;

}

// Replication
void AUGCardGroupInfo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGCardGroupInfo, Group);
	DOREPLIFETIME(AUGCardGroupInfo, Visualizer);
	DOREPLIFETIME(AUGCardGroupInfo, CardCount);
	DOREPLIFETIME(AUGCardGroupInfo, GroupName);
}

void AUGCardGroupInfo::OnRep_CardCount()
{
	Update();
}

// Called when the game starts or when spawned
void AUGCardGroupInfo::BeginPlay()
{
	Super::BeginPlay();
	
	// Forcibly assign as components don't replicate
	if (Visualizer)
	{
		Visualizer->Info = this;
	}

	Update();
}

void AUGCardGroupInfo::Update()
{
	if (GetWorld()->IsServer())
	{
		if (IsValid(Group))
		{
			CardCount = Group->Cards.Num();
		}
	}

	if (IsValid(Visualizer))
	{
		Visualizer->Update();
	}

	UE_LOG(LogTemp, Log, TEXT("%s: Update %s group info, %d cards."),
		GetWorld()->IsServer() ? _TEXT("Server") : _TEXT("Client"),
		Group ? *Group->FullName : *GetName(),
		CardCount
	);
}

AUGCardGroup* AUGCardGroupInfo::GetGroup(AController *AskingController)
{
	if (!IsValid(Group))
		return NULL;

	if (Group->Publicity == EUGCardGroupPublicityEnum::CGP_Public)
		return Group;

	if (!AskingController)
		AskingController = GetWorld()->GetFirstPlayerController();

	if (!AskingController)
		return NULL;

	if (Group->IsOwnedBy(AskingController) && Group->Publicity != EUGCardGroupPublicityEnum::CGP_Secret)
	{
		return Group;
	}

	return NULL;
}

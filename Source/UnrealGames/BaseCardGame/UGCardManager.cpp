// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGCardGroup.h"
#include "UGCardGroupInfo.h"
#include "UGCardGroupVizComponent.h"
#include "UGCard.h"
#include "../UGPS.h"
#include "UGCardManager.h"


// Sets default values
AUGCardManager::AUGCardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDestroyCardsOnDestroy = true;

}

// Called when the game starts or when spawned
void AUGCardManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUGCardManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AUGCardManager::Destroyed()
{
	Reset();
	Super::Destroyed();
}


void AUGCardManager::Reset()
{
	Super::Reset();

	for (auto g : CardGroups)
	{
		g->Destroy();
	}
	CardGroups.Empty();

	if (bDestroyCardsOnDestroy)
	{
		for (auto c : RegisteredCards)
		{
			c->Destroy();
		}
	}
	RegisteredCards.Empty();
}

TArray<AUGCardGroup*> AUGCardManager::GetGroups(AUGPS* Player)
{
	TArray<AUGCardGroup*> Result;
	for (const auto c : CardGroups)
	{
		if (Cast<AUGPS>(c->GetOwner()) == Player)
			Result.Add(c);
	}
	return Result;
}


AUGCardGroup* AUGCardManager::GetGroup(AUGPS* Player, FName Group)
{
	for (const auto c : CardGroups)
	{
		if (Cast<AUGPS>(c->GetOwner()) == Player && c->CardGroupName == Group)
			return c;
	}
	UE_LOG(LogTemp, Log, TEXT("Group %s for player %s does not exist."), *Group.ToString(), Player ? *Player->PlayerName : TEXT("the game"));
	return NULL;
}

void AUGCardManager::CreateGroup(AUGPS* Player, FName Name, EUGCardGroupPublicityEnum Publicity, AActor* Visualizer)
{
	if (GetGroup(Player, Name))
	{
		UE_LOG(LogTemp, Log, TEXT("Group %s for player %s already exists."), *Name.ToString(), Player ? *Player->PlayerName : TEXT("the game"));
		return;
	}

	AUGCardGroup* NewGroup = GetWorld()->SpawnActorDeferred<AUGCardGroup>(AUGCardGroup::StaticClass(), FTransform(), Player);
	if (NewGroup)
	{
		NewGroup->Publicity = Publicity;
		NewGroup->CardGroupName = Name;
		UGameplayStatics::FinishSpawningActor(NewGroup, FTransform());
	}

	if (IsValid(Visualizer))
	{
		auto Comp = Cast<UUGCardGroupVizComponent>(Visualizer->GetComponentByClass(UUGCardGroupVizComponent::StaticClass()));
		if (IsValid(Comp))
		{
			Comp->AssignCardGroup(NewGroup);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CardGroupRenderer is set but does not support rendering!"));
		}
	}

	CardGroups.Add(NewGroup);

	UE_LOG(LogTemp, Log, TEXT("Created group: %s for %s."), *Name.ToString(), Player ? *Player->PlayerName : TEXT("the game"));
}

void AUGCardManager::RegisterCard(AUGCard* Card, AUGPS* Owner, FName Group)
{
	auto g = GetGroup(Owner, Group);

	if (!IsValid(Card) || RegisteredCards.Contains(Card) || !g)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot register %s card."), *Card->CardName.ToString());
		return;
	}

	Card->CardManager = this;
	Card->InitialGroup = g;

	ResetCard(Card);
	RegisteredCards.Add(Card);
}

void AUGCardManager::ResetCard(AUGCard* Card)
{
	Card->Group = Card->InitialGroup;
	Card->Group->Cards.Add(Card);
	Card->Group->bIsOutOfDate = true;
}

bool AUGCardManager::IsValidCard(AUGCard* Card)
{
	return IsValid(Card) && RegisteredCards.Contains(Card);
}

bool AUGCardManager::TransferCard(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup, int32 InsertAt)
{
	auto g = GetGroup(ToPlayer, ToGroup);
	if (!IsValidCard(Card) || !g)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot transfer %s card."), *Card->CardName.ToString());
		return false;
	}

	// Remove the card and mark the group as out of date
	Card->Group->Cards.Remove(Card);
	Card->Group->bIsOutOfDate = true;

	// Insert the card into target group
	InsertAt = FMath::Clamp(InsertAt, 0, g->Cards.Num());
	g->Cards.Insert(Card, InsertAt);


	UE_LOG(LogTemp, Log, TEXT("Transferred %s from %s's %s to %s's %s at %d."),
		*Card->CardName.ToString(),
		Cast<AUGPS>(Card->Group->GetOwner()) ? *Cast<AUGPS>(Card->Group->GetOwner())->PlayerName : TEXT("the game"),
		*Card->Group->CardGroupName.ToString(),
		ToPlayer ? *ToPlayer->PlayerName : TEXT("the game"),
		*ToGroup.ToString(),
		InsertAt
		);


	Card->Group = g;
	g->bIsOutOfDate = true;

	return true;
}

bool AUGCardManager::Pop(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer, bool bBottom)
{
	auto From = GetGroup(FromPlayer, FromGroup);

	if (!From) return false;

	if (!From->Cards.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("Pop: Group %s for player %s is empty."), *FromGroup.ToString(), FromPlayer ? *FromPlayer->PlayerName : TEXT("the game"));
		return false;
	}

	return TransferCard(bBottom ? From->Cards[0] : From->Cards.Last(), ToPlayer, ToGroup, 100500);
}

bool AUGCardManager::PopTop(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer)
{
	return Pop(FromGroup, ToPlayer, ToGroup, FromPlayer, false);
}

bool AUGCardManager::PopBottom(FName FromGroup, AUGPS* ToPlayer, FName ToGroup, AUGPS* FromPlayer)
{
	return Pop(FromGroup, ToPlayer, ToGroup, FromPlayer, true);
}

bool AUGCardManager::PushTop(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup)
{
	return TransferCard(Card, ToPlayer, ToGroup, 100500);
}

bool AUGCardManager::PushBottom(AUGCard* Card, AUGPS* ToPlayer, FName ToGroup)
{
	return TransferCard(Card, ToPlayer, ToGroup, 0);
}

void AUGCardManager::Shuffle(AUGPS* Owner, FName Group)
{
	auto g = GetGroup(Owner, Group);
	if (g)
	{
		for (int32 i = 0; i < g->Cards.Num(); ++i)
		{
			int j = FMath::RandRange(0, i);
			auto t = g->Cards[i];
			g->Cards[i] = g->Cards[j];
			g->Cards[j] = t;
		}
	}
}

void AUGCardManager::Restart()
{
	for (auto g : CardGroups)
	{
		g->Cards.Empty();
		g->Info->Update();
	}
	for (auto c : RegisteredCards) ResetCard(c);
	UE_LOG(LogTemp, Log, TEXT("Reset all cards to their initial locations."));
}
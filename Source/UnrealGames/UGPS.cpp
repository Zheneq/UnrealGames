// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGPS.h"

#include "Net/UnrealNetwork.h"



void AUGPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGPS, bMyTurn);
	DOREPLIFETIME(AUGPS, bIsInGame);
	DOREPLIFETIME(AUGPS, UGScore);
	DOREPLIFETIME(AUGPS, PlayerIndex);
	DOREPLIFETIME(AUGPS, PlayerColor);
	DOREPLIFETIME(AUGPS, Next);

}



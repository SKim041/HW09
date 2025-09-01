// Fill out your copyright notice in the Description page of Project Settings.


#include "BACPlayerState.h"
#include "Net/UnrealNetwork.h"

ABACPlayerState::ABACPlayerState()
	: PlayerNameString(TEXT("None"))
	  , CurrentGuessCount(0)
	  , MaxGuessCount(3)
{
	bReplicates = true;
}

void ABACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
}

FString ABACPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") +
		FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}

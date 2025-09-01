// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BACPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABACPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FString GetPlayerInfoString();

	UPROPERTY(Replicated)
	FString PlayerNameString;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxGuessCount;
};

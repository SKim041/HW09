// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BACGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void PrintChatMessageString(class ABACPlayerController* InChattingPlayerController,
	                            const FString& InChatMessageString);
	void IncreaseGuessCount(ABACPlayerController* InChattingPlayerController);

	void ResetGame();
	void JudgeGame(ABACPlayerController* InChattingPlayerController, int InStrikeCount);
	void StartNextRound();

protected:
	FString SecretNumberString;
	TArray<TObjectPtr<class ABACPlayerController>> AllPlayerControllers;

	int32 CurrentRoundNum = 0;
	uint8 bIsWaiting : 1;
	float RoundInterval = 3.f;
	FTimerHandle StartRoundHandle;
};

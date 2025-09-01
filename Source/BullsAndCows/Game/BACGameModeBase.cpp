// Fill out your copyright notice in the Description page of Project Settings.


#include "BACGameModeBase.h"
#include "BACGameStateBase.h"
#include "EngineUtils.h"
#include "BullsAndCows/Player/BACPlayerController.h"
#include "BullsAndCows/Player/BACPlayerState.h"

void ABACGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ABACPlayerController* BACPlayerController = Cast<ABACPlayerController>(NewPlayer);
	if (IsValid(BACPlayerController) == true)
	{
		BACPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(BACPlayerController);

		ABACPlayerState* BACPS = BACPlayerController->GetPlayerState<ABACPlayerState>();
		if (IsValid(BACPS) == true)
		{
			BACPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ABACGameStateBase* BACGameStateBase = GetGameState<ABACGameStateBase>();
		if (IsValid(BACGameStateBase) == true)
		{
			BACGameStateBase->MulticastRPCBroadcastLoginMessage(BACPS->PlayerNameString);
		}
	}
}

FString ABACGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ABACGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (bIsWaiting) break;
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
	}
	while (false);

	return bCanPlay;
}


FString ABACGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABACGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	bIsWaiting = false;
	ResetGame();
}

void ABACGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(StartRoundHandle);
}

void ABACGameModeBase::PrintChatMessageString(class ABACPlayerController* InChattingPlayerController,
                                              const FString& InChatMessageString)
{
	FString PlayerMessageString = "";
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);
		ABACPlayerState* BACPS = InChattingPlayerController->GetPlayerState<ABACPlayerState>();
		if (IsValid(BACPS) == true)
		{
			PlayerMessageString = BACPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
		}
		for (TActorIterator<ABACPlayerController> It(GetWorld()); It; ++It)
		{
			ABACPlayerController* BACPlayerController = *It;
			if (IsValid(BACPlayerController) == true)
			{
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));

				if (IsValid(BACPS) == true)
				{
					if (BACPS->CurrentGuessCount > BACPS->MaxGuessCount)
					{
						JudgeResultString = TEXT("OUT OF CHANCES");
						StrikeCount = 0;
					}
				}
				FString CombinedMessageString = PlayerMessageString + TEXT(" -> ") + JudgeResultString;
				BACPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ABACPlayerController> It(GetWorld()); It; ++It)
		{
			ABACPlayerController* BACPlayerController = *It;
			if (IsValid(BACPlayerController) == true)
			{
				BACPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ABACGameModeBase::IncreaseGuessCount(ABACPlayerController* InChattingPlayerController)
{
	ABACPlayerState* BACPS = InChattingPlayerController->GetPlayerState<ABACPlayerState>();
	if (IsValid(BACPS) == true)
	{
		BACPS->CurrentGuessCount++;
	}
}

void ABACGameModeBase::ResetGame()
{
	if (bIsWaiting == true) return;
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
	for (const auto& BACPlayerController : AllPlayerControllers)
	{
		ABACPlayerState* BACPS = BACPlayerController->GetPlayerState<ABACPlayerState>();
		if (IsValid(BACPS) == true)
		{
			BACPS->CurrentGuessCount = 0;
		}
	}
	bIsWaiting = true;
	GetWorld()->GetTimerManager().SetTimer(StartRoundHandle, this, &ABACGameModeBase::StartNextRound, RoundInterval,
	                                       false);
}

void ABACGameModeBase::JudgeGame(ABACPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ABACPlayerState* BACPS = InChattingPlayerController->GetPlayerState<ABACPlayerState>();
		for (const auto& BACPlayerController : AllPlayerControllers)
		{
			if (IsValid(BACPS) == true)
			{
				FString CombinedMessageString = BACPS->PlayerNameString + TEXT(" has won the game.");
				BACPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}
		}
		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& BACPlayerController : AllPlayerControllers)
		{
			ABACPlayerState* BACPS = BACPlayerController->GetPlayerState<ABACPlayerState>();
			if (IsValid(BACPS) == true)
			{
				if (BACPS->CurrentGuessCount < BACPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& BACPlayerController : AllPlayerControllers)
			{
				BACPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			ResetGame();
		}
	}
}

void ABACGameModeBase::StartNextRound()
{
	CurrentRoundNum++;
	for (const auto& BACPlayerController : AllPlayerControllers)
	{
		BACPlayerController->NotificationText = FText::FromString(
			FString::Printf(TEXT("Round %d"), CurrentRoundNum));
	}
	bIsWaiting = false;
}

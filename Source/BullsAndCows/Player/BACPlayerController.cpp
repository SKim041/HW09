// Fill out your copyright notice in the Description page of Project Settings.


#include "BACPlayerController.h"
#include "BullsAndCows/BullsAndCows.h"
#include "BullsAndCows/UI/BACChatInput.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "BullsAndCows/Game/BACGameModeBase.h"
#include "BACPlayerState.h"

ABACPlayerController::ABACPlayerController()
{
	bReplicates = true;
}

void ABACPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ABACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UBACChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ABACPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	if (IsLocalController() == true)
	{
		ABACPlayerState* BACPS = GetPlayerState<ABACPlayerState>();
		if (IsValid(BACPS) == true)
		{
			FString CombinedMessageString = BACPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ABACPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	BACFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ABACPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ABACPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ABACGameModeBase* BACGM = Cast<ABACGameModeBase>(GM);
		if (IsValid(BACGM) == true)
		{
			BACGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

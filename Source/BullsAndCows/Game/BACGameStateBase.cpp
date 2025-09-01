// Fill out your copyright notice in the Description page of Project Settings.


#include "BACGameStateBase.h"
#include "BullsAndCows/Player/BACPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ABACGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ABACPlayerController* BACPC = Cast<ABACPlayerController>(PC);
			if (IsValid(BACPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				BACPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BACChatInput.h"
#include "BullsAndCows/Player/BACPlayerController.h"
#include "Components/EditableTextBox.h"

void UBACChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UBACChatInput::OnChatInputTextComitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UBACChatInput::OnChatInputTextComitted);
	}
}

void UBACChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UBACChatInput::OnChatInputTextComitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UBACChatInput::OnChatInputTextComitted);
	}
}

void UBACChatInput::OnChatInputTextComitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ABACPlayerController* PlayerController = Cast<ABACPlayerController>(GetOwningPlayer());
		if (IsValid(PlayerController))
		{
			PlayerController->SetChatMessageString(Text.ToString());
			EditableTextBox_ChatInput->SetText(FText::GetEmpty());
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BACPlayerController.generated.h"

class UBACChatInput;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABACPlayerController();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BAC")
	TSubclassOf<UBACChatInput> ChatInputWidgetClass;
	UPROPERTY()
	TObjectPtr<UBACChatInput> ChatInputWidgetInstance;
	UPROPERTY(EditDefaultsOnly, Category = "BAC")
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	FString ChatMessageString;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};

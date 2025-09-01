// Fill out your copyright notice in the Description page of Project Settings.


#include "BACPawn.h"
#include "BullsAndCows/BullsAndCows.h"


// Sets default values
ABACPawn::ABACPawn()
{
}

// Called when the game starts or when spawned
void ABACPawn::BeginPlay()
{
	Super::BeginPlay();
	FString RoleString = BACFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("BACPawn::BeginPlay() %s [%s]"), *BACFunctionLibrary::GetNetModeString(this), *RoleString);
	BACFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ABACPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	FString RoleString = BACFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("BACPawn::PossessedBy() %s [%s]"), *BACFunctionLibrary::GetNetModeString(this), *RoleString);
	BACFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

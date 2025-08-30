// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GdrPlayerController.h"

void AGdrPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		bShowMouseCursor = false;
	}
}

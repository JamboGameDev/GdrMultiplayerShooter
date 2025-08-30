// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GdrMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Framework/GdrGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"

//-------------------------------------------------------------------------------------------------------------------------------
void UGdrMainMenuWidget::NativeConstruct()
{
	Super::NativeOnInitialized();
	
	GameInstance = Cast<UGdrGameInstance>(GetGameInstance());

	if (InputPlayerName && GameInstance)
	{
		InputPlayerName->SetText(GameInstance->SPlayerProfileInfo.PlayerName);
	}
	
	if (BtnQuitGame)
	{
		BtnQuitGame->OnClicked.AddDynamic(this, &UGdrMainMenuWidget::QuitGame);
	}

	if (BtnSoloGame)
	{
		BtnSoloGame->OnClicked.AddDynamic(this, &UGdrMainMenuWidget::StartSoloGame);
	}

	if (InputPlayerName)
	{
		InputPlayerName->OnTextCommitted.AddDynamic(this, &UGdrMainMenuWidget::OnTextCommittedHandler);
	}

	if (BtnMultiplayerGame)
	{
		BtnMultiplayerGame->OnClicked.AddDynamic(this, &UGdrMainMenuWidget::UIShowMultiplayerMenu);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrMainMenuWidget::QuitGame()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrMainMenuWidget::StartSoloGame()
{
	
	if (GameInstance)
	{
		GameInstance->IsSoloGame = true;
		GameInstance->StartSoloGame("Map01");
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrMainMenuWidget::UIShowMultiplayerMenu()
{
	RemoveFromParent();
	GameInstance->UIShowMPMenu();
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrMainMenuWidget::OnTextCommittedHandler(const FText& Text, ETextCommit::Type CommitedMethod)
{
	 GameInstance->ChangePlayerName(Text);
}
//-------------------------------------------------------------------------------------------------------------------------------

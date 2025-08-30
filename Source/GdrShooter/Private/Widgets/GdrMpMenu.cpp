// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GdrMpMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Framework/GdrGameInstance.h"

void UGdrMpMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UGdrGameInstance>(GetGameInstance());
	GameInstance->IsLanConnection = SetConnectionTypeText(GameInstance->IsLanConnection);

	if (BtnMainMenu)
	{
		BtnMainMenu->OnClicked.AddDynamic(this, &UGdrMpMenu::ShowMainMenu);
	}

	if (BtnConnectionType)
	{
		BtnConnectionType->OnClicked.AddDynamic(this, &UGdrMpMenu::StartSoloGame);
	}
}
//---------------------------------------------------------------------------------------------------------------
bool UGdrMpMenu::SetConnectionTypeText(bool IsLanConnection)
{
	if (IsLanConnection)
	{
		TextConnectType->SetText(FText::FromString(TEXT("LAN")));
	}
	else
	{
		TextConnectType->SetText(FText::FromString(TEXT("Online")));
	}
	return IsLanConnection;
}
//---------------------------------------------------------------------------------------------------------------
void UGdrMpMenu::ShowMainMenu()
{
	RemoveFromParent();
	GameInstance->UIShowMenu();
}
//---------------------------------------------------------------------------------------------------------------
void UGdrMpMenu::StartSoloGame()
{
	GameInstance->StartSoloGame("Map01");
}
//---------------------------------------------------------------------------------------------------------------

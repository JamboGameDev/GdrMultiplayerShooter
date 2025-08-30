// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GdrMpMenu.generated.h"

class UTextBlock;
class UGdrGameInstance;
class UButton;

UCLASS()
class GDRSHOOTER_API UGdrMpMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	bool SetConnectionTypeText(bool IsLanConnection);

	UPROPERTY(Meta = (BindWidget))
	UButton* BtnMainMenu;

	UPROPERTY(Meta = (BindWidget))
	UButton* BtnConnectionType;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextConnectType;

protected:
	UPROPERTY()
	UGdrGameInstance* GameInstance;

private:
	UFUNCTION()
	void ShowMainMenu();

	UFUNCTION()
	void StartSoloGame();
	
};

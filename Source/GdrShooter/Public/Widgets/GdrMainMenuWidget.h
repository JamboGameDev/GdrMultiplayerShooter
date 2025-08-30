// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "GdrMainMenuWidget.generated.h"

class UGdrGameInstance;
class UButton;

UCLASS()
class GDRSHOOTER_API UGdrMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnQuitGame;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnSoloGame;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnMultiplayerGame;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputPlayerName;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	UGdrGameInstance* GameInstance;

private:
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void StartSoloGame();
	
	UFUNCTION()
	void UIShowMultiplayerMenu();
	
	UFUNCTION()
	void OnTextCommittedHandler(const FText& Text, ETextCommit::Type CommitedMethod);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveData/GdrStructPlayerProfile.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GdrGameInstance.generated.h"

class UGdrPlayerProfileSaveGame;

UCLASS()
class GDRSHOOTER_API UGdrGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UIShowMenu(); //use in Level Blueprint MainMenuMap

	UFUNCTION(BlueprintCallable)
	void UIShowMPMenu();

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> MpMenuWidgetClass;

	bool IsLanConnection;
	bool IsSoloGame = false;
	void StartSoloGame(FName LevelName);
	
	//SaveGame and LoadGame
	FPlayerProfile SPlayerProfileInfo;

	UPROPERTY()
	UGdrPlayerProfileSaveGame* SGPlayerProfile;

	UPROPERTY()
	FString SlotName;
	
	void CheckForSavedProfile();
	void LoadProfile();
	void SaveProfile();
	//SaveGame and LoadGame END

	void ChangePlayerName(FText PlayerName);
	bool ChangeConnectionType();
	
	void CreateMpSession(int32 NumPublicConnections);

protected:
	virtual void Init() override;

private:
	//Create Session
	TWeakPtr<IOnlineSession> SessionInterface;

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteHandle;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
};
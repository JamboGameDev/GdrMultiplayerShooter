// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GdrGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "SaveData/GdrPlayerProfileSaveGame.h"
#include "Widgets/GdrMainMenuWidget.h"

//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::Init()
{
	Super::Init();
	SlotName = TEXT("PlayerProfileSlot");
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::UIShowMenu()
{
	if (IsValid(WidgetClass))
	{
		if (UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass))
		{
			Widget->AddToViewport();

			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
			{
				//MenuWidget
				if (UGdrMainMenuWidget* MenuWidget  = Cast<UGdrMainMenuWidget>(Widget))
				{
					CheckForSavedProfile();

					if (MenuWidget->InputPlayerName)
					{
						MenuWidget->InputPlayerName->SetText(FText::FromString(SPlayerProfileInfo.PlayerName.ToString()));
						FInputModeUIOnly InputMode;
						InputMode.SetWidgetToFocus(MenuWidget->InputPlayerName->TakeWidget());
						InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
						PlayerController->SetInputMode(InputMode);
					}
					else
					{
						FInputModeUIOnly InputMode;
						Widget->SetIsFocusable(true);
						InputMode.SetWidgetToFocus(Widget->TakeWidget());
						InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
						PlayerController->SetInputMode(InputMode);
					}
				}
		
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::UIShowMPMenu()
{
	if (IsValid(MpMenuWidgetClass))
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, MpMenuWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::StartSoloGame(FName LevelName)
{
	if (IsSoloGame)
	{
		if (!LevelName.IsNone())
		{
			UGameplayStatics::OpenLevel(this, LevelName);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::CheckForSavedProfile()
{
	constexpr int32 UserIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		LoadProfile();
	}
	SaveProfile();
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::LoadProfile()
{
	constexpr int32 UserIndex = 0;

	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	if (!LoadedGame) return;

	UGdrPlayerProfileSaveGame* PlayerProfileSaveGame = Cast<UGdrPlayerProfileSaveGame>(LoadedGame);
	if (!PlayerProfileSaveGame) return;

	SPlayerProfileInfo = PlayerProfileSaveGame->SPlayerProfile;

	SGPlayerProfile = PlayerProfileSaveGame;
	
	UE_LOG(LogTemp, Warning, TEXT("Loaded player name: %s"), *SPlayerProfileInfo.PlayerName.ToString());
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::SaveProfile()
{
	if (!IsValid(SGPlayerProfile))
	{
		SGPlayerProfile = Cast<UGdrPlayerProfileSaveGame>(UGameplayStatics::CreateSaveGameObject(UGdrPlayerProfileSaveGame::StaticClass()));
	}

	if (IsValid(SGPlayerProfile))
	{
		SGPlayerProfile->SPlayerProfile = SPlayerProfileInfo;

		constexpr int32 UserIndex = 0;
		const bool bSaveSuccessful = UGameplayStatics::SaveGameToSlot(SGPlayerProfile, SlotName, UserIndex);
		
		if (!bSaveSuccessful)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save player profile!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Player profile saved successfully."));
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::ChangePlayerName(FText PlayerName)
{
	SPlayerProfileInfo.PlayerName = PlayerName;

	SaveProfile();

	UE_LOG(LogTemp, Warning, TEXT("Saving player name: %s"), *PlayerName.ToString());
}
//-------------------------------------------------------------------------------------------------------------------------------
bool UGdrGameInstance::ChangeConnectionType()
{
	if (IsLanConnection)
	{
		IsLanConnection = false;
	}
	else
	{
		IsLanConnection = true;
	}
	return IsLanConnection;
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::CreateMpSession(int32 NumPublicConnections)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			const auto ExitingSession = SessionInterface.Pin()->GetNamedSession(NAME_GameSession);
			if (ExitingSession)
			{
				SessionInterface.Pin()->DestroySession(NAME_GameSession);
			}

			OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UGdrGameInstance::OnCreateSessionComplete);
			OnCreateSessionCompleteHandle = SessionInterface.Pin()->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = IsLanConnection;
			SessionSettings.NumPublicConnections = NumPublicConnections;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;

			SessionInterface.Pin()->CreateSession(4, NAME_GameSession, SessionSettings);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void UGdrGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyManuMap"), true, "listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession failed!"));
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
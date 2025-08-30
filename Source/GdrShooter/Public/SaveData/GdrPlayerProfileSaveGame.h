// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GdrStructPlayerProfile.h"
#include "GameFramework/SaveGame.h"
#include "GdrPlayerProfileSaveGame.generated.h"


UCLASS()
class GDRSHOOTER_API UGdrPlayerProfileSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPlayerProfile SPlayerProfile;
	
};

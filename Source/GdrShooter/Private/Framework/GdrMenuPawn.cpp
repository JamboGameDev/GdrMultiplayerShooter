// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GdrMenuPawn.h"

// Sets default values
AGdrMenuPawn::AGdrMenuPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGdrMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGdrMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGdrMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


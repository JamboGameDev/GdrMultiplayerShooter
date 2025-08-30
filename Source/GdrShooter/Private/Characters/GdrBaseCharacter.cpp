// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GdrBaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AGdrBaseCharacter::AGdrBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCameraComponent"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArm);
	ThirdPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ThirdPersonCameraComponent->bUsePawnControlRotation = false;

	PistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol"));
	PistolMesh->SetupAttachment(GetMesh(), "weapon_r");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGdrBaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGdrBaseCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGdrBaseCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGdrBaseCharacter::StopJump);
		EnhancedInputComponent->BindAction(Ads, ETriggerEvent::Triggered, this, &AGdrBaseCharacter::StartAiming);
		EnhancedInputComponent->BindAction(Ads, ETriggerEvent::Completed, this, &AGdrBaseCharacter::StopAiming);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Started, this, &AGdrBaseCharacter::StartFire);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Completed, this, &AGdrBaseCharacter::StopFire);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AGdrBaseCharacter::ToggleCrouch);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	// Добавляем контекст ввода для локального игрока
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGdrBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGdrBaseCharacter, bIsAds);
}

//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StartJump()
{
	Jump();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StopJump()
{
	StopJumping();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StartAiming()
{
	if (HasAuthority())
	{
		bIsAds = true; // Прямое изменение на сервере
	}
	else
	{
		SvrSetIsAds(true); // RPC-запрос к серверу от клиента
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Starting Aiming"));
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StopAiming()
{
	if (HasAuthority())
	{
		bIsAds = false;
	}
	else
	{
		SvrSetIsAds(false);
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Stop Aiming"));
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::OnRep_IsAds()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Yellow, TEXT("OnRep"));
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::SpawnMuzzleFlash()
{
	if (PistolMesh && MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash,                         // Эффект частиц
			PistolMesh,                          // К чему прикрепить
			FName("Muzzle"),                     // Имя сокета
			FVector::ZeroVector,                 // Смещение позиции
			FRotator::ZeroRotator,               // Смещение вращения
			EAttachLocation::SnapToTarget,       // Привязка
			true                                 // Автоуничтожение
		);
	}

	
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}

	
	if (!PistolMesh) return;

	FTransform MuzzleTransform = PistolMesh->GetSocketTransform(FName("Muzzle"));
	FVector Start = MuzzleTransform.GetLocation();
	FVector End = Start + (MuzzleTransform.GetRotation().GetForwardVector() * 10000.0f);

	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Ignore Self

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		Params
	);
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Green, false, 1.0f);
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::SvrSetIsAds_Implementation(bool IsAds)
{
	bIsAds = IsAds;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StartFire()
{
	SpawnMuzzleFlash();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void AGdrBaseCharacter::StopFire()
{
	
}
//-------------------------------------------------------------------------------------------------------------------------------------

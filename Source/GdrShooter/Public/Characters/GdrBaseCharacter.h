// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GdrBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;
class USoundBase;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GDRSHOOTER_API AGdrBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGdrBaseCharacter();
	
	UPROPERTY(ReplicatedUsing = OnRep_IsAds, EditAnywhere, BlueprintReadOnly, Category = "Ads")
	bool bIsAds = false;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* Ads;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | Input")
	UInputAction* Fire;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void StartJump();
	
	void StopJump();

	void StartAiming();
	void StopAiming();

	void StartFire();
	void StopFire();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PistolMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;
	
	void SpawnMuzzleFlash();

	UFUNCTION(Server, Unreliable)
	void SvrSetIsAds(bool IsAds);

	UFUNCTION()
	void OnRep_IsAds();

};

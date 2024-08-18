// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class ADVMOVETUTORIAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

private:
	void HorizontalMove(float value);
	void VerticalMove(float value);
	void HorizontalRot(float value);
	void VerticalRot(float value);

	void CheckJump();

	void Zoom(float value);
	void Switch();

	void Sprint();

	UPROPERTY()
		bool jumping;
	UPROPERTY()
		int jumpCount;

	UPROPERTY()
		bool firstPerson;

	UPROPERTY()
		UCameraComponent* cam;
	UPROPERTY()
		USpringArmComponent* arm;

	void SwitchCharacter();

	UPROPERTY(EditAnywhere, Category = "Possession")
		AMyCharacter* otherCharacter;

	UPROPERTY()
		float speed;
	UPROPERTY()
		bool walking;
};

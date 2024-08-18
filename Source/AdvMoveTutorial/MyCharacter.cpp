// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//cam->AttachTo(RootComponent);

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	arm->AttachTo(RootComponent);
	arm->TargetArmLength = 300.f;
	arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	arm->bEnableCameraLag = true;
	arm->CameraLagSpeed = 2;
	arm->CameraLagMaxDistance = 1.5f;

	arm->bEnableCameraRotationLag = true;
	arm->CameraRotationLagSpeed = 4;
	arm->CameraLagMaxTimeStep = 1;

	cam->AttachTo(arm, USpringArmComponent::SocketName);

	jumping = false;
	jumpCount = 0;

	firstPerson = false;

	speed = 0.5f;
	walking = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (jumping)
	{
		Jump();
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Horizontal", this, &AMyCharacter::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &AMyCharacter::VerticalMove);
	InputComponent->BindAxis("HorizontalRot", this, &AMyCharacter::HorizontalRot);
	InputComponent->BindAxis("VerticalRot", this, &AMyCharacter::VerticalRot);

	InputComponent->BindAxis("Zoom", this, &AMyCharacter::Zoom);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::CheckJump);

	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AMyCharacter::Switch);

	InputComponent->BindAction("Switch", IE_Pressed, this, &AMyCharacter::SwitchCharacter);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::Sprint);
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	jumpCount = 0;
}

void AMyCharacter::Sprint()
{
	walking = !walking;
	if (walking)
	{
		speed = 0.5f;
	}
	else
	{
		speed = 1;
	}
}

void AMyCharacter::CheckJump()
{
	if (jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
		jumpCount++;
		if (jumpCount == 2)
		{
			LaunchCharacter(FVector(0, 0, 500), false, true);
		}
	}
}

void AMyCharacter::VerticalRot(float value)
{
	if (value)
	{
		float temp = 0;
		if (firstPerson)
		{
			temp = cam->GetRelativeRotation().Pitch + value;
			if (temp < 65 && temp > -65)
			{
				cam->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
		else
		{
			temp = arm->GetRelativeRotation().Pitch + value;
			if (temp < 25 && temp > -65)
			{
				arm->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
	}
}

void AMyCharacter::Zoom(float value)
{
	if (value)
	{
		float temp = arm->TargetArmLength + (value * -10);
		if (temp < 310 && temp > 140)
		{
			arm->TargetArmLength = temp;
		}
	}
}

void AMyCharacter::Switch()
{
	if (firstPerson)
	{
		arm->TargetArmLength = 300.f;
		arm->SetRelativeRotation(FRotator(-45, 0, 0));
		cam->SetRelativeRotation(FRotator(0, 0, 0));
		cam->AttachTo(arm, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		cam->AttachTo(RootComponent);
		firstPerson = true;
	}
}

void AMyCharacter::HorizontalRot(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void AMyCharacter::HorizontalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value * speed);
	}
}

void AMyCharacter::VerticalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value * speed);
	}
}

void AMyCharacter::SwitchCharacter()
{
	if (otherCharacter && GetController())
	{
		AController* temp = GetController();
		if(temp)
		{
			temp->UnPossess();
			temp->Possess(otherCharacter);
		}
	}
}


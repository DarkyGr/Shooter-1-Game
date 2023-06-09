// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Max Health to Actor
	Health = MaxHealth;
	
	// Get Spawn Actor (Gun) and set to Gun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// Hide The gun of skeleton
	GetMesh()->HideBoneByName(TEXT("weapon_R"), EPhysBodyOp::PBO_None);

	// Set the new socket was created in skeleton
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

	// Set the owner to gun
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set Player Input (MoveForward WS)
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);

	// Set Player Mouse on Y (Up & Down)
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	// Set Player Input (MoveForward AD)
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	// Set Player Mouse on X (Right & Left)
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// Set Player Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// Set Player Gamepad on Y (Up & Down)
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);

	// Set Player Gamepad on X (Right & Left)
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	// Set Left Mouse and Gamepad Tight Trigger for Shoot!
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// Damage Applied to Actor
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);		
	
	DamageToApply = FMath::Min(Health, DamageToApply);

	// Rest the Damage Applied of Helath
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	// Remove Capsule of collision for the Shooter Character
	if (IsDead())
	{
		// Set our Game mode 
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr)
		{
			// Set Pawn Killed
			GameMode->PawnKilled(this);
		}	

		// Destroy Controller
		DetachFromControllerPendingDestroy();

		// Disbale Collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
	}

	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	// Set Axis Move with value
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	// Set Axis Move with value
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	// Set Speed based on the World
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	// Set Speed based on the World
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	// Set Controller Pitch Input
// 	AddControllerPitchInput(AxisValue);
// }
// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FernGullyLike.h"
#include "FernGullyLikeCharacter.h"
#include "FernGullyLikeProjectile.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

const FName AFernGullyLikeCharacter::FireUpBinding("FireUp");
const FName AFernGullyLikeCharacter::FireRightBinding("FireRight");

AFernGullyLikeCharacter::AFernGullyLikeCharacter()
{
  // Cache our sound effect
  //static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
  //FireSound = FireAudio.Object;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 42.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 750.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera


  GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
  GetCharacterMovement()->GravityScale = 0.f;
  GetCharacterMovement()->AirControl = 1.f;
  GetCharacterMovement()->BrakingDecelerationFlying = 750.f;
  GetCharacterMovement()->MaxFlySpeed = 600.f;
  GetCharacterMovement()->SetMovementMode(MOVE_Flying);
  GetCharacterMovement()->SetGroundMovementMode(MOVE_Flying);

  // Movement
  // Weapon
  FireRate = 0.1f;
  bCanFire = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFernGullyLikeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
  // set up gameplay key bindings
  InputComponent->BindAxis("MoveRight", this, &AFernGullyLikeCharacter::MoveRight);
  InputComponent->BindAxis("MoveUp", this, &AFernGullyLikeCharacter::MoveUp);
  InputComponent->BindAxis(FireRightBinding);
  InputComponent->BindAxis(FireUpBinding);
}

void AFernGullyLikeCharacter::MoveRight(float Value)
{
  // add movement in that direction
  AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AFernGullyLikeCharacter::MoveUp(float Value)
{
  // add movement in that direction
  AddMovementInput(FVector(0.f, 0.f, 1.f), Value);
}

void AFernGullyLikeCharacter::Tick(float DeltaSeconds)
{

  // Create fire direction vector
  const float FireForwardValue = GetInputAxisValue(FireUpBinding);
  const float FireRightValue = GetInputAxisValue(FireRightBinding);
  const FVector FireDirection = FVector(0.f, FireRightValue, FireForwardValue);

  // Try and fire a shot
  FireShot(FireDirection);
}

void AFernGullyLikeCharacter::FireShot(FVector FireDirection)
{
  // If we it's ok to fire again
  if (bCanFire == true)
  {
    // If we are pressing fire stick in a direction
    if (FireDirection.SizeSquared() > 0.0f)
    {
      const FRotator FireRotation = FireDirection.Rotation();
      // Spawn projectile at an offset from this pawn
      const FVector SpawnLocation = GetActorLocation();

      const FVector EndLocation = SpawnLocation - FireDirection * 10000.f;

      UWorld* const World = GetWorld();
      if (World != NULL)
      {
        // spawn the projectile
        World->SpawnActor<AFernGullyLikeProjectile>(SpawnLocation, FireRotation);
        DrawDebugLine(World, SpawnLocation, EndLocation, FColor(255, 0, 0), false, 5.f, 0, 50.f);
      }

      bCanFire = false;
      World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AFernGullyLikeCharacter::ShotTimerExpired, FireRate);

      // try and play the sound if specified
      if (FireSound != nullptr)
      {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
      }

      bCanFire = false;
    }
  }
}

void AFernGullyLikeCharacter::ShotTimerExpired()
{
  bCanFire = true;
}
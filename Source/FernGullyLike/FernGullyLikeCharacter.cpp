// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FernGullyLike.h"
#include "FernGullyLikeCharacter.h"
#include "FernGullyLikeProjectile.h"
#include "TimerManager.h"

const FName AFernGullyLikeCharacter::MoveForwardBinding("MoveForward");
const FName AFernGullyLikeCharacter::MoveRightBinding("MoveRight");
const FName AFernGullyLikeCharacter::FireForwardBinding("FireForward");
const FName AFernGullyLikeCharacter::FireRightBinding("FireRight");

AFernGullyLikeCharacter::AFernGullyLikeCharacter()
{
  // Cache our sound effect
  //static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
  //FireSound = FireAudio.Object;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera



  GetCharacterMovement()->GravityScale = 0.f;
  GetCharacterMovement()->AirControl = 1.f;
  GetCharacterMovement()->SetMovementMode(MOVE_Flying);
  GetCharacterMovement()->SetGroundMovementMode(MOVE_Flying);

  // Movement
  MoveSpeed = 1000.0f;
  // Weapon
  GunOffset = FVector(0.f, 90.f, 0.f);
  FireRate = 0.1f;
  bCanFire = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input
void AFernGullyLikeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
  check(InputComponent);

  // set up gameplay key bindings
  InputComponent->BindAxis(MoveForwardBinding);
  InputComponent->BindAxis(MoveRightBinding);
  InputComponent->BindAxis(FireForwardBinding);
  InputComponent->BindAxis(FireRightBinding);
}

void AFernGullyLikeCharacter::Tick(float DeltaSeconds)
{
  // Find movement direction
  const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
  const float RightValue = GetInputAxisValue(MoveRightBinding);

  // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
  const FVector MoveDirection = FVector(0.f, -RightValue, ForwardValue).GetClampedToMaxSize(1.0f);

  // Calculate  movement
  Movement = MoveDirection * MoveSpeed * DeltaSeconds;

  // If non-zero size, move this actor
  if (Movement.SizeSquared() > 0.0f)
  {
    const FRotator NewRotation = Movement.Rotation();
    FHitResult Hit(1.f);
    RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

    if (Hit.IsValidBlockingHit())
    {
      const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
      const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
      RootComponent->MoveComponent(Deflection, NewRotation, true);
    }
  }

  // Create fire direction vector
  const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
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
      const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

      UWorld* const World = GetWorld();
      if (World != NULL)
      {
        // spawn the projectile
        World->SpawnActor<AFernGullyLikeProjectile>(SpawnLocation, FireRotation);
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
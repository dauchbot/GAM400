// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "FernGullyLikeCharacter.generated.h"

UCLASS(config=Game)
class AFernGullyLikeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

  /** Offset from the ships location to spawn projectiles */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector GunOffset;

  /** Offset from the ships location to spawn projectiles */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector Movement;

  /* How fast the weapon will fire */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float FireRate;

  /* The speed our ship moves around the level */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

  /** Sound to play each time we fire */
  UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
  class USoundBase* FireSound;

  // Begin Actor Interface
  virtual void Tick(float DeltaSeconds) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
  // End Actor Interface

  /* Fire a shot in the specified direction */
  void FireShot(FVector FireDirection);

  /* Handler for the fire timer expiry */
  void ShotTimerExpired();

  // Static names for axis bindings
  static const FName MoveForwardBinding;
  static const FName MoveRightBinding;
  static const FName FireForwardBinding;
  static const FName FireRightBinding;

private:

  /* Flag to control firing  */
  uint32 bCanFire : 1;

  /** Handle for efficient management of ShotTimerExpired timer */
  FTimerHandle TimerHandle_ShotTimerExpired;


public:
	AFernGullyLikeCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

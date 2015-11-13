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

  /* How fast the weapon will fire */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float FireRate;

  /* Where is the laser reaching to */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector LaserEnd;

  /** Sound to play each time we fire */
  UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
  class USoundBase* FireSound;

  // Begin Actor Interface
  virtual void Tick(float DeltaSeconds) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
  // End Actor Interface

  /** Called for side to side input */
  void MoveRight(float Val);

  /** Called for up-down input */
  void MoveUp(float Val);

  /* Fire a shot in the specified direction */
  void FireShot(FVector FireDirection);

  // Static names for axis bindings
  static const FName FireUpBinding;
  static const FName FireRightBinding;

  // Static name for trace params
  static const FName LaserTraceParams;

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

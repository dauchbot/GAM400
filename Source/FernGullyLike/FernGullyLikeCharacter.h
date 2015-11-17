// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "FernGullyLikeCharacter.generated.h"

#define EPSILON 0.000001f

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

  /* Timer for post damage invincibility */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float InvincibilityTimer;

  /* Max time for invincibility */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float InvincibilityTime;

  /* Where is the laser reaching to */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector LaserEnd;

  /* Collision Normal for bouncing away */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector collNormal;

  /* Last location */
  UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector lastPosition;

  /** Sound to play each time we fire */
  UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
  class USoundBase* FireSound;

  // Begin Actor Interface
  virtual void Tick(float DeltaSeconds) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  UFUNCTION(Category = Collision, BlueprintCallable)
  virtual void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
  // End Actor Interface

  /* Handle Collision */
  void CheckMovement(FVector& Movement);

  /* Fire a shot in the specified direction */
  void FireShot(FVector FireDirection);

  // Static names for axis bindings
  static const FName MoveUpBinding;
  static const FName MoveRightBinding;
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

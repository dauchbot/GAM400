// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FernGullyLike.h"
#include "FernGullyLikeGameMode.h"
#include "FernGullyLikeCharacter.h"

AFernGullyLikeGameMode::AFernGullyLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

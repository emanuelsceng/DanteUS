// Copyright Epic Games, Inc. All Rights Reserved.

#include "DanteUSGameMode.h"
#include "DanteUSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADanteUSGameMode::ADanteUSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

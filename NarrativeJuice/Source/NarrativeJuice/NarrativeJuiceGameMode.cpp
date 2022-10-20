// Copyright Epic Games, Inc. All Rights Reserved.

#include "NarrativeJuiceGameMode.h"
#include "NarrativeJuiceCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANarrativeJuiceGameMode::ANarrativeJuiceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

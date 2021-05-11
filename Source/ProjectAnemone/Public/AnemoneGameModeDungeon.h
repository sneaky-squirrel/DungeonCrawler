// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AnemoneGameModeDungeon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API AAnemoneGameModeDungeon : public AGameModeBase
{
	GENERATED_BODY()

	UFUNCTION( BlueprintCallable )
	AAnemoneDungeonLevel* FetchDungeon();
};

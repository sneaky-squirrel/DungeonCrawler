// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "AnemoneGameData.h"
#include "AnemoneGameModeDungeon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogDungeonMode, Log, All );

enum class EAnemoneDirection : uint8;
using NameList = TArray< FName >;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API AAnemoneGameStateDungeon : public AGameStateBase
{
	GENERATED_BODY()

public:
};
using DungeonState = AAnemoneGameStateDungeon;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API AAnemoneGameModeDungeon : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	AAnemoneDungeonLevel* FetchDungeon();

	AAnemoneGameModeDungeon();

protected:
	DungeonState* GameStateDungeon;

	DungeonState* GetGameState();
};
using DungeonMode = AAnemoneGameModeDungeon;

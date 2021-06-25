// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnemoneGameModeDungeon.h"
#include "AnemoneDungeonLevel.h"
#include "AnemoneGameStateSubsystem.h"
#include "AnemoneEncounterEntity.h"
#include "AnemoneWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY( LogDungeonMode );

DungeonMode::AAnemoneGameModeDungeon()
{
	GameStateClass = DungeonState::StaticClass();
}

AAnemoneDungeonLevel* DungeonMode::FetchDungeon()
{
	if ( !GetWorld() || !GetWorld()->GetCurrentLevel() )
	{
		return nullptr;
	}
	AAnemoneDungeonLevel* Dungeon = Cast<AAnemoneDungeonLevel>( GetWorld()->GetCurrentLevel()->GetWorldSettings() );
	return Dungeon;
}

DungeonState* DungeonMode::GetGameState()
{
	if( GameStateDungeon )
	{
		return GameStateDungeon;
	}
	GameStateDungeon = AGameModeBase::GetGameState< DungeonState >();
	return GameStateDungeon;
}

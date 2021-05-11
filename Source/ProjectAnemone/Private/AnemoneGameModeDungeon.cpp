// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnemoneGameModeDungeon.h"
#include "AnemoneDungeonLevel.h"

AAnemoneDungeonLevel* AAnemoneGameModeDungeon::FetchDungeon()
{
	if ( !GetWorld() || !GetWorld()->GetCurrentLevel() )
	{
		return nullptr;
	}
	AAnemoneDungeonLevel* Dungeon = Cast<AAnemoneDungeonLevel>( GetWorld()->GetCurrentLevel()->GetWorldSettings() );
	return Dungeon;
}

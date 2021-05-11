#include "AnemoneHexTile.h"
#include "AnemoneDungeonLevel.h"

void AAnemoneHexTile::OnConstruction( const FTransform& Transform )
{
    AAnemoneDungeonLevel* DungeonLevel = FetchDungeonLevel();
    if( DungeonLevel != nullptr )
    {
        DungeonLevel->AddTileToGrid( this );
    }
}

void AAnemoneHexTile::Destroyed()
{
    Super::Destroyed();
    AAnemoneDungeonLevel* DungeonLevel = FetchDungeonLevel();
    if( DungeonLevel != nullptr )
    {
        DungeonLevel->RemoveTileFromGrid( this );
    }
}

#if WITH_EDITOR
void AAnemoneHexTile::PostLoad()
{
    Super::PostLoad();
    bExistsInLevel = true;
}

void AAnemoneHexTile::PostEditMove( bool bFinished )
{
    Super::PostEditMove( bFinished );
    AAnemoneDungeonLevel* DungeonLevel = FetchDungeonLevel();
    if( bFinished )
    {
        if( DungeonLevel != nullptr )
        {
            DungeonLevel->AddTileToGrid( this );
        }
    }
}
#endif

AAnemoneDungeonLevel* AAnemoneHexTile::FetchDungeonLevel()
{
    AAnemoneDungeonLevel* DungeonLevel = nullptr;
    if( GetLevel() != nullptr )
    {
        DungeonLevel = Cast< AAnemoneDungeonLevel >( GetLevel()->GetWorldSettings() );
    }
    return DungeonLevel;
}

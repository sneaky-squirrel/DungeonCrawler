#include "AnemoneHexTile.h"
#include "AnemoneDungeonLevel.h"

void AAnemoneHexTile::OnConstruction( const FTransform& Transform )
{
    Super::OnConstruction( Transform );
    AAnemoneDungeonLevel* DungeonLevel = FetchDungeonLevel();
    if( DungeonLevel )
    {
        DungeonLevel->AddTileToGrid( this );
    }
}

void AAnemoneHexTile::Destroyed()
{
    AAnemoneDungeonLevel* DungeonLevel = FetchDungeonLevel();
    if( DungeonLevel )
    {
        DungeonLevel->RemoveTileFromGrid( this );
    }
    Super::Destroyed();
}

void AAnemoneHexTile::ExtractCoordinates( int32& InQ, int32& InR, int32& InH, int32& InAbsQ, int32& InAbsR, int32& InAbsH ) const
{
    InQ = Coordinates.Q;
    InR = Coordinates.R;
    InH = Coordinates.H;
    InAbsQ = FMath::Abs< int32 >( InQ );
    InAbsR = FMath::Abs< int32 >( InR );
    InAbsH = FMath::Abs< int32 >( InH );
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

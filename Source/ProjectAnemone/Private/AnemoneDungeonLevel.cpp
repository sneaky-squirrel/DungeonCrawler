#include "AnemoneDungeonLevel.h"
#include "AnemoneHexTile.h"

FAnemoneStartPosition::FAnemoneStartPosition()
{
}

FAnemoneDungeonPosition::FAnemoneDungeonPosition()
{
}

FAnemoneDungeonTileHeight::FAnemoneDungeonTileHeight()
{
}

FAnemoneDungeonTileRow::FAnemoneDungeonTileRow()
{
}

AAnemoneDungeonLevel::AAnemoneDungeonLevel()
{
}

bool AAnemoneDungeonLevel::AddTileToGrid( AAnemoneHexTile* InTile )
{
    FAxialCoordinates NewCoordinates = ConvertPositionToAxialCoordinates( InTile->GetActorLocation() );
    if( InTile->bExistsInLevel )
    {
        RemoveTileFromMap( InTile->Coordinates );
    }
    if( !AreCoordinatesEmpty( NewCoordinates.Q, NewCoordinates.R, NewCoordinates.H ) )
    {
    	UE_LOG( LogTemp, Warning, TEXT( "Different Tile." ) );
        FCubeCoordinates CenterCoordinates( NewCoordinates );
        FCubeCoordinates SpiralSearchCoordinates;
        if( !FindValidTileInSpiral( CenterCoordinates, SpiralSearchCoordinates ) )
        {
            return false;
        }
        NewCoordinates = SpiralSearchCoordinates;
    }
    UE_LOG( LogTemp, Warning, TEXT( "New Coordinates = Q: %d | R: %d | H: %d" ), NewCoordinates.Q, NewCoordinates.R, NewCoordinates.H );
    InTile->Coordinates = NewCoordinates;
    InTile->bExistsInLevel = true;
    AddTileToMap( InTile );
    SnapTileToPosition( InTile );
    return true;
}

void AAnemoneDungeonLevel::RemoveTileFromGrid( AAnemoneHexTile* InTile )
{
    UE_LOG( LogTemp, Warning, TEXT( "RemoveFromGrid" ) );
    RemoveTileFromMap( InTile->Coordinates );
}

AAnemoneHexTile* AAnemoneDungeonLevel::MovePlayer( bool bMoveForward )
{
    FCubeCoordinates P = CurrentPlayerPosition.Coordinates;
    switch( CurrentPlayerPosition.FacingAngle )
    {
        case EDirectionHexagonal::East:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 0 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 3 ] );
            }
            break;
        case EDirectionHexagonal::Southeast:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 5 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 2 ] );
            }
            break;
        case EDirectionHexagonal::Southwest:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 4 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 1 ] );
            }
            break;
        case EDirectionHexagonal::Northwest:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 2 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 5 ] );
            }
            break;
        case EDirectionHexagonal::West:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 3 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 0 ] );
            }
            break;
        case EDirectionHexagonal::Northeast:
            if( bMoveForward )
            {
                return MovePlayerTowardsDirection( CubeDirections[ 1 ] );
            }
            else
            {
                return MovePlayerTowardsDirection( CubeDirections[ 4 ] );
            }
            break;
    default:
            UE_LOG( LogTemp, Warning,
            TEXT( "bool AAnemoneDungeonLevel::MovePlayer( bool bMoveForward ).\nError: Default Switch Case." ) );
        break;
    }
    return nullptr;
}

void AAnemoneDungeonLevel::RotatePlayer( bool bRotateClockwise )
{
    switch ( CurrentPlayerPosition.FacingAngle )
    {
        case EDirectionHexagonal::East:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Southeast;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Northeast;
            }
            break;
        case EDirectionHexagonal::Southeast:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Southwest;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::East;
            }
            break;
        case EDirectionHexagonal::Southwest:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::West;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Southeast;
            }
            break;
        case EDirectionHexagonal::West:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Northwest;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Southwest;
            }
            break;
        case EDirectionHexagonal::Northwest:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Northeast;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::West;
            }
            break;
        case EDirectionHexagonal::Northeast:
            if ( bRotateClockwise )
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::East;
            }
            else
            {
                CurrentPlayerPosition.FacingAngle = EDirectionHexagonal::Northwest;
            }
            break;
        default:
            break;
    }
}

void AAnemoneDungeonLevel::PlayerInteract()
{
    switch ( CurrentPlayerPosition.FacingAngle )
    {
        case EDirectionHexagonal::East:
            return ActivateTileTowardsDirection( CubeDirections[ 0 ] );
            break;
        case EDirectionHexagonal::Southeast:
            return ActivateTileTowardsDirection( CubeDirections[ 5 ] );
            break;
        case EDirectionHexagonal::Southwest:
            return ActivateTileTowardsDirection( CubeDirections[ 4 ] );
            break;
        case EDirectionHexagonal::West:
            return ActivateTileTowardsDirection( CubeDirections[ 3 ] );
            break;
        case EDirectionHexagonal::Northwest:
            return ActivateTileTowardsDirection( CubeDirections[ 2 ] );
            break;
        case EDirectionHexagonal::Northeast:
            return ActivateTileTowardsDirection( CubeDirections[ 1 ] );
            break;
    }
}

FAxialCoordinates AAnemoneDungeonLevel::ConvertPositionToAxialCoordinates( const FVector& InTilePosition )
{
    FAxialCoordinates NewCoordinates;
	NewCoordinates.H = FMath::RoundToInt( InTilePosition.Z / TileHeight );
	NewCoordinates.R = FMath::RoundToInt( InTilePosition.Y * 2.0 / 3.0 / TileSize );
	NewCoordinates.Q = FMath::RoundToInt( ( InTilePosition.X / TileSize - FMath::Sqrt( 3.0 ) / 2 * NewCoordinates.R ) / FMath::Sqrt( 3.0 ) );
    return NewCoordinates;
}

bool AAnemoneDungeonLevel::AreCoordinatesEmpty( int32 InQ, int32 InR, int32 InH )
{
    if( !Grid.Contains( InH ) ||
        !Grid[ InH ].Height.Contains( InR ) ||
        !Grid[ InH ].Height[ InR ].Row.Contains( InQ ) )
    {
        return true;
    }
    return false;
}

bool AAnemoneDungeonLevel::AreCoordinatesEmpty( const FCubeCoordinates& InCube )
{
    if( !Grid.Contains( InCube.H ) ||
        !Grid[ InCube.H ].Height.Contains( InCube.Z ) ||
        !Grid[ InCube.H ].Height[ InCube.Z ].Row.Contains( InCube.X ) )
    {
        return true;
    }
    return false;
}

void AAnemoneDungeonLevel::SnapTileToPosition( AAnemoneHexTile* InTile )
{
    int32 Q, R, H, AbsQ, AbsR, AbsH;
    FVector Location;
    FString NewName;
    InTile->ExtractCoordinates( Q, R, H, AbsQ, AbsR, AbsH );
    NewName.Appendf( TEXT("Cell_%s%d%d%d_%s%d%d%d_%s%d%d%d"),
                    ( Q < 0 ) ? TEXT("-") : TEXT("+"), AbsQ / 100, AbsQ % 100 / 10, AbsQ % 10,
                    ( R < 0 ) ? TEXT("-") : TEXT("+"), AbsR / 100, AbsR % 100 / 10, AbsR % 10,
                    ( H < 0 ) ? TEXT("-") : TEXT("+"), AbsH / 100, AbsH % 100 / 10, AbsH % 10 );

    Location.X = ( FMath::Sqrt( 3.0 ) * Q + FMath::Sqrt( 3.0 ) / 2 * R ) * TileSize;
    Location.Y = 3.0 / 2.0 * R * TileSize;
    Location.Z = H * TileHeight;
    InTile->SetActorLocation( Location );
#if WITH_EDITOR
    InTile->SetActorLabel( *NewName );
#endif
}

bool AAnemoneDungeonLevel::FindValidTileInSpiral( FCubeCoordinates InCenterCube, FCubeCoordinates& OutCube )
{
    for( int32 Radius = 1; Radius < 99; Radius++ )
    {
        if( FindValidTileInRing( InCenterCube, Radius, OutCube ) )
        {
            return true;
        }
    }
    return false;
}

bool AAnemoneDungeonLevel::FindValidTileInRing( FCubeCoordinates InCenterCube, int32 InRadius, FCubeCoordinates& OutCube )
{
    FCubeCoordinates Navigator = InCenterCube + ( Cube_Direction( 4 ) * InRadius );
    for( int32 i = 0; i < 6; ++i )
    {
        for( int32 j = 0; j < InRadius; ++j )
        {
            if( AreCoordinatesEmpty( Navigator.X, Navigator.Z, Navigator.H ) )
            {
                OutCube = Navigator;
                return true;
            }
            Navigator = Cube_Neighbor( Navigator, i );
        }
    }
    return false;
}

void AAnemoneDungeonLevel::AddTileToMap( AAnemoneHexTile* InTile )
{
    int32 Q = InTile->Coordinates.Q;
    int32 R = InTile->Coordinates.R;
    int32 H = InTile->Coordinates.H;
    if( !Grid.Contains( H ) )
    {
        Grid.Emplace( H, FAnemoneDungeonTileHeight() );
    }
    if( !Grid[ H ].Height.Contains( R ) )
    {
        Grid[ H ].Height.Emplace( R, FAnemoneDungeonTileRow() );
    }
    if( !Grid[ H ].Height[ R ].Row.Contains( Q ) )
    {
        Grid[ H ].Height[ R ].Row.Emplace( Q, InTile );
    }
}

void AAnemoneDungeonLevel::RemoveTileFromMap( const FAxialCoordinates& InCoordinates )
{
   UE_LOG( LogTemp, Warning, TEXT( "RemoveFromMap" ) );
   if( Grid.Contains( InCoordinates.H ) &&
        Grid[ InCoordinates.H ].Height.Contains( InCoordinates.R ) &&
        Grid[ InCoordinates.H ].Height[ InCoordinates.R ].Row.Contains( InCoordinates.Q ) )
    {
        Grid[ InCoordinates.H ].Height[ InCoordinates.R ].Row.Remove( InCoordinates.Q );
        if( !DoesRowContainTiles( InCoordinates.H, InCoordinates.R ) )
        {
            Grid[ InCoordinates.H ].Height.Remove( InCoordinates.R );
            if( !DoesHeightContainTiles( InCoordinates.H ) )
            {
                Grid.Remove( InCoordinates.H );
            }
        }
    }
}

bool AAnemoneDungeonLevel::DoesHeightContainTiles( int32 InHeightIndex )
{
    for( TPair< int32, FAnemoneDungeonTileRow > TileRow : Grid[ InHeightIndex ].Height )
    {
        if( DoesRowContainTiles( InHeightIndex, TileRow.Key ) )
        {
            return true;
        }
    }
    return false;
}

bool AAnemoneDungeonLevel::DoesRowContainTiles( int32 InHeightIndex, int32 InRowIndex )
{
    for( TPair< int32, AAnemoneHexTile* > Tile : Grid[ InHeightIndex ].Height[ InRowIndex ].Row )
    {
        if( Tile.Value != nullptr )
        {
            return true;
        }
    }
    return false;
}

AAnemoneHexTile* AAnemoneDungeonLevel::MovePlayerTowardsDirection( const FCubeCoordinates& InCubeVectorDirection )
{
    FCubeCoordinates NewPosition = CurrentPlayerPosition.Coordinates + InCubeVectorDirection;
    AAnemoneHexTile* FetchedTile = FetchTileAtAdjacentHeights( NewPosition );
    UE_LOG( LogTemp, Warning, TEXT( "Destination: X: %d, Y: %d, Z: %d" ),
            NewPosition.X,
            NewPosition.Z,
            NewPosition.H );
    if( FetchedTile != nullptr && FetchedTile->TileType == EAnemoneTileType::Navigable )
    {
        CurrentPlayerPosition.Coordinates = FetchedTile->Coordinates;
        return FetchedTile;
    }
    return nullptr;
}

void AAnemoneDungeonLevel::ActivateTileTowardsDirection( const FCubeCoordinates& InCubeVectorDirection )
{
    FCubeCoordinates NewPosition = CurrentPlayerPosition.Coordinates + InCubeVectorDirection;
    AAnemoneHexTile* FetchedTile = FetchTileAtAdjacentHeights( NewPosition );
    UE_LOG( LogTemp, Warning, TEXT( "Interact With: X: %d, Y: %d, Z: %d" ),
            NewPosition.X,
            NewPosition.Z,
            NewPosition.H );
    if( FetchedTile != nullptr && FetchedTile->TileType == EAnemoneTileType::Interactive )
    {
        FetchedTile->ActivateInteraction();
    }
}

AAnemoneHexTile* AAnemoneDungeonLevel::FetchTileAtAdjacentHeights( const FCubeCoordinates& InCubeVectorCoordinate )
{
    FCubeCoordinates Below, Same, Above;
    Below = InCubeVectorCoordinate + FCubeCoordinates( 0, 0, 0, -1 );
    Same = InCubeVectorCoordinate;
    Above = InCubeVectorCoordinate + FCubeCoordinates( 0, 0, 0, 1 );
    if( !AreCoordinatesEmpty( Below ) )
    {
        return Grid[ Below.H ].Height[ Below.Z ].Row[ Below.X ];
    }
    if( !AreCoordinatesEmpty( Same ) )
    {
        return Grid[ Same.H ].Height[ Same.Z ].Row[ Same.X ];
    }
    if( !AreCoordinatesEmpty( Above ) )
    {
        return Grid[ Above.H ].Height[ Above.Z ].Row[ Above.X ];
    }
    return nullptr;
 }

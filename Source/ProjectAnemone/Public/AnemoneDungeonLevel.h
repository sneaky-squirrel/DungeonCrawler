#pragma once
#include "HexagonMath.h"
#include "AnemoneDungeonLevel.generated.h"

class AAnemoneHexTile;

USTRUCT( BlueprintType )
struct FAnemoneStartPosition
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	AAnemoneHexTile* Tile;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EDirectionHexagonal FacingAngle;

	FAnemoneStartPosition();
};

USTRUCT( BlueprintType )
struct FAnemoneDungeonPosition
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FAxialCoordinates Coordinates;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EDirectionHexagonal FacingAngle;

	FAnemoneDungeonPosition();
};

USTRUCT( BlueprintType )
struct FAnemoneDungeonTileRow
{
	GENERATED_BODY()

	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	TMap< int32, AAnemoneHexTile* > Row;
	FAnemoneDungeonTileRow();
};

USTRUCT( BlueprintType )
struct FAnemoneDungeonTileHeight
{
	GENERATED_BODY()

	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	TMap< int32, FAnemoneDungeonTileRow > Height;
	FAnemoneDungeonTileHeight();
};

/*
Container of all objects of an individual dungeon.  Such as the navigational grid, represented as a 3D TMap,
and interactable objects like doors, chests, and encounters.
*/
UCLASS( Blueprintable, BlueprintType )
class AAnemoneDungeonLevel : public AWorldSettings
{
	GENERATED_BODY()
public:
   	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "128.0", ClampMax = "512.0") )
	int32 TileSize = 128;

   	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "64.0", ClampMax = "512.0") )
	int32 TileHeight = 64;

	// 3D Hexagonal Grid
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TMap< int32, FAnemoneDungeonTileHeight > Grid;

   	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray< FAnemoneStartPosition > PlayerStartPositionList;

   	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FAnemoneDungeonPosition CurrentPlayerPosition;

	bool AddTileToGrid( AAnemoneHexTile* InTile );
	void RemoveTileFromGrid( AAnemoneHexTile* InTile );

	UFUNCTION( BlueprintCallable )
	AAnemoneHexTile* MovePlayer( bool bMoveForward );

	UFUNCTION( BlueprintCallable )
	void RotatePlayer( bool bRotateClockwise );

	UFUNCTION( BlueprintCallable )
	void PlayerInteract();

	AAnemoneDungeonLevel();
private:

	FAxialCoordinates ConvertPositionToAxialCoordinates( const FVector& InTilePosition );
	bool AreCoordinatesEmpty( int32 InQ, int32 InR, int32 InH );
	bool AreCoordinatesEmpty( const FCubeCoordinates& InCube );
	void SnapTileToPosition( AAnemoneHexTile* InTile );

	bool FindValidTileInSpiral( FCubeCoordinates InCenterCube, FCubeCoordinates& OutCube );
	bool FindValidTileInRing( FCubeCoordinates InCenterCube, int32 InRadius, FCubeCoordinates& OutCube );
	void AddTileToMap( AAnemoneHexTile* InTile );
	void RemoveTileFromMap( const FAxialCoordinates& InCoordinates );
	bool DoesHeightContainTiles( int32 InHeightIndex );
	bool DoesRowContainTiles( int32 InHeightIndex, int32 InRowIndex );

	AAnemoneHexTile* MovePlayerTowardsDirection( const FCubeCoordinates& InCubeVector );
	void ActivateTileTowardsDirection( const FCubeCoordinates& InCubeVector );
	AAnemoneHexTile* FetchTileAtAdjacentHeights( const FCubeCoordinates& InCubeVectorCoordinate );
};
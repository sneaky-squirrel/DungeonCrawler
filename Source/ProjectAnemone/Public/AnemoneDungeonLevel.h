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
	// Changes the Position of the Player inside the Dungeon Grid depending on the Direction the Player is Facing.
	UFUNCTION( BlueprintCallable )
	AAnemoneHexTile* MovePlayer( bool bMoveForward );

	//  Change the Direction the Player is Facing inside of the Dungeon Grid.
	UFUNCTION( BlueprintCallable )
	void RotatePlayer( bool bRotateClockwise );

	/*	Activate Interaction of Tile the Player is facing.
	*	e.g.  Opening a Door, Inspecting the environment, Activating a Switch, etc.
	*/
	UFUNCTION( BlueprintCallable )
	void PlayerInteract();

	AAnemoneDungeonLevel();

	// Add a new Tile to the Dungeon Grid.
	bool AddTileToGrid( AAnemoneHexTile* InTile );

	// Remove an existing Tile from the Dungeon Grid.
	void RemoveTileFromGrid( AAnemoneHexTile* InTile );

protected:
   	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "128.0", ClampMax = "512.0") )
	int32 TileSize = 128;

   	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "64.0", ClampMax = "512.0") )
	int32 TileHeight = 64;

   	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FAnemoneDungeonPosition CurrentPlayerPosition;

	// 3D Hexagonal Grid
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TMap< int32, FAnemoneDungeonTileHeight > Grid;

	// List of Coordinates where the Player can start.
   	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray< FAnemoneStartPosition > PlayerStartPositionList;

	FAxialCoordinates ConvertPositionToAxialCoordinates( const FVector& InTilePosition );
	bool AreCoordinatesEmpty( int32 InQ, int32 InR, int32 InH );
	bool AreCoordinatesEmpty( const FCubeCoordinates& InCube );

	//  Snaps the Actor's transform in the 3D viewport according to its hexagonal coordinate.
	void SnapTileToPosition( AAnemoneHexTile* InTile );

	/*	Fetch an unoccupied tile coordinate in an area defined by a pivot and radius, starting from the pivot.
	*	Returns: True if a coordinate exists, otherwise false.
	*/
	bool FindValidTileInSpiral( FCubeCoordinates InCenterCube, FCubeCoordinates& OutCube );

	/*	Fetch an unoccupied tile coordinate in ring defined by a pivot and radius.
	*	Returns: True if a coordinate exists, otherwise false.
	*/
	bool FindValidTileInRing( FCubeCoordinates InCenterCube, int32 InRadius, FCubeCoordinates& OutCube );

	//  Add tile to 3D list.
	void AddTileToMap( AAnemoneHexTile* InTile );

	//  Remove Tile from 3D list, and remove corresponding Row and Height if they become empty.
	void RemoveTileFromMap( const FAxialCoordinates& InCoordinates );

	bool DoesHeightContainTiles( int32 InHeightIndex );

	bool DoesRowContainTiles( int32 InHeightIndex, int32 InRowIndex );

	// Changes the Position of the Player inside the Grid with a fixed vector if the Destination is Valid.
	AAnemoneHexTile* MovePlayerTowardsDirection( const FCubeCoordinates& InCubeVector );

	void ActivateTileTowardsDirection( const FCubeCoordinates& InCubeVector );

	// Attempt to return target tile, or tile directly below or above it, in that order.  It is assumed that only 1 of these three tiles exists.
	AAnemoneHexTile* FetchTileAtAdjacentHeights( const FCubeCoordinates& InCubeVectorCoordinate );
};
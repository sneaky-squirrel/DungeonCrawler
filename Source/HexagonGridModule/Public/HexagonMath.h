#pragma once
#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "HexagonMath.generated.h"

UENUM( BlueprintType, meta = ( ScriptName="HexagonalDirection" ) )
 enum class EDirectionHexagonal : uint8
 {
    East        UMETA( DisplayName = "EAST" ),
    Southeast   UMETA( DisplayName = "SOUTH-EAST" ),
    Southwest   UMETA( DisplayName = "SOUTH-WEST" ),
    West        UMETA( DisplayName = "WEST" ),
    Northwest   UMETA( DisplayName = "NORTH-WEST" ),
    Northeast   UMETA( DisplayName = "NORTH-EAST" )
 };

USTRUCT( BlueprintType )
struct HEXAGONGRIDMODULE_API FAxialCoordinates
{
	GENERATED_BODY()

	// Column
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 Q = 0;

	// Row
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 R = 0;

	// Height
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 H = 0;

	FAxialCoordinates();
	FAxialCoordinates( const FCubeCoordinates& InCoordinates );

	FAxialCoordinates& operator=( const FAxialCoordinates& InCoordinates );
	FAxialCoordinates& operator=( const FCubeCoordinates& InCoordinates );
};

USTRUCT( BlueprintType )
struct HEXAGONGRIDMODULE_API FCubeCoordinates
{
	GENERATED_BODY()

	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 X;

	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 Y;

	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 Z;

	// Height
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 H;

	FCubeCoordinates();
	FCubeCoordinates( int32 InX, int32 InY, int32 InZ, int32 InH );
	FCubeCoordinates( const FAxialCoordinates& InCoordinates );
};

extern TArray< FCubeCoordinates > HEXAGONGRIDMODULE_API CubeDirections;
extern TMap< EDirectionHexagonal, float > HEXAGONGRIDMODULE_API FacingAngles;

FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Direction( const int32 InDirection );
FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Neighbor( const FCubeCoordinates& InCubeCoordinate, const int32 InDirection );

FCubeCoordinates HEXAGONGRIDMODULE_API operator+( const FCubeCoordinates& InA, const FCubeCoordinates& InB );
FCubeCoordinates HEXAGONGRIDMODULE_API operator*( const FCubeCoordinates& CubeVector, const int32 InScale );

/*	Fills out Vector2D parameter with the position of Hexagon-Grid Axial Coordinates.
*	OutVector = Vector that will be written to.
*	InCoordinates = Axial Coordinates of the Hexagonal Cell.
*	InSize = Size of the Hexagon.
*/
void HEXAGONGRIDMODULE_API ToVector( FVector2D& OutVector, const FAxialCoordinates& InCoordinates, const float InSize );

/*	Fills out Vector2D parameter with the position of Hexagon-Grid Cube Coordinates.
*	OutVector = Vector that will be written to.
*	InCoordinates = Cube Coordinates of the Hexagonal Cell.
*	InSize = Size of the Hexagon.
*/
void HEXAGONGRIDMODULE_API ToVector( FVector2D& OutVector, const FCubeCoordinates& InCoordinates, const float InSize );

FVector2D HEXAGONGRIDMODULE_API ToVector( const FCubeCoordinates& InCoordinates, const float InSize );

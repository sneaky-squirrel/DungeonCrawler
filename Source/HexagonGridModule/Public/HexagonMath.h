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
	int32 Q;

	// Row
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 R;

	// Height
	UPROPERTY( EditInstanceOnly, BlueprintReadOnly )
	int32 H;

	FAxialCoordinates();
	FAxialCoordinates( const FCubeCoordinates& InCoordinates );
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

FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Add( const FCubeCoordinates& InA, const FCubeCoordinates& InB );
FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Scale( const FCubeCoordinates& CubeVector, int32 InScale );
FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Direction( int32 InDirection );
FCubeCoordinates HEXAGONGRIDMODULE_API Cube_Neighbor( const FCubeCoordinates& InCubeCoordinate, int32 InDirection );

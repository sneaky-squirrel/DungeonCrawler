#include "HexagonMath.h"

TArray< FCubeCoordinates > CubeDirections =
	{
		FCubeCoordinates( 1, -1, 0, 0 ), FCubeCoordinates( 1, 0, -1, 0 ), FCubeCoordinates( 0, 1, -1, 0 ),
		FCubeCoordinates( -1, 1, 0, 0 ), FCubeCoordinates( -1, 0, 1, 0 ), FCubeCoordinates( 0, -1, 1, 0 )
	};

TMap< EDirectionHexagonal, float > FacingAngles =
	{
		{ EDirectionHexagonal::East, 0.0 },
		{ EDirectionHexagonal::Southeast, 60.0 },
		{ EDirectionHexagonal::Southwest, 120.0 },
		{ EDirectionHexagonal::West, 180.0 },
		{ EDirectionHexagonal::Northwest, 240.0 },
		{ EDirectionHexagonal::Northeast, 300.0 }
	};

FAxialCoordinates::FAxialCoordinates()
{
}

FAxialCoordinates::FAxialCoordinates(const FCubeCoordinates& InCoordinates)
:Q( InCoordinates.X ), R( InCoordinates.Z), H( InCoordinates.H )
{
}

FCubeCoordinates::FCubeCoordinates()
:FCubeCoordinates( 0, 0, 0, 0 )
{
}

FCubeCoordinates::FCubeCoordinates( int32 InX, int32 InY, int32 InZ, int32 InH )
:X( InX ), Y( InY ), Z( InZ ), H( InH )
{
}

FCubeCoordinates::FCubeCoordinates( const FAxialCoordinates& InCoordinates )
:FCubeCoordinates( InCoordinates.Q, 0 - InCoordinates.Q - InCoordinates.R, InCoordinates.R, InCoordinates.H )
{
}

FCubeCoordinates Cube_Add( const FCubeCoordinates& InA, const FCubeCoordinates& InB )
{
	FCubeCoordinates ResultCube( InA.X + InB.X, InA.Y + InB.Y, InA.Z + InB.Z, InA.H + InB.H );
	return ResultCube;
}

FCubeCoordinates Cube_Scale( const FCubeCoordinates& CubeVector, int32 InScale )
{
	FCubeCoordinates ResultCube( CubeVector.X * InScale, CubeVector.Y * InScale, CubeVector.Z * InScale, CubeVector.H * InScale );
	return ResultCube;
}

FCubeCoordinates Cube_Direction( int32 InDirection )
{
	return CubeDirections[ InDirection ];
}

FCubeCoordinates Cube_Neighbor( const FCubeCoordinates& InCubeCoordinate, int32 InDirection )
{
	return Cube_Add( InCubeCoordinate, Cube_Direction( InDirection ) );
}

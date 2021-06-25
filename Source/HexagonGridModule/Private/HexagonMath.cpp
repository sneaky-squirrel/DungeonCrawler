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

FAxialCoordinates& FAxialCoordinates::operator=( const FAxialCoordinates& InCoordinates )
{
	Q = InCoordinates.Q;
	R = InCoordinates.R;
	H = InCoordinates.H;
	return *this;
}

FAxialCoordinates& FAxialCoordinates::operator=( const FCubeCoordinates& InCoordinates )
{
	Q = InCoordinates.X;
	R = InCoordinates.Z;
	H = InCoordinates.H;
	return *this;
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

FCubeCoordinates Cube_Direction( const int32 InDirection )
{
	return CubeDirections[ InDirection ];
}

FCubeCoordinates Cube_Neighbor( const FCubeCoordinates& InCubeCoordinate, const int32 InDirection )
{
	return InCubeCoordinate + Cube_Direction( InDirection );
}

FCubeCoordinates operator+( const FCubeCoordinates& InA, const FCubeCoordinates& InB )
{
	FCubeCoordinates ResultCube( InA.X + InB.X, InA.Y + InB.Y, InA.Z + InB.Z, InA.H + InB.H );
	return ResultCube;
}

FCubeCoordinates operator*( const FCubeCoordinates& CubeVector, const int32 InScale )
{
	FCubeCoordinates ResultCube( CubeVector.X * InScale, CubeVector.Y * InScale, CubeVector.Z * InScale, CubeVector.H * InScale );
	return ResultCube;
}

void ToVector( FVector2D& OutVector, const FAxialCoordinates& InCoordinates, const float InSize )
{
    OutVector.X = ( FMath::Sqrt( 3.0 ) * InCoordinates.Q + FMath::Sqrt( 3.0 ) / 2 * InCoordinates.R ) * InSize;
    OutVector.Y = 3.0 / 2.0 * InCoordinates.R * InSize;
}

FVector2D ToVector( const FCubeCoordinates& InCoordinates, const float InSize )
{
	FVector2D Vector;
    Vector.X = ( FMath::Sqrt( 3.0 ) * InCoordinates.X + FMath::Sqrt( 3.0 ) / 2 * InCoordinates.Z ) * InSize;
    Vector.Y = 3.0 / 2.0 * InCoordinates.Z * InSize;
	return Vector;
}

void ToVector( FVector2D& OutVector, const FCubeCoordinates& InCoordinates, const float InSize )
{
    OutVector.X = ( FMath::Sqrt( 3.0 ) * InCoordinates.X + FMath::Sqrt( 3.0 ) / 2 * InCoordinates.Z ) * InSize;
    OutVector.Y = 3.0 / 2.0 * InCoordinates.Z * InSize;
}

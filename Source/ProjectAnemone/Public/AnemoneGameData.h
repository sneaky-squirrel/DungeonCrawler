#pragma once
#include "CoreMinimal.h"
#include "AnemoneEnum.h"
#include "Containers/Map.h"
#include "AnemoneGameData.generated.h"

/*
*	[ Character / Entity ] Data Structure Types
*/

USTRUCT( BlueprintType )
struct FAnemoneScore
{
	GENERATED_BODY()

	UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	int32 Base;

	UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	int32 Modifier;

	FAnemoneScore();
	FAnemoneScore( int32 Amount );

	int32 Total() const;

	friend FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score );
	friend FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneScore >& ScoreSheet );
};

USTRUCT( BlueprintType )
struct FAnemoneMessageModule
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, Instanced, BlueprintReadWrite )
	TArray< UObject* > ComponentChain;

	friend FArchive& operator<<( FArchive& Ar, FAnemoneMessageModule& Module );
	friend FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneMessageModule >& Module );
};

extern TArray< EScore > EntityScores;
extern TArray< EModule > EntityCombatModules;

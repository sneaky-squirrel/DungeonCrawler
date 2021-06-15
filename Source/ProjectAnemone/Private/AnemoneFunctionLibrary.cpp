// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneFunctionLibrary.h"
#include "AnemoneEncounterEntity.h"

bool UAnemoneFunctionLibrary::SortBroadcast_Action( TArray< UObject* > EntityList, UObject* Instigator )
{
	for( UObject* Object : EntityList )
	{
		if( !Object->Implements< UAnemoneEncounterEntity >() )
		{
			return false;
		}
	}
	Algo::Sort( EntityList,
		[ Instigator ]
		( UObject* A, UObject* B )
		{
			if( A == Instigator )
			{
				return true;
			}
			IAnemoneEncounterEntity* EntityA = Cast < IAnemoneEncounterEntity >( A );
			IAnemoneEncounterEntity* EntityB = Cast < IAnemoneEncounterEntity >( B );
			return IAnemoneEncounterEntity::Execute_GetScore( A, EAnemoneScore::Speed ) > IAnemoneEncounterEntity::Execute_GetScore( B, EAnemoneScore::Speed );
		}
	);
	return true;
}

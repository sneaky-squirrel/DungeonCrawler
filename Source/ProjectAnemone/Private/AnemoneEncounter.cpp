// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneEncounter.h"
#include "AnemoneGameData.h"

#include "AnemoneAction.h"
#include "AnemoneEvent.h"
#include "AnemoneGameStateSubsystem.h"
#include "AnemoneEncounterEntity.h"

Encounter::UAnemoneEncounter()
:PendingActions( CreateDefaultSubobject< ActionInstanceQueue >( TEXT("PendingActions") ) ),
PendingLogText( CreateDefaultSubobject< LogQueue >( TEXT("PendingLogText") ) )
{
}

Encounter* Encounter::CreateEncounter( TSubclassOf< Encounter > InEncounterType )
{
	Encounter* Item = NewObject< Encounter >( GlobalState, InEncounterType );
	Item->PendingActions = NewObject< ActionInstanceQueue >();
	Item->PendingLogText = NewObject< LogQueue >();
	return Item;
}

/*
void UAnemoneEncounter::ResolveAction( const ObjList& InInstigator, const ObjList& InDefender, const ObjList& InTarget, LogQueue* OutTextLog )
{
	ActionInstance Entry;
	EOutcome Outcome;
	while( PendingActions->Queue.Dequeue( Entry ) == true )
	{
		for( UObject* Target : InTarget )
		{
			Outcome = Entry.Item->RollOutcome( InInstigator, InDefender );
			EventData Data = { Target, InInstigator, InTarget, Outcome };
			Entry.Item->ExecuteOutcome( Data, EntityList, OutTextLog, Entry.Indentation );
		}
	}
}
*/

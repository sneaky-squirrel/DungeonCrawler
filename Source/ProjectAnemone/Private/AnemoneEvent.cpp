// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneEvent.h"
#include "AnemoneGameData.h"
#include "AnemoneAction.h"
#include "AnemoneGameStateSubsystem.h"

#include "AnemoneEncounterEntity.h"
#include "AnemoneEntityModule.h"

using GameStateSubsystem = UAnemoneGameStateSubsystem;

/*
UWorld* UAnemoneEvent::GetWorld() const
{
    if ( HasAllFlags( RF_ClassDefaultObject ) )
    {
        // If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
        return nullptr;
    }
    return ( GetOuter() ) ? GetOuter()->GetWorld() : nullptr;
}
*/

void UAnemoneEvent::PrintLogText( const FText& InText )
{
	if( TextLog == nullptr )
	{
		UE_LOG( LogEncounter, Error, TEXT( "UAnemoneEvent::PrintLogText():	TextLog == nullptr." ) );
		return;
	}
	TextLog->Push( InText );
}

FString UAnemoneEvent::GetIndentation() const
{
	FString Text;
	for( int32 i = 0; i < Indentation; ++i )
	{
		Text += FString( TEXT( "        " ) );
	}
	return Text;
}

ScoreEvent* ScoreEvent::CreateEvent( EAnemoneScore InScoreType, int32 InBaseValue )
{
	ScoreEvent* Event = NewObject< ScoreEvent >();
	Event->EventType = EAnemoneEvent::GetScore;
	Event->ScoreType = InScoreType;
	Event->ScoreValue = InBaseValue;
	return Event;
}

OutcomeEvent* OutcomeEvent::CreateEvent()
{
	UAnemoneEvent_Outcome* NewEvent = NewObject< UAnemoneEvent_Outcome >();
	NewEvent->EventType = EAnemoneEvent::Outcome;
	return NewEvent;
}

void EntityEvent::Execute_Implementation()
{
}

FEntityScorePercentage::FEntityScorePercentage()
	: FEntityScorePercentage( NAME_None, EAnemoneScore::Invalid, 0.0 )
{
}

FEntityScorePercentage::FEntityScorePercentage( FName InHost, EAnemoneScore InScore, float InPercentage )
	: Host( InHost ), Score( InScore ), Percentage( InPercentage )
{
}

UObject* EntityEvent::GetInstigator() const
{
	if( InstigatorList.Num() <= 0 || !GameState || !GameState->ContainsEntity( InstigatorList[ 0 ] ) )
	{
		return nullptr;
	}
	return GameState->GetEntity( InstigatorList[ 0 ] );
}

FString EntityEvent::GetInstigatorName() const
{
	FName InstigatorID;
	UObject* Entity;
	if( InstigatorList.Num() <= 0 )
	{
		UE_LOG( LogTemp, Error, TEXT( "EntityEvent::GetInstigatorName()" ) );
		return FString( TEXT( "Error: Empty List." ) );
	}
	InstigatorID = InstigatorList[ 0 ];
	Entity = GameState->GetEntity( InstigatorID );
	if( !Entity || !Entity->Implements< UEntity >() )
	{
		UE_LOG( LogTemp, Error, TEXT( "EntityEvent::GetInstigatorName()" ) );
		return FString( TEXT( "Error: Object does not implement Entity interface." ) );
	}
	return IEntity::Execute_GetName( Entity );
}

FString EntityEvent::GetTargetName() const
{
	UObject* Entity = GameState->GetEntity( Target );
	if( !Entity || !Entity->Implements< UEntity >() )
	{
		UE_LOG( LogTemp, Error, TEXT( "EntityEvent::GetTargetName()" ) );
		return FString( TEXT( "Error: Object does not implement Entity interface." ) );
	}
	return IEntity::Execute_GetName( Entity );
}

EntityEvent* EntityEvent::InitCopy( const Action* const InAction, const EventData& InData, LogQueue* const InTextLog ) const
{
	EntityEvent* Event = NewObject< EntityEvent >( GameState, GetClass() );
	if( InAction )
	{
		Event->HostID = InAction->HostID;
		Event->ActionID = InAction->ID;
	}
	Event->InstigatorList = InData.InstigatorList;
	Event->TargetList = InData.TargetList;
	Event->Outcome = InData.Outcome;
	Event->Target = InData.Target;
	Event->TextLog = InTextLog;
	Event->Indentation = InData.Indentation;
	return Event;
}

DeltaEvent::UAnemoneEvent_Delta()
{
	;
}

void DeltaEvent::AddConstantMod( const int32& InConstant )
{
	AddFlatConstant = AddFlatConstant + InConstant;
}

void DeltaEvent::AddScoreMod( FName InHost, const EAnemoneScore& InScore, const float& InPercentage )
{
	AddFlatScore.Emplace( FEntityScorePercentage( InHost, InScore, InPercentage ) );
}

void DeltaEvent::AddMultiplierMod( const float& InMultiplierMod )
{
	Multiplier = Multiplier + InMultiplierMod;
}

int32 DeltaEvent::GetDelta() const
{
	//GameStateSubsystem* State = GameStateSubsystem::GetSystem();
	UObject* CurrentEntity = GameState->GetEntity( Target );
	int32 Counter = Base + AddFlatConstant;
	for( FEntityScorePercentage Item : AddFlatScore )
	{
		CurrentEntity = GameState->GetEntity( Item.Host );
		if( !CurrentEntity || !CurrentEntity->Implements< UEntity >() )
		{
			continue;
		}
		Counter = Counter + IAnemoneEncounterEntity::Execute_GetScore( CurrentEntity, Item.Score ) * Item.Percentage;
	}
	return Counter * ( 1 + Multiplier );
}

void DeltaEvent::ApplyDelta( int32 InAmount )
{
	UObject* Entity = GameState->GetEntity( Target );
	if( Entity == nullptr || !Entity->Implements< UEntity >() )
	{
		UE_LOG( LogEncounter, Error, TEXT( "DeltaEvent::ApplyDelta()	:	Target == nullptr OR Target is NOT Entity" ) );
		return;
	}
	if( InstigatorList.Num() <= 0 || InstigatorList[ 0 ] == NAME_None )
	{
		UE_LOG( LogEncounter, Error, TEXT( "DeltaEvent::ApplyDelta()	:	Intigator List is empty OR Instigator == NAME_None" ) );
		return;
	}
	IEntity::Execute_AddModule( Entity, DeltaModule::CreateDelta( Target, InstigatorList[ 0 ], ActionID, TagSet, Score, InAmount ) );
}

void DeltaEvent::Execute_Implementation()
{
	ApplyDelta( GetDelta() );
}

UAnemoneEvent_ModuleAdd::UAnemoneEvent_ModuleAdd()
{
	EventType = EAnemoneEvent::AddModule;
}

UAnemoneEvent_ModuleRemove::UAnemoneEvent_ModuleRemove()
{
	;
}

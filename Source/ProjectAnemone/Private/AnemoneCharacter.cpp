// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneCharacter.h"
#include "AnemoneGameData.h"
#include "AnemoneGameStateSubsystem.h"
#include "OutcomeTracker.h"
#include "AnemoneEntityModule.h"

#include "AnemoneEvent.h"
#include "AnemoneAction.h"

#include "GameplayTagContainer.h"

Character::UAnemoneCharacter()
{
	for( EAnemoneScore Score : ScoresPoolEncounter )
	{
		ScoreSheet.Emplace( Score, 100 );
	}
	for( EAnemoneScore Score : ScoresComparison )
	{
		ScoreSheet.Emplace( Score, 10 );
	}
}

FName Character::CreateCharacter( const FName InCharacterID, TSubclassOf< Character > InArchetype )
{
	Character* Item = NewObject< Character >( GameState, InArchetype );
	Item->Tracker = NewObject< OutcomeTracker >( Item, OutcomeTracker::StaticClass() );
	for( Module* m : Item->ModuleList )
	{
		m->Init( InCharacterID, NAME_None, NAME_None );
	}
	if( InCharacterID != NAME_None )
	{
		Item->Name = InCharacterID;
	}
	return ( GameState ) ? GameState->AddEntity( Item->Name, Item ) : NAME_None;
}

FString Character::GetName_Implementation() const
{
	return Name.ToString();
}

void Character::HandleEvent_Implementation( EntityEvent* InEvent )
{
	for( Module* Item : ModuleList )
	{
		if( Item->HandleEvent( InEvent ) == false )
		{
			;//UE_LOG( LogTemp, Warning, TEXT("Module does not implement Encounter Event.") );
		}
	}
}

int32 Character::GetScore_Implementation( const EAnemoneScore& InScore ) const
{
	ScoreEvent* Event = ScoreEvent::CreateEvent( InScore, ScoreSheet[ InScore ] );
	for( UAnemoneEntityModule* Module : ModuleList )
	{
		Module->HandleEvent( Event );
	}
	return Event->ScoreValue;
}

EOutcome Character::RollOutcome_Implementation( UAnemoneEvent_Outcome* InEvent )
{
	if( Cast< OutcomeTracker >( Tracker ) == nullptr )
	{
		UE_LOG( LogTemp, Error, TEXT( "UAnemoneCharacter::RollOutcome()	:	Outcome Tracker == nullptr." ) );
		return EOutcome::Invalid;
	}
	return Tracker->ChooseOutcome( InEvent->OutcomeRatio, InEvent->OutcomeBonus );
}

void Character::AddModule_Implementation( UAnemoneEntityModule* InModule )
{
	ModuleList.Emplace( InModule );
}

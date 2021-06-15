// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneEntityModule.h"
#include "AnemoneGameStateSubsystem.h"
#include "AnemoneEvent.h"
#include "GameplayTagContainer.h"
#include "AnemoneEncounterEntity.h"

bool Module::HandleEvent_Implementation( UAnemoneEvent* InEvent )
{
	return false;
}

Module* Module::CreateCopy( const FName InHost, const FName InInstigator, const FName InAction )
{
	UAnemoneEntityModule* NewCopy = NewObject< UAnemoneEntityModule >( GameState, GetClass() );
	NewCopy->HostID = InHost;
	NewCopy->InstigatorID = InInstigator;
	NewCopy->ActionID = InAction;
	return NewCopy;
}

void Module::Init( const FName InHost, const FName InInstigator, const FName InAction )
{
	HostID = InHost;
	InstigatorID = InInstigator;
	ActionID = InAction;
}

DeltaModule* DeltaModule::CreateDelta( const FName InHostID, const FName InInstigatorID, const FName InActionID, const Tags& InTagSet, EAnemoneScore InScoreType, int32 InValue )
{
	UAnemoneEntityModule_Delta* Delta = NewObject< UAnemoneEntityModule_Delta >();
	Delta->HostID = InHostID;
	Delta->InstigatorID = InInstigatorID;
	Delta->ActionID = InActionID;
	Delta->ScoreType = InScoreType;
	Delta->Value = InValue;
	Delta->TagSet.AppendTags( InTagSet );
	Delta->CountTurn = -1;
	Delta->CountUse = -1;
	return Delta;
}

bool DeltaModule::HandleEvent_Implementation( UAnemoneEvent* InEvent )
{
	UAnemoneEvent_GetScore* Event = Cast< UAnemoneEvent_GetScore >( InEvent );
	if( Event == nullptr )
	{
		return false;
	}
	if( Event->ScoreType != ScoreType )
	{
		return false;
	}
	Event->ScoreValue = Event->ScoreValue + Value;
	return true;
}

Module* DeltaModule::CreateCopy( const FName InHost, const FName InInstigator, const FName InAction )
{
	DeltaModule* NewCopy = Cast< DeltaModule >( Super::CreateCopy( InHost, InInstigator, InAction ) );
	NewCopy->ScoreType = ScoreType;
	NewCopy->Value = Value;	
	return NewCopy;
}

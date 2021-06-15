// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneAction.h"
#include "AnemoneGameData.h"
#include "AnemoneGameStateSubsystem.h"
#include "AnemoneEvent.h"
#include "AnemoneEncounterEntity.h"

using GameStateSubsystem = UAnemoneGameStateSubsystem;

Action* Action::CreateAction( FName InHostID, TSubclassOf< Action > InActionType )
{
	Action* Item = NewObject< Action >( GameState, *InActionType );
	Item->HostID = InHostID;
	return Item;
}

void Action::ExecuteAction( const ActionInstance& InEntry, const NameList& InEntityAll, LogQueue* OutTextLog )
{
	Action* ActionItem = InEntry.Action;
	EOutcome Outcome;
	SubActionOutcome* ActionOutcome;
	int32 Counter, SelectedMaximum;
	if( !ActionItem )
	{
		UE_LOG( LogEncounter, Error, TEXT( "Action::ExecuteOutcome()	:	!ActionItem" ) );
		return;
	}
	// TODO: Fetch List of Defenders
	Outcome = ActionItem->RollOutcome( InEntry.InstigatorList, InEntry.TargetList );
	for( TargetSelection CurrentSelection : ActionItem->SelectionList )
	{
		Counter = 0;
		for( SubAction CurrentAction : CurrentSelection.ActionList )
		{
			ActionOutcome = CurrentAction.OutcomeMap.Find( Outcome );
			if( !ActionOutcome )
			{
				UE_LOG( LogEncounter, Error, TEXT( "Action::ExecuteOutcome()	:	!Item" ) );
				continue;
			}
			SelectedMaximum = ActionOutcome->SelectedCountCap;
			for( FName TargetName : InEntry.TargetList )
			{
				if( Counter >= SelectedMaximum )
				{
					break;
				}
				++Counter;
				for( const EntityEvent* const Event : ActionOutcome->EventList )
				{
					EventData Data = { TargetName, InEntry.InstigatorList, InEntry.TargetList, Outcome, InEntry.Indentation };
					EntityEvent* Copy = Event->InitCopy( ActionItem, Data, OutTextLog );
					ExecuteEvent( Copy, InEntityAll );
				}
			}
		}
	}
}

EOutcome Action::RollOutcome( const NameList& InInstigator, const NameList& InDefender )
{
	RankTracker RankTracker;
	OutcomeEvent* Event = OutcomeEvent::CreateEvent();

	for( EScoreSet Item : ScoreSetList )
	{
		const ScoreCheck* Entry = ScoreCheckMap.Find( Item );
		if( !Entry )
		{
			UE_LOG( LogEncounter, Error, TEXT( "Action::RollOutcome()	:	!ScoreCheck" ) );
			continue;
		}
		GetRankLists( Entry->ScoreSet, InInstigator, InDefender, RankTracker );
		AddOutcomes( Entry->InstigatorOutcomeList, RankTracker.Instigator, Event );
		AddOutcomes( Entry->DefenderOutcomeList, RankTracker.Defender, Event );
	}
/*
	int32 debugCount = 0;
	UE_LOG( LogEncounter, Warning, TEXT( "Printing Outcome Event..." ) );
	for( auto Item : Event->OutcomeRatio )
	{
		UE_LOG( LogEncounter, Warning, TEXT( "Ratio #%d = %d." ), ++debugCount, Item.Value );
	}
*/
	UObject* Entity = GameState->GetEntity( HostID );
	if( !Entity || !Entity->Implements< UEntity >() )
	{
		UE_LOG( LogEncounter, Error, TEXT( "Action::RollOutcome()	:	Host == nullptr" ) );
		return EOutcome::Error;
	}
	//UE_LOG( LogEncounter, Warning, TEXT( "Action :: Sanity." ) );
	return IEntity::Execute_RollOutcome( Entity, Event );
}

void Action::ExecuteEvent( EntityEvent* const InEvent, const NameList& InEntity )
{
	DispatchEvent( InEvent, InEntity );
	InEvent->Execute();
	InEvent->bIsBroadcast = true;
	DispatchEvent( InEvent, InEntity );
}

void Action::DispatchEvent( EntityEvent* const InEvent, const NameList& InEntity )
{
	for( FName Item : InEntity )
	{
		UObject* Entity = GameState->GetEntity( Item );
		if( !Entity || !Entity->Implements< UEntity >() )
		{
			UE_LOG( LogEncounter, Warning, TEXT("Action::DispatchEvent()	:	Object == nullptr || Not_Entity") );
			continue;
		}
		IEntity::Execute_HandleEvent( Entity, InEvent );
	}
}

void Action::GetRankLists( const ScorePair& InScores, const NameList& InInstigator, const NameList& InDefender, RankTracker& OutRanks )
{
	TArray< int32 > ScoresInstigator, ScoresTarget;
	int32 MaxScore = 0;
	GetScoreList( InInstigator, InScores.Instigator, ScoresInstigator );
	GetScoreList( InDefender, InScores.Defender, ScoresTarget );
/*
	for( int32 i : ScoresInstigator )
	{
		UE_LOG( LogEncounter, Warning, TEXT("Instigator Score: %d"), i );
	}
	for( auto i : InInstigator )
	{
		if( i->Implements< UEntity >() )
		{
			UE_LOG( LogEncounter, Warning, TEXT("InInstigator Strength: %d"), IEntity::Execute_GetScore( i, EScore::Strength ) );
		}
	}
	for( int32 i : ScoresTarget )
	{
		UE_LOG( LogEncounter, Warning, TEXT("Target Score: %d"), i );
	}
*/
	OutRanks.Instigator.Empty( InInstigator.Num() );
	OutRanks.Defender.Empty( InDefender.Num() );
	for( int32 i : ScoresInstigator )
	{
		MaxScore = ( i > MaxScore ) ? i : MaxScore;
	}
	for( int32 i : ScoresTarget )
	{
		MaxScore = ( i > MaxScore ) ? i : MaxScore;
	}
	for( int32 i : ScoresInstigator )
	{
		AddRankToList( i, MaxScore, OutRanks.Instigator );
	}
	for( int32 i : ScoresTarget )
	{
		AddRankToList( i, MaxScore, OutRanks.Defender );
	}
/*
	for( ERank i : OutRanks.Instigator)
	{
		UE_LOG( LogEncounter, Warning, TEXT("Rank Instigators: %d"), i );
	}
	for( ERank i : OutRanks.Target )
	{
		UE_LOG( LogEncounter, Warning, TEXT("Rank Targets: %d"), i );
	}
*/
}

void Action::AddOutcomes( const TArray< EOutcome >& InOutcomeList, const TArray< ERank >& InRankList, OutcomeEvent* OutEvent )
{
	if( OutEvent == nullptr )
	{
		UE_LOG( LogEncounter, Error, TEXT("Actions::AddOutcomes()	:	OutEvent == nullptr") );
		return;
	}
	//UE_LOG( LogEncounter, Warning, TEXT("OutcomeChance Size = %d."), OutcomeChance.Num() );
	int32 Counter;
	int32* Contribution;
	OutcomeChance* Chance;
	for( EOutcome Outcome : InOutcomeList )
	{
		Counter = 0;
		Chance = OutcomeChanceMap.Find( Outcome );
		if( Chance == nullptr )
		{
			continue;
		}
		Counter = Chance->BaseRate;
		for( EAnemoneScoreRank Rank : InRankList )
		{
			Contribution = Chance->EntityContribution.Find( Rank );
			if( Contribution == nullptr )
			{
				continue;
			}
			Counter = Counter + *Contribution;
		}
/*
		if( Counter > 0 )
		{
			UE_LOG( LogEncounter, Warning, TEXT("AddOutcomes() : Counter = %d."), Counter );
		}
*/
		OutEvent->OutcomeRatio.FindOrAdd( Outcome ) += Counter;
	}
}

void Action::AddRankToList( const int32& InBaseScore, const int32& InMaxScore, TArray< ERank >& OutRankList )
{
	float ScorePercentage = ( (float) InBaseScore ) / ( (float) InMaxScore );
	if( ScorePercentage >= 0.875 )
	{
		OutRankList.Emplace( ERank::Top );
	}
	else if( ScorePercentage >= 0.75 )
	{
		OutRankList.Emplace( ERank::Elite );
	}
	else if( ScorePercentage >= 0.625 )
	{
		OutRankList.Emplace( ERank::Strong );
	}
	else if( ScorePercentage >= 0.5 )
	{
		OutRankList.Emplace( ERank::Average );
	}
	else if( ScorePercentage >= 0.375 )
	{
		OutRankList.Emplace( ERank::Inferior );
	}
	else if( ScorePercentage >= 0.25 )
	{
		OutRankList.Emplace( ERank::Weak );
	}
	else
	{
		OutRankList.Emplace( ERank::Fodder );
	}
}

void UAnemoneAction::GetScoreList( const NameList& InEntity, const EScore& InScoreName, IntList& OutScores )
{
	UObject* Entity;
	for( FName Item : InEntity )
	{
		Entity = GameState->GetEntity( Item );
		if( !Entity->Implements< UEntity >() )
		{
			continue;
		}
		OutScores.Emplace( IEntity::Execute_GetScore( Entity, InScoreName ) );
	}
}

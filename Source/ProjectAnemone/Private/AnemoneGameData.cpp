#include "AnemoneGameData.h"
#include "OutcomeTracker.h"
#include "AnemoneGameStateSubsystem.h"

DEFINE_LOG_CATEGORY( LogEncounter );

/*
*	Score Lists
*/

const ScoreList ScoresAll =
	{
		EAnemoneScore::Constitution, EAnemoneScore::Agility, EAnemoneScore::Finesse, EAnemoneScore::Mind,
		EAnemoneScore::Hitpoints, EAnemoneScore::Stamina, EAnemoneScore::Fortitude, EAnemoneScore::Strength,
		EAnemoneScore::Speed, EAnemoneScore::Evasion, EAnemoneScore::Skulking, EAnemoneScore::Block,
		EAnemoneScore::Prowess, EAnemoneScore::Accuracy, EAnemoneScore::Technique, EAnemoneScore::Lock,
		EAnemoneScore::Mana, EAnemoneScore::Resolve, EAnemoneScore::Cunning, EAnemoneScore::Perception
	};

const ScoreList ScoresPoolEncounter =
	{
		EAnemoneScore::Hitpoints, EAnemoneScore::Stamina, EAnemoneScore::Fortitude, EAnemoneScore::Mana
	};

const ScoreList ScoresComparison =
	{
		EAnemoneScore::Strength,
		EAnemoneScore::Speed, EAnemoneScore::Evasion, EAnemoneScore::Skulking, EAnemoneScore::Block,
		EAnemoneScore::Prowess, EAnemoneScore::Accuracy, EAnemoneScore::Technique, EAnemoneScore::Lock,
		EAnemoneScore::Resolve, EAnemoneScore::Cunning, EAnemoneScore::Perception
	};

const ScoreList ScoresOffense =
	{
		EAnemoneScore::Strength, EAnemoneScore::Speed, EAnemoneScore::Accuracy
	};

const ScoreList ScoresDefense =
	{
		EAnemoneScore::Evasion, EAnemoneScore::Block, EAnemoneScore::Technique
	};

const ScorePair ScorePairTechnique = { EAnemoneScore::Speed, EAnemoneScore::Technique };
const ScorePair ScorePairEvasion = { EAnemoneScore::Accuracy, EAnemoneScore::Evasion };
const ScorePair ScorePairBlock = { EAnemoneScore::Strength, EAnemoneScore::Block };

/*
*	Outcome Lists
*/

const OutcomeList OutcomesTechnique =
	{
		EOutcome::Counter, EOutcome::Parry, EOutcome::Retaliate
	};

const OutcomeList OutcomesEvasion =
	{
		EOutcome::Disappear, EOutcome::Evade, EOutcome::Graze
	};

const OutcomeList OutcomesBlock =
	{
		EOutcome::Deflect, EOutcome::Absorb, EOutcome::PushedBack
	};

const OutcomeList OutcomesHit =
	{
		EOutcome::Hit
	};

const TMap< EScoreSet, ScoreCheck > ScoreCheckMap =
	{
		{ EScoreSet::Technique, ScoreCheck( ScorePairTechnique, OutcomesHit, OutcomesTechnique ) },
		{ EScoreSet::Evasion, ScoreCheck( ScorePairEvasion, OutcomesHit, OutcomesEvasion ) },
		{ EScoreSet::Block, ScoreCheck( ScorePairBlock, OutcomesHit, OutcomesBlock ) }
	};

FAnemoneScore::FAnemoneScore()
:FAnemoneScore( 100 )
{
}

FAnemoneScore::FAnemoneScore( int32 Score )
:Base( Score ), Delta( 0 )
{
}

int32 FAnemoneScore::GetTotal() const
{
	return Base + Delta;
}

FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score )
{
	Ar << Score.Base;
	Ar << Score.Delta;
	return Ar;
}

FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneScore >& ScoreSheet )
{
	int32 Length = ScoreSheet.Num();
	FName Name;
	FAnemoneScore LoadedScore;
	Ar << Length;
	if( Ar.IsLoading() )
	{
		for( int32 i = 0; i < Length; ++i )
		{
			Ar << Name;
			Ar << LoadedScore;
			ScoreSheet.Emplace( Name, LoadedScore );
		}
	}
	else
	{
		for( TTuple< FName, FAnemoneScore > Score : ScoreSheet )
		{
			Name = Score.Key;
			LoadedScore = Score.Value;
			Ar << Name;
			Ar << LoadedScore;
		}
	}
	return Ar;
}

ScorePair::FAnemoneScorePair( EAnemoneScore InInstigator, EAnemoneScore InDefender )
	:Instigator( InInstigator ), Defender( InDefender )
{
}

ScorePair::FAnemoneScorePair( const FAnemoneScorePair& InSet )
	:Instigator( InSet.Instigator ), Defender( InSet.Defender )
{
}

ScoreCheck::FAnemoneScoreCheck( const ScorePair& InScoreSet, const TArray< EOutcome >& InInstigator, const TArray< EOutcome >& InDefender )
	:ScoreSet( InScoreSet ), InstigatorOutcomeList( InInstigator ), DefenderOutcomeList( InDefender )
{
}

EventData::FAnemoneEventData()
{
}

EventData::FAnemoneEventData( const FName InTarget, const NameList& InInstigator, const NameList& InTargetList, const EOutcome InOutcome, const int32 InIndentation )
:Target( InTarget ), InstigatorList( InInstigator ), TargetList( InTargetList ), Outcome( InOutcome ), Indentation( InIndentation )
{
}

ActionInstance::FAnemoneActionInstance()
{
}

ActionInstance::FAnemoneActionInstance( UAnemoneAction* InAction, int32 InIndentation, const TArray< FName >& InInstigator, const TArray< FName >& InTarget )
:Action( InAction ), Indentation( InIndentation ), InstigatorList( InInstigator ), TargetList( InTarget )
{
}

void ActionInstanceQueue::PushAction( UAnemoneAction* InItem, int32 InIndentation, const TArray< FName >& InInstigator, const TArray< FName >& InTarget )
{
	Queue.Enqueue( ActionInstance( InItem, InIndentation, InInstigator, InTarget ) );
}

bool UAnemoneActionInstanceQueue::PopAction( ActionInstance& Entry )
{
	return Queue.Dequeue( Entry );
}

void LogQueue::Push( const FText& InLogText )
{
	Queue.Enqueue( InLogText );
}

bool LogQueue::Pop( FText& OutLogText )
{
	return Queue.Dequeue( OutLogText );
}

/*
FArchive& operator<<( FArchive& Ar, FAnemoneMessageModule& Module )
{
	if( Ar.ArIsSaveGame )
	{
		SerializeArray( Ar, Module.ComponentChain );
	}
	return Ar;
}

FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneMessageModule >& ModuleMap )
{
	if( !Ar.ArIsSaveGame )
	{
		return Ar;
	}
	//UE_LOG( LogTemp, Display, TEXT("FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneMessageModule > ModuleMap )") );
	int32 Length = ModuleMap.Num();
	FName Name;
	FAnemoneMessageModule LoadedModule;
	Ar << Length;
	if( Ar.IsLoading() )
	{
		for( int32 i = 0; i < Length; ++i )
		{
			Ar << Name;
			Ar << LoadedModule;
			ModuleMap.Emplace( Name, LoadedModule );
		}
	}
	else
	{
		for( TTuple< FName, FAnemoneMessageModule > Module : ModuleMap )
		{
			Name = Module.Key;
			LoadedModule = Module.Value;
			Ar << Name;
			Ar << LoadedModule;
		}
	}
	return Ar;
}

*/

#include "AnemoneGameData.h"
#include "AnemoneGameStateSubsystem.h"

TArray< EScore > EntityScores =
	{
		EScore::Constitution, EScore::Agility, EScore::Finesse, EScore::Mind,
		EScore::Hitpoints, EScore::Stamina, EScore::Strength, EScore::Block,
		EScore::Speed, EScore::Evasion, EScore::Skulking,
		EScore::Discipline, EScore::Accuracy, EScore::Parry, EScore::Lock,
		EScore::Mana, EScore::Resolve, EScore::Foresight, EScore::Perception
	};

TArray< EModule > EntityCombatModules = 
	{
		EModule::StartAction,
		EModule::SendQueryOutcome, EModule::GetQueryOutcome,
		EModule::GiveScoreDelta, EModule::GainScoreDelta,
		EModule::Parry, EModule::Evasion, EModule::Block,
		EModule::GiveStatus, EModule::GainStatus,
		EModule::RemoveStatus, EModule::LoseStatus
	};

/*
TArray< FOutcomeTableRow > OutcomeRateTableRows =
	{
		{ EOutcomeTableRow::Fodder, 8 },
		{ EOutcomeTableRow::Ahead, 4 },
		{ EOutcomeTableRow::Advantage, 2 },
		{ EOutcomeTableRow::Even, 0 },
		{ EOutcomeTableRow::Disadvantage, -4 },
		{ EOutcomeTableRow::Behind, -8 },
		{ EOutcomeTableRow::Despair, -12 }
	};

TMap< EDamageType, FString > DamageTypeMap =
	{
		{ EDamageType::Physical, FString( TEXT( "" ) ) },
		{ EDamageType::Magic, FString( TEXT( " magic" ) ) },
		{ EDamageType::Poison, FString( TEXT( " poison" ) ) },
		{ EDamageType::Fire, FString( TEXT( " fire" ) ) },
		{ EDamageType::Water, FString( TEXT( " water" ) ) },
		{ EDamageType::Earth, FString( TEXT( " earth" ) ) },
		{ EDamageType::Wind, FString( TEXT( " wind" ) ) },
	};
*/

FAnemoneScore::FAnemoneScore()
:FAnemoneScore( 100 )
{
}

FAnemoneScore::FAnemoneScore( int32 Score )
:Base( Score ), Modifier( 0 )
{
}

int32 FAnemoneScore::Total() const
{
	return Base + Modifier;
}

FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score )
{
	Ar << Score.Base;
	Ar << Score.Modifier;
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

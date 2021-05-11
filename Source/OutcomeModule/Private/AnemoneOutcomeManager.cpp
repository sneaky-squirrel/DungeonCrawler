// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneOutcomeManager.h"

const int NUMBER_OF_COMMON_OUTCOMES = 16;
const int GRANULARITY = 100;
thread_local Mersenne_Twister Engine_Mersenne_Twister( Create_PRNG_Engine() );

TArray< EOutcomes > BinaryOutcomes =
{
	EOutcomes::Loss,
	EOutcomes::Win
};

TArray< EOutcomes > PrimarySkillOutcomes =
{
	EOutcomes::Parry,
	EOutcomes::Evade,
	EOutcomes::Graze,
	EOutcomes::Pushback,
	EOutcomes::Deflect,
	EOutcomes::Absorb,
	EOutcomes::FailBlock,
	EOutcomes::Knockback,
	EOutcomes::Knockdown
};

Mersenne_Twister Create_PRNG_Engine( )
{
	Mersenne_Twister::result_type Random_Data[ Mersenne_Twister::state_size ];
	std::random_device Entropy_Source;
	std::generate( std::begin( Random_Data ), std::end( Random_Data ), std::ref( Entropy_Source ) );
	std::seed_seq Entropy_Seed( std::begin( Random_Data ), std::end( Random_Data ) );
	return Mersenne_Twister{ Entropy_Seed };
}

unsigned gcd_recursive( unsigned a, unsigned b )
 {
  if (b)
   return gcd_recursive(b, a % b);
  else
   return a;
 }

UAnemoneOutcomeList::UAnemoneOutcomeList()
{
}

UAnemoneOutcomeManager* UAnemoneOutcomeManager::CreateOutcomeManager( const TArray<EOutcomes> &ListOfOutcomes )
{
	UAnemoneOutcomeManager* OutcomeManager = NewObject< UAnemoneOutcomeManager >( GetTransientPackage(), TEXT("OutcomeManager") );
	int32 Length = OutcomeManager->NumberOfOutcomeTypes = ListOfOutcomes.Num();
	//UE_LOG( LogTemp, Warning, TEXT("Init: NumberOfOutcomeTypes: %d"), NumberOfOutcomeTypes );

	OutcomeManager->Init();
	return OutcomeManager;
}

UAnemoneOutcomeManager::UAnemoneOutcomeManager()
{
	Init();
}

void UAnemoneOutcomeManager::Init()
{
	NumberOfOutcomeTypes = ( OutcomeList ) ? OutcomeList->List.Num() : 0;

	Count.Init( 0, NumberOfOutcomeTypes );
	Max_Permanent.Init( 0, NumberOfOutcomeTypes );
	Max_Persistent.Init( 0, NumberOfOutcomeTypes );

	Max_Net.Init( 0, NumberOfOutcomeTypes );
	Remaining.Init( 0, NumberOfOutcomeTypes );
}

#if WITH_EDITOR
void UAnemoneOutcomeManager::PostEditChangeProperty( FPropertyChangedEvent& Event )
{
	FName PropertyName = ( Event.Property != nullptr ) ? FName( Event.Property->GetName() ) : NAME_None;
	if( PropertyName == GET_MEMBER_NAME_CHECKED( UAnemoneOutcomeManager, OutcomeList ) )
	{
		Init();
	}
}
#endif

EOutcomes UAnemoneOutcomeManager::ChooseOutcome( const TArray<int32> &Ratios_Array, const TArray<int32> &Immediate_Bonuses )
{
	if( ( Ratios_Array.Num() != Count.Num() ) || ( Ratios_Array.Num() != Immediate_Bonuses.Num() ) )
	{
		UE_LOG( LogTemp, Warning, TEXT("ChooseOutcome()  Ratios=%d != Count=%d or != Immediate=%d"), Ratios_Array.Num(), Count.Num(), Immediate_Bonuses.Num() );
		return EOutcomes::Error;
	}
	EOutcomes Chosen_Outcome = EOutcomes::Error;
	SetBaseVariables( Ratios_Array );
	CalculateRemainingOutcomes( Immediate_Bonuses );
	Chosen_Outcome = RandomOutcome();
	ResetState();
	return Chosen_Outcome;
}

void UAnemoneOutcomeManager::SetBaseVariables( const TArray<int32> &Ratios_Array )
{
	for( int i = 0; i < Ratios_Array.Num(); ++i )
	{
		Max_Net[ i ] = Ratios_Array[ i ] + Max_Permanent[ i ] + Max_Persistent[ i ];
		//UE_LOG( LogTemp, Warning, TEXT("SetBaseVars: Max_Net[%d]= %d, Ratios[%d]= %d, Perma= %d  Persist= %d"),
		//								i, Max_Net[ i ], i, Ratios_Array[ i ], Max_Permanent[ i ], Max_Persistent[ i ] );
	}
}

void UAnemoneOutcomeManager::CalculateRemainingOutcomes( const TArray<int32> &Immediate_Bonuses )
{
	int ActualRemainingTokens = 0;
	int TotalRemainingTokens = 0;
	int OnePerCentQuantity = 0;
	int AllOtherOutcomeTypes = NumberOfOutcomeTypes - 1;
	for( int i = 0; i < NumberOfOutcomeTypes; ++i )
	{
		ActualRemainingTokens = Max_Net[ i ] - Count[ i ];
		Remaining[ i ] = ActualRemainingTokens * GRANULARITY * AllOtherOutcomeTypes;
		TotalRemainingTokens += Remaining[ i ];
		//UE_LOG( LogTemp, Warning, TEXT("1st-Loop: Max_Net[%d]= %d, Count[%d]= %d, Actual= %d  Remaining= %d  Total= %d"),
		//i, Max_Net[ i ], i, Count[ i ], ActualRemainingTokens, Remaining[ i ], TotalRemainingTokens );
	}
	OnePerCentQuantity = TotalRemainingTokens / GRANULARITY;
	for( int ImmediateBonusIndex = 0; ImmediateBonusIndex < NumberOfOutcomeTypes; ++ImmediateBonusIndex )
	{
		if( Remaining[ ImmediateBonusIndex ] > 0 )
		{
			for( int i = 0; i < NumberOfOutcomeTypes; ++i )
			{
				if( i == ImmediateBonusIndex )
				{
					Remaining[ i ] += OnePerCentQuantity * Immediate_Bonuses[ ImmediateBonusIndex ];
				}
				else
				{
					Remaining[ i ] -= OnePerCentQuantity * Immediate_Bonuses[ ImmediateBonusIndex ] / AllOtherOutcomeTypes;
				}
				//UE_LOG( LogTemp, Warning, TEXT("2nd-Loop:  Imm= %d  i= %d  OnePerCentQuantity= %d  Remaining[ %d ]= %d"),
				//ImmediateBonusIndex, i, OnePerCentQuantity, i, Remaining[ i ] );
			}
		}
	}
}

EOutcomes UAnemoneOutcomeManager::RandomOutcome( )
{
	int RemainingOutcomes = AddArrayValuesUpToLength( Remaining, NumberOfOutcomeTypes );
	if( RemainingOutcomes <= 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT("RandomOutcome() || RemainingOutcomes <= 0 || NumberOfOutcomeTypes = %d"), NumberOfOutcomeTypes );
		return EOutcomes::Error;
	}
	int32 Random_Number = Fetch_Random_Number( 0, RemainingOutcomes - 1 );
	//UE_LOG( LogTemp, Warning, TEXT("Random Number: %d"), Random_Number );
	for( int i = 0; i < NumberOfOutcomeTypes; ++i )
	{
		if( Random_Number < AddArrayValuesUpToLength( Remaining, i + 1 ) )
		{
			++Count[ i ];
			//return CorrespondingOutcomes[ i ];
			return OutcomeList->List[ i ];
		}
	}
	return EOutcomes::Error;
}

void UAnemoneOutcomeManager::ResetState( )
{
	for( int i = 0; i < NumberOfOutcomeTypes; ++i )
	{
		if( Count[ i ] < Max_Net[ i ] )
		{
			return;
		}
	}
	for( int i = 0; i < NumberOfOutcomeTypes; ++i )
	{
		Count[ i ] -= Max_Net[ i ];
		Max_Persistent[ i ] = 0;
	}
}

int32 UAnemoneOutcomeManager::AddArrayValuesUpToLength( const TArray<int32> &Array, const int32 Max_Index )
{
	int Sum = 0;
	for( int i = 0; i < Max_Index; ++i )
	{
		Sum += Array[ i ];
	}
	return Sum;
}

int32 UAnemoneOutcomeManager::Fetch_Random_Number( const int32 First, const int32 Last )
{
	std::uniform_int_distribution<int32> Distribution( First, Last );
	return Distribution( Engine_Mersenne_Twister );
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "OutcomeTracker.h"

DEFINE_LOG_CATEGORY( LogOutcome );

const int NUMBER_OF_COMMON_OUTCOMES = 16;
const int GRANULARITY = 100;
thread_local Mersenne_Twister Engine_Mersenne_Twister( Create_PRNG_Engine() );

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EOutcome UOutcomeTracker::ChooseOutcome( const TMap< EOutcome, int32 >& InOutcomeRatio, TMap< EOutcome, int32 >& InImmediateBonuses )
{
	TMap< EOutcome, uint64 > AvailableOutcomes;
	InImmediateBonuses.KeySort( [] ( const EOutcome& A, const EOutcome& B )
		{ return A < B; }
	);
	DetermineOutcomeRatio( InOutcomeRatio, InImmediateBonuses, AvailableOutcomes );
	AvailableOutcomes.KeySort( [] ( const EOutcome& A, const EOutcome& B )
		{ return A < B; }
	);
	return SelectRandomOutcome( AvailableOutcomes, InOutcomeRatio );
}

void UOutcomeTracker::DetermineOutcomeRatio( const TMap< EOutcome, int32 >& InOutcomeRatio, const TMap< EOutcome, int32 >& InImmediateBonuses, TMap< EOutcome, uint64 >& OutAvailableOutcomes ) const
{
	const int32* ValuePtr = nullptr;
	int32 Value = -1;
	for( const TPair< EOutcome, int32 >& Item : InOutcomeRatio )
	{
		ValuePtr = Count.Find( Item.Key );
		Value = ( ValuePtr ) ? *ValuePtr : 0;
		OutAvailableOutcomes.FindOrAdd( Item.Key ) = Item.Value - Value;
		UE_LOG( LogOutcome, Verbose, TEXT("Determine-Loop: Item: %d."), Item.Value );
		UE_LOG( LogOutcome, Verbose, TEXT("Determine-Loop: *TempCount: %d."), Value );
		UE_LOG( LogOutcome, Verbose, TEXT("Determine-Loop: OutAvailableOutcomes.FindOrAdd( Item.Key ): %d."), OutAvailableOutcomes[ Item.Key ] );
	}
	UE_LOG( LogOutcome, Warning, TEXT("InOutcomeRatios: %d	:	OutAvailableOutcomes = %d."), InOutcomeRatio.Num() , OutAvailableOutcomes.Num() );
	for( TPair< EOutcome, int32 > BonusItem : InImmediateBonuses )
	{
		if( BonusItem.Value != 0 && OutAvailableOutcomes.Contains( BonusItem.Key ) )
		{
			// TODO: Check that percentage does not exceed the complement of 100%.
			IncreaseRateByPercentage( OutAvailableOutcomes, BonusItem );
		}
	}
}

void UOutcomeTracker::IncreaseRateByPercentage( TMap< EOutcome, uint64 >& OutAvailableOutcomes, const TPair< EOutcome, int32 >& InRateBonus )
{
	int64 PercentageIncrease = 0;
	int64 RatioUnitAmount = 0;
	int64 RelativeRatioSum = SumOfComplement( OutAvailableOutcomes, InRateBonus.Key );
	for( const TPair< EOutcome, uint64 >& Item : OutAvailableOutcomes )
	{
		PercentageIncrease = PercentageIncrease + Item.Value;
	}
	PercentageIncrease = PercentageIncrease * InRateBonus.Value;
	RatioUnitAmount = PercentageIncrease / RelativeRatioSum;
	for( TPair< EOutcome, uint64 >& Item : OutAvailableOutcomes )
	{
		if( Item.Key == InRateBonus.Key )
		{
			Item.Value = Item.Value * GRANULARITY * RelativeRatioSum + PercentageIncrease;
		}
		else
		{
			//Item.Value = Item.Value * GRANULARITY * RelativeRatioSum - RatioUnitAmount * Item.Value );
			Item.Value = Item.Value * ( GRANULARITY * RelativeRatioSum - RatioUnitAmount );
		}
	}
}

EOutcome UOutcomeTracker::SelectRandomOutcome( const TMap< EOutcome, uint64 >& InAvailableOutcomes, const TMap< EOutcome, int32 >& InOutcomeRatio )
{
	UE_LOG( LogOutcome, Verbose, TEXT("InAvailableOutcomes.Num(): %d."), InAvailableOutcomes.Num() );
	for( auto Item : InAvailableOutcomes )
	{
		UE_LOG( LogOutcome, Verbose, TEXT("Item: %d."), Item.Value );
	}
	uint64 AvailableOutcomeCount = AddMapValuesUpToLength( InAvailableOutcomes, InAvailableOutcomes.Num() );
	uint64 RandomNumber = -1;
	if( AvailableOutcomeCount <= 0 )
	{
		UE_LOG( LogOutcome, Error, TEXT("RandomOutcome()	:	AvailableOutcomeCount <= 0  ||  NumberOfOutcomeTypes = %d"), InAvailableOutcomes.Num() );
		return EOutcome::Invalid;
	}
	RandomNumber = FetchRandomNumber( 0, AvailableOutcomeCount - 1 );
	UE_LOG( LogOutcome, Verbose, TEXT("Random Number: %d	:	AvailableOutcomeCount = %d."), RandomNumber , AvailableOutcomeCount );
	int32 i = 1;
	for( auto Item : InAvailableOutcomes )
	{
		if( RandomNumber < AddMapValuesUpToLength( InAvailableOutcomes, i ) )
		{
			++Count.FindOrAdd( Item.Key );
			//UpdateState( InAvailableOutcomes );
			UpdateState( InOutcomeRatio );
			return Item.Key;
		}
		++i;
	}
	UE_LOG( LogOutcome, Error, TEXT("Could not find Outcome among available Outcomes.") );
	return EOutcome::Invalid;
}

uint64 UOutcomeTracker::SumOfComplement( const TMap< EOutcome, uint64 >& InMap, const EOutcome& InException )
{
	uint64 Sum = 0;
	for( TPair< EOutcome, uint64 > Item : InMap )
	{
		if( Item.Key == InException )
		{
			continue;
		}
		Sum = Sum + Item.Value;
	}
	return Sum;
}

uint64 UOutcomeTracker::AddMapValuesUpToLength( const TMap< EOutcome, uint64 >& InMap, int32 InMaxIndex )
{
	int Sum = 0;
	int ItemCount = 0;
	for( auto Item : InMap )
	{
		if( ItemCount >= InMaxIndex )
		{
			break;
		}
		Sum = Sum + Item.Value;
		++ItemCount;
	}
	return Sum;
}

uint64 UOutcomeTracker::FetchRandomNumber( uint64 First, uint64 Last )
{
	std::uniform_int_distribution< uint64 > Distribution( First, Last );
	return Distribution( Engine_Mersenne_Twister );
}

void UOutcomeTracker::UpdateState( const TMap< EOutcome, int32 >& InMap )
{
	int32* Number = nullptr;
	for( const TPair< EOutcome, int32 > Item : InMap )
	{
		Number = Count.Find( Item.Key );
		if( !Number || *Number < Item.Value )
		{
			return;
		}
		UE_LOG( LogOutcome, Verbose, TEXT("UOutcomeTracker::UpdateState()	:	Count = %d : Item Value = %d."), *Number, Item.Value );
	}
	UE_LOG( LogOutcome, Verbose, TEXT("UOutcomeTracker::UpdateState()	:	BAG RESET.") );
	for( auto Item : InMap )
	{
		Count[ Item.Key ] -= Item.Value;
	}
}
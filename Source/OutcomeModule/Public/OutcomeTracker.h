// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <random>
#include "OutcomeTracker.generated.h"
using Mersenne_Twister = std::mt19937;

DECLARE_LOG_CATEGORY_EXTERN( LogOutcome, Log, All );

UENUM( Blueprintable )
enum class EOutcome : uint8
{
	Invalid			UMETA(DisplayName = "INVALID"),
	Loss			UMETA(DisplayName = "LOSS"),
	Win				UMETA(DisplayName = "WIN"),

	Weak1			UMETA(DisplayName = "WEAK 1"),
	Weak2			UMETA(DisplayName = "WEAK 2"),
	Weak3			UMETA(DisplayName = "WEAK 3"),
	Hit				UMETA(DisplayName = "HIT"),
	Strong1			UMETA(DisplayName = "STRONG 1"),
	Strong2			UMETA(DisplayName = "STRONG 2"),

	Retaliate		UMETA(DisplayName = "TECH: RETALIATE"),
	Parry			UMETA(DisplayName = "TECH: PARRY"),
	Counter			UMETA(DisplayName = "TECH: COUNTER"),
	//FailTechnique	UMETA(DisplayName = "FAIL PARRY"),

	Disappear		UMETA(DisplayName = "EVAD: DISAPPEAR"),
	Evade			UMETA(DisplayName = "EVAD: EVADE"),
	Graze			UMETA(DisplayName = "EVAD: GRAZE"),
	Torso			UMETA(DisplayName = "TORSO"),
	Arms			UMETA(DisplayName = "ARMS"),
	Legs			UMETA(DisplayName = "LEGS"),
	Head			UMETA(DisplayName = "HEAD"),

	//Pushback		UMETA(DisplayName = "BLCK: PUSH BACK"),
	Deflect			UMETA(DisplayName = "BLCK: DEFLECT"),
	Absorb			UMETA(DisplayName = "BLCK: ABSORB"),
	//FailBlock		UMETA(DisplayName = "FAIL BLOCK"),
	PushedBack		UMETA(DisplayName = "BLCK: PUSHED_BACK"),
	//Knockdown		UMETA(DisplayName = "KNOCKDOWN"),
	Error			UMETA(DisplayName = "ERROR")
};
using OutcomeList = TArray< EOutcome >;

extern const int GRANULARITY;
extern thread_local Mersenne_Twister Engine_Mersenne_Twister;

Mersenne_Twister Create_PRNG_Engine( );
unsigned gcd_recursive( unsigned a, unsigned b );

/**
 *	Rolls for a Random Outcome from the limited contents of a bag,
*	removing it in the process.
* 
*	When the bag is empty, it is replaced with a new one.
 */
UCLASS( Blueprintable, DisplayName="Outcome Tracker", collapseCategories )
class OUTCOMEMODULE_API UOutcomeTracker : public UObject
{
	GENERATED_BODY()
public:
	// Keeps track of Past Outcomes rolled.
	UPROPERTY( VisibleAnywhere )
	TMap< EOutcome, int32 > Count;

	// Draw a Random Outcome from the Bag.
	UFUNCTION( BlueprintCallable, Category="Outcome")
	EOutcome ChooseOutcome( const TMap< EOutcome, int32 >& Outcome_Ratios, TMap< EOutcome, int32 >& Immediate_Bonuses );

	/*	Determines the Ratio or Odds of an Outcome being selected.
	*	InOutcomeRatios is the base ratio of Outcomes.
	*	InImmediateBonuses is the bonus percentage odds of an Outcome being selected this time.
	*	OutAvailableOutcomes is the TMap that will be filled out by the function.
	*/
	void DetermineOutcomeRatio(
		const TMap< EOutcome, int32 >& InOutcomeRatio,
		const TMap< EOutcome, int32 >& InImmediateBonuses,
		TMap< EOutcome, uint64 >& OutAvailableOutcomes ) const;

	/*	Increase the Ratio of a specific Outcome by a percentage of the total Outcomes.  e.g.  Odds of "Dodge" +12%
	*	This function preserves the ratio that the remaining Outcomes have relative to each other.
	*/
	static void IncreaseRateByPercentage( TMap< EOutcome, uint64 >& OutAvailableOutcomes, const TPair< EOutcome, int32 >& InRateBonus );

	// Select Random Outcome among Remaining Outcomes.
	EOutcome SelectRandomOutcome( const TMap< EOutcome, uint64 >& InAvailableOutcomes, const TMap< EOutcome, int32 >& InOutcomeRatio );

	//	Ignoring the Exception Key, returns the sum of Values of the Map.
	static uint64 SumOfComplement( const TMap< EOutcome, uint64 >& InMap, const EOutcome& InException );

	/*	Adds Values in Map up until a certain integer amount.
	*	This function assumes the Map is already sorted by Key.
	*/
	static uint64 AddMapValuesUpToLength( const TMap< EOutcome, uint64 >& InMap, int32 InMaxIndex );

	// Generates Pseudo-Random Number within range First-Last.
	static uint64 FetchRandomNumber( uint64 First, uint64 Last );

	// Decrements Past Outcome Count if a full ratio threshold is reached.
	void UpdateState( const TMap< EOutcome, int32 >& InMap );
};

using OutcomeTracker = UOutcomeTracker;
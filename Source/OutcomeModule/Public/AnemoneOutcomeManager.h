// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <random>
#include "AnemoneOutcomeManager.generated.h"
using Mersenne_Twister = std::mt19937;

UENUM( Blueprintable )
enum class EOutcomes : uint8
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

	Retaliate		UMETA(DisplayName = "RETALIATE"),
	Parry			UMETA(DisplayName = "PARRY"),
	FailParry		UMETA(DisplayName = "FAIL PARRY"),

	Evade			UMETA(DisplayName = "EVADE"),
	Graze			UMETA(DisplayName = "GRAZE"),
	Torso			UMETA(DisplayName = "TORSO"),
	Arms			UMETA(DisplayName = "ARMS"),
	Legs			UMETA(DisplayName = "LEGS"),
	Head			UMETA(DisplayName = "HEAD"),

	Pushback		UMETA(DisplayName = "PUSH BACK"),
	Deflect			UMETA(DisplayName = "DEFLECT"),
	Absorb			UMETA(DisplayName = "ABSORB"),
	FailBlock		UMETA(DisplayName = "FAIL BLOCK"),
	Knockback		UMETA(DisplayName = "KNOCKBACK"),
	Knockdown		UMETA(DisplayName = "KNOCKDOWN"),
	Error			UMETA(DisplayName = "ERROR")
};

UCLASS( DisplayName="Outcome List" )
//class COMBAT_DEMO_API UAnemoneOutcomeList : public UDataAsset
class OUTCOMEMODULE_API UAnemoneOutcomeList : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY( SaveGame, EditAnywhere )
	TArray< EOutcomes > List;

	UAnemoneOutcomeList();
};

extern const int NUMBER_OF_COMMON_OUTCOMES;
extern const int GRANULARITY;
extern thread_local Mersenne_Twister Engine_Mersenne_Twister;

extern TArray< EOutcomes > BinaryOutcomes;
extern TArray< EOutcomes > PrimarySkillOutcomes;

Mersenne_Twister Create_PRNG_Engine( );
unsigned gcd_recursive( unsigned a, unsigned b );

/**
 *	Rolls for a Random Outcome from the limited contents of a bag,
*	removing it in the process.
* 
*	When the bag is empty, it is replaced with a new one.
 */
UCLASS( Blueprintable, DisplayName="Outcome Manager", collapseCategories )
//class COMBAT_DEMO_API UAnemoneOutcomeManager : public UObject
class OUTCOMEMODULE_API UAnemoneOutcomeManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY( SaveGame, EditAnywhere )
	UAnemoneOutcomeList* OutcomeList;

	UAnemoneOutcomeManager();
	void Init();

#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif

	UFUNCTION( BlueprintCallable, Category="Outcome")
	static UAnemoneOutcomeManager* CreateOutcomeManager( const TArray<EOutcomes> &ListOfOutcomes );

/**
 *	Draw a Random Outcome from the Bag.
 */
	UFUNCTION( BlueprintCallable, Category="Outcome")
	EOutcomes ChooseOutcome( const TArray<int32> &Outcome_Ratios, const TArray<int32> &Immediate_Bonuses );
private:
	UPROPERTY( SaveGame )
	int32 NumberOfOutcomeTypes;

	UPROPERTY( SaveGame, VisibleAnywhere )
	TArray< int32 > Count;

	UPROPERTY( SaveGame )
	TArray< int32 > Max_Permanent;

	UPROPERTY( SaveGame )
	TArray< int32 > Max_Persistent;

	UPROPERTY()
	TArray< int32 > Max_Net;

	UPROPERTY()
	TArray< int32 > Remaining;

	void SetBaseVariables( const TArray<int32> &Max_Base_Input );
	void CalculateRemainingOutcomes( const TArray<int32> &Immediate_Bonuses );
	EOutcomes RandomOutcome();
	void ResetState( );

	int32 AddArrayValuesUpToLength( const TArray<int32> &Array, const int32 Count );
	int32 Fetch_Random_Number( const int32 First, const int32 Last );
};

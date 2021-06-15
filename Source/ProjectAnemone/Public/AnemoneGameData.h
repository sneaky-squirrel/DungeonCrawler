#pragma once
#include "CoreMinimal.h"
#include "AnemoneEnum.h"

#include "Containers/Map.h"
#include "Containers/Queue.h"

#include "AnemoneGameData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogEncounter, Log, All );

enum class EOutcome : uint8;
enum class EAnemoneScoreRank : uint8;

using OutcomeList = TArray< EOutcome >;
using NameList = TArray< FName >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////	Entity Scores - e.g. Characters, and Monsters.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT( BlueprintType )
struct FAnemoneScore
{
	GENERATED_BODY()

	UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	int32 Base;

	UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	int32 Delta;

	FAnemoneScore();
	FAnemoneScore( int32 Amount );

	int32 GetTotal() const;

	//friend FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score );
	//friend FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneScore >& ScoreSheet );
};
using Score = FAnemoneScore;

	FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score );
	FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneScore >& ScoreSheet );

//	Defines the Score that the Instigators and Defenders use to compare against each other.		e.g. Strength vs Block.
struct FAnemoneScorePair
{
	EAnemoneScore Instigator;
	EAnemoneScore Defender;

	FAnemoneScorePair( EAnemoneScore InInstigator, EAnemoneScore InTarget );
	FAnemoneScorePair( const FAnemoneScorePair& InSet );
};
using ScorePair = FAnemoneScorePair;

//	Keeps track of the Rank Lists for Instigators and Defenders.
struct FAnemoneRankTracker
{
public:
	TArray< EAnemoneScoreRank > Instigator;
	TArray< EAnemoneScoreRank > Defender;
};
using RankTracker = FAnemoneRankTracker;

/*	Defines the Score that the Instigators and Defenders use to compare against each other.		e.g. Strength vs Block.
*	Also defines the types of Outcomes an Instigator or Defender contributes to.
*/
struct FAnemoneScoreCheck
{
public:
	FAnemoneScoreCheck( const ScorePair& InScoreSet, const TArray< EOutcome >& InInstigator, const TArray< EOutcome >& InTarget );

	//	e.g. Strength vs Block.
	ScorePair ScoreSet;

	//	List of Outcomes an Instigator contributes to.	e.g. [ Hit ]
	TArray< EOutcome > InstigatorOutcomeList;

	//	List of Outcomes a Target contributes to.	e.g. [ Deflect, Absorb, Pushed Back ]
	TArray< EOutcome > DefenderOutcomeList;
};
using ScoreCheck = FAnemoneScoreCheck;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////	Event Structures
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//USTRUCT( BlueprintType )
USTRUCT()
struct FAnemoneEventData
{
	GENERATED_BODY()

	//UPROPERTY( BlueprintReadWrite )
	UPROPERTY()
	FName Target;

	//UPROPERTY( BlueprintReadWrite )
	UPROPERTY()
	TArray< FName > InstigatorList;

	//UPROPERTY( BlueprintReadWrite )
	UPROPERTY()
	TArray< FName > TargetList;

	//UPROPERTY( BlueprintReadWrite )
	UPROPERTY()
	EOutcome Outcome;

	UPROPERTY()
	int32 Indentation;

	FAnemoneEventData();
	//FAnemoneEventData( const FName InTarget, const NameList& InInstigator, const NameList& InTargetList, const EOutcome InOutcome );
	FAnemoneEventData(	const FName InTarget, const NameList& InInstigator, const NameList& InTargetList,
						const EOutcome InOutcome, int32 InIndentation );
};
using EventData = FAnemoneEventData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////	Action Queue
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UAnemoneAction;

USTRUCT( BlueprintType )
struct FAnemoneActionInstance
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UAnemoneAction* Action;

	UPROPERTY()
	int32 Indentation;

	UPROPERTY()
	TArray< FName > InstigatorList;

	UPROPERTY()
	TArray< FName > TargetList;

	FAnemoneActionInstance();
	FAnemoneActionInstance( UAnemoneAction* InAction, int32 InIndentation, const TArray< FName >& InInstigator, const TArray< FName >& InTarget );
};
using ActionInstance = FAnemoneActionInstance;

UCLASS( Blueprintable )
class PROJECTANEMONE_API UAnemoneActionInstanceQueue : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable, Category="ActionQueue" )
	void PushAction( UAnemoneAction* InItem, int32 InIndentation, const TArray< FName >& InInstigator, const TArray< FName >& InTarget );

	UFUNCTION( BlueprintCallable, Category="ActionQueue" )
	bool PopAction( FAnemoneActionInstance& Entry );

protected:
	TQueue< ActionInstance > Queue;
};
using ActionInstanceQueue = UAnemoneActionInstanceQueue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////	Log Queue
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS( Blueprintable )
class PROJECTANEMONE_API UAnemoneLogQueue : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable, Category="Log" )
	void Push( const FText& InLogText );

	UFUNCTION( BlueprintCallable, Category="Log" )
	bool Pop( FText& OutLogText);

protected:
	TQueue< FText > Queue;
};
using LogQueue = UAnemoneLogQueue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////	Global Variables
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Score
extern const TArray< EScore > ScoresAll;
extern const ScoreList ScoresPoolEncounter;
extern const ScoreList ScoresComparison;
extern const ScoreList ScoresOffense;
extern const ScoreList ScoresDefense;

//	Score Pair
extern const ScorePair ScorePairTechnique;
extern const ScorePair ScorePairEvasion;
extern const ScorePair ScorePairBlock;

//	Outcome List
extern const OutcomeList OutcomesTechnique;
extern const OutcomeList OutcomesEvasion;
extern const OutcomeList OutcomesBlock;
extern const OutcomeList OutcomesHit;

//	Score Checks	e.g.	Strength vs Block.
extern const TMap< EScoreSet, ScoreCheck > ScoreCheckMap;

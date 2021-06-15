// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AnemoneGameData.h"
#include "AnemoneAction.generated.h"

class UAnemoneEvent_ChangeEntity;
using EntityEvent = UAnemoneEvent_ChangeEntity;

class UAnemoneEvent_Outcome;
using OutcomeEvent = UAnemoneEvent_Outcome;

struct FAnemoneScorePair;
using ScorePair = FAnemoneScorePair;

struct FAnemoneRankTracker;
using RankTracker = FAnemoneRankTracker;

enum class EOutcome : uint8;

using NameList = TArray< FName >;
using IntList = TArray< int32 >;

/*	An Outcome defines the Event List corresponding to a [EOutcome, SubAction] pair.
*/
USTRUCT( BlueprintType )
struct FAnemoneSubActionOutcome
{
	GENERATED_BODY()

	/*	Maximum number of selected Entities that the Action will affect for this particular Outcome.
	*	Similar to the Action's [MaximumManualCount], but this one can decrease the Maximum even further. e.g. Unfavorable Outcome.
	*/
	UPROPERTY( EditAnywhere )
	int32 SelectedCountCap;

	/*	Maximum number of times that the Action will target a random Entity.
	*	Limited by the Maximum Iterations for Group and Entity.
	*/
	UPROPERTY( EditAnywhere )
	int32 RandomCountCap;

	/*	List of Events to execute when this Outcome is selected.
	*	May include events that alter an Entity's score or add status conditions.
	*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Instanced )
	TArray< UAnemoneEvent_ChangeEntity* > EventList;
};
using SubActionOutcome = FAnemoneSubActionOutcome;

/*	Defines a Sub-Action.  A Sub-Action holds an Outcome Map, which maps an EOutcome to a SubAction-Outcome, which holds an Event List.
*	When the Action rolls an EOutcome, it will map the EOutcome to the Outcome Map of each Sub-Action.
*	e.g.  Fire and Frost rolls Success, map [Success] into the Sub-Actions [Fire] and [Ice].
*/
USTRUCT( BlueprintType )
struct FAnemoneSubAction
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere )
	TMap< EOutcome, FAnemoneSubActionOutcome > OutcomeMap;
};
using SubAction = FAnemoneSubAction;

/*	Defines an Action Selection.  An Action Selection holds a list of Sub-Actions that share a common Target Selection restriction.
*	e.g.  An Action that inflicts fire damage to one target, and inflicts ice damage to a different target.
*	The same target may not be selected more than once in this case.
*/
USTRUCT( BlueprintType )
struct FAnemoneTargetSelection
{
	GENERATED_BODY()

	/*	Maximum number of times the User may select a Target.
	*	e.g. Strike up to [3] different targets.	( Each target may only be targeted once )
	*	e.g. Strike up to [8] targets.				( Each individual Entity may only be targeted up to 3 times )
	*/
	UPROPERTY( EditAnywhere )
	int32 MaximumSelectedCount;

	/*	Maximum number of times that an Entity inside the same Group may be targeted.
	*	Mainly intended to stop Area-Effect actions from repeatedly bombarding the same group.
	*/
	UPROPERTY( EditAnywhere )
	int32 MaximumIterationGroup;

	/*	Maximum number of times that an individual Entity may be targeted.
	*	Mainly intended to stop multi-hit actions from annihalating a single target with too many iterations.
	*/
	UPROPERTY( EditAnywhere )
	int32 MaximumIterationEntity;

	//	List of SubActions corresponding to this list of Targets.
	UPROPERTY( EditAnywhere )
	TArray< FAnemoneSubAction > ActionList;
};
using TargetSelection = FAnemoneTargetSelection;

/*	Outcome Chance defines the odds of an Outcome being selected.
*	Includes Base Rate, Entity Contribution, and Assist Contribution.
*/
USTRUCT( BlueprintType )
struct FAnemoneOutcomeChance
{
	GENERATED_BODY()

	//	Base Amount of "Marbles" for this Outcome.
	UPROPERTY( SaveGame, EditAnywhere )
	//UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	int32 BaseRate;

	/*	Defines how many "Marbles" or Outcome Instances an Entity contributes.
	*	Entities contribute "Marbles" based on their Score Ranking.
	*	The Entities with the highest Scores contribute the most Marbles.
	*	Score Ranking is determined by looking at a Pair of Scores.
	*	e.g. Speed and Technique, Accuracy and Evasion, Strength and Block, Speed and Speed.
	*/
	UPROPERTY( SaveGame, EditAnywhere )
	//UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	TMap< EAnemoneScoreRank, int32 > EntityContribution;

	/*	Similar to EntityContribution, but excludes both the main instigator and defender.
	*/
	UPROPERTY( SaveGame, EditAnywhere )
	//UPROPERTY( SaveGame, EditAnywhere, BlueprintReadWrite )
	TMap< EAnemoneScoreRank, int32 > AssistContribution;
};
using OutcomeChance = FAnemoneOutcomeChance;

/**	Defines an Action.  Including the possible Outcomes, the odds of an Outcome, Target Selection, and the Event Lists to execute for each
*	Outcome.
*	e.g.	Fireball may define "Hit" and "Evade" Outcomes, "Hit" means the Fireball hit its target
*			and may now deal Fire damage and possibly a Burn status condition.
*			"Evade" means the target dodged the Fireball and will not take damage, but may be inflicted with a status
*			condition such as "Distracted" or "Startled".
*/
UCLASS( Blueprintable, DefaultToInstanced )
class PROJECTANEMONE_API UAnemoneAction : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName HostID;

	UFUNCTION( BlueprintCallable )
	static UAnemoneAction* CreateAction( FName InHostID, TSubclassOf< UAnemoneAction > InActionType );

	//	Executes an Encounter Action.
	UFUNCTION( BlueprintCallable )
	static void ExecuteAction( const FAnemoneActionInstance& InEntry, const TArray< FName >& InEntityAll, UAnemoneLogQueue* OutTextLog );

protected:
	//	This Action's Identifier
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ID;

	UPROPERTY( EditAnywhere )
	TSet< EAnemoneScoreSet > ScoreSetList;

	UPROPERTY( EditAnywhere )
	TMap< EOutcome, FAnemoneOutcomeChance > OutcomeChanceMap;

	UPROPERTY( EditAnywhere )
	TArray< FAnemoneTargetSelection > SelectionList;

	EOutcome RollOutcome( const TArray< FName >& InInstigator, const TArray< FName >& InDefender );

	static void ExecuteEvent( EntityEvent* const InEvent, const NameList& InEntity );

	//	Pass the Event through every module in the encounter.
	static void DispatchEvent( EntityEvent* const InEvent, const NameList& InEntity );

	// Create Score Rank lists corresponding to one Score Pair.
	static void GetRankLists( const ScorePair& InScores, const NameList& InInstigator, const NameList& InDefender, RankTracker& OutRanks );

	// Add Outcomes to Outcome Event based on Rank List.  Only add Outcome types included inside InOutcmesList.
	void AddOutcomes( const TArray< EOutcome >& InOutcomesList, const TArray< ERank >& InRankList, OutcomeEvent* OutEvent );

	static void AddRankToList( const int32& InBaseScore, const int32& InMaxScore, TArray< ERank >& OutRankList );
	static void GetScoreList( const NameList& InEntity, const EScore& InScoreName, IntList& OutScores );

	friend class UAnemoneEvent_ChangeEntity;
};
using Action = UAnemoneAction;

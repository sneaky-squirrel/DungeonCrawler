// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "OutcomeTracker.h"
#include "AnemoneEnum.h"

#include "AnemoneEvent.generated.h"

struct FAnemoneEventData;
using EventData = FAnemoneEventData;

class UAnemoneEvent_ChangeEntity;
using EntityEvent = UAnemoneEvent_ChangeEntity;

class UAnemoneEvent_Outcome;
using OutcomeEvent = UAnemoneEvent_Outcome;

class UAnemoneEvent_GetScore;
using ScoreEvent = UAnemoneEvent_GetScore;

class UAnemoneEvent_Delta;
using DeltaEvent = UAnemoneEvent_Delta;

class UAnemoneAction;
using Action = UAnemoneAction;

class UAnemoneLogQueue;
using LogQueue = UAnemoneLogQueue;

UENUM( BlueprintType, meta = ( ScriptName = "EnumeratedEvent" ) )
enum class EAnemoneEvent : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid             UMETA( DisplayName = "INVALID" ),

    // Broadcast that a Round has started.
    RoundStart			UMETA( DisplayName = "ROUND_START" ),

    // Broadcast that a Root Action has taken place.
    //Root				UMETA( DisplayName = "ROOT" ),

    // Fetches the total value of an Entity's specific score.
    GetScore			UMETA( DisplayName = "GET_SCORE" ),

    // Fetches the Outcome ratio for an Outcome.
    Outcome				UMETA( DisplayName = "OUTCOME" ),

	// Adds or Removes Modules from an Entity.
    AddModule			UMETA( DisplayName = "ADD_MODULE" )
};

using EEvent = EAnemoneEvent;

class UAnemoneEntityModule;

/**
 *		An Encounter Event represents an event that occurs during an encounter.
*			An Encounter is a turn-based RPG fight.
*		e.g. When damage is inflicted on an entity.
*		e.g. When an entity executes an action.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent : public UObject
{
	GENERATED_BODY()
public:
	// Identifies the Entity that instigated the event.
	UPROPERTY( BlueprintReadWrite )
	FName HostID;

	// Identifies Action the Event corresponds to.
	UPROPERTY( BlueprintReadWrite )
	FName ActionID;

	/*	Contains most details regarding the Action.
	*	Is it an Offense / Defense / Support action?  Is it Magic?  Does it inflict Damage?
	*	Is the damage Physical / Elemental?  Fire, Water, Earth, Wi-nd, Ice?
	*	What Reach / Range does the action have?  Melee, Short, Long?
	*/
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FGameplayTagContainer TagSet;

	/*	The Event's Outcome.
	*	Depending on the event, the Outcome is either being determined or is already determined.
	*/
	UPROPERTY( BlueprintReadWrite )
	EOutcome Outcome;

	/*	An event is either a Query or a Broadcast.
	*	A Query allows an Entity to alter an event, a Broadcast allows an Entity to respond to a resolved event.
	*/
	UPROPERTY( BlueprintReadWrite )
	bool bIsBroadcast = false;

	UFUNCTION( BlueprintCallable )
	void PrintLogText( const FText& InText );

	UFUNCTION( BlueprintCallable )
	FString GetIndentation() const;

	//virtual UWorld* GetWorld() const override;

protected:
	// Communicates the event's type to the Module so that the Module knows whether or not to handle it.
	UPROPERTY( BlueprintReadWrite )
	EAnemoneEvent EventType;

	UPROPERTY()
	int32 Indentation;

	UPROPERTY( BlueprintReadOnly )
	TArray< FName > InstigatorList;

	UPROPERTY( BlueprintReadOnly )
	TArray< FName > TargetList;

	UPROPERTY( BlueprintReadWrite )
	FName Target;

	TWeakObjectPtr< LogQueue > TextLog;
};

/**
 *		An Encounter Event that fetches the total score of an entity.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_GetScore : public UAnemoneEvent
{
	GENERATED_BODY()
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EAnemoneScore ScoreType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int32 ScoreValue;

	static ScoreEvent* CreateEvent( EAnemoneScore InScoreType, int32 InScoreValue );
};

/**
 *		An Encounter Event that fetches data to determine an Outcome or "Dice Roll".
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_Outcome : public UAnemoneEvent
{
	GENERATED_BODY()
public:
	/*	Counts the Total Outcome Ratio.
	*	This decides the Outcome Rate of an Entity's actions.
	*/
	UPROPERTY( BlueprintReadWrite )
	TMap< EOutcome, int32 > OutcomeRatio;

	/*	Counts the Bonus Percentage chance of the Outcomes.
	*	e.g. Block: 20 means +20% Chance to Block this turn
	*		If the base ratio were 3 : 2 | Block : Hit,
			Block would have a total chance of 60% + 20% = 80%
			chance to be selected this time.
	*/
	UPROPERTY( BlueprintReadWrite )
	TMap< EOutcome, int32 > OutcomeBonus;

	static OutcomeEvent* CreateEvent();
};

/**
 *		An Event that changes an Entity.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_ChangeEntity : public UAnemoneEvent
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable )
	UObject* GetInstigator() const;

	UFUNCTION( BlueprintCallable )
	FString GetInstigatorName() const;

	UFUNCTION( BlueprintCallable )
	FString GetTargetName() const;

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void Execute();

	virtual void Execute_Implementation();

	virtual EntityEvent* InitCopy( const Action* const InAction, const EventData& InData, LogQueue* const InTextLog ) const;
};

USTRUCT( BlueprintType )
struct FEntityScorePercentage
{
	GENERATED_BODY()
public:
	FName Host;
	EAnemoneScore Score;
	float Percentage;

	FEntityScorePercentage();
	FEntityScorePercentage( FName InHost, EAnemoneScore InScore, float InPercentage );
};

/**
 *		An Encounter Event that changes an Entity's score.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_Delta : public UAnemoneEvent_ChangeEntity
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable )
	void AddConstantMod( const int32& InConstant );

	UFUNCTION( BlueprintCallable )
	void AddScoreMod( FName InHost, const EAnemoneScore& InScore, const float& InPercentage );

	UFUNCTION( BlueprintCallable )
	void AddMultiplierMod( const float& InMultiplierMod );

	UFUNCTION( BlueprintCallable )
	int32 GetDelta() const;

	UFUNCTION( BlueprintCallable )
	void ApplyDelta( int32 InAmount );

	void Execute_Implementation();

protected:
	//	Additional Constant Value.
	UPROPERTY()
	int32 AddFlatConstant = 0;

	//	Additional Value, value is a percentage of an Entity's score
	UPROPERTY()
	TArray< FEntityScorePercentage > AddFlatScore;

	//	Percentage Multiplier.
	UPROPERTY()
	float Multiplier = 0.0;

	//	Score Type.
	UPROPERTY( EditAnywhere )
	EAnemoneScore Score;

	//	Base Value.
	UPROPERTY( EditAnywhere )
	int32 Base;

	UAnemoneEvent_Delta();
};

/**
 *		An Encounter Event that Adds a Module to an Entity.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_ModuleAdd : public UAnemoneEvent_ChangeEntity
{
	GENERATED_BODY()
public:

protected:
	//	Module to Add or Remove.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Instanced )
	UAnemoneEntityModule* Module;

	UAnemoneEvent_ModuleAdd();
};

/**
 *		An Encounter Event that Removes Modules from an Entity that meet certain criteria.
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEvent_ModuleRemove : public UAnemoneEvent_ChangeEntity
{
	GENERATED_BODY()
public:

protected:
	//	Modules with this Identifier will be removed.
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ModuleIdentifier;

	//	Modules whose tags match this Query will be removed.
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FGameplayTagQuery Query;

	UAnemoneEvent_ModuleRemove();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameplayTagContainer.h"

#include "AnemoneEntityModule.generated.h"

//	EScore
enum class EAnemoneScore : uint8;
using EScore = EAnemoneScore;

//	Event
class UAnemoneEvent;
using Event = UAnemoneEvent;

//	Tag Container
using Tags = FGameplayTagContainer;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Module
class UAnemoneEntityModule;
using Module = UAnemoneEntityModule;
/**
 *	All changes to Entities, such as Monsters or Characters, are implemented through Modules.
 *	e.g. Taking Damage, other Score changes, Gaining Status Conditions, etc.  
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneEntityModule : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent  )
	bool HandleEvent( UAnemoneEvent* InEvent );

	virtual Module* CreateCopy( const FName InHost, const FName InInstigator, const FName InAction );

	void Init( const FName InHost, const FName InInstigator, const FName InAction );
protected:
	//	The Host posseses the Module.
	FName HostID;

	//	The Instigator created the Module.
	FName InstigatorID;

	//	Action that created the Module.
	FName ActionID;

	UPROPERTY( EditAnywhere )
	FGameplayTagContainer TagSet;

	//	Remaining Amount of Turns before the Module expires.  -1 == Infinite Turns
	UPROPERTY( EditAnywhere )
	int32 CountTurn = -1;

	//	Remaining Amount of Uses before the Module expires.  -1 == Infinite Uses
	UPROPERTY( EditAnywhere )
	int32 CountUse = -1;
};

//	Delta Module
class UAnemoneEntityModule_Delta;
using DeltaModule = UAnemoneEntityModule_Delta;
/**
 *	This Module type implements a list of Score Deltas.
 *	e.g. Taking Damage, Healing, Positive / Negative Strength modifier, etc.  
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneEntityModule_Delta : public UAnemoneEntityModule
{
	GENERATED_BODY()
public:
	static DeltaModule* CreateDelta(	const FName InHostID, const FName InInstigatorID, const FName InActionID,
										const Tags& InTagSet, EScore InScoreType, int32 InValue );

	bool HandleEvent_Implementation( Event* InEvent );

	virtual Module* CreateCopy( const FName InHost, const FName InInstigator, const FName InAction ) override;

protected:
	UPROPERTY( EditAnywhere )
	EAnemoneScore ScoreType;

	UPROPERTY( EditAnywhere )
	int32 Value;
};

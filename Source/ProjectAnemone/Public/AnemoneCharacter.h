// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AnemoneEncounterEntity.h"

#include "AnemoneCharacter.generated.h"

enum class EAnemoneScore : uint8;
class UOutcomeTracker;

class UAnemoneEntityModule;
using Module = UAnemoneEntityModule;

class UAnemoneAction;

class UAnemoneGroup;

/**
 * 
 */
UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, HideDropdown )
class PROJECTANEMONE_API UAnemoneCharacter : public UObject, public IAnemoneEncounterEntity
{
	GENERATED_BODY()
public:
	UAnemoneCharacter();

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	UTexture2D* Portrait;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TMap< EAnemoneScore, int32 > ScoreSheet;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Instanced )
	TArray< UAnemoneEntityModule* > ModuleList;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TArray< TSubclassOf< UAnemoneAction > > ActionList;

	UFUNCTION( BlueprintCallable )
	static FName CreateCharacter( const FName InIdentifier, TSubclassOf< UAnemoneCharacter > InArchetype );

	virtual bool IsPlayerEntity_Implementation() const override;

	virtual FString GetName_Implementation() const override;

	virtual UTexture2D* GetTexture_Implementation() const;

	virtual int32 GetScore_Implementation( const EAnemoneScore& InScore ) const override;

	virtual void HandleEvent_Implementation( UAnemoneEvent_ChangeEntity* InEvent ) override;

	virtual EOutcome RollOutcome_Implementation( UAnemoneEvent_Outcome* InEvent ) override;

	void AddModule_Implementation( UAnemoneEntityModule* InModule );

	UAnemoneGroup* SoloGroup;
	UAnemoneGroup* CurrentGroup;

protected:
	//	Does the Character belong to the Player.
	UPROPERTY( EditAnywhere )
	bool bIsPlayerEntity;

	//	Character Name
	UPROPERTY( EditAnywhere )
	FName Name;

	//	Decides the Order in which Entities will Act in a Round.  Lowest number is highest priority.
	UPROPERTY( BlueprintReadWrite )
	int32 EncounterActionPriority;

	// Keeps track of the Character's Action Outcome History.  The Outcome History determines the odds of future Outcomes.
	UPROPERTY()
	UOutcomeTracker* Tracker;
};

using Character = UAnemoneCharacter;

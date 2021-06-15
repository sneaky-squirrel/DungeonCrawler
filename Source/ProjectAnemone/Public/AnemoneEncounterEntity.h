// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "AnemoneEnum.h"

#include "AnemoneEncounterEntity.generated.h"

class UAnemoneEvent;
class UAnemoneEvent_Outcome;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnemoneEncounterEntity : public UInterface
{
	GENERATED_BODY()
};

/**	All classes that represent an Entity inside of a Combat Encounter must implement this interface.
 *	The Entity interface offers access to Score values, the Entity's name, and allows the class to handle Encounter Events.
 */
class PROJECTANEMONE_API IAnemoneEncounterEntity
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	FString GetName() const;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	int32 GetScore( const EAnemoneScore& InScore ) const;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void HandleEvent( UAnemoneEvent_ChangeEntity* InEvent );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	EOutcome RollOutcome( UAnemoneEvent_Outcome* InEvent );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void AddModule( UAnemoneEntityModule* InModule );
};

using UEntity = UAnemoneEncounterEntity;
using IEntity = IAnemoneEncounterEntity;

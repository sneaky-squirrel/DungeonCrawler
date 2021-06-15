// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnemoneFunctionLibrary.generated.h"

class UAnemoneEvent;
class UAnemoneEncounterEntity;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static bool SortBroadcast_Action( TArray< UObject* > EntityList, UObject* Instigator );
};

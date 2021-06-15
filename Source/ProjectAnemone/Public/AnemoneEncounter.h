// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AnemoneEnum.h"
#include "AnemoneGameData.h"

#include "AnemoneEncounter.generated.h"

class UAnemoneAction;
class UAnemoneEvent_Outcome;

/**
 * 
 */
UCLASS( Blueprintable )
class PROJECTANEMONE_API UAnemoneEncounter : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray< FName > EntityList;

	UPROPERTY( BlueprintReadWrite )
	UAnemoneActionInstanceQueue* PendingActions;

	UPROPERTY( BlueprintReadWrite )
	UAnemoneLogQueue* PendingLogText;

	UPROPERTY( BlueprintReadWrite )
	TArray< FText > EncounterLog;

	UFUNCTION( BlueprintCallable )
	static UAnemoneEncounter* CreateEncounter( TSubclassOf< UAnemoneEncounter > InEncounterType );

/*
	UFUNCTION( BlueprintCallable )
	void ResolveAction( const TArray< UObject* >& InInstigator, const TArray< UObject* >& InDefender,
						const TArray< UObject* >& InTarget, UAnemoneLogQueue* OutTextLog );
*/

	UAnemoneEncounter();
};
using Encounter = UAnemoneEncounter;

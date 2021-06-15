// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnemoneGroup.generated.h"

//class UAnemoneEvent;

/**
 * 
 */
UCLASS( Blueprintable, DefaultToInstanced )
class PROJECTANEMONE_API UAnemoneGroup : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite )
	TArray< UObject* > List;
};
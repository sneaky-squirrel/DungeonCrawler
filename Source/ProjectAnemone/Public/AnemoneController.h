// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AnemoneController.generated.h"

class UAnemoneWidgetMenu;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API AAnemoneController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable )
	UAnemoneWidgetMenu* OpenMenu( TSubclassOf< UAnemoneWidgetMenu > MenuType );

	UFUNCTION( BlueprintCallable )
	float GetTieBreakerHorizontal();

	UFUNCTION( BlueprintCallable )
	float GetTieBreakerVertical();

	// Necessary override in order to be able to read Stick Axis values from the Player Controller.
	virtual void SetupInputComponent() override;
protected:
	UPROPERTY()
	UAnemoneWidgetMenu* CurrentMenu;
};

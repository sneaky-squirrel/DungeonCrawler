// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetNavigation.h"

#include "AnemoneGameModeDungeon.h"
#include "AnemoneController.h"

#include "AnemoneWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogUserInterface, Log, All );

enum class EAnemoneWidgetItemState : uint8;

//class AAnemoneController;

struct FAnemoneSelectionEntryEntity
{
	FCubeCoordinates Coordinates;
	bool bIsPlayerEntity;
	FName Name;
	FAnemoneSelectionEntryEntity();
	FAnemoneSelectionEntryEntity( const FCubeCoordinates& InCoordinates, bool InIsPlayerEntity, const FName InName );
};

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneWidgetItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
	void UpdatePortrait( const UTexture2D* const Texture );

	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
	void UpdateState( const EAnemoneWidgetItemState InState );

	UFUNCTION( BlueprintImplementableEvent )
	void GetFocusableWidget( UWidget*& OutWidget );

	UFUNCTION( BlueprintCallable )
	UAnemoneWidgetMenu* GetMenu();

	UFUNCTION( BlueprintCallable )
	AAnemoneController* GetController();

	void Init( AAnemoneController* const InController, UAnemoneWidgetMenu* const InMenu );

protected:
	FName ID;

	TWeakObjectPtr< UAnemoneWidgetMenu > OwningMenu;
	TWeakObjectPtr< AAnemoneController > OwningController;
};
using ItemWidget = UAnemoneWidgetItem;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneWidgetMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere )
	TSubclassOf< UAnemoneWidgetItem > ItemWidgetType;

	UPROPERTY( EditAnywhere )
	int32 EntityPageSize = 14;

	UFUNCTION( BlueprintCallable )
	void OpenEntitySelectionMenu( const TArray< FName >& InEntityList );

	UFUNCTION( BlueprintCallable )
	UWidget* EntitySelectionMove( const EAnemoneDirection InDirection );

	void InitEntitySelectionPage( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex );

	//UFUNCTION( BlueprintCallable )
	UWidget* Update( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex );

protected:
	// List of Widgets of the Current Page.
	UPROPERTY()
	TArray< UAnemoneWidgetItem* > WidgetList;

	// Selection Indices
	int32 SelectionIndexEntity;
	int32 SelectionIndexAction;
	int32 SelectionIndexTarget;

	// Selection Lists
	TArray< FAnemoneSelectionEntryEntity > SelectionListEntity;
	NameList SelectionListAction;
	NameList SelectionListTarget;

	// Widget Page Indices
	int32 PageIndex = -1;
	int32 ItemIndex = -1;
	int32 NumberOfItems = 0;

	// Initializes List of Acting Entities.
	void InitEntitySelectionList( const TArray< FName >& InEntityList );

	// Advances Hexagonal Coordinate to the next Valid position in an Outward Spiral.
	static void NextCoordinateInOutwardSpiral( FCubeCoordinates& OutCoordinate, int32& OutDirection, int32& OutIterationCount, int32& OutVectorLength, int32& OutRotationCount, int32& OutCount, const int32 InPageLength );

	// Updates Widget Page Indices.
	void UpdateIndex( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex );

	// Creates a new Widget Page.
	void CreateItemPage( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex );

	// Changes the Focus State of the Widget.
	void UpdateCurrentWidget( const EAnemoneWidgetItemState InState );
};
using MenuWidget = UAnemoneWidgetMenu;

// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneWidget.h"
#include "AnemoneEnum.h"
#include "AnemoneFunctionLibraryHexagon.h"
#include "AnemoneGameStateSubsystem.h"
#include "AnemoneEncounterEntity.h"

#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"

DEFINE_LOG_CATEGORY( LogUserInterface );

using EntityList = TArray< FAnemoneSelectionEntryEntity >;
using UMenu = UAnemoneWidgetMenu;

using EntityEntry = FAnemoneSelectionEntryEntity;
using EntityList = TArray< FAnemoneSelectionEntryEntity >;

FAnemoneSelectionEntryEntity::FAnemoneSelectionEntryEntity()
	: Coordinates( FCubeCoordinates() ), bIsPlayerEntity( false ), Name( NAME_None )
{
}

FAnemoneSelectionEntryEntity::FAnemoneSelectionEntryEntity( const FCubeCoordinates& InCoordinates, bool InIsPlayerEntity, const FName InName )
	: Coordinates( InCoordinates ), bIsPlayerEntity( InIsPlayerEntity ), Name( InName )
{
}

void ItemWidget::Init( AAnemoneController* const InController, UAnemoneWidgetMenu* const InMenu )
{
	OwningController = InController;
	OwningMenu = InMenu;
}

UAnemoneWidgetMenu* ItemWidget::GetMenu()
{
	return OwningMenu.IsValid() ? OwningMenu.Get() : nullptr;
}

AAnemoneController* ItemWidget::GetController()
{
	return OwningController.IsValid() ? OwningController.Get() : nullptr;
}

void UMenu::OpenEntitySelectionMenu( const NameList& InEntityList )
{
	InitEntitySelectionList( InEntityList );
	InitEntitySelectionPage( SelectionListEntity, 0 );
	if( SelectionListEntity.Num() > 0 )
	{
		Update( SelectionListEntity, 0 );	
	}
}

UWidget* UMenu::EntitySelectionMove( const EAnemoneDirection InDirection )
{
	int32& Index = SelectionIndexEntity;
	const int32 Length = SelectionListEntity.Num();
	const FCubeCoordinates CurrentCoordinates = SelectionListEntity[ Index ].Coordinates;
	int32 NewIndex = INDEX_NONE;
	switch( InDirection )
	{
		case EAnemoneDirection::Previous:
			if( Index - 1 >= 0 )
			{
				--Index;
			}
			break;
		case EAnemoneDirection::Next:
			if( Index + 1 < Length )
			{
				++Index;
			}
			break;
		case EAnemoneDirection::Southeast:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.X == CurrentCoordinates.X && Item.Coordinates.Z > CurrentCoordinates.Z;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		case EAnemoneDirection::Southwest:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.Y == CurrentCoordinates.Y && Item.Coordinates.Z > CurrentCoordinates.Z;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		case EAnemoneDirection::West:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.Z == CurrentCoordinates.Z && Item.Coordinates.X < CurrentCoordinates.X;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		case EAnemoneDirection::Northwest:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.X == CurrentCoordinates.X && Item.Coordinates.Z < CurrentCoordinates.Z;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		case EAnemoneDirection::Northeast:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.Y == CurrentCoordinates.Y && Item.Coordinates.Z < CurrentCoordinates.Z;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		case EAnemoneDirection::East:
			NewIndex = SelectionListEntity.IndexOfByPredicate( [ &CurrentCoordinates ] ( const EntityEntry& Item )
				{
					return Item.Coordinates.Z == CurrentCoordinates.Z && Item.Coordinates.X > CurrentCoordinates.X;
				} );
			Index = NewIndex == INDEX_NONE ? Index : NewIndex;
			break;
		default:
			break;
	}
	return Update( SelectionListEntity, Index );
}

void UMenu::InitEntitySelectionList( const NameList& InEntityList )
{
	// Used to determine the next Hexagonal Coordinate to assign to an Item.
	FCubeCoordinates CurrentCoordinate;
	int32 CurrentHexagonalDirection = 5;
	int32 IterationCount = 0;
	int32 VectorLength = 1;
	int32 RotationCount = 0;
	int32 Count = 0;
	for( const FName EntityName : InEntityList )
	{
		UObject* Entity = GlobalState->GetEntity( EntityName );
		if( !Entity )
		{
			UE_LOG( LogDungeonMode, Error, TEXT( "DungeonMode::OpenEntitySelectionMenu()	:	!Entity" ) );
			continue;
		}
		bool IsPlayer = IEntity::Execute_IsPlayerEntity( Entity );
		SelectionListEntity.Emplace( FAnemoneSelectionEntryEntity( CurrentCoordinate, IsPlayer, EntityName ) );
		NextCoordinateInOutwardSpiral( CurrentCoordinate, CurrentHexagonalDirection, IterationCount, VectorLength, RotationCount, Count, EntityPageSize );
	}
}

void UMenu::NextCoordinateInOutwardSpiral( FCubeCoordinates& OutCoordinate, int32& OutDirection, int32& OutIterationCount, int32& OutVectorLength, int32& OutRotationCount, int32& OutCount, const int32 InPageLength )
{
	OutCoordinate = Cube_Neighbor( OutCoordinate, OutDirection );
	if( ++OutIterationCount >= OutVectorLength )
	{
		OutIterationCount = 0;
		// Rotate Clockwise
		--OutDirection;
		++OutRotationCount;
	}
	if( OutDirection < 0 )
	{
		// Reset Index when outside Bounds of the array.
		OutDirection = 5;
	}
	if( OutRotationCount >= 3 )
	{
		OutRotationCount = 0;
		++OutVectorLength;
	}
	if( ++OutCount >= InPageLength )
	{
		OutCoordinate = FCubeCoordinates();
		OutCount = 0;
		OutDirection = 5;
		OutIterationCount = 0;
		OutVectorLength = 1;
		OutRotationCount = 0;
	}
}

void UMenu::InitEntitySelectionPage( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex )
{
	WidgetList.Init( nullptr, EntityPageSize );
	CreateItemPage( InEntityList, InCurrentIndex );
}

UWidget* UMenu::Update( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex )
{
	UWidget* FocusableItem = nullptr;
	UWidget* Item = nullptr;
	UpdateCurrentWidget( EAnemoneWidgetItemState::NotFocused );
	UpdateIndex( InEntityList, InCurrentIndex );
	const int32 MinimumIndex = PageIndex * InEntityList.Num();
	const int32 MaximumIndex = MinimumIndex + NumberOfItems - 1;
	if( InCurrentIndex < MinimumIndex || InCurrentIndex > MaximumIndex )
	{
		CreateItemPage( InEntityList, InCurrentIndex );
	}
	UpdateCurrentWidget( EAnemoneWidgetItemState::Focused );
	Item = WidgetList[ ItemIndex ];
	if( !Item )
	{
		UE_LOG( LogUserInterface, Error, TEXT( "UWidget* UMenu::Update()	:	!Item" ) );
		return nullptr;
	}
	WidgetList[ ItemIndex ]->GetFocusableWidget( FocusableItem );
	return FocusableItem;
}

void UMenu::UpdateIndex( const TArray< struct FAnemoneSelectionEntryEntity >& InEntityList, const int32 InCurrentIndex )
{
	const int32 NumberOfEntities = InEntityList.Num();
	const int32 WidgetListLength = WidgetList.Num();
	if( WidgetListLength <= 0 || NumberOfEntities <= 0 )
	{
		UE_LOG( LogUserInterface, Error, TEXT( "void MenuWidget::UpdateIndex()	:	Empty Lists" ) );
		return;
	}
	ItemIndex = InCurrentIndex % WidgetListLength;
	PageIndex = InCurrentIndex / WidgetListLength;
	NumberOfItems = ( NumberOfEntities < ( PageIndex + 1 ) * WidgetListLength ) ? ( NumberOfEntities % WidgetListLength ) : WidgetListLength;
	UE_LOG( LogUserInterface, Warning, TEXT( "ItemIndex = %d, PageIndex =  %d, PageIndex = %d, InEntityList.Num() = %d" ), ItemIndex, PageIndex, NumberOfItems, InEntityList.Num() );
}

void UMenu::CreateItemPage( const EntityList& InEntityList, const int32 InCurrentIndex )
{
	UClass* const ItemType = ItemWidgetType.Get();
	UCanvasPanel* const Canvas = Cast< UCanvasPanel >( GetRootWidget() );
	const int32 NumOfWidgets = WidgetList.Num();
	const int32 NumberOfEntities = InEntityList.Num();
	APlayerController* Controller = UGameplayStatics::GetPlayerController( this, 0 );
	if( !ItemType || !Canvas || !Controller )
	{
		UE_LOG( LogUserInterface, Error, TEXT( "MenuWidget::ShowActingEntities()	:	!ItemWidgetType || !Canvas || !Controller" ) );
		return;
	}
	for( int32 Index = InCurrentIndex, WidgetIndex = 0; WidgetIndex < NumOfWidgets; ++Index, ++WidgetIndex )
	{
		ItemWidget*& Item = WidgetList[ WidgetIndex ];
		if( Index >= NumberOfEntities )
		{
			if( Item )
			{
				Item->RemoveFromParent();
				Item = nullptr;
			}
			continue;
		}
		const FAnemoneSelectionEntryEntity& Entry = InEntityList[ Index ];
		if( !Item )
		{
			FString BaseName( FString( TEXT( "SomeItemWidget_" ) ) );
			BaseName.AppendInt( WidgetIndex );
			Item = CreateWidget< ItemWidget, UMenu >( this, ItemType, FName( BaseName ) );
			Item->Init( Cast< AAnemoneController >( Controller ), this );
		}
		UAnemoneFunctionLibraryHexagon::AnchorHexagonWidget( Item, Canvas, 0, Entry.Coordinates, 72.0 );
		if( WidgetIndex == 0 )
		{
			UpdateCurrentWidget( EAnemoneWidgetItemState::Focused );
		}
		UObject* Entity = GlobalState->GetEntity( Entry.Name );
		if( Entity )
		{
			Item->UpdatePortrait( IEntity::Execute_GetTexture( Entity ) );
		}
	}
}

void UMenu::UpdateCurrentWidget( const EAnemoneWidgetItemState InState )
{
	if( ItemIndex < 0 )
	{
		UE_LOG( LogUserInterface, Error, TEXT( "void MenuWidget::UpdateCurrentWidget()	:	ItemIndex < 0 = %d" ), ItemIndex );
		return;
	}
	ItemWidget* Item = WidgetList[ ItemIndex ];
	if( !Item )
	{
		UE_LOG( LogUserInterface, Error, TEXT( "void MenuWidget::UpdateCurrentWidget()	:	!Item" ) );
		UE_LOG( LogUserInterface, Warning, TEXT( "WidgetList.Num() = %d, ItemIndex = %d" ), WidgetList.Num(), ItemIndex );
		return;
	}
	Item->UpdateState( InState );
}

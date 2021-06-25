// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneFunctionLibraryHexagon.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

DEFINE_LOG_CATEGORY( LogHexagonLibrary );

using Library = UAnemoneFunctionLibraryHexagon;

UCanvasPanelSlot* Library::AnchorHexagonWidget( UWidget* const InItem, UCanvasPanel* const InCanvas, const int32 InZOrder, const FCubeCoordinates& InCoordinates, const float InGridSize )
{
	if( !InCanvas )
	{
		UE_LOG( LogHexagonLibrary, Error, TEXT( "UAnemoneFunctionLibraryHexagon::AnchorHexagonWidget()	:	!InCanvas" ) );
		return nullptr;
	}
	UCanvasPanelSlot* Slot = InCanvas->AddChildToCanvas( InItem );
	Slot->SetAnchors( FAnchors( 0.5, 0.5 ) );
	Slot->SetSize( FVector2D( InGridSize * 2 ) );
	Slot->SetAlignment( FVector2D( 0.5 ) );
	Slot->SetPosition( ToVector( InCoordinates, InGridSize ) );
	Slot->SetZOrder( InZOrder );
	return Slot;
}

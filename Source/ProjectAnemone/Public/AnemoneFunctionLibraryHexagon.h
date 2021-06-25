// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Layout/Anchors.h"

#include "HexagonMath.h"

#include "AnemoneFunctionLibraryHexagon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogHexagonLibrary, Log, All );

class UWidget;
class UCanvasPanel;
class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneFunctionLibraryHexagon : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UCanvasPanelSlot* AnchorHexagonWidget( UWidget* const InItem, UCanvasPanel* const InCanvas, const int32 InZOrder, const FCubeCoordinates& InCoordinates, const float InGridSize );
};

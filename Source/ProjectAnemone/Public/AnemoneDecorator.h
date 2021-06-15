// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockImageDecorator.h"

#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Components/RichTextBlock.h"

#include "Framework/Text/TextLayout.h"

#include "AnemoneDecorator.generated.h"

/* 
*	TODO: After implementing an image tooltip, try somehow:
*			Referencing a Table with a custom UserWidgetSubclass
*			Use the referenced Widget Subclass to create the custom Widget as a new Widget Menu (Hyperlink)
*	TODO: Implement a menu GameState demo (Inside of a Demo UserWidget), and utilize the TableStruct to link
*			embedded subwidgets (Inside of your URichTextBlock) to your GameState / Indices / Arrays so that you can navigate
*			between embedded widgets and open their corresponding assigned Tooltip UserWidgets.
* 
*			Series of GameState variables.
*			Arrays containing Structures
*				Structures indicate:
					Arrays of SlateWidgets.
					Corresponding URichTextBlock.
*/

/** Simple struct for Images Embedded in RichTextBlocks with Tooltips or Hyperlinks.*/
USTRUCT(Blueprintable, BlueprintType)
struct PROJECTANEMONE_API FAnemoneRichImageRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, Category = Appearance )
	FSlateBrush BrushUnselected;

	UPROPERTY( EditAnywhere, Category = Appearance )
	FSlateBrush BrushSelected;

	UPROPERTY( EditAnywhere, Category = Appearance )
	FSlateBrush BrushClicked;

	UPROPERTY( EditAnywhere, Category = Appearance )
	TSubclassOf< UUserWidget > ToolTipBrush;
};


class SAnemoneTooltip : public SCompoundWidget, public IToolTip
{
public:
	SLATE_BEGIN_ARGS( SAnemoneTooltip )
	{
		
	}

	/** The text displayed in this tool tip */
	SLATE_ATTRIBUTE(FText, Text)

	/** Arbitrary content to be displayed in the tool tip; overrides any text that may be set. */
	SLATE_DEFAULT_SLOT(FArguments, Content)

	/** The font to use for this tool tip */
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)
		
	/** Font color and opacity */
	SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
		
	/** Margin between the tool tip border and the text content */
	SLATE_ATTRIBUTE(FMargin, TextMargin)

	/** The background/border image to display */
	SLATE_ATTRIBUTE(const FSlateBrush*, BorderImage)

	/** Whether the tooltip should be considered interactive */
	SLATE_ATTRIBUTE(bool, IsInteractive)

	SLATE_END_ARGS()

public:
	void Construct( const FArguments& InArgs );

	virtual TSharedRef< class SWidget > AsWidget( ) override;

	virtual TSharedRef<SWidget> GetContentWidget( ) override;

	virtual void SetContentWidget( const TSharedRef<SWidget>& InContentWidget ) override;

	virtual void ResetContentWidget();

	virtual bool IsEmpty( ) const override;

	virtual bool IsInteractive( ) const override;

	virtual void OnOpening( ) override;

	virtual void OnClosed( ) override;

private:

	// Text block widget.
	TAttribute<FText> TextContent;

	// Content widget.
	TWeakPtr<SWidget> WidgetContent;

	// Wrapped content within the widget;
	TSharedPtr<SWidget> ToolTipContent;

	// Font used for the text displayed (where applicable)
	TAttribute<FSlateFontInfo> Font;
	
	// Color and opacity used for the text displayed (where applicable)
	TAttribute<FSlateColor> ColorAndOpacity;

	// Margin between the tool tip border and the text content
	TAttribute<FMargin> TextMargin;

	// The background/border image to display
	TAttribute<const FSlateBrush*> BorderImage;
	
	// Whether the tooltip should be considered interactive.
	TAttribute<bool> bIsInteractive;
};

class SAnemoneRichInlineImage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SAnemoneRichInlineImage )
	{
	}

	SLATE_ARGUMENT( UAnemoneRichTextBlock*, TextBlock )

	//SLATE_EVENT( FSimpleDelegate, OnIconHovered )

	//SLATE_EVENT( FSimpleDelegate, OnIconUnhovered )

	SLATE_END_ARGS()

public:
	void Construct( const FArguments& InArgs, const FSlateBrush* Brush, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch);

	void OnHoverHandler();

private:
	//UAnemoneRichTextBlock* TextBlock;
};

/**
 * 
 */
UCLASS()
class PROJECTANEMONE_API UAnemoneDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
public:
	virtual TSharedPtr< ITextDecorator > CreateDecorator( URichTextBlock* InOwner ) override;

	const FSlateBrush* FindImageBrush( FName TagOrId, bool bWarnIfMissing );

private:
	FAnemoneRichImageRow* FindImageRow( FName TagOrId, bool bWarnIfMissing );

	//TWeakObjectPtr< UObject > MyOwner;

	UPROPERTY( EditAnywhere, Category=Appearance, meta = (RequiredAssetDataTags = "RowStructure=RichImageRow" ) )
	class UDataTable* IconTable;
};

/*	Rich Text Block with embedded icons for use inside Project Anemone.
*	Each icon is an image with three different brushes corresponding to
*	the states of unselected, selected, and clicked.
*	Each icon also has a tooltip and opens a new widget when clicked.
*/
UCLASS()
class UAnemoneRichTextBlock : public URichTextBlock
{
	GENERATED_BODY()
public:
	void AddIconWidget( const TSharedRef< SWidget >& InIcon );

protected:
	TArray< TSharedRef< SWidget > > EmbeddedIconList;
};

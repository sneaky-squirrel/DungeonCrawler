// Fill out your copyright notice in the Description page of Project Settings.

#include "AnemoneDecorator.h"

#include "Misc/DefaultValueHelper.h"
#include "Slate.h"

#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Framework/Text/RichTextMarkupProcessing.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"

void SAnemoneTooltip::Construct( const FArguments& InArgs )
{
	TextContent = InArgs._Text;
	bIsInteractive = InArgs._IsInteractive;
	Font = InArgs._Font;
	ColorAndOpacity = InArgs._ColorAndOpacity;
	TextMargin = InArgs._TextMargin;
	BorderImage = InArgs._BorderImage;
	
	SetContentWidget(InArgs._Content.Widget);
}

TSharedRef< class SWidget > SAnemoneTooltip::AsWidget()
{
	return AsShared();
}

TSharedRef<SWidget> SAnemoneTooltip::GetContentWidget( )
{
	return ToolTipContent.ToSharedRef();
}

void SAnemoneTooltip::SetContentWidget( const TSharedRef<SWidget>& InContentWidget )
{
	if (InContentWidget != SNullWidget::NullWidget)
	{
		// Widget content argument takes precedence over the text content.
		WidgetContent = InContentWidget;
	}

	TSharedPtr< SWidget > PinnedWidgetContent = WidgetContent.Pin();
	if( PinnedWidgetContent.IsValid() )
	{
		ToolTipContent = PinnedWidgetContent;

		// Tool-tip with entirely custom content.  We'll create a border with some padding (as customized by the user), then
		// embed their custom widget right inside the border.  This tool-tip currently has a different styling than tool-tips
		// that only contain text.
		ChildSlot
		[
			SNew( SBorder )
			.BorderImage( BorderImage )
			.Padding( TextMargin )
			[
				ToolTipContent.ToSharedRef()
			]
		];

	}
	else
	{
		ToolTipContent =
			SNew( STextBlock )
			.Text( TextContent )
			.Font( Font )
			.ColorAndOpacity( FLinearColor::Black )
			.WrapTextAt_Static( &SToolTip::GetToolTipWrapWidth );

		// Text-only tool-tip.  This tool-tip currently has a different styling than tool-tips with custom content.  We always want basic
		// text tool-tips to look consistent.
		ChildSlot
		[
			SNew( SBorder )
			.BorderImage( FCoreStyle::Get().GetBrush( "ToolTip.BrightBackground" ) )
			.Padding( FMargin( 11.0f ) )
			[
				ToolTipContent.ToSharedRef()
			]
		];
	}
}

void SAnemoneTooltip::ResetContentWidget()
{
	WidgetContent.Reset();
	SetContentWidget( SNullWidget::NullWidget );
}

bool SAnemoneTooltip::IsEmpty() const
{
	return !WidgetContent.IsValid() && TextContent.Get().IsEmpty();
}

bool SAnemoneTooltip::IsInteractive() const
{
	return bIsInteractive.Get();
}

void SAnemoneTooltip::OnOpening()
{
}

void SAnemoneTooltip::OnClosed()
{
}


void SAnemoneRichInlineImage::Construct( const FArguments& InArgs, const FSlateBrush* Brush, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch)
{
	UAnemoneRichTextBlock* TextBlock = InArgs._TextBlock;
	if( TextBlock )
	{
		TextBlock->AddIconWidget( SharedThis( this ) );
	}
	if ( ensure( Brush ) )
	{
		const TSharedRef< FSlateFontMeasure > FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float IconHeight = FMath::Min( ( float ) FontMeasure->GetMaxCharacterHeight( TextStyle.Font, 1.0f ), Brush->ImageSize.Y );
		float IconWidth = IconHeight;

		if ( Width.IsSet() )
		{
			IconWidth = Width.GetValue();
		}

		if ( Height.IsSet() )
		{
			IconHeight = Height.GetValue();
		}

		ChildSlot
		[
			SNew( SBox )
			.HeightOverride( IconHeight )
			.WidthOverride( IconWidth )
			[
				SNew( SScaleBox )
				.Stretch( Stretch )
				.StretchDirection( EStretchDirection::DownOnly )
				.VAlign( VAlign_Center )
				[
					SNew( SButton )
					.ToolTip( SNew( SAnemoneTooltip ) )
					.OnHovered( this, &SAnemoneRichInlineImage::OnHoverHandler )
					//.OnUnhovered( InArgs._OnIconUnhovered )
					[
						SNew( SOverlay )
						+SOverlay::Slot()
						[
							SNew( SImage )
							.Image( Brush )
						]
					]
				]
			]
		];
	}
}

void SAnemoneRichInlineImage::OnHoverHandler()
{
	UE_LOG( LogTemp, Warning, TEXT( "SlateTestSneakySquirrel" ) );
}

class FAnemoneRichInlineImage : public FRichTextDecorator
{
public:
	FAnemoneRichInlineImage( URichTextBlock* InOwner, UAnemoneDecorator* InDecorator)
		: FRichTextDecorator( InOwner )
		, TextBlock( InOwner )
		, Decorator( InDecorator )
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("icn") && RunParseResult.MetaData.Contains( TEXT( "id" ) ) )
		{
			const FTextRange& IdRange = RunParseResult.MetaData[ TEXT( "id" ) ];
			const FString TagId = Text.Mid( IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex );

			const bool bWarnIfMissing = false;
			return Decorator->FindImageBrush( *TagId, bWarnIfMissing ) != nullptr;
		}
		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		const bool bWarnIfMissing = true;
		const FSlateBrush* Brush = Decorator->FindImageBrush( *RunInfo.MetaData[ TEXT( "id" ) ], bWarnIfMissing );

		UE_LOG( LogTemp, Warning, TEXT( "CreateDecoratorWidget()" ) );

		TOptional<int32> Width;
		if ( const FString* WidthString = RunInfo.MetaData.Find(TEXT("width")))
		{
			int32 WidthTemp;
			UE_LOG( LogTemp, Warning, TEXT( "FDefaultValueHelper::ParseInt() : Width	:	%d" ), FDefaultValueHelper::ParseInt( *WidthString, WidthTemp ) );
			Width = FDefaultValueHelper::ParseInt( *WidthString, WidthTemp ) ? WidthTemp : TOptional<int32>();
		}

		TOptional<int32> Height;
		if ( const FString* HeightString = RunInfo.MetaData.Find( TEXT( "height" ) ) )
		{
			int32 HeightTemp;
			UE_LOG( LogTemp, Warning, TEXT( "FDefaultValueHelper::ParseInt() : Height	:	%d" ), FDefaultValueHelper::ParseInt(*HeightString, HeightTemp) );
			Height = FDefaultValueHelper::ParseInt( *HeightString, HeightTemp ) ? HeightTemp : TOptional<int32>();
		}

		EStretch::Type Stretch = EStretch::ScaleToFit;
		if (const FString* SstretchString = RunInfo.MetaData.Find( TEXT( "stretch" ) ) )
		{
			static const UEnum* StretchEnum = StaticEnum<EStretch::Type>();
			int64 StretchValue = StretchEnum->GetValueByNameString( *SstretchString );
			if (StretchValue != INDEX_NONE)
			{
				Stretch = static_cast<EStretch::Type>( StretchValue );
			}
		}
		return SNew( SAnemoneRichInlineImage, Brush, TextStyle, Width, Height, Stretch )
					.TextBlock( Cast< UAnemoneRichTextBlock >( TextBlock ) );
	}

private:
	URichTextBlock* TextBlock;
	UAnemoneDecorator* Decorator;
};

TSharedPtr< ITextDecorator > UAnemoneDecorator::CreateDecorator( URichTextBlock* InOwner )
{
	return MakeShareable( new FAnemoneRichInlineImage( InOwner, this ) );
}

const FSlateBrush* UAnemoneDecorator::FindImageBrush( FName TagOrId, bool bWarnIfMissing )
{
	const FAnemoneRichImageRow* ImageRow = FindImageRow( TagOrId, bWarnIfMissing );
	if ( ImageRow )
	{
		return &ImageRow->BrushUnselected;
	}
	return nullptr;
}

FAnemoneRichImageRow* UAnemoneDecorator::FindImageRow( FName TagOrId, bool bWarnIfMissing )
{
	if( IconTable )
	{
		FString ContextString;
		return IconTable->FindRow< FAnemoneRichImageRow >( TagOrId, ContextString, bWarnIfMissing );
	}
	return nullptr;
}

void UAnemoneRichTextBlock::AddIconWidget( const TSharedRef< SWidget >& InIcon )
{
	EmbeddedIconList.Emplace( InIcon );
}


/*
TSharedRef<SWidget> UAnemoneRichTextBlock::RebuildWidget()
{
	UpdateStyleData();

	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;
	CreateDecorators( CreatedDecorators );

	TSharedRef< FRichTextLayoutMarshaller > Marshaller = FRichTextLayoutMarshaller::Create(CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators, StyleInstance.Get());

	MyRichTextBlock =
		SNew( SRichTextBlock )
		.TextStyle(bOverrideDefaultStyle ? &DefaultTextStyleOverride : &DefaultTextStyle)
		.Marshaller(Marshaller);
	
	return MyRichTextBlock.ToSharedRef();
}
*/

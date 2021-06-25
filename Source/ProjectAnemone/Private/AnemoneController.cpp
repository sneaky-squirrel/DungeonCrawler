// Fill out your copyright notice in the Description page of Project Settings.

//#include "Blueprints/Widget.h"

#include "AnemoneController.h"
#include "AnemoneWidget.h"

using UMenu = UAnemoneWidgetMenu;

UMenu* AAnemoneController::OpenMenu( TSubclassOf< UMenu > MenuType )
{
	if( CurrentMenu )
	{
		CurrentMenu->RemoveFromParent();
	}
	UMenu* Menu = CreateWidget< UMenu, AAnemoneController >( this, MenuType.Get() );
	Menu->AddToViewport();
	return Menu;
}

float AAnemoneController::GetTieBreakerHorizontal()
{
	return GetInputAxisValue( FName( TEXT( "TieBreakerHorizontal" ) ) );
}

float AAnemoneController::GetTieBreakerVertical()
{
	return GetInputAxisValue( FName( TEXT( "TieBreakerVertical" ) ) );
}

void AAnemoneController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis( "TieBreakerHorizontal" );
	InputComponent->BindAxis( "TieBreakerVertical" );
}

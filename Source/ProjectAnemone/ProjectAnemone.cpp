// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAnemone.h"
#include "Modules/ModuleManager.h"

//	Localization Indludes
#include "Internationalization/StringTableRegistry.h"
//#include "Paths.h"

void FProjectAnemone::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();

	//UE_LOG( LogTemp, Warning, TEXT("FProjectAnemone::StartupModule() Sneaky- UIStrings") )
	LOCTABLE_FROMFILE_GAME( "UIStrings", "UIStrings", "ProjectAnemone/Localization/UIStringTable.csv" );
}


void FProjectAnemone::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FProjectAnemone, ProjectAnemone, "ProjectAnemone" );

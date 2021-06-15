#include "AnemoneGameStateSubsystem.h"

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "PlatformFeatures.h"

#include "Kismet/GameplayStatics.h"

using GameStateSubsystem = UAnemoneGameStateSubsystem;

TMap< FName, UObject* > ObjectMap;

//GameStateSubsystem* GameStateSubsystem::GameState = nullptr;
GameStateSubsystem* GameState = nullptr;

void GameStateSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	GameState = this;
}

void GameStateSubsystem::Deinitialize()
{
}

bool GameStateSubsystem::ContainsEntity( const FName InIdentifier )
{
	return EntityMap.Contains( InIdentifier );
}

FName GameStateSubsystem::AddEntity( const FName InIdentifier, UObject* InEntity )
{
	FName CurrentName = NAME_None;
	if( !EntityMap.Contains( InIdentifier ) )
	{
		EntityMap.Emplace( InIdentifier, InEntity );
		return InIdentifier;
	}
	FString BaseName = InIdentifier.ToString();
	for( int i = 0; i < 26; ++i )
	{
		CurrentName = FName( FString::Printf( TEXT( "%s_%c" ), *BaseName, 'A' + i ) );
		if( !EntityMap.Contains( CurrentName ) )
		{
			EntityMap.Emplace( CurrentName, InEntity );
			return CurrentName;
		}
	}
	UE_LOG( LogTemp, Error, TEXT( "GameStateSubsystem::AddEntity()" ) );
	return NAME_None;
}

void GameStateSubsystem::RemoveEntity( const FName InIdentifier )
{
	EntityMap.Remove( InIdentifier );
}

UObject* GameStateSubsystem::GetEntity( const FName InIdentifier )
{
	check( this );
	UObject* Entity = EntityMap.FindRef( InIdentifier );
	return ( Entity ) ? Entity : nullptr;
}

bool GameStateSubsystem::SaveGameStateToSlot( const FString& SlotName, const int32 UserIndex )
{
	TArray<uint8> StoredData;
	FMemoryWriter Writer( StoredData );
	FObjectAndNameAsStringProxyArchive Ar( Writer, true );
	Ar.ArIsSaveGame = true;
	ObjectMap.Reset();

/*
	for( int32 i = 0; i < 3; ++i )
	{
		UAnemoneCharacter* CurrentCharacter = NewObject<UAnemoneCharacter>( this );
		CharacterList.Emplace( CurrentCharacter );
	}
	CharacterList[ 0 ]->ScoreSheet[ EAnemoneScore::Constitution ].Base = 300;

	for( int32 i = 0; i < CharacterList.Num(); ++i )
	{
		UE_LOG( LogTemp, Display, TEXT("Base Constitution: %d"),
		CharacterList[ i ]->ScoreSheet[ EAnemoneScore::Constitution ].Base );
	}
	SerializeArray( Ar, CharacterList );
*/

	SaveDataToDisk( StoredData, FString("TheSlot"), 0 );

	Writer.FlushCache();
	StoredData.Empty();
	return true;
}

bool GameStateSubsystem::LoadGameStateFromSlot( const FString& SlotName, const int32 UserIndex )
{
	TArray<uint8> LoadedData;
	FMemoryReader Reader( LoadedData );
	FObjectAndNameAsStringProxyArchive Ar( Reader, true );
	Ar.ArIsSaveGame = true;
	ObjectMap.Reset();
/*
	LoadDataFromDisk( LoadedData, FString("TheSlot"), 0 );

	SerializeArray( Ar, CharacterList );

	for( int32 i = 0; i < CharacterList.Num(); ++i )
	{
		UE_LOG( LogTemp, Display, TEXT("Base Constitution: %d"),
		CharacterList[ i ]->ScoreSheet[ EAnemoneScore::Constitution ].Base );
	}
*/

	Reader.FlushCache();
	LoadedData.Empty();
	return true;
}

UObject* SerializeReference( FArchive& Ar, TWeakObjectPtr<UObject> ObjectReference )
{
	FName Name( NAME_None );
	UObject* Object = ObjectReference.Get();
	if( Ar.IsSaving() )
	{
		if( !ObjectReference.IsValid() )
		{
			Ar << Name;
			//UE_LOG( LogTemp, Warning, TEXT( "ERROR: Attempted to Serialize [nullptr] Object Reference." ) );
			return nullptr;
		}
		Name = FName( ObjectReference->GetName() );
		Ar << Name;
		if( !ObjectMap.Contains( Name ) )
		{
			Object = ObjectReference->GetClass();
			ObjectMap.Emplace( Name, Object );
			Ar << Object;
			ObjectReference->Serialize( Ar );
		}
		return ObjectReference.Get();
	}
	else
	{
		Ar << Name;
		if( Name.IsNone() )
		{
			//UE_LOG( LogTemp, Warning, TEXT( "Attempted to Deserialize [nullptr] Object Reference." ) );
			return nullptr;
		}
		if( !ObjectMap.Contains( Name ) )
		{
			Ar << Object;
			if( !Object )
			{
				UE_LOG( LogTemp, Warning, TEXT( "Failed to Deserialize UClass of Object Reference." ) );
				return false;
			}
			Object = NewObject<UObject>( GetTransientPackage(), Cast<UClass>( Object ), Name );
			ObjectMap.Emplace( Name, Object );
			Object->Serialize( Ar );
		}
		return ObjectMap[ Name ];
	}
	return nullptr;
}

void SerializeArray( FArchive& Ar, TArray<UObject*>& ObjectList )
{
	int32 Length = ObjectList.Num();
	Ar << Length;
	for( int32 i = 0; i < Length; ++i )
	{
		if( Ar.IsLoading() )
		{
			ObjectList.Emplace( SerializeReference( Ar, nullptr ) );
		}
		else
		{
			SerializeReference( Ar, ObjectList[ i ] );
		}
	}
}

bool GameStateSubsystem::SaveDataToDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex )
{
	//ISaveGameSystem* SaveSystem = GetSaveGameSystem();
	FString SaveFileName = FString( SlotName );
	SaveFileName.AppendInt( UserIndex );
	if (FFileHelper::SaveArrayToFile( StoredData, *SaveFileName ) )
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Success! %s"), FPlatformProcess::BaseDir());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Failed!"));
		return false;
	} 
	StoredData.Empty();
	return true;
}

bool GameStateSubsystem::LoadDataFromDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex )
{
	FString SaveFileName = FString( SlotName );
	SaveFileName.AppendInt( UserIndex );
	if ( !FFileHelper::LoadFileToArray( StoredData, *SaveFileName ) )
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Failed!"));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Succeeded!"));
	}
	return true;
}

void GameStateSubsystem::RemoteLog( FString String )
{
	UE_LOG( LogTemp, Warning, TEXT("%s"), *String );
}
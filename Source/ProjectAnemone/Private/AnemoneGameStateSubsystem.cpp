#include "AnemoneGameStateSubsystem.h"
#include "AnemoneGameModeDungeon.h"
#include "AnemoneEncounterEntity.h"

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "PlatformFeatures.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY( LogGameStateSubsystem );

TMap< FName, UObject* > ObjectMap;

GameStateSubsystem* GlobalState = nullptr;

void GameStateSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	GlobalState = this;
	//LoadGameStateFromSlot( TEXT( "StoreStringDemo" ), 0 );
}

void GameStateSubsystem::Deinitialize()
{
	//SaveGameStateToSlot( TEXT( "StoreStringDemo" ), 0 );
}

bool GameStateSubsystem::ContainsEntity( const FName InIdentifier )
{
	return EntityMap.Contains( InIdentifier );
}

FName GameStateSubsystem::AddEntity( const FName InIdentifier, UObject* InEntity )
{
	FName CurrentName = NAME_None;
	if( !InEntity->Implements< UEntity >() )
	{
		return NAME_None;
	}
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
	UE_LOG( LogGameStateSubsystem, Error, TEXT( "GameStateSubsystem::AddEntity()" ) );
	return NAME_None;
}

void GameStateSubsystem::RemoveEntity( const FName InIdentifier )
{
	EntityMap.Remove( InIdentifier );
}

UObject* GameStateSubsystem::GetEntity( const FName InIdentifier )
{
	UObject* Entity = EntityMap.FindRef( InIdentifier );
	return Entity;
}

bool GameStateSubsystem::SaveGameStateToSlot( const FString& InSlotName, const int32 UserIndex )
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

	//FString LevelName = GetWorld()->GetMapName();
	//LevelName.RemoveFromStart(World->StreamingLevelsPrefix);
	FString LevelName = UGameplayStatics::GetCurrentLevelName( this, true );
	UE_LOG( LogGameStateSubsystem, Warning, TEXT( "Stored String: %s." ), *LevelName );
	Ar << LevelName;

	SaveDataToDisk( StoredData, InSlotName, 0 );

	Writer.FlushCache();
	StoredData.Empty();
	return true;
}

bool GameStateSubsystem::LoadGameStateFromSlot( const FString& InSlotName, const int32 UserIndex )
{
	TArray<uint8> LoadedData;
	FMemoryReader Reader( LoadedData );
	FObjectAndNameAsStringProxyArchive Ar( Reader, true );
	Ar.ArIsSaveGame = true;
	ObjectMap.Reset();

	LoadDataFromDisk( LoadedData, InSlotName, 0 );
/*

	SerializeArray( Ar, CharacterList );

	for( int32 i = 0; i < CharacterList.Num(); ++i )
	{
		UE_LOG( LogTemp, Display, TEXT("Base Constitution: %d"),
		CharacterList[ i ]->ScoreSheet[ EAnemoneScore::Constitution ].Base );
	}
*/

	FString LevelName;
	Ar << LevelName;
	UE_LOG( LogGameStateSubsystem, Warning, TEXT( "Loaded String: %s." ), *LevelName );
	UGameplayStatics::OpenLevel( this, FName( LevelName ) );

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
				UE_LOG( LogGameStateSubsystem, Warning, TEXT( "Failed to Deserialize UClass of Object Reference." ) );
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

void GameStateSubsystem::foo()
{
	DungeonMode* GameMode = GetGameMode< DungeonMode >();
}

bool GameStateSubsystem::SaveDataToDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex )
{
	//ISaveGameSystem* SaveSystem = GetSaveGameSystem();
	FString SaveFileName = FString( SlotName );
	SaveFileName.AppendInt( UserIndex );
	if (FFileHelper::SaveArrayToFile( StoredData, *SaveFileName ) )
	{
		UE_LOG( LogGameStateSubsystem, Warning, TEXT("Save Success! %s"), FPlatformProcess::BaseDir());
	}
	else
	{
		UE_LOG( LogGameStateSubsystem, Warning, TEXT("Save Failed!"));
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
		UE_LOG( LogGameStateSubsystem, Warning, TEXT("Load Failed!"));
		return false;
	}
	else
	{
		UE_LOG( LogGameStateSubsystem, Warning, TEXT("Load Succeeded!"));
	}
	return true;
}

template< class T >
T* GameStateSubsystem::GetGameMode() const
{
	UGameInstance* Instance = GetGameInstance();
	UWorld* World;
	if( !Instance )
	{
		return nullptr;
	}
	World = Instance->GetWorld();
	if( !World )
	{
		return nullptr;
	}
	return World->GetAuthGameMode< T >();
}

UWorld* GameStateSubsystem::GetWorld() const
{
	UGameInstance* Instance = GetGameInstance();
	if( !Instance )
	{
		return nullptr;
	}
	return Instance->GetWorld();
}

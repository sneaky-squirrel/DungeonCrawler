#pragma once
#include "Subsystems/GameInstanceSubsystem.h"

#include "AnemoneGameStateSubsystem.generated.h"

extern TMap< FName, UObject* > ObjectMap;
extern UAnemoneGameStateSubsystem* GameState;

UObject* SerializeReference( FArchive& Ar, TWeakObjectPtr<UObject> ObjectReference );
void SerializeArray( FArchive& Ar, TArray<UObject*>& ObjectList );

UCLASS()
class UAnemoneGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	virtual void Deinitialize() override;

	/*	Interface with the global Entity Collection.
	*/
	UFUNCTION( BlueprintCallable, Category="Entity")
	bool ContainsEntity( const FName InIdentifier );

	UFUNCTION( BlueprintCallable, Category="Entity")
	FName AddEntity( const FName InIdentifier, UObject* InEntity );

	UFUNCTION( BlueprintCallable, Category="Entity")
	void RemoveEntity( const FName InIdentifier );

	UFUNCTION( BlueprintCallable, Category="Entity")
	UObject* GetEntity( const FName InIdentifier );

	/*	Interface with the Save File serialization.
	*/
	UFUNCTION( BlueprintCallable, Category="SaveGame")
	bool SaveGameStateToSlot( const FString& SlotName, const int32 UserIndex );

	UFUNCTION( BlueprintCallable, Category="SaveGame")
	bool LoadGameStateFromSlot( const FString& SlotName, const int32 UserIndex );

	//	Fetches the GameState subsystem.
	//static UAnemoneGameStateSubsystem* GetSystem();

	void RemoteLog( FString String );

private:
	//	Global Entity Collection.
	UPROPERTY()
	TMap< FName, UObject* > EntityMap;

	bool SaveDataToDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex );
	bool LoadDataFromDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex );

	//static UAnemoneGameStateSubsystem* GameState;
};


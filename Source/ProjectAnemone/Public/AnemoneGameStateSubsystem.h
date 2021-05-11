#pragma once
#include "Subsystems/GameInstanceSubsystem.h"

#include "AnemoneGameStateSubsystem.generated.h"

extern TMap< FName, UObject* > ObjectMap;
extern UGameInstance* CurrentGameInstance;

UObject* SerializeReference( FArchive& Ar, TWeakObjectPtr<UObject> ObjectReference );
void SerializeArray( FArchive& Ar, TArray<UObject*>& ObjectList );

UCLASS()
class UAnemoneGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	virtual void Deinitialize() override;

	UFUNCTION( BlueprintCallable, Category="SaveGame")
	bool SaveGameStateToSlot( const FString& SlotName, const int32 UserIndex );

	UFUNCTION( BlueprintCallable, Category="SaveGame")
	bool LoadGameStateFromSlot( const FString& SlotName, const int32 UserIndex );

//	UFUNCTION( BlueprintCallable, Category="Character")
//	UAnemoneCharacter* CreateNewCharacter();

	void RemoteLog( FString String );

//	UPROPERTY( BlueprintReadWrite )
//	TArray<UAnemoneCharacter*> CharacterList;

private:
	bool SaveDataToDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex );
	bool LoadDataFromDisk( TArray<uint8>& StoredData, const FString& SlotName, const int32 UserIndex );
};
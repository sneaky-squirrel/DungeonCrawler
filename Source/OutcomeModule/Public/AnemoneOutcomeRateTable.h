#pragma once
#include "AnemoneOutcomeRateTable.generated.h"

class UAnemoneOutcomeList;
//enum class EOutcomeTableRow : uint8;

UENUM( BlueprintType )
enum class EOutcomeTableRow : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid         UMETA( DisplayName = "INVALID" ),
    // Deadly Attack.
    Despair         UMETA( DisplayName = "DESPAIR" ),
    // Dangerous Attack.
    Behind          UMETA( DisplayName = "BEHIND" ),
    // Strong Attack.
    Disadvantage    UMETA( DisplayName = "DISADVANTAGE" ),
    // Normal Attack.
    Even            UMETA( DisplayName = "EVEN" ),
    // Weak Attack.
    Advantage       UMETA( DisplayName = "ADVANTAGE" ),
    // Very weak Attack.
    Ahead           UMETA( DisplayName = "AHEAD" ),
    // Extremely weak Attack.
    Fodder          UMETA( DisplayName = "FODDER" ),
};

USTRUCT( BlueprintType )
struct OUTCOMEMODULE_API FOutcomeTableRow
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadOnly )
	EOutcomeTableRow Name;

	UPROPERTY( BlueprintReadOnly )
	int32 Threshold;
};

USTRUCT( BlueprintType )
struct OUTCOMEMODULE_API FAnemoneOutcomeRateRow
{
	GENERATED_BODY()

	UPROPERTY( SaveGame, VisibleAnywhere, BlueprintReadWrite )
	EOutcomeTableRow Name;

	UPROPERTY( SaveGame, EditDefaultsOnly, EditFixedSize, BlueprintReadWrite )
	TArray< int32 > List;

	FAnemoneOutcomeRateRow();
	FAnemoneOutcomeRateRow( EOutcomeTableRow InName, int32 InLength );

	//friend FArchive& operator<<( FArchive& Ar, FAnemoneScore& Score );
	//friend FArchive& operator<<( FArchive& Ar, TMap< FName, FAnemoneScore >& ScoreSheet );
};

UCLASS( Blueprintable, DefaultToInstanced, EditInLineNew, DisplayName="Outcome Rate Table", collapseCategories )
class OUTCOMEMODULE_API UAnemoneOutcomeRateTable : public UObject
{
	GENERATED_BODY()
public:
	UAnemoneOutcomeRateTable();
	void Init();

#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif

	UFUNCTION( BlueprintCallable, Category="OutcomeTable")
	UAnemoneOutcomeRateTable* CreateModifiedTable( const UAnemoneOutcomeRateTable* InTargetTable ) const;

	UFUNCTION( BlueprintCallable, Category="OutcomeTable")
	void MergeOutcomeRateModifierTables( const UAnemoneOutcomeRateTable* InSource );

	UFUNCTION( BlueprintCallable, Category="Log")
	void PrintToLog() const;

	UFUNCTION( BlueprintCallable, Category="OutcomeTable")
	TArray<int32> ConvertDifferenceToOutcomeRate( int32 ScoreDifference );
private:
	UPROPERTY( EditDefaultsOnly )
	bool bIsTableModifier;

	UPROPERTY( EditDefaultsOnly )
	UAnemoneOutcomeList* OutcomeList;

	UPROPERTY( EditDefaultsOnly, EditFixedSize, BlueprintReadWrite, meta = ( AllowPrivateAccess = "true" ) )
	TArray< FAnemoneOutcomeRateRow > OutcomeRateTable;

	friend UAnemoneOutcomeRateTable* CreateTableCopy( const UAnemoneOutcomeRateTable* InTable );

	friend class UAnemoneMessageQueryOutcome;
	friend class UAnemoneComponent_MessageRouter;
};

void ApplyModifierToArray( TArray<int32>& TargetArray, int32 ArrayIndex, int32 Amount );
void MergeArrays( TArray<int32>& OutDestination, const TArray<int32>& InSource );
void PrintArray( TArray<int32> Array );

extern TArray< FOutcomeTableRow > OutcomeRateTableRows;

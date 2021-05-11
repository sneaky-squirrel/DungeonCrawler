#include "AnemoneOutcomeRateTable.h"
#include "AnemoneOutcomeManager.h"
//#include "AnemoneGameData.h"

TArray< FOutcomeTableRow > OutcomeRateTableRows =
	{
		{ EOutcomeTableRow::Fodder, 8 },
		{ EOutcomeTableRow::Ahead, 4 },
		{ EOutcomeTableRow::Advantage, 2 },
		{ EOutcomeTableRow::Even, 0 },
		{ EOutcomeTableRow::Disadvantage, -4 },
		{ EOutcomeTableRow::Behind, -8 },
		{ EOutcomeTableRow::Despair, -12 }
	};

FAnemoneOutcomeRateRow::FAnemoneOutcomeRateRow()
{
}

FAnemoneOutcomeRateRow::FAnemoneOutcomeRateRow( EOutcomeTableRow InName, int32 InLength )
{
	Name = InName;
	List.Init( 0, InLength );
}

UAnemoneOutcomeRateTable::UAnemoneOutcomeRateTable()
{
	Init();
}

void UAnemoneOutcomeRateTable::Init()
{
	OutcomeRateTable.Empty();
	int32 RowLength = ( OutcomeList ) ?
							( ( !bIsTableModifier ) ?
									OutcomeList->List.Num()
									: OutcomeList->List.Num() - 1 )
							: 0;
	for( FOutcomeTableRow Row : OutcomeRateTableRows )
	{
		OutcomeRateTable.Emplace( FAnemoneOutcomeRateRow( Row.Name, RowLength ) );
	}
}

#if WITH_EDITOR
void UAnemoneOutcomeRateTable::PostInitProperties()
{
	Super::PostInitProperties();
	UE_LOG( LogTemp, Warning, TEXT("DEBUG: PostInitProperties()") );
	//if( GetWorld() ? GetWorld()->WorldType == EWorldType::Editor : false )
	if( GEngine ? GEngine->IsEditor() : false )
	{
		UE_LOG( LogTemp, Warning, TEXT("+ Editor") );
		Init();
	}
}

void UAnemoneOutcomeRateTable::PostEditChangeProperty( FPropertyChangedEvent& Event )
{
	FName PropertyName = ( Event.Property != nullptr ) ? FName( Event.Property->GetName() ) : NAME_None;
	if( PropertyName == GET_MEMBER_NAME_CHECKED( UAnemoneOutcomeRateTable, OutcomeList ) ||
		PropertyName == GET_MEMBER_NAME_CHECKED( UAnemoneOutcomeRateTable, bIsTableModifier ) )
	{
		Init();
	}
}
#endif

UAnemoneOutcomeRateTable* UAnemoneOutcomeRateTable::CreateModifiedTable( const UAnemoneOutcomeRateTable* InTargetTable ) const
{
	int32 Length = OutcomeList->List.Num();
	int32 Amount;
	FAnemoneOutcomeRateRow* TempRow;
	UAnemoneOutcomeRateTable* TempTable = CreateTableCopy( InTargetTable );
	if( OutcomeList->List != InTargetTable->OutcomeList->List || !bIsTableModifier || InTargetTable->bIsTableModifier )
	{
		UE_LOG( LogTemp, Warning, TEXT("ERROR: OutComeRateTable::ApplyModifiersToTable() || Outcome Lists must match. Caller must be modifier, target must not be a modifier.") );
		return nullptr;
	}
	for( auto Row : OutcomeRateTable )
	{
		for( uint8 i = 0; i < Length; ++i )
		{
			Amount = Row.List[ i ];
			TempRow = TempTable->OutcomeRateTable.FindByPredicate(
				[ Row ] ( FAnemoneOutcomeRateRow& CurrentRow ) { return CurrentRow.Name == Row.Name; } );
			ApplyModifierToArray( TempRow->List, i, Amount );
		}
	}
	return TempTable;
}

void UAnemoneOutcomeRateTable::MergeOutcomeRateModifierTables( const UAnemoneOutcomeRateTable* InSource )
{
	if( OutcomeList != InSource->OutcomeList )
	{
		UE_LOG( LogTemp, Warning, TEXT("ERROR: OutComeRateTable::MergeOutcomeRateModifierTables() || SRC and DST are of different length.") );
		return;
	}
	for( auto InRow : InSource->OutcomeRateTable )
	{
		FAnemoneOutcomeRateRow* TableRow = OutcomeRateTable.FindByPredicate(
			[ InRow ] ( const FAnemoneOutcomeRateRow& CurrentRow ) { return CurrentRow.Name == InRow.Name; } );
		if( TableRow != nullptr )
		{
			MergeArrays( TableRow->List, InRow.List );
		}
	}
}

void UAnemoneOutcomeRateTable::PrintToLog() const
{
	for( auto Row : OutcomeRateTable )
	{
		static const UEnum* OutcomeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EOutcomeTableRow"));
		UE_LOG( LogTemp, Warning, TEXT( "Row = %s" ), *OutcomeEnum->GetNameStringByIndex( static_cast<uint32>( Row.Name ) ) );
		//UE_LOG( LogTemp, Warning, TEXT("%s"), *Row.Key.ToString() );
		PrintArray( Row.List );
	}
}

TArray<int32> UAnemoneOutcomeRateTable::ConvertDifferenceToOutcomeRate( int32 ScoreDifference )
{
	FAnemoneOutcomeRateRow* Row = nullptr;
	FOutcomeTableRow* RowEntry = OutcomeRateTableRows.FindByPredicate(
		[ ScoreDifference ] ( const FOutcomeTableRow& CurrentRow ) { return ScoreDifference > CurrentRow.Threshold; } );
	if( RowEntry != nullptr )
	{
		Row = OutcomeRateTable.FindByPredicate( [ RowEntry ] ( const FAnemoneOutcomeRateRow& TableRow ) { return RowEntry->Name == TableRow.Name; } );
	}
	return ( Row != nullptr ) ? Row->List : TArray< int32 >();
}

UAnemoneOutcomeRateTable* CreateTableCopy( const UAnemoneOutcomeRateTable* InTable )
{
	int32 RowLength = ( !InTable->bIsTableModifier ) ? InTable->OutcomeList->List.Num() : InTable->OutcomeList->List.Num() - 1;
	UAnemoneOutcomeRateTable* NewTable = NewObject< UAnemoneOutcomeRateTable >( GetTransientPackage(), FName( TEXT( "OutcomeRateTable" ) ) );
	const FAnemoneOutcomeRateRow* InRow;
	for( FOutcomeTableRow RowEntry : OutcomeRateTableRows )
	{
		FAnemoneOutcomeRateRow NewArray( RowEntry.Name, RowLength );
		InRow = InTable->OutcomeRateTable.FindByPredicate( [ RowEntry ] ( const FAnemoneOutcomeRateRow& Row ) { return RowEntry.Name == Row.Name; } );
		if( InRow != nullptr )
		{
			MergeArrays( NewArray.List, InRow->List );
			NewTable->OutcomeRateTable.Emplace( NewArray );
		}
	}
	return NewTable;
}

void ApplyModifierToArray( TArray<int32>& OutTargetArray, int32 StartingIndex, int32 Amount )
{
	if( Amount == 0 )
	{
		return;
	}
	int32 LastValidIndex = OutTargetArray.Num() - 2;
	if( StartingIndex > LastValidIndex )
	{
		UE_LOG( LogTemp, Warning, TEXT("ERROR: OutComeRateTable::ApplyModifier() || Starting Index is Out of Bounds.") );
		return;
	}
	int32 RemainingAmount = Amount;
	// Favor Offense
	if( RemainingAmount < 0 )
	{
		for( uint8 i = StartingIndex; RemainingAmount < 0 && i >= 0; )
		{
			if( OutTargetArray[ i ] > 1 )
			{
				--OutTargetArray[ i ];
				++OutTargetArray[ i + 1 ];
				++RemainingAmount;
			}
			else
			{
				--i;
			}
		}
	}
	// Favor Defense
	else
	{
		for( uint8 i = StartingIndex; RemainingAmount > 0 && i >= 0; )
		{
			if( OutTargetArray[ i + 1 ] > 1 )
			{
				--OutTargetArray[ i + 1 ];
				++OutTargetArray[ i ];
				--RemainingAmount;
			}
			else
			{
				--i;
			}
		}
	}
}

void MergeArrays( TArray<int32>& OutDestination, const TArray<int32>& InSource )
{
	int Length = InSource.Num();
	if( Length != OutDestination.Num() )
	{
		UE_LOG( LogTemp, Warning, TEXT("ERROR: OutComeRateTable::MergeArrays() || SRC and DST Arrays are of different length.") );
		return;
	}
	for( uint8 i = 0; i < Length; ++i )
	{
		OutDestination[ i ] += InSource[ i ];
	}
}

void PrintArray( TArray<int32> Array )
{
	uint8 Length = Array.Num();
	FString OutputString;
	for( int i = 0; i < Length; ++i )
	{
		OutputString.Appendf( TEXT("%d |X| "), Array[ i ] );
	}
	UE_LOG( LogTemp, Warning, TEXT("%s") , *OutputString );
}

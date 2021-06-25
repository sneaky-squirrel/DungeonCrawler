#pragma once
#include "AnemoneEnum.generated.h"

UENUM( BlueprintType, meta = ( ScriptName="AnemoneGameState" ) )
 enum class EAnemoneGameState: uint8
 {
    DungeonCrawl                UMETA( DisplayName = "DUNGEON CRAWL" ),
    DungeonStatusMenuMain       UMETA( DisplayName = "DUNGEON STATUS MENU MAIN" ),
    DungeonStatusMenuParty      UMETA( DisplayName = "DUNGEON STATUS MENU PARTY" ),
    DungeonStatusMenuInventory  UMETA( DisplayName = "DUNGEON STATUS MENU INVENTORY" )
 };
using EGameState = EAnemoneGameState;

UENUM( BlueprintType )
enum class EAnemoneScore : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid        UMETA( DisplayName = "INVALID" ),
    Constitution   UMETA( DisplayName = "CONSTITUTION" ),
    Agility        UMETA( DisplayName = "AGILITY" ),
    Finesse        UMETA( DisplayName = "FINESSE" ),
    Mind           UMETA( DisplayName = "MIND" ),

    Health          UMETA( DisplayName = "OW : HEALTH" ),
    Rest            UMETA( DisplayName = "OW : REST" ),
    Practice        UMETA( DisplayName = "OW : PRACTICE" ),
    Confidence      UMETA( DisplayName = "OW : CONFIDENCE" ),

    Hitpoints       UMETA( DisplayName = "HITPOINTS" ),
    Stamina         UMETA( DisplayName = "STAMINA" ),
    Fortitude       UMETA( DisplayName = "FORTITUDE" ),
    Strength        UMETA( DisplayName = "STRENGTH" ),

    Speed           UMETA( DisplayName = "SPEED" ),
    Evasion         UMETA( DisplayName = "EVASION" ),
    Skulking        UMETA( DisplayName = "SKULKING" ),
    Lock            UMETA( DisplayName = "LOCK" ),

    Prowess         UMETA( DisplayName = "PROWESS" ),
    Accuracy        UMETA( DisplayName = "ACCURACY" ),
    Technique       UMETA( DisplayName = "TECHNIQUE" ),
    Block           UMETA( DisplayName = "BLOCK" ),

    Mana            UMETA( DisplayName = "MANA" ),
    Resolve         UMETA( DisplayName = "RESOLVE" ),
    Cunning         UMETA( DisplayName = "CUNNING" ),
    Perception      UMETA( DisplayName = "PERCEPTION" )
};
using EScore = EAnemoneScore;
using ScoreList = TArray< EAnemoneScore >;

UENUM( BlueprintType )
enum class EAnemoneScoreSet : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid         UMETA( DisplayName = "INVALID" ),

    // Speed    VS  Technique.
    Technique       UMETA( DisplayName = "TECHNIQUE" ),

    // Accuracy VS  Evasion.
    Evasion         UMETA( DisplayName = "EVASION" ),

    // Strength VS  Block.
    Block           UMETA( DisplayName = "BLOCK" )
};
using EScoreSet = EAnemoneScoreSet;

UENUM( BlueprintType )
enum class EAnemoneScoreRank : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid         UMETA( DisplayName = "INVALID" ),
    // Top Rank.
    Top             UMETA( DisplayName = "1: TOP" ),
    // 2nd.
    Elite           UMETA( DisplayName = "2: ELITE" ),
    // 3rd.
    Strong          UMETA( DisplayName = "3: STRONG" ),
    // Middle.
    Average         UMETA( DisplayName = "4: AVERAGE" ),
    // 5th.
    Inferior        UMETA( DisplayName = "5: INFERIOR" ),
    // 6th.
    Weak            UMETA( DisplayName = "6: WEAK" ),
    // Bottom Rank.
    Fodder          UMETA( DisplayName = "7: FODDER" )
};
using ERank = EAnemoneScoreRank;

/*
* Order in this particular enumerator is critical to sorting.
* Make sure to always leave "LastComponent" as the last value
* in the enumerator.
*/

/*
UENUM( BlueprintType )
enum class EAnemoneComponentPriority: uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid         UMETA( DisplayName = "INVALID" ),

    // This component is meant to silently omit messages before they reach "Fire Event" components.
    MessageFilter   UMETA( DisplayName = "MESSAGE FILTER" ),

    MessageModifier UMETA( DisplayName = "MESSAGE MODIFIER" ),

    FireEvent       UMETA( DisplayName = "FIRE EVENT" )
};
*/

UENUM( BlueprintType )
 enum class EAnemoneDirection : uint8
 {
    Left        UMETA( DisplayName = "LEFT" ),
    Right       UMETA( DisplayName = "RIGHT" ),
    Up          UMETA( DisplayName = "UP" ),
    Down        UMETA( DisplayName = "DOWN" ),

    Next        UMETA( DisplayName = "NEXT" ),
    Previous    UMETA( DisplayName = "PREVIOUS" ),

    Southwest   UMETA( DisplayName = "SOUTH_WEST" ),
    Southeast   UMETA( DisplayName = "SOUTH_EAST" ),
    East        UMETA( DisplayName = "EAST" ),
    Northeast   UMETA( DisplayName = "NORTH_EAST" ),
    Northwest   UMETA( DisplayName = "NORTH_WEST" ),
    West        UMETA( DisplayName = "WEST" )
 };

UENUM( BlueprintType )
 enum class EAnemoneWidgetItemState : uint8
 {
    Focused     UMETA( DisplayName = "HAS_FOCUS" ),
    Pressed     UMETA( DisplayName = "CURRENTLY_PRESSED" ),
    NotFocused  UMETA( DisplayName = "NO_FOCUS" )
 };

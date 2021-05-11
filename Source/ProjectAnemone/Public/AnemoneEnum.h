#pragma once
#include "AnemoneEnum.generated.h"

UENUM( BlueprintType, meta = ( ScriptName="AnemoneGameState" ) )
 enum class EGameState: uint8
 {
    DungeonCrawl                UMETA( DisplayName = "DUNGEON CRAWL" ),
    DungeonStatusMenuMain       UMETA( DisplayName = "DUNGEON STATUS MENU MAIN" ),
    DungeonStatusMenuParty      UMETA( DisplayName = "DUNGEON STATUS MENU PARTY" ),
    DungeonStatusMenuInventory  UMETA( DisplayName = "DUNGEON STATUS MENU INVENTORY" )
 };

UENUM( BlueprintType )
enum class EScore : uint8
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
    Strength        UMETA( DisplayName = "STRENGTH" ),
    Block           UMETA( DisplayName = "BLOCK" ),

    Speed           UMETA( DisplayName = "SPEED" ),
    Evasion         UMETA( DisplayName = "EVASION" ),
    Skulking        UMETA( DisplayName = "SKULKING" ),

    Discipline      UMETA( DisplayName = "DISCIPLINE" ),
    Accuracy        UMETA( DisplayName = "ACCURACY" ),
    Parry           UMETA( DisplayName = "PARRY" ),
    Lock            UMETA( DisplayName = "LOCK" ),

    Mana            UMETA( DisplayName = "MANA" ),
    Resolve         UMETA( DisplayName = "RESOLVE" ),
    Foresight       UMETA( DisplayName = "FORESIGHT" ),
    Perception      UMETA( DisplayName = "PERCEPTION" )
};

UENUM( BlueprintType )
enum class EModule : uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid             UMETA( DisplayName = "INVALID" ),

    // Handles messages from the Instigator "Requesting that the Group poll all members to act".
    QueryAssist         UMETA( DisplayName = "ON QUERY ASSIST" ),

    // Handles messages from the Group "Ordering the Entity to Act".
    StartAction         UMETA( DisplayName = "ON START ACTION" ),

    // Instigator handles Skill messages he sends.
    SendQueryOutcome    UMETA( DisplayName = "ON SEND QUERY OUTCOME" ),

    // [Target] or [Group] handles Skill messages from an Instigator.
    GetQueryOutcome     UMETA( DisplayName = "ON GET QUERY OUTCOME" ),

    // Instigator handles ScoreDelta message.  Instigator handles it after the target.
    GiveScoreDelta  UMETA( DisplayName = "ON GIVE SCORE DELTA" ),

    // Target handles ScoreDelta message.  Target handles it before the instigator.
    GainScoreDelta  UMETA( DisplayName = "ON GAIN SCORE DELTA" ),

    // Target handles a parry outcome.
    Parry           UMETA( DisplayName = "ON PARRY" ),
    // Target handles an evasion outcome.
    Evasion         UMETA( DisplayName = "ON EVASION" ),
    // Target handles a block outcome.
    Block           UMETA( DisplayName = "ON BLOCK" ),

    // Instigator handles GiveStatus message.
    GiveStatus      UMETA( DisplayName = "ON GIVE STATUS" ),
    // Target handles GiveStatus message.
    GainStatus      UMETA( DisplayName = "ON GAIN STATUS" ),

    // Instigator handles RemoveStatus message.
    RemoveStatus    UMETA( DisplayName = "ON REMOVE STATUS" ),
    // Target handles RemoveStatus message.
    LoseStatus      UMETA( DisplayName = "ON LOSE STATUS" )
};

/*
* Order in this particular enumerator is critical to sorting.
* Make sure to always leave "LastComponent" as the last value
* in the enumerator.
*/

UENUM( BlueprintType )
enum class EComponentPriority: uint8
{
    // Invalid Value, you must replace this with a relevant value.
    Invalid         UMETA( DisplayName = "INVALID" ),

    // This component is meant to silently omit messages before they reach "Fire Event" components.
    MessageFilter   UMETA( DisplayName = "MESSAGE FILTER" ),

    /*
    *  This component is meant to change the message to reflect the entity receiving the message.
    *  Modifier components are meant to handle message before "Fire Event" components.
    *  e.g. Entity receives increased / unchanged / reduced fire damage.
    *  e.g. Entity changes fire damage received to health restored instead.
    */
    MessageModifier UMETA( DisplayName = "MESSAGE MODIFIER" ),

    /*
    *  This component is meant to fire an additional event if a particular message is received.
    *  e.g. Entity counterattacks if it receives more than 10 fire damage from an enemy.
    */
    FireEvent       UMETA( DisplayName = "FIRE EVENT" ),

    // Router Priority: Parry
    Parry           UMETA( DisplayName = "PARRY" ),
    // Router Priority: Evasion
    Evasion         UMETA( DisplayName = "EVASION" ),
    // Router Priority: Block
    Block           UMETA( DisplayName = "BLOCK" ),

    /*
    *  This component is meant to be the last message in the queue, which usually resolves a message.
    *  e.g. The last component in the parry module will Resolve the "Parry" by calling
    *  InstigatorSkill->ResolveOutcome( Parry );
    */
    LastComponent   UMETA( DisplayName = "LAST COMPONENT" )
};

UENUM( BlueprintType )
enum class EMessageType : uint8
{
    Base            UMETA( DisplayName = "BASE" ),
    QueryAssist     UMETA( DisplayName = "QUERY ASSIST" ),
    StartAction     UMETA( DisplayName = "START ACTION" ),
    QueryOutcome    UMETA( DisplayName = "QUERY OUTCOME" ),
//  ReportOutcome   UMETA( DisplayName = "REPORT OUTCOME" ),
    ScoreDelta      UMETA( DisplayName = "SCORE DELTA" ),
    Status          UMETA( DisplayName = "APPLY STATUS" )
};

UENUM( BlueprintType )
 enum class EDefenseCategory : uint8
 {
    Parry       UMETA( DisplayName = "PARRY" ),
    Evasion     UMETA( DisplayName = "EVASION" ),
    Block       UMETA( DisplayName = "BLOCK" ),
    Foresight   UMETA( DisplayName = "FORESIGHT" ),
    Resolve     UMETA( DisplayName = "RESOLVE" )
 };

UENUM( BlueprintType )
 enum class EModifier : uint8
 {
    Plus    UMETA( DisplayName = "PLUS" ),
    Minus   UMETA( DisplayName = "MINUS" )
 };

UENUM( BlueprintType )
 enum class EAnemoneDirection : uint8
 {
    Left        UMETA( DisplayName = "LEFT" ),
    Right       UMETA( DisplayName = "RIGHT" ),
    Up          UMETA( DisplayName = "UP" ),
    Down        UMETA( DisplayName = "DOWN" )
 };

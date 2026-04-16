/*
 * DB2 Data Access Example
 * Demonstrates how to use TrinityCore's already-loaded DB2 data in AngelScript.
 * No custom loading needed — TC loads all DB2 at server startup.
 */

#include "../ScriptFramework.as"

// =============================================================================
// EXAMPLE 1: Spell Data Lookup
// =============================================================================

void OnSpellLookupCommand(Player@ player, uint32 spellId)
{
    if (player is null) return;
    
    // Check if spell exists in DB2
    if (!HasSpellDB(spellId))
    {
        SendSystemMessage(player, "Spell ID " + spellId + " not found in DB2");
        return;
    }
    
    // Get spell info from DB2
    string name = GetSpellName(spellId);
    uint32 school = GetSpellSchool(spellId);
    uint32 castTime = GetSpellCastTimeDB(spellId);
    uint32 duration = GetSpellDurationDB(spellId);
    
    SendSystemMessage(player, "=== Spell DB2 Data ===");
    SendSystemMessage(player, "Name: " + name);
    SendSystemMessage(player, "School: " + school);
    SendSystemMessage(player, "Cast Time: " + castTime + "ms");
    SendSystemMessage(player, "Duration: " + duration);
}

// =============================================================================
// EXAMPLE 2: Item Data Lookup
// =============================================================================

void OnItemLookupCommand(Player@ player, uint32 itemId)
{
    if (player is null) return;
    
    if (!HasItemDB(itemId))
    {
        SendSystemMessage(player, "Item ID " + itemId + " not found in DB2");
        return;
    }
    
    string name = GetItemName(itemId);
    uint32 classId = GetItemClass(itemId);
    uint32 subClass = GetItemSubClass(itemId);
    int32 invType = GetItemInventoryType(itemId);
    bool isToy = IsToyItem(itemId);
    
    SendSystemMessage(player, "=== Item DB2 Data ===");
    SendSystemMessage(player, "Name: " + name);
    SendSystemMessage(player, "Class: " + classId + ", SubClass: " + subClass);
    SendSystemMessage(player, "Inventory Type: " + invType);
    if (isToy) SendSystemMessage(player, "This is a toy item!");
}

// =============================================================================
// EXAMPLE 3: Map & Area Data
// =============================================================================

void OnMapInfoCommand(Player@ player)
{
    if (player is null) return;
    
    uint32 mapId = player.GetMapId();
    string mapName = GetMapName(mapId);
    uint32 zoneId = player.GetZoneId();
    string zoneName = GetAreaName(zoneId);
    uint32 areaId = player.GetAreaId();
    string areaName = GetAreaName(areaId);
    
    bool isDungeon = IsDungeonMap(mapId);
    bool isRaid = IsRaidMap(mapId);
    bool isBG = IsBattlegroundMap(mapId);
    
    SendSystemMessage(player, "=== Map & Area DB2 Data ===");
    SendSystemMessage(player, "Map: " + mapName + " (ID: " + mapId + ")");
    SendSystemMessage(player, "Zone: " + zoneName + " (ID: " + zoneId + ")");
    SendSystemMessage(player, "Area: " + areaName + " (ID: " + areaId + ")");
    
    if (isDungeon) SendSystemMessage(player, "Type: Dungeon");
    else if (isRaid) SendSystemMessage(player, "Type: Raid");
    else if (isBG) SendSystemMessage(player, "Type: Battleground");
}

// =============================================================================
// EXAMPLE 4: Race/Class/Faction Data
// =============================================================================

void OnPlayerLogin(Player@ player)
{
    if (player is null) return;
    
    string className = GetClassName(player.GetClass());
    string raceName = GetRaceName(player.GetRace());
    uint32 raceFaction = GetRaceFactionId(player.GetRace());
    string factionName = GetFactionName(raceFaction);
    
    SendSystemMessage(player, "Welcome " + raceName + " " + className + "!");
    SendSystemMessage(player, "Your faction: " + factionName + " (ID: " + raceFaction + ")");
}

// =============================================================================
// EXAMPLE 5: Curve Values & Broadcast Text
// =============================================================================

void OnCurveExample(Player@ player, uint32 curveId)
{
    if (player is null) return;
    
    // Get curve value at specific point
    float value = GetCurveValue(curveId, 1.0f);
    SendSystemMessage(player, "Curve " + curveId + " at x=1.0: " + value);
    
    // Get global curve ID
    uint32 globalCurve = GetGlobalCurveId(0);
    SendSystemMessage(player, "Global curve 0: " + globalCurve);
}

// =============================================================================
// EXAMPLE 6: Generic DB2 Store Queries
// =============================================================================

void OnStoreInfoCommand(Player@ player, const string& in storeName)
{
    if (player is null) return;
    
    // Get entry count for any DB2 store
    uint32 count = GetDB2StoreEntryCount(storeName);
    SendSystemMessage(player, "DB2 Store '" + storeName + "': " + count + " entries");
    
    // Check if specific entry exists
    if (HasDB2StoreEntry(storeName, 1))
        SendSystemMessage(player, "  Entry 1 exists in " + storeName);
}

// =============================================================================
// MAIN REGISTRATION
// =============================================================================

void main()
{
    Print("DB2 Data Example loaded!");
    Print("Available DB2 functions:");
    Print("  GetSpellName, GetSpellSchool, HasSpellDB");
    Print("  GetItemName, GetItemClass, HasItemDB, IsToyItem");
    Print("  GetMapName, IsDungeonMap, IsRaidMap");
    Print("  GetAreaName, GetAreaParentAreaId");
    Print("  GetFactionName, GetClassName, GetRaceName");
    Print("  GetSkillName, GetDifficultyName");
    Print("  GetCurveValue, GetBroadcastText");
    Print("  GetDB2StoreEntryCount, HasDB2StoreEntry");
    
    RegisterPlayerScript(PLAYER_ON_LOGIN, @OnPlayerLogin);
}

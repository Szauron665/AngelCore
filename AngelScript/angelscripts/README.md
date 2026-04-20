# AngelScript for TrinityCore

AngelScript integration for TrinityCore — write game scripts in a C++-like scripting language with hot-reload support.

## Quick Start

1. Build the server with `ANGELSCRIPT_INTEGRATION` defined (enabled by default)
2. Place `.as` script files in the `angelscripts/` directory next to your worldserver binary
3. Use `.reload angelscript` in-game to reload all scripts, or `.reload angelscript <name>` to reload a single script

## Architecture

```
AngelScript/
├── AngelScriptMgr.cpp/h    — Engine lifecycle, script loading, hook dispatch
├── API/                     — Per-type API registration files
│   ├── ASPlayerAPI          — Player methods (45+)
│   ├── ASCreatureAPI        — Creature methods (55+)
│   ├── ASGameObjectAPI      — GameObject methods (35+)
│   ├── ASUnitAPI            — Unit methods (50+)
│   ├── ASSpellAPI           — Spell methods (18+)
│   ├── ASGlobalFunctions    — Print, SendSystemMessage, FindPlayer, World accessors, Math
│   ├── ASDatabaseAPI        — MySQL query/execute (Character/World/Login DB)
│   ├── ASDB2API             — DB2 data store access (shared from TC)
│   ├── ASGossipAPI          — NPC gossip menu creation
│   └── ASPacketAPI          — Packet read/write/bits + SendPacket
├── Dispatch/                — TC ScriptObject subclasses (route through sScriptMgr)
├── Hooks/                   — Hook managers (per-type hook storage)
├── SDK/                     — AngelScript SDK (compiler, VM)
└── angelscripts/            — Script files (.as)
    ├── ScriptFramework.as   — Constants, hook types, helper functions
    ├── Common.as            — Type aliases, math constants, utilities
    └── examples/            — Example scripts
```

## Script Defense Mechanism

The AngelScript compiler rejects problematic scripts and reports detailed errors to the console:

- **Syntax errors**: Line number, column, and description of the parse error
- **Undefined types/functions**: Reports which symbol is missing and where it was referenced
- **Type mismatches**: Reports expected vs actual type in assignments/calls
- **Compile failures**: The failed module is discarded — it won't pollute the engine
- **Runtime errors**: If `main()` or `RegisterHooks()` fails during execution, it's logged with the script name

Console output format:
```
[AS-ERR] my_script.as(15): Function 'SendFloatingText' not found
[AS-WRN] my_script.as(20): Implicit conversion from 'int' to 'float'
[DEFENSE] Script 'my_script.as' failed to compile (1 errors, 1 warnings):
  ERROR: my_script.as(15): Function 'SendFloatingText' not found
```

Failed scripts are **not loaded** — they cannot crash or corrupt the server.

## Shared Headers

### ScriptFramework.as
Include this in every script for constants and helpers:
```angelscript
#include "../ScriptFramework.as"
```

Provides:
- Hook type constants (`PLAYER_ON_LOGIN`, `CREATURE_ON_DEATH`, etc.)
- Power type constants (`POWER_MANA`, `POWER_RAGE`, etc.)
- Class/race/team constants
- Item class constants, spell school masks
- DB2 store name constants (`DB2_STORE_SPELL_NAME`, `DB2_STORE_ITEM`, etc.)
- Helper functions: `GetTeam()`, `GiveXP()`, `SendFloatingText()`, `PlaySoundToPlayer()`

### Common.as
```angelscript
#include "../Common.as"
```
Provides type aliases, math constants (PI, DEG2RAD), utility functions.

## Hook Registration

```angelscript
#include "../ScriptFramework.as"

void OnLogin(Player@ player)
{
    SendSystemMessage(player, "Welcome!");
}

void main()
{
    RegisterPlayerScript(PLAYER_ON_LOGIN, @OnLogin);
}
```

### Available Hook Types

| Category | Constants | Registration Function |
|----------|-----------|----------------------|
| World | `WORLD_ON_STARTUP`, `WORLD_ON_SHUTDOWN`, `WORLD_ON_UPDATE`, `WORLD_ON_CONFIG_LOAD` | `RegisterWorldScript()` |
| Console | `WORLD_ON_CONSOLE_COMMAND` | `RegisterConsoleCommandHook()` - receives command string |
| Player | `PLAYER_ON_LOGIN`, `PLAYER_ON_LOGOUT`, `PLAYER_ON_CHAT`, `PLAYER_ON_LEVEL_UP`, `PLAYER_ON_DEATH`, `PLAYER_ON_KILL_CREATURE`, `PLAYER_ON_KILL_PLAYER`, `PLAYER_ON_KILLED_BY_CREATURE`, `PLAYER_ON_DUEL_START`, `PLAYER_ON_DUEL_END`, `PLAYER_ON_MONEY_CHANGE`, `PLAYER_ON_GIVE_XP`, `PLAYER_ON_REPUTATION_CHANGE`, `PLAYER_ON_UPDATE_ZONE`, `PLAYER_ON_MAP_CHANGE` | `RegisterPlayerScript()` |
| Creature | `CREATURE_ON_SPAWN`, `CREATURE_ON_DEATH`, `CREATURE_ON_ENTER_COMBAT`, `CREATURE_ON_LEAVE_COMBAT`, `CREATURE_ON_DAMAGE`, `CREATURE_ON_GOSSIP_HELLO`, `CREATURE_ON_GOSSIP_SELECT` | `RegisterCreatureScript()` |
| GameObject | `GO_ON_SPAWN`, `GO_ON_USE`, `GO_ON_DESTROY`, `GO_ON_GOSSIP_HELLO`, `GO_ON_GOSSIP_SELECT` | `RegisterGameObjectScript()` |
| Spell | `SPELL_ON_CAST`, `SPELL_ON_HIT`, `SPELL_ON_EFFECT`, `SPELL_ON_EFFECT_HIT`, `SPELL_ON_CHECK_CAST` | `RegisterSpellHook()` |
| Quest | `QUEST_ON_ACCEPT`, `QUEST_ON_COMPLETE`, `QUEST_ON_REWARD`, `QUEST_ON_STATUS_CHANGE`, `QUEST_ON_OBJECTIVE_UPDATE` | `RegisterQuestScript()` |
| Packet | `PACKET_ON_RECEIVE`, `PACKET_ON_SEND` | `RegisterPacketScript()` |
| Instance | Instance hooks | `RegisterInstanceScript()` |
| Battleground | BG hooks | `RegisterBattlegroundScript()` |

### Per-Entry Dispatch

Creature and GameObject AI can be registered per-entry:
```angelscript
// Register a creature AI factory for entry 1234
RegisterCreatureAI(1234, @MyCreatureAIFactory);
```

### Spell Effect Handlers

Override specific spell effects:
```angelscript
RegisterSpellEffectHandler(133, 0, @OnFireballEffect);  // Spell 133, effect 0
```

## API Reference

### Player (45+ methods)
```
GetName, GetLevel, GetClass, GetRace, GetGender
IsAlive, IsDead, IsInCombat, IsOnline, IsAFK, IsDND, IsGM, SetGM, IsMounted
GetMapId, GetZoneId, GetAreaId, GetPositionXYZO
TeleportTo, GetHealth, GetMaxHealth, GetHealthPct
GetMoney, ModifyMoney, GetItemCount, HasItemCount, AddItem
GetPower, SetPower, GetQuestStatus, IsQuestRewarded, CompleteQuest, FailQuest
GetReputation, SetReputation, GetGuildId, GetAccountId, GetAccountName
SendNotification, HasAura, AddAura, RemoveAura, RemoveAllAuras
CastSpell, CastSpellSelf, ToCreature, Kill, GiveXP
```

### Creature (55+ methods)
```
GetName, GetEntry, GetGUID, GetSpawnId, GetLevel
IsAlive, IsDead, IsInCombat, IsInCombatWith, CanHaveThreatList
SetInCombatWith, GetHealth, GetMaxHealth, SetHealth, SetFullHealth, GetHealthPct
HasAura, GetReactState, SetReactState, HasLootRecipient, IsTappedBy, HasQuest
Respawn, DespawnOrUnsummon, GetFaction, IsFriendlyTo, IsHostileTo
GetMapId, GetPositionXYZO, AttackStart, CastSpell, CastSpellSelf
AddAura, RemoveAura, ToPlayer, Kill
```

### GameObject (35+ methods)
```
GetName, GetEntry, GetGUID, GetDisplayId, GetGoType
IsSpawned, IsTransport, GetLootState, GetGoState, SetGoState
SetRespawnTime, Respawn, GetFaction, GetMapId, GetPositionXYZO
Use, Delete, Despawn, Enable, Disable, SetFlags, HasFlags
```

### Unit (50+ methods)
```
GetName, GetLevel, GetClass, GetRace, GetGender, GetGUID
IsAlive, IsDead, IsInCombat, IsMounted
GetHealth, GetMaxHealth, SetHealth, SetFullHealth, GetHealthPct
GetPower, GetMaxPower, SetPower, HasAura, GetAuraCount
RemoveAura, RemoveAllAuras, GetFaction, IsFriendlyTo, IsHostileTo
HasUnitFlag, GetDistanceTo, IsWithinDist, GetMapId, GetPositionXYZO
CastSpell, CastSpellSelf, AddAura, ToPlayer, ToCreature, Kill
```

### Spell (18+ methods)
```
GetSpellId, GetCaster, GetOriginalCaster, GetCastTime
Cancel, Finish, IsTriggered, IsChannelActive
GetTarget, GetUnitTarget, GetItemTarget, GetGOTarget
GetRange, IsPositiveSpell, HasEffectType
```

### Global Functions
```
Print(msg)                          — Log to console
SendSystemMessage(player, msg)      — Send system message to player
SendFloatingText(player, text, color) — Floating text (falls back to system message)
PlaySoundToPlayer(player, soundId)  — Play sound kit to player
FindPlayerByName(name)              — Find online player by name
FindPlayerByGUID(guid)              — Find player by GUID counter
GetGameTime()                       — Current game time
GetRealmID(), GetRealmName()        — Realm info
GetMapById(mapId)                   — Get map instance
```

### DB2 Data Access

TrinityCore loads all DB2 data at server startup. AngelScript shares the same data — no custom loading needed.

```angelscript
// Spell data
string name = GetSpellName(133);
uint32 school = GetSpellSchool(133);
bool exists = HasSpellDB(133);

// Item data
string itemName = GetItemName(17349);
uint32 itemClass = GetItemClass(17349);
bool isToy = IsToyItem(17349);

// Map & Area data
string mapName = GetMapName(0);
bool isRaid = IsRaidMap(532);
string areaName = GetAreaName(1519);

// Race/Class/Faction data
string className = GetClassName(1);
string raceName = GetRaceName(1);
string factionName = GetFactionName(1);

// Curve values (spell scaling, XP tables, etc.)
float value = GetCurveValue(1234, 1.0f);

// Broadcast text
string text = GetBroadcastText(12345);

// Generic store queries
uint32 count = GetDB2StoreEntryCount(DB2_STORE_SPELL_NAME);
bool hasEntry = HasDB2StoreEntry(DB2_STORE_ITEM, 17349);
```

Available DB2 functions:
```
GetSpellName, GetSpellSchool, GetSpellCastTimeDB, GetSpellDurationDB, HasSpellDB
GetItemName, GetItemClass, GetItemSubClass, GetItemInventoryType, HasItemDB, IsToyItem
GetCreatureDisplayScale, GetCreatureDisplayModel, HasCreatureDisplayDB
GetMapName, GetMapAreaId, IsDungeonMap, IsRaidMap, IsBattlegroundMap
GetAreaName, GetAreaParentAreaId, GetAreaMapId
GetFactionName, GetFactionTemplateFaction, GetFactionTemplateFlags
GetClassName, GetRaceName, GetRaceFactionId
GetSkillName, GetEmoteAnimState, GetDifficultyName, GetCharTitleName
GetCurveValue, GetBroadcastText, GetCreatureFamilyPetName
GetGlobalCurveId
GetDB2StoreEntryCount, HasDB2StoreEntry
```

### Database API (MySQL)

Execute queries and statements against TC's databases:
```angelscript
// Execute SQL (non-query — INSERT/UPDATE/DELETE)
CharacterExecute("INSERT INTO custom_table (player_id, value) VALUES (1, 100)");
WorldExecute("UPDATE custom_config SET value = 1 WHERE key = 'feature'");

// Query (returns QueryResult handle — currently async-limited)
QueryResult@ result = CharacterQuery("SELECT * FROM custom_table WHERE player_id = 1");
if (result !is null)
{
    while (result.NextRow())
    {
        string val = result.GetString(0);
        int32 num = result.GetInt32(1);
    }
}

// Prepared statement by index
CharacterPreparedStatementExecute(1);
```

### Gossip API

Create NPC gossip menus from scripts:
```angelscript
void OnGossipHello(Creature@ creature, Player@ player)
{
    GossipStartMenu(player, creature, "Welcome! What would you like?");
    GossipAddMenuItem(player, 1, "Tell me about this place");
    GossipAddMenuItem(player, 2, "I want to fight!");
    GossipSendMenu(player);
}
```

### Packet API

Read and write custom packets:
```angelscript
// Read a received packet
void OnPacketReceive(WorldSession@ session, uint32 opcode, uint32 size)
{
    // PacketData reading: ReadUInt8/16/32/64, ReadString, ReadBit, ReadBits
    // PacketData writing: WriteUInt8/16/32/64, WriteString, WriteBit, WriteBits
    return false; // not handled
}

// Send a custom packet to a player
void SendCustomPacket(Player@ player, uint32 opcode)
{
    PacketData@ data = CreatePacketData(opcode);
    data.WriteUInt32(player.GetGUID().GetCounter());
    data.WriteString("Hello");
    SendPacketToPlayer(player, data);
}
```

## Reload Command

| Command | Description |
|---------|-------------|
| `.reload angelscript` | Reload all `.as` scripts from the angelscripts directory |
| `.reload angelscript <name>` | Reload a specific script by filename |

Reload recompiles the script with full defense mechanism — errors are reported and the script is rejected if compilation fails. Existing hooks from the old version are preserved until the new version registers its own.

## Example Scripts

| File | Description |
|------|-------------|
| `simple_spell_example.as` | Spell effect handlers, spell hooks, teleport by faction |
| `simple_instance_example.as` | Instance enter/leave, boss kill tracking |
| `simple_battleground_example.as` | BG start, player join, kill tracking |
| `db2_data_example.as` | DB2 data access: spells, items, maps, areas, factions, curves |

## Notes

- Scripts are compiled at load time — syntax errors are caught and reported with line numbers
- Failed scripts are **rejected** — they cannot crash or corrupt the server
- All hook callbacks should check for null pointers before using objects
- AngelScript runs in the main server thread — avoid blocking operations
- Keep heavy operations out of `WORLD_ON_UPDATE` as it runs every tick
- Return values matter for `PACKET_ON_RECEIVE` (true = handled) and spell check hooks
- DB2 data is shared from TrinityCore — no custom loading needed, always up-to-date
- The `angelscripts/` folder is created automatically at build time

/*
 * Copyright (C) 2008-2025 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2025-2026 AngelScript Integration
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ASSCRIPTATTRIBUTES_H
#define ASSCRIPTATTRIBUTES_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "Define.h"
#include <string>
#include <vector>
#include <map>

// Forward declarations
class asIScriptFunction;
class asIScriptObject;

namespace AngelScript
{
    // Script attribute types for automatic registration
    enum class ScriptAttributeType
    {
        SPELL_SCRIPT,           // [SpellScript(spellId)]
        CREATURE_SCRIPT,        // [CreatureScript(entry)]
        GAMEOBJECT_SCRIPT,      // [GameObjectScript(entry)]
        PLAYER_SCRIPT,          // [PlayerScript]
        INSTANCE_SCRIPT,        // [InstanceScript(mapId)]
        BATTLEGROUND_SCRIPT,    // [BattlegroundScript(bgType)]
        ARENA_SCRIPT,           // [ArenaScript(arenaType)]
        QUEST_SCRIPT,           // [QuestScript(questId)]
        ITEM_SCRIPT,            // [ItemScript(itemId)]
        OPCODE_HANDLER,         // [OpcodeHandler(opcode)]
        WORLD_SCRIPT,           // [WorldScript]
        MAP_SCRIPT,             // [MapScript(mapId)]
        ZONE_SCRIPT,            // [ZoneScript(zoneId)]
        OUTDOOR_PVP_SCRIPT,     // [OutdoorPvPScript(scriptId)]
        GROUP_SCRIPT,           // [GroupScript]
        GUILD_SCRIPT,           // [GuildScript]
        AUCTION_SCRIPT,         // [AuctionScript]
        ACHIEVEMENT_SCRIPT,     // [AchievementScript(achievementId)]
        VEHICLE_SCRIPT,         // [VehicleScript(vehicleId)]
        TRANSPORT_SCRIPT,       // [TransportScript(transportId)]
        CONDITION_SCRIPT,       // [ConditionScript(conditionId)]
        SMART_SCRIPT            // [SmartScript(entryOrGuid, sourceType)]
    };

    // Script attribute data
    struct ScriptAttribute
    {
        ScriptAttributeType type;
        std::vector<uint32> parameters;
        std::string name;
        std::string description;
    };

    // Enhanced script registration with attributes
    class TC_GAME_API ASScriptAttributeParser
    {
    public:
        static ASScriptAttributeParser* instance();

        // Parse class attributes and register scripts
        void ParseAndRegisterClass(asIScriptObject* obj, const std::string& className);
        void ParseAndRegisterFunction(asIScriptFunction* func, const std::string& functionName);
        
        // Attribute parsing
        std::vector<ScriptAttribute> ParseAttributes(const std::string& attributeString);
        ScriptAttribute ParseSingleAttribute(const std::string& attribute);
        
        // Registration helpers
        void RegisterSpellScriptFromAttribute(asIScriptObject* obj, const ScriptAttribute& attr);
        void RegisterCreatureScriptFromAttribute(asIScriptObject* obj, const ScriptAttribute& attr);
        void RegisterGameObjectScriptFromAttribute(asIScriptObject* obj, const ScriptAttribute& attr);
        void RegisterInstanceScriptFromAttribute(asIScriptObject* obj, const ScriptAttribute& attr);
        void RegisterBattlegroundScriptFromAttribute(asIScriptObject* obj, const ScriptAttribute& attr);
        void RegisterOpcodeHandlerFromAttribute(asIScriptFunction* func, const ScriptAttribute& attr);
        
        // Automatic script discovery
        void DiscoverAndRegisterAllScripts();
        
        void Clear();

    private:
        std::map<std::string, std::vector<ScriptAttribute>> _classAttributes;
        std::map<std::string, std::vector<ScriptAttribute>> _functionAttributes;
    };

    // Base script classes with simplified registration
    
    // Spell script with attribute support
    class TC_GAME_API ASSpellScriptBase
    {
    public:
        ASSpellScriptBase(const char* name) : _scriptName(name) {}
        virtual ~ASSpellScriptBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // Spell events (implement what you need)
        virtual bool OnCheckCast() { return true; }
        virtual void OnPrepare() {}
        virtual void OnTakePower() {}
        virtual void OnBeforeCast() {}
        virtual void OnCast() {}
        virtual void OnHit() {}
        virtual void OnEffectHit(uint8 /*effIndex*/) {}
        virtual int32 OnDamageCalculate(int32 damage) { return damage; }
        virtual int32 OnHealCalculate(int32 heal) { return heal; }
        virtual void OnChannelStart() {}
        virtual void OnChannelUpdate() {}
        virtual void OnChannelEnd(bool /*interrupted*/) {}
        virtual void OnAfterCast() {}

    private:
        std::string _scriptName;
    };

    // Creature script with attribute support
    class TC_GAME_API ASCreatureScriptBase
    {
    public:
        ASCreatureScriptBase(const char* name) : _scriptName(name) {}
        virtual ~ASCreatureScriptBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // Creature events (implement what you need)
        virtual void OnInitialize() {}
        virtual void OnJustEngagedWith() {}
        virtual void OnJustDied() {}
        virtual void OnKilledUnit() {}
        virtual void OnJustSummoned() {}
        virtual void OnIsSummonedBy() {}
        virtual void OnSummonDespawn() {}
        virtual void OnSummonDies() {}
        virtual void OnUpdateAI(uint32 /*diff*/) {}
        virtual void OnMovementInform(uint32 /*type*/, uint32 /*id*/) {}
        virtual void OnDamageTaken(uint32& /*damage*/) {}
        virtual void OnSpellHit() {}
        virtual void OnGossipHello() {}
        virtual void OnGossipSelect(uint32 /*sender*/, uint32 /*action*/) {}

    private:
        std::string _scriptName;
    };

    // GameObject script with attribute support
    class TC_GAME_API ASGameObjectScriptBase
    {
    public:
        ASGameObjectScriptBase(const char* name) : _scriptName(name) {}
        virtual ~ASGameObjectScriptBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // GameObject events (implement what you need)
        virtual void OnInitialize() {}
        virtual void OnUpdate(uint32 /*diff*/) {}
        virtual bool OnGossipHello() { return false; }
        virtual bool OnGossipSelect(uint32 /*sender*/, uint32 /*action*/) { return false; }
        virtual bool OnUse() { return false; }
        virtual void OnLootStateChanged(uint32 /*state*/) {}
        virtual void OnGameObjectStateChanged(uint32 /*state*/) {}

    private:
        std::string _scriptName;
    };

    // Player script with attribute support  
    class TC_GAME_API ASPlayerScriptBase
    {
    public:
        ASPlayerScriptBase(const char* name) : _scriptName(name) {}
        virtual ~ASPlayerScriptBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // Player events (implement what you need)
        virtual void OnLogin() {}
        virtual void OnLogout() {}
        virtual void OnLevelUp(uint8 /*newLevel*/) {}
        virtual void OnKill() {}
        virtual void OnDeath() {}
        virtual void OnChat(uint32 /*type*/, uint32 /*lang*/, std::string& /*message*/) {}
        virtual void OnDuelStart() {}
        virtual void OnDuelEnd(uint32 /*result*/) {}
        virtual void OnMoneyChanged(int32 /*amount*/) {}
        virtual void OnGiveXP(uint32& /*xp*/) {}
        virtual void OnReputationChange(uint32 /*factionId*/, int32& /*standing*/) {}
        virtual void OnItemEquip(uint32 /*itemId*/, uint8 /*slot*/) {}
        virtual void OnItemUnequip(uint32 /*itemId*/, uint8 /*slot*/) {}
        virtual void OnSpellCast(uint32 /*spellId*/) {}
        virtual void OnQuestAccept(uint32 /*questId*/) {}
        virtual void OnQuestComplete(uint32 /*questId*/) {}
        virtual void OnEnterArea(uint32 /*areaId*/) {}
        virtual void OnLeaveArea(uint32 /*areaId*/) {}

    private:
        std::string _scriptName;
    };

    // Quest script with attribute support
    class TC_GAME_API ASQuestScriptBase
    {
    public:
        ASQuestScriptBase(const char* name) : _scriptName(name) {}
        virtual ~ASQuestScriptBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // Quest events (implement what you need)
        virtual void OnAccept() {}
        virtual void OnComplete() {}
        virtual void OnReward() {}
        virtual void OnAddKill(uint32 /*entry*/, uint32 /*count*/) {}
        virtual void OnAddGather(uint32 /*entry*/, uint32 /*count*/) {}
        virtual void OnStatusCheck() {}
        virtual void OnAbandon() {}
        virtual void OnObjectiveUpdate(uint32 /*objectiveId*/, uint32 /*progress*/) {}

    private:
        std::string _scriptName;
    };

    // Opcode handler with attribute support
    class TC_GAME_API ASOpcodeHandlerBase
    {
    public:
        ASOpcodeHandlerBase(const char* name) : _scriptName(name) {}
        virtual ~ASOpcodeHandlerBase() = default;

        const char* GetScriptName() const { return _scriptName.c_str(); }

        // Opcode handling (implement what you need)
        virtual bool OnReceive() { return false; } // Return true to block original handler
        virtual void OnSend() {}

    private:
        std::string _scriptName;
    };

#define sASScriptAttributeParser AngelScript::ASScriptAttributeParser::instance()

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASSCRIPTATTRIBUTES_H

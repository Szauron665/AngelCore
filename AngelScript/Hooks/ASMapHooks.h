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

#ifndef AS_MAP_HOOKS_H
#define AS_MAP_HOOKS_H

#include "Define.h"
#include "Map.h"
#include "InstanceScript.h"
#include <string>
#include <vector>
#include <functional>

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    // Forward declarations
    class AngelScriptMapScript;

    // Map hook types
    enum class MapHookType
    {
        // General map hooks
        ON_PLAYER_ENTER,
        ON_PLAYER_LEAVE,
        ON_UPDATE,
        
        // Instance-specific hooks
        ON_INSTANCE_INIT,
        ON_BOSS_KILL,
        ON_ENCOUNTER_COMPLETE,
        ON_UNIT_DIE,
        ON_CREATURE_CREATE,
        ON_GAMEOBJECT_CREATE,
        ON_CHECK_ENCOUNTER_STATE,
        ON_FILL_INITIAL_WORLD_STATES,
        
        // Battleground hooks
        ON_BG_START,
        ON_BG_END,
        ON_PLAYER_JOIN_BG,
        ON_PLAYER_LEAVE_BG,
        
        MAX_HOOK_TYPES
    };

    // Map script base class for AngelScript
    class AngelScriptMapScript
    {
    public:
        AngelScriptMapScript(const char* name, uint32 mapId);
        virtual ~AngelScriptMapScript() = default;

        const char* GetName() const { return _name.c_str(); }
        uint32 GetMapId() const { return _mapId; }
        bool IsInstance() const { return _isInstance; }

        // Virtual hook methods - override in AngelScript classes
        virtual void OnPlayerEnter(Map* /*map*/, Player* /*player*/) {}
        virtual void OnPlayerLeave(Map* /*map*/, Player* /*player*/) {}
        virtual void OnUpdate(Map* /*map*/, uint32 /*diff*/) {}
        
        // Instance-specific hooks
        virtual void OnInitialize(InstanceScript* /*instance*/) {}
        virtual void OnBossKill(InstanceScript* /*instance*/, Creature* /*boss*/) {}
        virtual void OnEncounterComplete(InstanceScript* /*instance*/, uint32 /*encounterId*/, bool /*success*/) {}
        virtual void OnUnitDie(InstanceScript* /*instance*/, Unit* /*unit*/) {}
        virtual void OnCreatureCreate(InstanceScript* /*instance*/, Creature* /*creature*/) {}
        virtual void OnGameObjectCreate(InstanceScript* /*instance*/, GameObject* /*go*/) {}
        virtual bool OnCheckEncounterState(InstanceScript* /*instance*/) { return true; }
        virtual void OnFillInitialWorldStates(InstanceScript* /*instance*/, WorldPacket& /*data*/) {}

    private:
        std::string _name;
        uint32 _mapId;
        bool _isInstance;
    };

    // Map script registry
    class MapScriptRegistry
    {
    public:
        static MapScriptRegistry* instance();

        void RegisterMapScript(uint32 mapId, AngelScriptMapScript* script);
        void UnregisterMapScript(uint32 mapId, const char* name);
        void UnregisterAllMapScripts();

        // Get script for a specific map
        AngelScriptMapScript* GetMapScript(uint32 mapId, const char* name);
        std::vector<AngelScriptMapScript*> GetMapScripts(uint32 mapId);

        // Hook dispatchers - called from core
        void OnPlayerEnter(Map* map, Player* player);
        void OnPlayerLeave(Map* map, Player* player);
        void OnUpdate(Map* map, uint32 diff);
        
        // Instance hooks
        void OnInitialize(InstanceScript* instance);
        void OnBossKill(InstanceScript* instance, Creature* boss);
        void OnEncounterComplete(InstanceScript* instance, uint32 encounterId, bool success);
        void OnUnitDie(InstanceScript* instance, Unit* unit);
        void OnCreatureCreate(InstanceScript* instance, Creature* creature);
        void OnGameObjectCreate(InstanceScript* instance, GameObject* go);
        bool OnCheckEncounterState(InstanceScript* instance);
        void OnFillInitialWorldStates(InstanceScript* instance, WorldPacket& data);

    private:
        std::unordered_map<uint32, std::vector<AngelScriptMapScript*>> _mapScripts;
    };

#define sASMapScriptRegistry AngelScript::MapScriptRegistry::instance()

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // AS_MAP_HOOKS_H

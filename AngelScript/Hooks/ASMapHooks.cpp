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

#include "ASMapHooks.h"
#include "Log.h"
#include "Map.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Creature.h"
#include "GameObject.h"
#include "DB2Stores.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{

AngelScriptMapScript::AngelScriptMapScript(const char* name, uint32 mapId)
    : _name(name), _mapId(mapId)
{
    _isInstance = sMapStore.LookupEntry(mapId) && sMapStore.LookupEntry(mapId)->Instanceable();
}

MapScriptRegistry* MapScriptRegistry::instance()
{
    static MapScriptRegistry instance;
    return &instance;
}

void MapScriptRegistry::RegisterMapScript(uint32 mapId, AngelScriptMapScript* script)
{
    if (!script)
        return;
    
    _mapScripts[mapId].push_back(script);
    TC_LOG_INFO("angelscript.maps", "Registered map script '{}' for map {}", script->GetName(), mapId);
}

void MapScriptRegistry::UnregisterMapScript(uint32 mapId, const char* name)
{
    auto it = _mapScripts.find(mapId);
    if (it != _mapScripts.end())
    {
        auto& scripts = it->second;
        scripts.erase(
            std::remove_if(scripts.begin(), scripts.end(),
                [name](AngelScriptMapScript* script) {
                    return script && strcmp(script->GetName(), name) == 0;
                }),
            scripts.end()
        );
    }
}

void MapScriptRegistry::UnregisterAllMapScripts()
{
    _mapScripts.clear();
    TC_LOG_INFO("angelscript.maps", "Unregistered all map scripts");
}

AngelScriptMapScript* MapScriptRegistry::GetMapScript(uint32 mapId, const char* name)
{
    auto it = _mapScripts.find(mapId);
    if (it != _mapScripts.end())
    {
        for (auto* script : it->second)
        {
            if (script && strcmp(script->GetName(), name) == 0)
                return script;
        }
    }
    return nullptr;
}

std::vector<AngelScriptMapScript*> MapScriptRegistry::GetMapScripts(uint32 mapId)
{
    auto it = _mapScripts.find(mapId);
    if (it != _mapScripts.end())
        return it->second;
    return {};
}

// Hook dispatchers
void MapScriptRegistry::OnPlayerEnter(Map* map, Player* player)
{
    if (!map || !player)
        return;
    
    auto scripts = GetMapScripts(map->GetId());
    for (auto* script : scripts)
    {
        if (script)
            script->OnPlayerEnter(map, player);
    }
}

void MapScriptRegistry::OnPlayerLeave(Map* map, Player* player)
{
    if (!map || !player)
        return;
    
    auto scripts = GetMapScripts(map->GetId());
    for (auto* script : scripts)
    {
        if (script)
            script->OnPlayerLeave(map, player);
    }
}

void MapScriptRegistry::OnUpdate(Map* map, uint32 diff)
{
    if (!map)
        return;
    
    auto scripts = GetMapScripts(map->GetId());
    for (auto* script : scripts)
    {
        if (script)
            script->OnUpdate(map, diff);
    }
}

// Instance hooks
void MapScriptRegistry::OnInitialize(InstanceScript* instance)
{
    if (!instance || !instance->instance)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnInitialize(instance);
    }
}

void MapScriptRegistry::OnBossKill(InstanceScript* instance, Creature* boss)
{
    if (!instance || !instance->instance || !boss)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnBossKill(instance, boss);
    }
}

void MapScriptRegistry::OnEncounterComplete(InstanceScript* instance, uint32 encounterId, bool success)
{
    if (!instance || !instance->instance)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnEncounterComplete(instance, encounterId, success);
    }
}

void MapScriptRegistry::OnUnitDie(InstanceScript* instance, Unit* unit)
{
    if (!instance || !instance->instance || !unit)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnUnitDie(instance, unit);
    }
}

void MapScriptRegistry::OnCreatureCreate(InstanceScript* instance, Creature* creature)
{
    if (!instance || !instance->instance || !creature)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnCreatureCreate(instance, creature);
    }
}

void MapScriptRegistry::OnGameObjectCreate(InstanceScript* instance, GameObject* go)
{
    if (!instance || !instance->instance || !go)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnGameObjectCreate(instance, go);
    }
}

bool MapScriptRegistry::OnCheckEncounterState(InstanceScript* instance)
{
    if (!instance || !instance->instance)
        return true;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    bool result = true;
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
        {
            if (!script->OnCheckEncounterState(instance))
                result = false;
        }
    }
    return result;
}

void MapScriptRegistry::OnFillInitialWorldStates(InstanceScript* instance, WorldPacket& data)
{
    if (!instance || !instance->instance)
        return;
    
    auto scripts = GetMapScripts(instance->instance->GetId());
    for (auto* script : scripts)
    {
        if (script && script->IsInstance())
            script->OnFillInitialWorldStates(instance, data);
    }
}

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

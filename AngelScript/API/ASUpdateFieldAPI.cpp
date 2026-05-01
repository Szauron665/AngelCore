/*
 * AngelScript UpdateField API - Full Implementation
 * 
 * Exposes TrinityCore update field setters to AngelScript.
 * Uses TC's SetUpdateFieldValue pattern with m_values.ModifyValue().
 */

#ifndef ANGELSCRIPT_INTEGRATION
    #error "ANGELSCRIPT_INTEGRATION macro must be defined"
#endif

#include "ASUpdateFieldAPI.h"
#include "angelscript.h"
#include "Player.h"
#include "Creature.h"
#include "GameObject.h"
#include "Unit.h"
#include "Object.h"
#include "BaseEntity.h"
#include "UpdateFields.h"
#include "Log.h"

namespace AngelScript
{
    // ========================================================================
    // ObjectData Fields (all objects have these)
    // ========================================================================
    static void Object_SetEntryID(Object* obj, int32 value)
    {
        if (!obj) return;
        obj->SetEntry(value);
    }

    static void Object_SetDynamicFlags(Object* obj, uint32 value)
    {
        if (!obj) return;
        obj->ReplaceAllDynamicFlags(value);
    }

    static void Object_SetScale(Object* obj, float value)
    {
        if (!obj) return;
        obj->SetObjectScale(value);
    }

    // ========================================================================
    // UnitData Fields
    // ========================================================================
    static void Unit_SetDisplayID(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->SetDisplayId(value);
    }

    static void Unit_SetFaction(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->SetFaction(value);
    }

    static void Unit_SetLevel(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->SetLevel(uint8(value));
    }

    static void Unit_SetUnitFlags(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->ReplaceAllUnitFlags(UnitFlags(value));
    }

    static void Unit_SetUnitFlags2(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->ReplaceAllUnitFlags2(UnitFlags2(value));
    }

    static void Unit_SetUnitFlags3(Unit* unit, uint32 value)
    {
        if (!unit) return;
        unit->ReplaceAllUnitFlags3(UnitFlags3(value));
    }

    static void Unit_SetEmoteState(Unit* unit, int32 value)
    {
        if (!unit) return;
        unit->SetEmoteState(Emote(value));
    }

    static void Unit_SetSheathState(Unit* unit, uint8 value)
    {
        if (!unit) return;
        unit->SetSheath(SheathState(value));
    }

    // ========================================================================
    // PlayerData Fields
    // ========================================================================
    static void Player_SetPlayerFlags(Player* player, uint32 value)
    {
        if (!player) return;
        player->ReplaceAllPlayerFlags(PlayerFlags(value));
    }

    static void Player_SetPlayerFlagsEx(Player* player, uint32 value)
    {
        if (!player) return;
        player->ReplaceAllPlayerFlagsEx(PlayerFlagsEx(value));
    }

    static void Player_SetHonorLevel(Player* player, uint32 value)
    {
        if (!player) return;
        player->SetHonorLevel(uint8(value));
    }

    static void Player_SetWatchedFactionIndex(Player* player, int32 value)
    {
        if (!player) return;
        player->SetWatchedFactionIndex(value);
    }

    // ========================================================================
    // GameObjectData Fields
    // ========================================================================
    static void GameObject_SetDisplayID(GameObject* go, uint32 value)
    {
        if (!go) return;
        go->SetDisplayId(value);
    }

    static void GameObject_SetFaction(GameObject* go, uint32 value)
    {
        if (!go) return;
        go->SetFaction(value);
    }

    static void GameObject_SetLevel(GameObject* go, uint32 value)
    {
        if (!go) return;
        go->SetLevel(value);
    }

    // ========================================================================
    // Legacy string-based API (kept for compatibility, logs warning)
    // ========================================================================
    static void SetUpdateFieldInt32(Unit* unit, const std::string& fieldPath, int32 /*value*/)
    {
        if (!unit) return;
        TC_LOG_WARN("angelscript", "SetUpdateFieldInt32 with string path '{}' is deprecated. Use specific Set* functions.", fieldPath);
        // Could implement string parsing here if needed
    }

    static void SetUpdateFieldFloat(Unit* unit, const std::string& fieldPath, float /*value*/)
    {
        if (!unit) return;
        TC_LOG_WARN("angelscript", "SetUpdateFieldFloat with string path '{}' is deprecated. Use specific Set* functions.", fieldPath);
    }

    static void SetGOUpdateFieldInt32(GameObject* go, const std::string& fieldPath, int32 /*value*/)
    {
        if (!go) return;
        TC_LOG_WARN("angelscript", "SetGOUpdateFieldInt32 with string path '{}' is deprecated. Use specific Set* functions.", fieldPath);
    }

    void RegisterUpdateFieldAPI(asIScriptEngine* _scriptEngine)
    {
        int r;

        // ObjectData functions
        r = _scriptEngine->RegisterGlobalFunction("void Object_SetEntryID(Unit@, int32)", asFUNCTION(Object_SetEntryID), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Object_SetDynamicFlags(Unit@, uint32)", asFUNCTION(Object_SetDynamicFlags), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Object_SetScale(Unit@, float)", asFUNCTION(Object_SetScale), asCALL_CDECL);

        // UnitData functions
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetDisplayID(Unit@, uint32)", asFUNCTION(Unit_SetDisplayID), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetFaction(Unit@, uint32)", asFUNCTION(Unit_SetFaction), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetLevel(Unit@, uint32)", asFUNCTION(Unit_SetLevel), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetUnitFlags(Unit@, uint32)", asFUNCTION(Unit_SetUnitFlags), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetUnitFlags2(Unit@, uint32)", asFUNCTION(Unit_SetUnitFlags2), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetUnitFlags3(Unit@, uint32)", asFUNCTION(Unit_SetUnitFlags3), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetEmoteState(Unit@, int32)", asFUNCTION(Unit_SetEmoteState), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Unit_SetSheathState(Unit@, uint8)", asFUNCTION(Unit_SetSheathState), asCALL_CDECL);

        // PlayerData functions
        r = _scriptEngine->RegisterGlobalFunction("void Player_SetPlayerFlags(Player@, uint32)", asFUNCTION(Player_SetPlayerFlags), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Player_SetPlayerFlagsEx(Player@, uint32)", asFUNCTION(Player_SetPlayerFlagsEx), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Player_SetHonorLevel(Player@, uint32)", asFUNCTION(Player_SetHonorLevel), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void Player_SetWatchedFactionIndex(Player@, int32)", asFUNCTION(Player_SetWatchedFactionIndex), asCALL_CDECL);

        // GameObjectData functions
        r = _scriptEngine->RegisterGlobalFunction("void GameObject_SetDisplayID(GameObject@, uint32)", asFUNCTION(GameObject_SetDisplayID), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GameObject_SetFaction(GameObject@, uint32)", asFUNCTION(GameObject_SetFaction), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GameObject_SetLevel(GameObject@, uint32)", asFUNCTION(GameObject_SetLevel), asCALL_CDECL);

        // Legacy string-based API (deprecated)
        r = _scriptEngine->RegisterGlobalFunction("void SetUpdateFieldInt32(Unit@, const string& in, int32)", asFUNCTION(SetUpdateFieldInt32), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void SetUpdateFieldFloat(Unit@, const string& in, float)", asFUNCTION(SetUpdateFieldFloat), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void SetGOUpdateFieldInt32(GameObject@, const string& in, int32)", asFUNCTION(SetGOUpdateFieldInt32), asCALL_CDECL);

        TC_LOG_INFO("server.angelscript", "UpdateField API registered ({} direct field setters)", 17);
    }

} // namespace AngelScript

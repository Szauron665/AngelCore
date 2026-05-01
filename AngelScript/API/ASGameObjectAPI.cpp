/*
 * AngelScript GameObject API
 * Wrapper functions and type registration for GameObject
 */

#ifndef ANGELSCRIPT_INTEGRATION
    #error "ANGELSCRIPT_INTEGRATION macro must be defined"
#endif

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#pragma push_macro("IN")
#pragma push_macro("OUT")
#pragma push_macro("OPTIONAL")
#undef IN
#undef OUT
#undef OPTIONAL

#include <angelscript.h>

#pragma pop_macro("OPTIONAL")
#pragma pop_macro("OUT")
#pragma pop_macro("IN")

#include "GameObject.h"
#include "Player.h"
#include "Log.h"

namespace AngelScript
{
    // ---- GameObject wrapper functions ----
    static std::string GO_GetName(GameObject* go) { return go ? go->GetName() : ""; }
    static uint32 GO_GetEntry(GameObject* go) { return go ? go->GetEntry() : 0; }
    static uint64 GO_GetGUID(GameObject* go) { return go ? go->GetGUID().GetRawValue(0) : 0; }
    static uint32 GO_GetGUIDLow(GameObject* go) { return go ? static_cast<uint32>(go->GetGUID().GetCounter()) : 0; }
    static uint32 GO_GetDisplayId(GameObject* go) { return go ? go->GetDisplayId() : 0; }
    static uint8 GO_GetGoType(GameObject* go) { return go ? static_cast<uint8>(go->GetGoType()) : 0; }
    static bool GO_IsSpawned(GameObject* go) { return go ? go->isSpawned() : false; }
    static bool GO_IsTransport(GameObject* go) { return go ? go->IsTransport() : false; }
    static uint8 GO_GetLootState(GameObject* go) { return go ? static_cast<uint8>(go->getLootState()) : 0; }
    static uint8 GO_GetGoState(GameObject* go) { return go ? static_cast<uint8>(go->GetGoState()) : 0; }
    static void GO_SetGoState(GameObject* go, uint8 state) { if (go) go->SetGoState(static_cast<GOState>(state)); }
    static void GO_SetRespawnTime(GameObject* go, int32 seconds) { if (go) go->SetRespawnTime(seconds); }
    static void GO_Respawn(GameObject* go) { if (go) go->Respawn(); }
    static uint32 GO_GetFaction(GameObject* go) { return go ? go->GetFaction() : 0; }
    static uint32 GO_GetMapId(GameObject* go) { return go ? go->GetMapId() : 0; }
    static float GO_GetPositionX(GameObject* go) { return go ? go->GetPositionX() : 0.f; }
    static float GO_GetPositionY(GameObject* go) { return go ? go->GetPositionY() : 0.f; }
    static float GO_GetPositionZ(GameObject* go) { return go ? go->GetPositionZ() : 0.f; }
    static float GO_GetOrientation(GameObject* go) { return go ? go->GetOrientation() : 0.f; }

    // ---- NEW: Missing critical API wrappers ----
    static bool GO_IsInWorld(GameObject* go) { return go ? go->IsInWorld() : false; }
    static void GO_SetFlag(GameObject* go, uint32 flags) { if (go) go->SetFlag(static_cast<GameObjectFlags>(flags)); }
    static void GO_RemoveFlag(GameObject* go, uint32 flags) { if (go) go->RemoveFlag(static_cast<GameObjectFlags>(flags)); }
    static bool GO_HasFlag(GameObject* go, uint32 flags) { return go ? go->HasFlag(static_cast<GameObjectFlags>(flags)) : false; }
    static void GO_Use(GameObject* go, Player* player) { if (go && player) go->Use(player); }
    static void GO_Delete(GameObject* go) { if (go) go->Delete(); }
    static void GO_Despawn(GameObject* go) { if (go) go->DespawnOrUnsummon(Milliseconds(0)); }
    static float GO_GetDistanceTo(GameObject* go, Unit* target) { return (go && target) ? go->GetDistance(target) : 0.f; }
    static bool GO_IsFriendlyTo(GameObject* go, Unit* target) { return (go && target) ? go->IsFriendlyTo(target) : false; }
    static bool GO_IsHostileTo(GameObject* go, Unit* target) { return (go && target) ? go->IsHostileTo(target) : false; }
    static void GO_SetGoAnimProgress(GameObject* go, uint8 progress) { if (go) go->SetGoAnimProgress(progress); }
    static void GO_Enable(GameObject* go) { if (go) go->EnableCollision(true); }
    static void GO_Disable(GameObject* go) { if (go) go->EnableCollision(false); }

    void RegisterGameObjectAPI(asIScriptEngine* _scriptEngine)
    {
        int r = _scriptEngine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT);
        if (r < 0 && r != asALREADY_REGISTERED)
        {
            TC_LOG_ERROR("angelscript", "Failed to register GameObject type: {}", r);
            return;
        }

        // Basic info
        r = _scriptEngine->RegisterObjectMethod("GameObject", "string GetName() const", asFUNCTION(GO_GetName), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint32 GetEntry() const", asFUNCTION(GO_GetEntry), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint64 GetGUID() const", asFUNCTION(GO_GetGUID), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint32 GetGUIDLow() const", asFUNCTION(GO_GetGUIDLow), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint32 GetDisplayId() const", asFUNCTION(GO_GetDisplayId), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint8 GetGoType() const", asFUNCTION(GO_GetGoType), asCALL_CDECL_OBJFIRST);

        // Status
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool IsSpawned() const", asFUNCTION(GO_IsSpawned), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool IsTransport() const", asFUNCTION(GO_IsTransport), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool IsInWorld() const", asFUNCTION(GO_IsInWorld), asCALL_CDECL_OBJFIRST);

        // State
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint8 GetLootState() const", asFUNCTION(GO_GetLootState), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint8 GetGoState() const", asFUNCTION(GO_GetGoState), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void SetGoState(uint8)", asFUNCTION(GO_SetGoState), asCALL_CDECL_OBJFIRST);

        // Flags
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool HasFlag(uint32) const", asFUNCTION(GO_HasFlag), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void SetFlag(uint32)", asFUNCTION(GO_SetFlag), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void RemoveFlag(uint32)", asFUNCTION(GO_RemoveFlag), asCALL_CDECL_OBJFIRST);

        // Spawn/Despawn
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void SetRespawnTime(int32)", asFUNCTION(GO_SetRespawnTime), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Respawn()", asFUNCTION(GO_Respawn), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Despawn()", asFUNCTION(GO_Despawn), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Delete()", asFUNCTION(GO_Delete), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Enable()", asFUNCTION(GO_Enable), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Disable()", asFUNCTION(GO_Disable), asCALL_CDECL_OBJFIRST);

        // Interaction
        r = _scriptEngine->RegisterObjectMethod("GameObject", "void Use(Player@)", asFUNCTION(GO_Use), asCALL_CDECL_OBJFIRST);

        // Faction
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint32 GetFaction() const", asFUNCTION(GO_GetFaction), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool IsFriendlyTo(Unit@) const", asFUNCTION(GO_IsFriendlyTo), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "bool IsHostileTo(Unit@) const", asFUNCTION(GO_IsHostileTo), asCALL_CDECL_OBJFIRST);

        // Position
        r = _scriptEngine->RegisterObjectMethod("GameObject", "uint32 GetMapId() const", asFUNCTION(GO_GetMapId), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "float GetPositionX() const", asFUNCTION(GO_GetPositionX), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "float GetPositionY() const", asFUNCTION(GO_GetPositionY), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "float GetPositionZ() const", asFUNCTION(GO_GetPositionZ), asCALL_CDECL_OBJFIRST);
        r = _scriptEngine->RegisterObjectMethod("GameObject", "float GetOrientation() const", asFUNCTION(GO_GetOrientation), asCALL_CDECL_OBJFIRST);

        // Distance
        r = _scriptEngine->RegisterObjectMethod("GameObject", "float GetDistanceTo(Unit@) const", asFUNCTION(GO_GetDistanceTo), asCALL_CDECL_OBJFIRST);

        TC_LOG_INFO("server.angelscript", "GameObject API registered ({} methods)", 35);
    }

} // namespace AngelScript

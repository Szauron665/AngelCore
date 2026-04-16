#include "ASBattlegroundHooks.h"
#include "angelscript.h"
#include "Log.h"
#include "Player.h"
#include "Battleground.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASBattlegroundHooks* ASBattlegroundHooks::instance()
    {
        static ASBattlegroundHooks instance;
        return &instance;
    }

    void ASBattlegroundHooks::RegisterScript(BattlegroundHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= static_cast<uint32>(BattlegroundHookType::COUNT))
            return;

        if (_hooks.size() <= hookIndex)
            _hooks.resize(static_cast<size_t>(BattlegroundHookType::COUNT));

        _hooks[hookIndex].push_back(func);
    }

    void ASBattlegroundHooks::RegisterBattlegroundScript(uint32 bgTypeId, ASBattlegroundScript* script)
    {
        if (!script)
            return;

        _battlegroundScripts[bgTypeId] = script;
    }

    void ASBattlegroundHooks::RegisterArenaScript(ArenaHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= static_cast<uint32>(ArenaHookType::COUNT))
            return;

        if (_arenaHooks.size() <= hookIndex)
            _arenaHooks.resize(static_cast<size_t>(ArenaHookType::COUNT));

        _arenaHooks[hookIndex].push_back(func);
    }

    void ASBattlegroundHooks::RegisterArenaScript(uint32 arenaTypeId, ASArenaScript* script)
    {
        if (!script)
            return;

        _arenaScripts[arenaTypeId] = script;
    }

    const std::vector<asIScriptFunction*>& ASBattlegroundHooks::GetHooks(BattlegroundHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= _hooks.size())
            return empty;

        return _hooks[hookIndex];
    }

    const std::vector<asIScriptFunction*>& ASBattlegroundHooks::GetArenaHooks(ArenaHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= _arenaHooks.size())
            return empty;

        return _arenaHooks[hookIndex];
    }

    ASBattlegroundScript* ASBattlegroundHooks::GetBattlegroundScript(uint32 bgTypeId) const
    {
        auto it = _battlegroundScripts.find(bgTypeId);
        if (it != _battlegroundScripts.end())
            return it->second;

        return nullptr;
    }

    ASArenaScript* ASBattlegroundHooks::GetArenaScript(uint32 arenaTypeId) const
    {
        auto it = _arenaScripts.find(arenaTypeId);
        if (it != _arenaScripts.end())
            return it->second;

        return nullptr;
    }

    void ASBattlegroundHooks::Clear()
    {
        _hooks.clear();
        _arenaHooks.clear();
        _battlegroundScripts.clear();
        _arenaScripts.clear();
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

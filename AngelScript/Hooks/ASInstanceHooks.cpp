#include "ASInstanceHooks.h"
#include "angelscript.h"
#include "Log.h"
#include "Player.h"
#include "InstanceScript.h"
#include "Map.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASInstanceHooks* ASInstanceHooks::instance()
    {
        static ASInstanceHooks instance;
        return &instance;
    }

    void ASInstanceHooks::RegisterScript(InstanceHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= static_cast<uint32>(InstanceHookType::COUNT))
            return;

        if (_hooks.size() <= hookIndex)
            _hooks.resize(static_cast<size_t>(InstanceHookType::COUNT));

        _hooks[hookIndex].push_back(func);
    }

    void ASInstanceHooks::RegisterInstanceScript(uint32 mapId, ASInstanceScript* script)
    {
        if (!script)
            return;

        _instanceScripts[mapId].push_back(script);
    }

    const std::vector<asIScriptFunction*>& ASInstanceHooks::GetHooks(InstanceHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        uint32 hookIndex = static_cast<uint32>(type);
        if (hookIndex >= _hooks.size())
            return empty;

        return _hooks[hookIndex];
    }

    std::vector<ASInstanceScript*> ASInstanceHooks::GetInstanceScripts(uint32 mapId) const
    {
        auto it = _instanceScripts.find(mapId);
        if (it != _instanceScripts.end())
            return it->second;

        return {};
    }

    void ASInstanceHooks::Clear()
    {
        _hooks.clear();
        _instanceScripts.clear();
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

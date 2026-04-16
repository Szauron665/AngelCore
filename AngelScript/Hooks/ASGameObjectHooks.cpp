#include "ASGameObjectHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASGameObjectHooks* ASGameObjectHooks::instance()
    {
        static ASGameObjectHooks instance;
        return &instance;
    }

    void ASGameObjectHooks::RegisterScript(GameObjectHookType type, asIScriptFunction* func)
    {
        if (!func) return;
        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(GameObjectHookType::COUNT)) return;
        if (_hooks.empty()) _hooks.resize(static_cast<size_t>(GameObjectHookType::COUNT));
        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASGameObjectHooks::GetHooks(GameObjectHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size()) return empty;
        return _hooks[index];
    }

    void ASGameObjectHooks::Clear() { _hooks.clear(); }
}
#endif

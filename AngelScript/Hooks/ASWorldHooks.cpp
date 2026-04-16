#include "ASWorldHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASWorldHooks* ASWorldHooks::instance()
    {
        static ASWorldHooks instance;
        return &instance;
    }

    void ASWorldHooks::RegisterScript(WorldHookType type, asIScriptFunction* func)
    {
        if (!func) return;
        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(WorldHookType::COUNT)) return;
        if (_hooks.empty()) _hooks.resize(static_cast<size_t>(WorldHookType::COUNT));
        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASWorldHooks::GetHooks(WorldHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size()) return empty;
        return _hooks[index];
    }

    void ASWorldHooks::Clear() { _hooks.clear(); }
}
#endif

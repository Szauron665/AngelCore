#include "ASCreatureHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASCreatureHooks* ASCreatureHooks::instance()
    {
        static ASCreatureHooks instance;
        return &instance;
    }

    void ASCreatureHooks::RegisterScript(CreatureHookType type, asIScriptFunction* func)
    {
        if (!func) return;
        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(CreatureHookType::COUNT)) return;
        if (_hooks.empty()) _hooks.resize(static_cast<size_t>(CreatureHookType::COUNT));
        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASCreatureHooks::GetHooks(CreatureHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size()) return empty;
        return _hooks[index];
    }

    void ASCreatureHooks::Clear() { _hooks.clear(); }
}
#endif

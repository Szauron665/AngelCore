#include "ASPlayerHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASPlayerHooks* ASPlayerHooks::instance()
    {
        static ASPlayerHooks instance;
        return &instance;
    }

    void ASPlayerHooks::RegisterScript(PlayerHookType type, asIScriptFunction* func)
    {
        if (!func) return;
        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(PlayerHookType::COUNT)) return;
        if (_hooks.empty()) _hooks.resize(static_cast<size_t>(PlayerHookType::COUNT));
        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASPlayerHooks::GetHooks(PlayerHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size()) return empty;
        return _hooks[index];
    }

    void ASPlayerHooks::Clear() { _hooks.clear(); }
}
#endif

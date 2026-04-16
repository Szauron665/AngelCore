#include "ASCraftingHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASCraftingHooks* ASCraftingHooks::instance()
    {
        static ASCraftingHooks instance;
        return &instance;
    }

    void ASCraftingHooks::RegisterScript(CraftingHookType type, asIScriptFunction* func)
    {
        if (!func) return;
        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(CraftingHookType::COUNT)) return;
        if (_hooks.empty()) _hooks.resize(static_cast<size_t>(CraftingHookType::COUNT));
        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASCraftingHooks::GetHooks(CraftingHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size()) return empty;
        return _hooks[index];
    }

    void ASCraftingHooks::Clear() { _hooks.clear(); }
}
#endif

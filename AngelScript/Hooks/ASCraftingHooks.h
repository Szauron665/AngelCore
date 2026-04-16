#ifndef ASCRAFTINGHOOKS_H
#define ASCRAFTINGHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include <vector>

class asIScriptFunction;
class Player;

namespace AngelScript
{
    class ASCraftingHooks
    {
    public:
        static ASCraftingHooks* instance();
        void RegisterScript(CraftingHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(CraftingHookType type) const;
        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
    };
}

#endif
#endif

#ifndef ASPLAYERHOOKS_H
#define ASPLAYERHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include <vector>

class asIScriptFunction;
class Player;

namespace AngelScript
{
    class ASPlayerHooks
    {
    public:
        static ASPlayerHooks* instance();
        void RegisterScript(PlayerHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(PlayerHookType type) const;
        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
    };
}

#endif
#endif

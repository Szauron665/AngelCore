#ifndef ASWORLDHOOKS_H
#define ASWORLDHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include <vector>

class asIScriptFunction;

namespace AngelScript
{
    class ASWorldHooks
    {
    public:
        static ASWorldHooks* instance();
        void RegisterScript(WorldHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(WorldHookType type) const;
        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
    };
}

#endif
#endif

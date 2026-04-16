#ifndef ASCREATUREHOOKS_H
#define ASCREATUREHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include <vector>

class asIScriptFunction;
class Creature;
class Player;

namespace AngelScript
{
    class ASCreatureHooks
    {
    public:
        static ASCreatureHooks* instance();
        void RegisterScript(CreatureHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(CreatureHookType type) const;
        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
    };
}

#endif
#endif

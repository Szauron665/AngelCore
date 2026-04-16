#ifndef ASGAMEOBJECTHOOKS_H
#define ASGAMEOBJECTHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include <vector>

class asIScriptFunction;
class GameObject;
class Player;

namespace AngelScript
{
    class ASGameObjectHooks
    {
    public:
        static ASGameObjectHooks* instance();
        void RegisterScript(GameObjectHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(GameObjectHookType type) const;
        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
    };
}

#endif
#endif

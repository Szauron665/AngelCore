#ifndef ASINSTANCEHOOKS_H
#define ASINSTANCEHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include "Define.h"
#include <vector>
#include <map>
#include <string>

// Forward declarations
class asIScriptFunction;
class Player;
class InstanceScript;
class Map;

namespace AngelScript
{
    // Instance Script base class
    class ASInstanceScript
    {
    public:
        ASInstanceScript(const std::string& name) : _name(name) {}
        virtual ~ASInstanceScript() {}

        const std::string& GetScriptName() const { return _name; }

        virtual void OnInitialize() {}
        virtual void OnPlayerEnter(Player* /*player*/) {}
        virtual void OnPlayerLeave(Player* /*player*/) {}
        virtual void OnBossKilled(uint32 /*bossId*/) {}
        virtual void OnReset() {}

    private:
        std::string _name;
    };

    // Instance hooks manager
    class ASInstanceHooks
    {
    public:
        static ASInstanceHooks* instance();

        void RegisterScript(InstanceHookType type, asIScriptFunction* func);
        void RegisterInstanceScript(uint32 mapId, ASInstanceScript* script);
        
        const std::vector<asIScriptFunction*>& GetHooks(InstanceHookType type) const;
        std::vector<ASInstanceScript*> GetInstanceScripts(uint32 mapId) const;

        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
        std::map<uint32, std::vector<ASInstanceScript*>> _instanceScripts;
    };

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASINSTANCEHOOKS_H

#ifndef ASBATTLEGROUNDHOOKS_H
#define ASBATTLEGROUNDHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include "Define.h"
#include <vector>
#include <map>
#include <string>

// Forward declarations
class asIScriptFunction;
class Player;
class Battleground;

namespace AngelScript
{
    // Battleground Script base class
    class ASBattlegroundScript
    {
    public:
        ASBattlegroundScript(const std::string& name) : _name(name) {}
        virtual ~ASBattlegroundScript() {}

        const std::string& GetScriptName() const { return _name; }

        virtual void OnInitialize() {}
        virtual void OnPlayerEnter(Player* /*player*/) {}
        virtual void OnPlayerLeave(Player* /*player*/) {}
        virtual void OnBattlegroundStart() {}
        virtual void OnBattlegroundEnd(uint32 /*winnerTeam*/) {}

    private:
        std::string _name;
    };

    // Arena Script base class
    class ASArenaScript
    {
    public:
        ASArenaScript(const std::string& name) : _name(name) {}
        virtual ~ASArenaScript() {}

        const std::string& GetScriptName() const { return _name; }

        virtual void OnInitialize() {}
        virtual void OnArenaStart() {}
        virtual void OnArenaEnd(uint32 /*winnerTeam*/) {}

    private:
        std::string _name;
    };

    // Battleground hooks manager
    class ASBattlegroundHooks
    {
    public:
        static ASBattlegroundHooks* instance();

        void RegisterScript(BattlegroundHookType type, asIScriptFunction* func);
        void RegisterBattlegroundScript(uint32 bgTypeId, ASBattlegroundScript* script);
        
        void RegisterArenaScript(ArenaHookType type, asIScriptFunction* func);
        void RegisterArenaScript(uint32 arenaTypeId, ASArenaScript* script);

        const std::vector<asIScriptFunction*>& GetHooks(BattlegroundHookType type) const;
        const std::vector<asIScriptFunction*>& GetArenaHooks(ArenaHookType type) const;

        ASBattlegroundScript* GetBattlegroundScript(uint32 bgTypeId) const;
        ASArenaScript* GetArenaScript(uint32 arenaTypeId) const;

        void Clear();
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
        std::vector<std::vector<asIScriptFunction*>> _arenaHooks;
        std::map<uint32, ASBattlegroundScript*> _battlegroundScripts;
        std::map<uint32, ASArenaScript*> _arenaScripts;
    };

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASBATTLEGROUNDHOOKS_H

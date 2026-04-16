#ifndef ASQUESTHOOKS_H
#define ASQUESTHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include "Define.h"
#include "QuestDef.h"
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>

// Forward declarations
class asIScriptFunction;
class Player;
class Quest;
class Creature;
class GameObject;
class Item;
class SpellCastTargets;

namespace AngelScript
{
    // Quest Script base class
    class ASQuestScript
    {
    public:
        ASQuestScript(const std::string& name) : _name(name) {}
        virtual ~ASQuestScript() {}

        const std::string& GetName() const { return _name; }

        // Hooks that can be overridden by scripts
        virtual void OnQuestAccept(Player* /*player*/, Quest const* /*quest*/) {}
        virtual void OnQuestComplete(Player* /*player*/, Quest const* /*quest*/) {}
        virtual void OnQuestReward(Player* /*player*/, Quest const* /*quest*/) {}
        virtual void OnQuestStatusChange(Player* /*player*/, Quest const* /*quest*/, uint32 /*status*/) {}

    private:
        std::string _name;
    };

    // Quest hooks manager
    class ASQuestHooks
    {
    public:
        ASQuestHooks();
        ~ASQuestHooks();

        // Global hooks registration
        void RegisterGlobalHook(QuestHookType type, asIScriptFunction* func);
        
        // Per-quest hooks registration
        void RegisterQuestHook(uint32 questId, QuestHookType type, asIScriptFunction* func);
        
        // Script object registration
        void RegisterQuestScript(uint32 questId, ASQuestScript* script);

        // Hook execution
        const std::vector<asIScriptFunction*>& GetGlobalHooks(QuestHookType type) const;
        const std::vector<asIScriptFunction*>& GetQuestHooks(uint32 questId, QuestHookType type) const;
        ASQuestScript* GetQuestScript(uint32 questId) const;
        
        static ASQuestHooks* instance();
        void RegisterScript(QuestHookType type, asIScriptFunction* func);

        const std::vector<asIScriptFunction*>& GetHooks(QuestHookType type) const;

        void Clear();

    private:
        // Global hooks: [hookType][functions]
        std::vector<std::vector<asIScriptFunction*>> _globalHooks;
        
        // Per-quest hooks: [questId][hookType][functions]
        std::unordered_map<uint32, std::vector<std::vector<asIScriptFunction*>>> _perQuestHooks;
        
        // Per-quest script objects
        std::unordered_map<uint32, ASQuestScript*> _questScripts;
        
        // Empty vector for failed lookups
        std::vector<asIScriptFunction*> _emptyHooks;
    };

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASQUESTHOOKS_H

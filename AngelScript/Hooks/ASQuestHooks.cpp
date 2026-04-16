#include "ASQuestHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASQuestHooks::ASQuestHooks() {}
    ASQuestHooks::~ASQuestHooks() {}

    ASQuestHooks* ASQuestHooks::instance()
    {
        static ASQuestHooks instance;
        return &instance;
    }

    void ASQuestHooks::RegisterScript(QuestHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(QuestHookType::COUNT))
            return;

        if (_globalHooks.empty())
            _globalHooks.resize(static_cast<size_t>(QuestHookType::COUNT));

        _globalHooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASQuestHooks::GetHooks(QuestHookType type) const
    {
        size_t index = static_cast<size_t>(type);
        if (index >= _globalHooks.size())
            return _emptyHooks;
        return _globalHooks[index];
    }

    void ASQuestHooks::Clear()
    {
        _globalHooks.clear();
        _perQuestHooks.clear();
        _questScripts.clear();
    }

    // Helper methods (compatibility)
    void ASQuestHooks::RegisterGlobalHook(QuestHookType type, asIScriptFunction* func) { RegisterScript(type, func); }
    const std::vector<asIScriptFunction*>& ASQuestHooks::GetGlobalHooks(QuestHookType type) const { return GetHooks(type); }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

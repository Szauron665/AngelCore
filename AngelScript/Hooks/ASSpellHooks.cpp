#include "ASSpellHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASSpellHooks* ASSpellHooks::instance()
    {
        static ASSpellHooks instance;
        return &instance;
    }

    void ASSpellHooks::RegisterScript(SpellHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(SpellHookType::COUNT))
            return;

        if (_hooks.empty())
            _hooks.resize(static_cast<size_t>(SpellHookType::COUNT));

        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASSpellHooks::GetHooks(SpellHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size())
            return empty;
        return _hooks[index];
    }

    void ASSpellHooks::RegisterSpellHook(uint32 spellId, SpellHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(SpellHookType::COUNT))
            return;

        // Initialize the spell-specific hook vector if needed
        auto it = _spellHooks.find(spellId);
        if (it == _spellHooks.end())
        {
            _spellHooks[spellId].resize(static_cast<size_t>(SpellHookType::COUNT));
            _scriptedSpellIds.insert(spellId);
        }

        _spellHooks[spellId][index].push_back(func);
    }

    std::vector<asIScriptFunction*> ASSpellHooks::GetSpellHooks(uint32 spellId, SpellHookType type) const
    {
        size_t index = static_cast<size_t>(type);
        auto it = _spellHooks.find(spellId);
        if (it == _spellHooks.end())
            return {};
        
        if (index >= it->second.size())
            return {};
        
        return it->second[index];
    }

    void ASSpellHooks::RegisterEffectHandler(uint32 spellId, uint8 effIndex, asIScriptFunction* func)
    {
        if (!func)
            return;

        uint32 key = spellId * 10 + effIndex;
        _effectHandlers[key].push_back(func);
        _scriptedSpellIds.insert(spellId);
    }

    std::vector<asIScriptFunction*> ASSpellHooks::GetEffectHandlers(uint32 spellId, uint8 effIndex) const
    {
        uint32 key = spellId * 10 + effIndex;
        auto it = _effectHandlers.find(key);
        if (it == _effectHandlers.end())
            return {};
        return it->second;
    }

    bool ASSpellHooks::HasSpellHandlers(uint32 spellId) const
    {
        return _scriptedSpellIds.find(spellId) != _scriptedSpellIds.end();
    }

    std::vector<uint32> ASSpellHooks::GetScriptedSpellIds() const
    {
        return std::vector<uint32>(_scriptedSpellIds.begin(), _scriptedSpellIds.end());
    }

    void ASSpellHooks::Clear()
    {
        _hooks.clear();
        _spellHooks.clear();
        _effectHandlers.clear();
        _scriptedSpellIds.clear();
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

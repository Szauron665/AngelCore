#ifndef ASSPELLHOOKS_H
#define ASSPELLHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include "Define.h"
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

// Forward declarations
class asIScriptFunction;
class Spell;
class Unit;

namespace AngelScript
{
    // Spell effect handle mode (matches SpellEffectHandleMode in Spell.h)
    enum class SpellEffectHandleMode : uint8
    {
        LAUNCH,     // After spell is launched
        LAUNCH_TARGET, // When spell reaches target
        HIT,        // On spell hit
        HIT_TARGET  // On spell effect hit on target
    };
    // Spell-specific handler entry
    struct SpellSpecificHandler
    {
        uint32 spellId;
        SpellHookType hookType;
        asIScriptFunction* handler;
    };

    // Spell hooks manager
    class ASSpellHooks
    {
    public:
        static ASSpellHooks* instance();

        // General hooks (called for all spells)
        void RegisterScript(SpellHookType type, asIScriptFunction* func);
        const std::vector<asIScriptFunction*>& GetHooks(SpellHookType type) const;
        
        // Spell-specific hooks (called only for specific spell ID)
        void RegisterSpellHook(uint32 spellId, SpellHookType type, asIScriptFunction* func);
        std::vector<asIScriptFunction*> GetSpellHooks(uint32 spellId, SpellHookType type) const;
        
        // Spell effect handlers for specific spell IDs
        void RegisterEffectHandler(uint32 spellId, uint8 effIndex, asIScriptFunction* func);
        std::vector<asIScriptFunction*> GetEffectHandlers(uint32 spellId, uint8 effIndex) const;
        
        // Check if spell has any AngelScript handlers
        bool HasSpellHandlers(uint32 spellId) const;
        
        void Clear();
        
        // Get all spell IDs with registered handlers
        std::vector<uint32> GetScriptedSpellIds() const;
        
    private:
        std::vector<std::vector<asIScriptFunction*>> _hooks;
        
        // Spell-specific handlers: Key = spellId, Value = map of hookType -> handlers
        std::unordered_map<uint32, std::vector<std::vector<asIScriptFunction*>>> _spellHooks;
        
        // Spell effect handlers registered by spell ID and effect index
        // Key: spellId * 10 + effIndex, Value: list of handler functions
        std::unordered_map<uint32, std::vector<asIScriptFunction*>> _effectHandlers;
        
        // Set of all spell IDs with handlers (for quick lookup)
        std::unordered_set<uint32> _scriptedSpellIds;
    };

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASSPELLHOOKS_H

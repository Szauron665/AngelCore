#ifndef ASSPELLSCRIPTCLASS_H
#define ASSPELLSCRIPTCLASS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "Define.h"
#include <map>
#include <vector>
#include <memory>

class asIScriptFunction;
class asIScriptObject;

namespace AngelScript
{
    // Spell script class interface for AngelScript
    // This allows registering a class instance that handles multiple hooks
    
    class ASSpellScriptClass
    {
    public:
        ASSpellScriptClass();
        ~ASSpellScriptClass();
        
        // Initialize from AngelScript object
        bool Init(asIScriptObject* scriptObject);
        
        // Hook methods - call the stored AngelScript methods
        bool OnCheckCast(class Spell* spell);
        void OnPrepare(class Spell* spell);
        void OnTakePower(class Spell* spell);
        void OnBeforeCast(class Spell* spell);
        void OnCast(class Spell* spell);
        void OnHit(class Spell* spell, class Unit* target);
        void OnEffectHit(class Spell* spell, class Unit* target, uint8 effIndex);
        void OnDamageCalc(class Spell* spell, class Unit* target, int32& damage);
        void OnHealCalc(class Spell* spell, class Unit* target, int32& healing);
        void OnChannelStart(class Spell* spell);
        void OnChannelUpdate(class Spell* spell);
        void OnChannelEnd(class Spell* spell, bool interrupted);
        bool OnCooldownCheck(class Spell* spell);
        void OnCastTimeCalc(class Spell* spell, uint32& castTime);
        
        bool IsValid() const { return _scriptObject != nullptr; }
        asIScriptObject* GetScriptObject() const { return _scriptObject; }
        
    private:
        asIScriptObject* _scriptObject;  // The AngelScript class instance
        asIScriptFunction* _onCheckCast;
        asIScriptFunction* _onPrepare;
        asIScriptFunction* _onTakePower;
        asIScriptFunction* _onBeforeCast;
        asIScriptFunction* _onCast;
        asIScriptFunction* _onHit;
        asIScriptFunction* _onEffectHit;
        asIScriptFunction* _onDamageCalc;
        asIScriptFunction* _onHealCalc;
        asIScriptFunction* _onChannelStart;
        asIScriptFunction* _onChannelUpdate;
        asIScriptFunction* _onChannelEnd;
        asIScriptFunction* _onCooldownCheck;
        asIScriptFunction* _onCastTimeCalc;
    };

    // Manager for spell script classes (by spell ID)
    class ASSpellScriptClassManager
    {
    public:
        static ASSpellScriptClassManager* instance();
        
        // Register a script class for a spell ID
        void RegisterScript(uint32 spellId, std::unique_ptr<ASSpellScriptClass> script);
        
        // Get script for spell ID
        ASSpellScriptClass* GetScript(uint32 spellId) const;
        
        // Check if spell has a script class
        bool HasScript(uint32 spellId) const;
        
        // Unregister script
        void UnregisterScript(uint32 spellId);
        
        // Clear all
        void Clear();
        
        // Get all scripted spell IDs
        std::vector<uint32> GetScriptedSpellIds() const;
        
    private:
        ASSpellScriptClassManager() = default;
        ~ASSpellScriptClassManager() = default;
        
        std::map<uint32, std::unique_ptr<ASSpellScriptClass>> _scripts;
    };

} // namespace AngelScript

#define sASSpellScriptClassMgr AngelScript::ASSpellScriptClassManager::instance()

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASSPELLSCRIPTCLASS_H

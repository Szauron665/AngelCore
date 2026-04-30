#include "ASSpellScriptClass.h"
#include "angelscript.h"
#include "Spell.h"
#include "Unit.h"
#include "Log.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASSpellScriptClass::ASSpellScriptClass()
        : _scriptObject(nullptr)
        , _onCheckCast(nullptr)
        , _onPrepare(nullptr)
        , _onTakePower(nullptr)
        , _onBeforeCast(nullptr)
        , _onCast(nullptr)
        , _onHit(nullptr)
        , _onEffectHit(nullptr)
        , _onDamageCalc(nullptr)
        , _onHealCalc(nullptr)
        , _onChannelStart(nullptr)
        , _onChannelUpdate(nullptr)
        , _onChannelEnd(nullptr)
        , _onCooldownCheck(nullptr)
        , _onCastTimeCalc(nullptr)
    {
    }

    ASSpellScriptClass::~ASSpellScriptClass()
    {
        // Don't release the script object here - it's managed by AngelScript
    }

    bool ASSpellScriptClass::Init(asIScriptObject* scriptObject)
    {
        _scriptObject = scriptObject;
        if (!_scriptObject)
            return false;

        // Add reference to keep the object alive
        _scriptObject->AddRef();

        // Get the object type
        asITypeInfo* type = _scriptObject->GetObjectType();
        if (!type)
        {
            TC_LOG_ERROR("server.angelscript", "Failed to get object type from spell script class");
            return false;
        }

        // Cache method pointers for performance
        _onCheckCast = type->GetMethodByName("OnCheckCast");
        _onPrepare = type->GetMethodByName("OnPrepare");
        _onTakePower = type->GetMethodByName("OnTakePower");
        _onBeforeCast = type->GetMethodByName("OnBeforeCast");
        _onCast = type->GetMethodByName("OnCast");
        _onHit = type->GetMethodByName("OnHit");
        _onEffectHit = type->GetMethodByName("OnEffectHit");
        _onDamageCalc = type->GetMethodByName("OnDamageCalc");
        _onHealCalc = type->GetMethodByName("OnHealCalc");
        _onChannelStart = type->GetMethodByName("OnChannelStart");
        _onChannelUpdate = type->GetMethodByName("OnChannelUpdate");
        _onChannelEnd = type->GetMethodByName("OnChannelEnd");
        _onCooldownCheck = type->GetMethodByName("OnCooldownCheck");
        _onCastTimeCalc = type->GetMethodByName("OnCastTimeCalc");

        TC_LOG_DEBUG("server.angelscript", "Initialized spell script class with {} methods",
            (_onCheckCast ? 1 : 0) + (_onPrepare ? 1 : 0) + (_onCast ? 1 : 0));

        return true;
    }

    bool ASSpellScriptClass::OnCheckCast(Spell* /*spell*/)
    {
        if (!_onCheckCast || !_scriptObject)
            return true; // Allow cast by default

        // TODO: Execute method with proper context
        // This requires access to AngelScript context from AngelScriptMgr
        // For now, return default
        return true;
    }

    void ASSpellScriptClass::OnPrepare(Spell* /*spell*/)
    {
        if (!_onPrepare || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnTakePower(Spell* /*spell*/)
    {
        if (!_onTakePower || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnBeforeCast(Spell* /*spell*/)
    {
        if (!_onBeforeCast || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnCast(Spell* /*spell*/)
    {
        if (!_onCast || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnHit(Spell* /*spell*/, Unit* /*target*/)
    {
        if (!_onHit || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnEffectHit(Spell* /*spell*/, Unit* /*target*/, uint8 /*effIndex*/)
    {
        if (!_onEffectHit || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnDamageCalc(Spell* /*spell*/, Unit* /*target*/, int32& /*damage*/)
    {
        if (!_onDamageCalc || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnHealCalc(Spell* /*spell*/, Unit* /*target*/, int32& /*healing*/)
    {
        if (!_onHealCalc || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnChannelStart(Spell* /*spell*/)
    {
        if (!_onChannelStart || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnChannelUpdate(Spell* /*spell*/)
    {
        if (!_onChannelUpdate || !_scriptObject)
            return;
        // TODO: Execute
    }

    void ASSpellScriptClass::OnChannelEnd(Spell* /*spell*/, bool /*interrupted*/)
    {
        if (!_onChannelEnd || !_scriptObject)
            return;
        // TODO: Execute
    }

    bool ASSpellScriptClass::OnCooldownCheck(Spell* /*spell*/)
    {
        if (!_onCooldownCheck || !_scriptObject)
            return true;
        // TODO: Execute
        return true;
    }

    void ASSpellScriptClass::OnCastTimeCalc(Spell* /*spell*/, uint32& /*castTime*/)
    {
        if (!_onCastTimeCalc || !_scriptObject)
            return;
        // TODO: Execute
    }

    // Manager implementation
    ASSpellScriptClassManager* ASSpellScriptClassManager::instance()
    {
        static ASSpellScriptClassManager instance;
        return &instance;
    }

    void ASSpellScriptClassManager::RegisterScript(uint32 spellId, std::unique_ptr<ASSpellScriptClass> script)
    {
        if (script && script->IsValid())
        {
            _scripts[spellId] = std::move(script);
            TC_LOG_INFO("server.angelscript", "Registered spell script class for spell {}", spellId);
        }
    }

    ASSpellScriptClass* ASSpellScriptClassManager::GetScript(uint32 spellId) const
    {
        auto it = _scripts.find(spellId);
        if (it == _scripts.end())
            return nullptr;
        return it->second.get();
    }

    bool ASSpellScriptClassManager::HasScript(uint32 spellId) const
    {
        return _scripts.find(spellId) != _scripts.end();
    }

    void ASSpellScriptClassManager::UnregisterScript(uint32 spellId)
    {
        _scripts.erase(spellId);
    }

    void ASSpellScriptClassManager::Clear()
    {
        _scripts.clear();
    }

    std::vector<uint32> ASSpellScriptClassManager::GetScriptedSpellIds() const
    {
        std::vector<uint32> ids;
        ids.reserve(_scripts.size());
        for (const auto& pair : _scripts)
            ids.push_back(pair.first);
        return ids;
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

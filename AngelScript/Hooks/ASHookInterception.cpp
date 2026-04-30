/*
 * AngelScript Hook Interception Implementation
 */

#include "ASHookInterception.h"
#include "Log.h"
#include "Player.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{

// Helper functions for setting hook arguments
template<typename T>
void SetArg(asIScriptContext* ctx, int idx, T* val) { ctx->SetArgObject(idx, val); }
inline void SetArg(asIScriptContext* ctx, int idx, uint32 val) { ctx->SetArgDWord(idx, val); }
inline void SetArg(asIScriptContext* ctx, int idx, int32 val) { ctx->SetArgDWord(idx, static_cast<asDWORD>(val)); }
inline void SetArg(asIScriptContext* ctx, int idx, uint64 val) { ctx->SetArgQWord(idx, val); }
inline void SetArg(asIScriptContext* ctx, int idx, int64 val) { ctx->SetArgQWord(idx, static_cast<asQWORD>(val)); }
inline void SetArg(asIScriptContext* ctx, int idx, float val) { ctx->SetArgFloat(idx, val); }
inline void SetArg(asIScriptContext* ctx, int idx, double val) { ctx->SetArgDouble(idx, val); }
inline void SetArg(asIScriptContext* ctx, int idx, bool val) { ctx->SetArgByte(idx, val ? 1 : 0); }
inline void SetArg(asIScriptContext* ctx, int idx, const std::string& val) { ctx->SetArgObject(idx, const_cast<std::string*>(&val)); }

FunctionHookRegistry* FunctionHookRegistry::instance()
{
    static FunctionHookRegistry instance;
    return &instance;
}

void FunctionHookRegistry::RegisterHook(const std::string& hookName, asIScriptFunction* func)
{
    if (!func)
        return;

    TC_LOG_INFO("server.angelscript", "Registering hook: {}", hookName);
    _hooks[hookName] = func;
}

void FunctionHookRegistry::UnregisterHook(const std::string& hookName)
{
    auto it = _hooks.find(hookName);
    if (it != _hooks.end())
    {
        TC_LOG_INFO("server.angelscript", "Unregistering hook: {}", hookName);
        _hooks.erase(it);
    }
}

bool FunctionHookRegistry::HasHook(const std::string& hookName) const
{
    return _hooks.find(hookName) != _hooks.end();
}

void FunctionHookRegistry::ClearAllHooks()
{
    TC_LOG_INFO("server.angelscript", "Clearing all function hooks");
    _hooks.clear();
}

template<typename Ret, typename... Args>
InterceptResult<Ret> FunctionHookRegistry::ExecuteHook(asIScriptFunction* func, Args... args)
{
    InterceptResult<Ret> result;
    result.action = HookAction::CONTINUE;

    if (!func)
        return result;

    asIScriptEngine* engine = func->GetEngine();
    asIScriptContext* ctx = engine->CreateContext();
    if (!ctx)
        return result;

    int r = ctx->Prepare(func);
    if (r < 0)
    {
        ctx->Release();
        return result;
    }

    // Set arguments using helper functions
    int argIdx = 0;
    (void)std::initializer_list<int>{(SetArg(ctx, argIdx++, args), 0)...};

    r = ctx->Execute();
    if (r != asEXECUTION_FINISHED)
    {
        TC_LOG_ERROR("server.angelscript", "Hook execution failed: {}", r);
        ctx->Release();
        return result;
    }

    // Get return value (should be HookAction enum)
    result.action = static_cast<HookAction>(ctx->GetReturnDWord());

    ctx->Release();
    return result;
}

// Explicit template instantiations
template InterceptResult<bool> FunctionHookRegistry::ExecuteHook<bool, Player*>(asIScriptFunction*, Player*);
template InterceptResult<void> FunctionHookRegistry::ExecuteHook<void, Player*, uint32>(asIScriptFunction*, Player*, uint32);

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

/*
 * AngelScript Hook Interception System
 * Allows AngelScript to intercept and optionally override C++ behavior
 */

#ifndef __ASHOOK_INTERCEPTION_H
#define __ASHOOK_INTERCEPTION_H

#include "Common.h"
#include "angelscript.h"
#include <functional>
#include <unordered_map>
#include <string>

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    // Hook result - tells C++ code what to do
    enum class HookAction : uint8
    {
        CONTINUE,       // Continue with normal C++ processing
        OVERRIDE,       // Skip C++ code, use AngelScript result
        BLOCK,          // Block the action entirely
        MODIFY          // Run C++ code but with modified parameters
    };

    // Intercept result with return value
    template<typename T>
    struct InterceptResult
    {
        HookAction action = HookAction::CONTINUE;
        T returnValue{};  // Value to return if OVERRIDE
    };

    // Template specialization for void
    template<>
    struct InterceptResult<void>
    {
        HookAction action = HookAction::CONTINUE;
    };

    // Function hook registry - manages interceptors
    class FunctionHookRegistry
    {
    public:
        static FunctionHookRegistry* instance();

        // Register a hook by name
        void RegisterHook(const std::string& hookName, asIScriptFunction* func);
        void UnregisterHook(const std::string& hookName);

        // Call hooks and get result
        template<typename Ret, typename... Args>
        InterceptResult<Ret> CallHook(const std::string& hookName, Args... args)
        {
            auto it = _hooks.find(hookName);
            if (it == _hooks.end() || !it->second)
                return { HookAction::CONTINUE };

            return ExecuteHook<Ret>(it->second, args...);
        }

        // Check if hook exists
        bool HasHook(const std::string& hookName) const;

        // Clear all hooks
        void ClearAllHooks();

    private:
        std::unordered_map<std::string, asIScriptFunction*> _hooks;

        template<typename Ret, typename... Args>
        InterceptResult<Ret> ExecuteHook(asIScriptFunction* func, Args... args);
    };

#define sFunctionHookRegistry AngelScript::FunctionHookRegistry::instance()

    // Helper macros for easy hook interception in C++ code
    #define AS_HOOK_WITH_RETURN(retType, hookName, defaultReturn, ...) \
        do { \
            if (sFunctionHookRegistry->HasHook(hookName)) { \
                auto result = sFunctionHookRegistry->CallHook<retType>(hookName, __VA_ARGS__); \
                if (result.action == AngelScript::HookAction::OVERRIDE) \
                    return result.returnValue; \
                if (result.action == AngelScript::HookAction::BLOCK) \
                    return defaultReturn; \
            } \
        } while(0)

    #define AS_HOOK_VOID(hookName, ...) \
        do { \
            if (sFunctionHookRegistry->HasHook(hookName)) { \
                auto result = sFunctionHookRegistry->CallHook<void>(hookName, __VA_ARGS__); \
                if (result.action == AngelScript::HookAction::BLOCK) \
                    return; \
            } \
        } while(0)

    #define AS_HOOK_MODIFY_PARAMS(hookName, modifyFunc, ...) \
        do { \
            if (sFunctionHookRegistry->HasHook(hookName)) { \
                auto result = sFunctionHookRegistry->CallHook<void>(hookName, __VA_ARGS__); \
                if (result.action == AngelScript::HookAction::BLOCK) \
                    return; \
                if (result.action == AngelScript::HookAction::MODIFY) \
                    modifyFunc; \
            } \
        } while(0)

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

#endif // __ASHOOK_INTERCEPTION_H

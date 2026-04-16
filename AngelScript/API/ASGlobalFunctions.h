#ifndef ASGLOBALFUNCTIONS_H
#define ASGLOBALFUNCTIONS_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterGlobalFunctions(asIScriptEngine* engine);
    void RegisterWorldAPI(asIScriptEngine* engine);
    void RegisterMathAPI(asIScriptEngine* engine);
    void RegisterStringAPI(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASGLOBALFUNCTIONS_H

#ifndef ASCREATUREAPI_H
#define ASCREATUREAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterCreatureAPI(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASCREATUREAPI_H

#ifndef ASPACKETAPI_H
#define ASPACKETAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterPacketAPI(asIScriptEngine* engine);
    void RegisterEnhancedPacketAPI(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPACKETAPI_H

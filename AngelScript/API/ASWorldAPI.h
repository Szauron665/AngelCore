/*
 * AngelScript World API
 * Exposes World functions, phase system, and update field access
 */

#ifndef ASWORLDAPI_H
#define ASWORLDAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterWorldAPI(asIScriptEngine* engine);
}

#endif
#endif

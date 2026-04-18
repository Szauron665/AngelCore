/*
 * AngelScript DB2 Data API
 * Exposes TrinityCore's already-loaded DB2 stores to AngelScript
 * No custom loading needed — TC loads all DB2 data at startup
 */

#ifndef ASDB2API_H
#define ASDB2API_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterDB2API(asIScriptEngine* engine);
    void RegisterDynamicDB2API(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASDB2API_H

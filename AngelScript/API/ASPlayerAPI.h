/*
 * AngelScript Player API
 * Extracted from AngelScriptMgr.cpp monolith split
 */

#ifndef ASPLAYERAPI_H
#define ASPLAYERAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterPlayerAPI(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPLAYERAPI_H

#ifndef ASGAMEOBJECTAPI_H
#define ASGAMEOBJECTAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterGameObjectAPI(asIScriptEngine* engine);
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASGAMEOBJECTAPI_H

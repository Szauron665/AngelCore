/*
 * AngelScript UpdateField API
 * Generic update field setter/getter for all object types
 * Allows setting any update field value by name
 */

#ifndef ASUPDATEFIELDAPI_H
#define ASUPDATEFIELDAPI_H

#ifdef ANGELSCRIPT_INTEGRATION

class asIScriptEngine;

namespace AngelScript
{
    void RegisterUpdateFieldAPI(asIScriptEngine* engine);
}

#endif
#endif

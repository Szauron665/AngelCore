/*
 * AngelScript Database API
 * Prepared statement support for Character/World/Login databases
 */

#ifndef ANGELSCRIPT_INTEGRATION
    #error "ANGELSCRIPT_INTEGRATION macro must be defined"
#endif

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#pragma push_macro("IN")
#pragma push_macro("OUT")
#pragma push_macro("OPTIONAL")
#undef IN
#undef OUT
#undef OPTIONAL

#include <angelscript.h>

#pragma pop_macro("OPTIONAL")
#pragma pop_macro("OUT")
#pragma pop_macro("IN")

#include "DatabaseEnv.h"
#include "DatabaseEnvFwd.h"
#include "QueryResult.h"
#include "Log.h"
#include <string>
#include <vector>
#include <algorithm>

namespace AngelScript
{
    // ---- ResultSet wrapper for AngelScript ----
    static uint32 ResultSet_GetRowCount(ResultSet* rs) { return rs ? static_cast<uint32>(rs->GetRowCount()) : 0; }
    static uint32 ResultSet_GetFieldCount(ResultSet* rs) { return rs ? rs->GetFieldCount() : 0; }
    static bool ResultSet_NextRow(ResultSet* rs) { return rs ? rs->NextRow() : false; }
    static std::string ResultSet_GetString(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetString() : ""; }
    static int32 ResultSet_GetInt32(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetInt32() : 0; }
    static uint32 ResultSet_GetUInt32(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetUInt32() : 0; }
    static int64 ResultSet_GetInt64(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetInt64() : 0; }
    static uint64 ResultSet_GetUInt64(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetUInt64() : 0; }
    static float ResultSet_GetFloat(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].GetFloat() : 0.f; }
    static bool ResultSet_IsNull(ResultSet* rs, uint32 idx) { return rs ? (*rs)[idx].IsNull() : true; }

    // ---- Global database query functions ----
    // These execute synchronously — use with caution on the world thread

    // Store query results in a static vector to keep shared_ptr alive
    // AngelScript uses asOBJ_REF | asOBJ_NOCOUNT so we manage lifetime ourselves
    static std::vector<QueryResult> _activeQueryResults;

    static ResultSet* DB_CharacterQuery(const std::string& sql)
    {
        QueryResult result = CharacterDatabase.Query(sql.c_str());
        if (!result) return nullptr;
        ResultSet* raw = result.get();
        _activeQueryResults.push_back(std::move(result));
        return raw;
    }

    static ResultSet* DB_WorldQuery(const std::string& sql)
    {
        QueryResult result = WorldDatabase.Query(sql.c_str());
        if (!result) return nullptr;
        ResultSet* raw = result.get();
        _activeQueryResults.push_back(std::move(result));
        return raw;
    }

    static ResultSet* DB_LoginQuery(const std::string& sql)
    {
        QueryResult result = LoginDatabase.Query(sql.c_str());
        if (!result) return nullptr;
        ResultSet* raw = result.get();
        _activeQueryResults.push_back(std::move(result));
        return raw;
    }

    // Call this periodically or on script unload to free completed query results
    static void DB_ClearStaleResults()
    {
        // Remove results that are no longer referenced (shared_ptr use_count == 1 means only we hold it)
        auto it = std::remove_if(_activeQueryResults.begin(), _activeQueryResults.end(),
            [](QueryResult const& qr) { return qr.use_count() == 1; });
        _activeQueryResults.erase(it, _activeQueryResults.end());
    }

    static std::string DB_EscapeString(const std::string& str)
    {
        std::string escaped = str;
        CharacterDatabase.EscapeString(escaped);
        return escaped;
    }

    static void DB_CharacterExecute(const std::string& sql)
    {
        CharacterDatabase.Execute(sql.c_str());
    }

    static void DB_WorldExecute(const std::string& sql)
    {
        WorldDatabase.Execute(sql.c_str());
    }

    static void DB_LoginExecute(const std::string& sql)
    {
        LoginDatabase.Execute(sql.c_str());
    }

    // Prepared statement support — execute by index
    static void DB_CharacterPreparedStatementExecute(uint32 index)
    {
        auto stmt = CharacterDatabase.GetPreparedStatement(static_cast<CharacterDatabaseStatements>(index));
        if (stmt)
            CharacterDatabase.Execute(stmt);
        else
            TC_LOG_ERROR("server.angelscript", "Invalid prepared statement index: {}", index);
    }

    void RegisterDatabaseAPI(asIScriptEngine* _scriptEngine)
    {
        // Register ResultSet type (shared_ptr managed, no AS ref counting)
        int r = _scriptEngine->RegisterObjectType("QueryResult", 0, asOBJ_REF | asOBJ_NOCOUNT);
        if (r >= 0 || r == asALREADY_REGISTERED)
        {
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "uint32 GetRowCount() const", asFUNCTION(ResultSet_GetRowCount), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "uint32 GetFieldCount() const", asFUNCTION(ResultSet_GetFieldCount), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "bool NextRow()", asFUNCTION(ResultSet_NextRow), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "string GetString(uint32) const", asFUNCTION(ResultSet_GetString), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "int32 GetInt32(uint32) const", asFUNCTION(ResultSet_GetInt32), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "uint32 GetUInt32(uint32) const", asFUNCTION(ResultSet_GetUInt32), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "int64 GetInt64(uint32) const", asFUNCTION(ResultSet_GetInt64), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "uint64 GetUInt64(uint32) const", asFUNCTION(ResultSet_GetUInt64), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "float GetFloat(uint32) const", asFUNCTION(ResultSet_GetFloat), asCALL_CDECL_OBJFIRST);
            r = _scriptEngine->RegisterObjectMethod("QueryResult", "bool IsNull(uint32) const", asFUNCTION(ResultSet_IsNull), asCALL_CDECL_OBJFIRST);
        }

        // Global database functions
        r = _scriptEngine->RegisterGlobalFunction("QueryResult@ CharacterQuery(const string& in)", asFUNCTION(DB_CharacterQuery), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("QueryResult@ WorldQuery(const string& in)", asFUNCTION(DB_WorldQuery), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("QueryResult@ LoginQuery(const string& in)", asFUNCTION(DB_LoginQuery), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void CharacterExecute(const string& in)", asFUNCTION(DB_CharacterExecute), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void WorldExecute(const string& in)", asFUNCTION(DB_WorldExecute), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void LoginExecute(const string& in)", asFUNCTION(DB_LoginExecute), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void CharacterPreparedStatementExecute(uint32)", asFUNCTION(DB_CharacterPreparedStatementExecute), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void ClearStaleQueryResults()", asFUNCTION(DB_ClearStaleResults), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("string DBEscapeString(const string& in)", asFUNCTION(DB_EscapeString), asCALL_CDECL);

        TC_LOG_INFO("server.angelscript", "Database API registered");
    }

} // namespace AngelScript

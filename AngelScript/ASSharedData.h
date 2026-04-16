#ifndef ASSHAREDDATA_H
#define ASSHAREDDATA_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "Define.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

class asIScriptObject;
class asIScriptFunction;

namespace AngelScript
{
    // Shared data entry - can hold any AngelScript-compatible data
    class SharedDataEntry
    {
    public:
        enum Type
        {
            TYPE_NULL,
            TYPE_INT,
            TYPE_FLOAT,
            TYPE_STRING,
            TYPE_DB2FILE,       // Reference to loaded DB2
            TYPE_SCRIPT_OBJECT, // Custom AngelScript class
            TYPE_ARRAY,         // Array of values
            TYPE_MAP            // Key-value map
        };

        SharedDataEntry() : _type(TYPE_NULL), _intValue(0), _floatValue(0.0f) {}
        
        // Setters
        void SetInt(int32 value) { _type = TYPE_INT; _intValue = value; }
        void SetFloat(float value) { _type = TYPE_FLOAT; _floatValue = value; }
        void SetString(const std::string& value) { _type = TYPE_STRING; _stringValue = value; }
        void SetDB2File(uint32 handle) { _type = TYPE_DB2FILE; _intValue = handle; }
        void SetScriptObject(asIScriptObject* obj);
        
        // Getters
        int32 GetInt() const { return _type == TYPE_INT ? _intValue : 0; }
        float GetFloat() const { return _type == TYPE_FLOAT ? _floatValue : 0.0f; }
        std::string GetString() const { return _type == TYPE_STRING ? _stringValue : ""; }
        uint32 GetDB2File() const { return _type == TYPE_DB2FILE ? _intValue : 0; }
        asIScriptObject* GetScriptObject() const { return _scriptObject; }
        
        Type GetType() const { return _type; }
        bool IsValid() const { return _type != TYPE_NULL; }
        
    private:
        Type _type;
        int32 _intValue;
        float _floatValue;
        std::string _stringValue;
        asIScriptObject* _scriptObject;
    };

    // Shared data registry - allows scripts to share data
    class SharedDataRegistry
    {
    public:
        static SharedDataRegistry* instance();
        
        // Set data (from any script)
        void Set(const std::string& key, const SharedDataEntry& value);
        void SetInt(const std::string& key, int32 value);
        void SetFloat(const std::string& key, float value);
        void SetString(const std::string& key, const std::string& value);
        void SetDB2File(const std::string& key, uint32 db2Handle);
        
        // Get data (from any script)
        SharedDataEntry Get(const std::string& key) const;
        int32 GetInt(const std::string& key, int32 defaultValue = 0) const;
        float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
        std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
        uint32 GetDB2File(const std::string& key, uint32 defaultHandle = 0) const;
        
        // Check existence
        bool Has(const std::string& key) const;
        
        // Remove data
        void Remove(const std::string& key);
        void Clear();
        
        // List all keys
        std::vector<std::string> GetKeys() const;
        
        // Namespace support (prefix keys like "db2:items", "config:server")
        std::vector<std::string> GetKeysInNamespace(const std::string& ns) const;
        void ClearNamespace(const std::string& ns);
        
    private:
        SharedDataRegistry() = default;
        ~SharedDataRegistry() = default;
        
        std::map<std::string, SharedDataEntry> _data;
    };

} // namespace AngelScript

#define sASSharedData AngelScript::SharedDataRegistry::instance()

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASSHAREDDATA_H

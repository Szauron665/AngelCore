/*
 * ASDB2Storage.h
 * 
 * Dynamic DB2 storage using TC's DB2Storage infrastructure.
 * Allows loading any DB2 file with a runtime-defined schema.
 */

#ifndef ASDB2STORAGE_H
#define ASDB2STORAGE_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "Define.h"
#include "ASDB2Schema.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Forward declarations
namespace AngelScript {
    class GenericDB2Loader;
}

namespace AngelScript
{
    // Dynamic record that can hold any DB2 data based on schema
    class ASDB2DynamicRecord
    {
    public:
        ASDB2DynamicRecord();
        ~ASDB2DynamicRecord();

        // Set raw data from DB2 storage
        void SetData(const uint8* data, uint32 size, const ASDB2Schema* schema);
        
        // Getters by field name
        int8 GetInt8(const std::string& fieldName) const;
        uint8 GetUInt8(const std::string& fieldName) const;
        int16 GetInt16(const std::string& fieldName) const;
        uint16 GetUInt16(const std::string& fieldName) const;
        int32 GetInt32(const std::string& fieldName) const;
        uint32 GetUInt32(const std::string& fieldName) const;
        int64 GetInt64(const std::string& fieldName) const;
        uint64 GetUInt64(const std::string& fieldName) const;
        float GetFloat(const std::string& fieldName) const;
        double GetDouble(const std::string& fieldName) const;
        std::string GetString(const std::string& fieldName) const;
        bool GetBool(const std::string& fieldName) const;

        // Getters by field index
        int8 GetInt8(uint32 fieldIndex) const;
        uint8 GetUInt8(uint32 fieldIndex) const;
        int16 GetInt16(uint32 fieldIndex) const;
        uint16 GetUInt16(uint32 fieldIndex) const;
        int32 GetInt32(uint32 fieldIndex) const;
        uint32 GetUInt32(uint32 fieldIndex) const;
        int64 GetInt64(uint32 fieldIndex) const;
        uint64 GetUInt64(uint32 fieldIndex) const;
        float GetFloat(uint32 fieldIndex) const;
        double GetDouble(uint32 fieldIndex) const;
        std::string GetString(uint32 fieldIndex) const;
        bool GetBool(uint32 fieldIndex) const;

        // Array field getters (for array fields)
        int32 GetInt32(const std::string& fieldName, uint32 arrayIndex) const;
        uint32 GetUInt32(const std::string& fieldName, uint32 arrayIndex) const;
        float GetFloat(const std::string& fieldName, uint32 arrayIndex) const;

        // Schema access
        const ASDB2Schema* GetSchema() const { return _schema; }
        uint32 GetFieldCount() const;
        bool HasField(const std::string& name) const;

    private:
        const uint8* GetFieldPointer(uint32 fieldIndex, uint32 arrayIndex = 0) const;
        const uint8* GetFieldPointer(const std::string& fieldName, uint32 arrayIndex = 0) const;

        std::vector<uint8> _data;
        const ASDB2Schema* _schema;
    };

    // Dynamic DB2 storage class
    class ASDB2Storage
    {
    public:
        ASDB2Storage(const std::string& name, std::shared_ptr<ASDB2Schema> schema);
        ~ASDB2Storage();

        // Load from file
        bool Load(const std::string& db2Path);
        
        // Load from raw data
        bool LoadFromData(const std::vector<uint8>& data);

        // Query methods
        bool HasRecord(uint32 id) const;
        const ASDB2DynamicRecord* GetRecord(uint32 id) const;
        const ASDB2DynamicRecord* LookupEntry(uint32 id) const { return GetRecord(id); }
        
        // Iteration
        uint32 GetNumRows() const;
        std::vector<uint32> GetAllIds() const;
        
        // Info
        const std::string& GetName() const { return _name; }
        const ASDB2Schema* GetSchema() const { return _schema.get(); }
        bool IsLoaded() const { return _loaded; }

        // Find by field value
        std::vector<uint32> FindRecords(const std::string& fieldName, uint32 value) const;
        std::vector<uint32> FindRecords(const std::string& fieldName, const std::string& value) const;

    private:
        std::string _name;
        std::shared_ptr<ASDB2Schema> _schema;
        std::unique_ptr<GenericDB2Loader> _loader;
        std::unordered_map<uint32, std::unique_ptr<ASDB2DynamicRecord>> _records;
        bool _loaded;
    };

    // Storage registry for managing loaded DB2s
    class ASDB2StorageRegistry
    {
    public:
        static ASDB2StorageRegistry* instance();

        // Create and load a new DB2 storage
        uint32 CreateStorage(const std::string& name, const std::string& filePath, 
                            std::shared_ptr<ASDB2Schema> schema);
        
        // Get storage by handle
        ASDB2Storage* GetStorage(uint32 handle);
        ASDB2Storage* GetStorage(const std::string& name);
        
        // Unload storage
        void RemoveStorage(uint32 handle);
        void RemoveStorage(const std::string& name);
        
        // List storages
        std::vector<std::string> GetStorageNames() const;
        std::vector<uint32> GetStorageHandles() const;

        // Reload storage
        bool ReloadStorage(uint32 handle);
        bool ReloadStorage(const std::string& name);

    private:
        ASDB2StorageRegistry() = default;
        ~ASDB2StorageRegistry() = default;

        std::unordered_map<uint32, std::unique_ptr<ASDB2Storage>> _storages;
        std::unordered_map<std::string, uint32> _nameToHandle;
        uint32 _nextHandle = 1;
    };

} // namespace AngelScript

#define sASDB2StorageRegistry AngelScript::ASDB2StorageRegistry::instance()

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASDB2STORAGE_H

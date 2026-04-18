/*
 * ASDB2Storage.cpp
 * 
 * Simplified implementation using existing GenericDB2Loader infrastructure.
 */

#include "ASDB2Storage.h"
#include "ASGenericDB2Loader.h"
#include "Log.h"
#include <cstring>

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    // ========================================================================
    // ASDB2DynamicRecord Implementation - simplified using raw data access
    // ========================================================================

    ASDB2DynamicRecord::ASDB2DynamicRecord() : _schema(nullptr)
    {
    }

    ASDB2DynamicRecord::~ASDB2DynamicRecord() = default;

    void ASDB2DynamicRecord::SetData(const uint8* data, uint32 size, const ASDB2Schema* schema)
    {
        _schema = schema;
        _data.resize(size);
        if (data && size > 0)
            std::memcpy(_data.data(), data, size);
    }

    const uint8* ASDB2DynamicRecord::GetFieldPointer(uint32 fieldIndex, uint32 arrayIndex) const
    {
        if (!_schema || fieldIndex >= _schema->GetFieldCount())
            return nullptr;

        const ASDB2FieldDef* field = _schema->GetField(fieldIndex);
        if (!field)
            return nullptr;

        if (arrayIndex >= field->ArraySize)
            return nullptr;

        uint32 offset = field->Offset + (arrayIndex * ASDB2Schema::GetFieldTypeSize(field->Type));
        if (offset + ASDB2Schema::GetFieldTypeSize(field->Type) > _data.size())
            return nullptr;

        return &_data[offset];
    }

    const uint8* ASDB2DynamicRecord::GetFieldPointer(const std::string& fieldName, uint32 arrayIndex) const
    {
        if (!_schema)
            return nullptr;

        int32 index = _schema->GetFieldIndex(fieldName);
        if (index < 0)
            return nullptr;

        return GetFieldPointer(static_cast<uint32>(index), arrayIndex);
    }

    // Getter implementations
    int8 ASDB2DynamicRecord::GetInt8(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const int8*>(ptr) : 0;
    }

    uint8 ASDB2DynamicRecord::GetUInt8(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const uint8*>(ptr) : 0;
    }

    int16 ASDB2DynamicRecord::GetInt16(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const int16*>(ptr) : 0;
    }

    uint16 ASDB2DynamicRecord::GetUInt16(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const uint16*>(ptr) : 0;
    }

    int32 ASDB2DynamicRecord::GetInt32(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const int32*>(ptr) : 0;
    }

    uint32 ASDB2DynamicRecord::GetUInt32(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const uint32*>(ptr) : 0;
    }

    int64 ASDB2DynamicRecord::GetInt64(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const int64*>(ptr) : 0;
    }

    uint64 ASDB2DynamicRecord::GetUInt64(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const uint64*>(ptr) : 0;
    }

    float ASDB2DynamicRecord::GetFloat(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const float*>(ptr) : 0.0f;
    }

    double ASDB2DynamicRecord::GetDouble(const std::string& fieldName) const
    {
        const uint8* ptr = GetFieldPointer(fieldName);
        return ptr ? *reinterpret_cast<const double*>(ptr) : 0.0;
    }

    std::string ASDB2DynamicRecord::GetString(const std::string& /*fieldName*/) const
    {
        // String handling requires access to string pool - simplified for now
        return "";
    }

    bool ASDB2DynamicRecord::GetBool(const std::string& fieldName) const
    {
        return GetUInt8(fieldName) != 0;
    }

    // Index-based getters
    int8 ASDB2DynamicRecord::GetInt8(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const int8*>(ptr) : 0;
    }

    uint8 ASDB2DynamicRecord::GetUInt8(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const uint8*>(ptr) : 0;
    }

    int16 ASDB2DynamicRecord::GetInt16(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const int16*>(ptr) : 0;
    }

    uint16 ASDB2DynamicRecord::GetUInt16(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const uint16*>(ptr) : 0;
    }

    int32 ASDB2DynamicRecord::GetInt32(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const int32*>(ptr) : 0;
    }

    uint32 ASDB2DynamicRecord::GetUInt32(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const uint32*>(ptr) : 0;
    }

    int64 ASDB2DynamicRecord::GetInt64(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const int64*>(ptr) : 0;
    }

    uint64 ASDB2DynamicRecord::GetUInt64(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const uint64*>(ptr) : 0;
    }

    float ASDB2DynamicRecord::GetFloat(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const float*>(ptr) : 0.0f;
    }

    double ASDB2DynamicRecord::GetDouble(uint32 fieldIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldIndex);
        return ptr ? *reinterpret_cast<const double*>(ptr) : 0.0;
    }

    std::string ASDB2DynamicRecord::GetString(uint32 /*fieldIndex*/) const
    {
        return "";
    }

    bool ASDB2DynamicRecord::GetBool(uint32 fieldIndex) const
    {
        return GetUInt8(fieldIndex) != 0;
    }

    // Array getters
    int32 ASDB2DynamicRecord::GetInt32(const std::string& fieldName, uint32 arrayIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldName, arrayIndex);
        return ptr ? *reinterpret_cast<const int32*>(ptr) : 0;
    }

    uint32 ASDB2DynamicRecord::GetUInt32(const std::string& fieldName, uint32 arrayIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldName, arrayIndex);
        return ptr ? *reinterpret_cast<const uint32*>(ptr) : 0;
    }

    float ASDB2DynamicRecord::GetFloat(const std::string& fieldName, uint32 arrayIndex) const
    {
        const uint8* ptr = GetFieldPointer(fieldName, arrayIndex);
        return ptr ? *reinterpret_cast<const float*>(ptr) : 0.0f;
    }

    uint32 ASDB2DynamicRecord::GetFieldCount() const
    {
        return _schema ? _schema->GetFieldCount() : 0;
    }

    bool ASDB2DynamicRecord::HasField(const std::string& name) const
    {
        return _schema && _schema->GetFieldIndex(name) >= 0;
    }

    // ========================================================================
    // ASDB2Storage Implementation - simplified using GenericDB2Loader
    // ========================================================================

    ASDB2Storage::ASDB2Storage(const std::string& name, std::shared_ptr<ASDB2Schema> schema)
        : _name(name), _schema(schema), _loader(nullptr), _loaded(false)
    {
    }

    ASDB2Storage::~ASDB2Storage() = default;

    bool ASDB2Storage::Load(const std::string& db2Path)
    {
        if (!_schema)
        {
            TC_LOG_ERROR("angelscript.db2", "Cannot load storage '{}': no schema", _name);
            return false;
        }

        if (!_schema->IsFinalized())
            _schema->Finalize();

        TC_LOG_INFO("angelscript.db2", "Loading DB2 '{}' from '{}'...", _name, db2Path);

        // Use existing GenericDB2Loader for parsing
        _loader = std::make_unique<GenericDB2Loader>();
        
        if (!_loader->LoadFile(db2Path))
        {
            TC_LOG_ERROR("angelscript.db2", "Failed to load DB2 file: {}", db2Path);
            return false;
        }

        // Build records from loader
        uint32 recordCount = _loader->GetRecordCount();
        uint32 minId = 0xFFFFFFFF;
        uint32 maxId = 0;
        uint32 loadedCount = 0;

        for (uint32 i = 0; i < recordCount; ++i)
        {
            const DB2DynamicRecord* srcRecord = _loader->GetRecordByIndex(i);
            if (!srcRecord)
                continue;

            uint32 id = srcRecord->GetId();
            
            // Create our schema-aware record
            auto dstRecord = std::make_unique<ASDB2DynamicRecord>();
            dstRecord->SetData(srcRecord->GetRawData(), srcRecord->GetSize(), _schema.get());
            _records[id] = std::move(dstRecord);

            if (id < minId) minId = id;
            if (id > maxId) maxId = id;
            loadedCount++;
        }

        _loaded = true;
        TC_LOG_INFO("angelscript.db2", "Loaded DB2 '{}': {} records (ID range: {}-{})", 
            _name, loadedCount, minId == 0xFFFFFFFF ? 0 : minId, maxId);
        return true;
    }

    bool ASDB2Storage::HasRecord(uint32 id) const
    {
        return _records.find(id) != _records.end();
    }

    const ASDB2DynamicRecord* ASDB2Storage::GetRecord(uint32 id) const
    {
        auto it = _records.find(id);
        if (it == _records.end())
            return nullptr;
        return it->second.get();
    }

    uint32 ASDB2Storage::GetNumRows() const
    {
        return static_cast<uint32>(_records.size());
    }

    std::vector<uint32> ASDB2Storage::GetAllIds() const
    {
        std::vector<uint32> ids;
        for (const auto& pair : _records)
            ids.push_back(pair.first);
        return ids;
    }

    std::vector<uint32> ASDB2Storage::FindRecords(const std::string& fieldName, uint32 value) const
    {
        std::vector<uint32> results;
        
        if (!_schema || _schema->GetFieldIndex(fieldName) < 0)
            return results;

        for (const auto& pair : _records)
        {
            if (pair.second->GetUInt32(fieldName) == value)
                results.push_back(pair.first);
        }
        return results;
    }

    std::vector<uint32> ASDB2Storage::FindRecords(const std::string& /*fieldName*/, const std::string& /*value*/) const
    {
        std::vector<uint32> results;
        // String comparison would need proper implementation
        return results;
    }

    // ========================================================================
    // ASDB2StorageRegistry Implementation
    // ========================================================================

    ASDB2StorageRegistry* ASDB2StorageRegistry::instance()
    {
        static ASDB2StorageRegistry instance;
        return &instance;
    }

    uint32 ASDB2StorageRegistry::CreateStorage(const std::string& name, const std::string& filePath,
                                              std::shared_ptr<ASDB2Schema> schema)
    {
        if (_nameToHandle.find(name) != _nameToHandle.end())
        {
            TC_LOG_WARN("angelscript.db2", "Storage '{}' already exists, returning existing handle", name);
            return _nameToHandle[name];
        }

        uint32 handle = _nextHandle++;
        auto storage = std::make_unique<ASDB2Storage>(name, schema);
        
        if (!storage->Load(filePath))
        {
            TC_LOG_ERROR("angelscript.db2", "Failed to create storage '{}' from '{}'", name, filePath);
            return 0;
        }

        _storages[handle] = std::move(storage);
        _nameToHandle[name] = handle;

        TC_LOG_INFO("angelscript.db2", "Created DB2 storage '{}' with handle {}", name, handle);
        return handle;
    }

    ASDB2Storage* ASDB2StorageRegistry::GetStorage(uint32 handle)
    {
        auto it = _storages.find(handle);
        if (it == _storages.end())
            return nullptr;
        return it->second.get();
    }

    ASDB2Storage* ASDB2StorageRegistry::GetStorage(const std::string& name)
    {
        auto it = _nameToHandle.find(name);
        if (it == _nameToHandle.end())
            return nullptr;
        return GetStorage(it->second);
    }

    void ASDB2StorageRegistry::RemoveStorage(uint32 handle)
    {
        auto it = _storages.find(handle);
        if (it != _storages.end())
        {
            _nameToHandle.erase(it->second->GetName());
            _storages.erase(it);
        }
    }

    void ASDB2StorageRegistry::RemoveStorage(const std::string& name)
    {
        auto it = _nameToHandle.find(name);
        if (it != _nameToHandle.end())
            RemoveStorage(it->second);
    }

    std::vector<std::string> ASDB2StorageRegistry::GetStorageNames() const
    {
        std::vector<std::string> names;
        for (const auto& pair : _nameToHandle)
            names.push_back(pair.first);
        return names;
    }

    std::vector<uint32> ASDB2StorageRegistry::GetStorageHandles() const
    {
        std::vector<uint32> handles;
        for (const auto& pair : _storages)
            handles.push_back(pair.first);
        return handles;
    }

    bool ASDB2StorageRegistry::ReloadStorage(uint32 handle)
    {
        ASDB2Storage* storage = GetStorage(handle);
        if (!storage)
            return false;

        // For reload, we would need to store file path and schema
        // For now, just remove and require re-creation
        RemoveStorage(handle);
        return false;
    }

    bool ASDB2StorageRegistry::ReloadStorage(const std::string& name)
    {
        auto it = _nameToHandle.find(name);
        if (it == _nameToHandle.end())
            return false;
        return ReloadStorage(it->second);
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

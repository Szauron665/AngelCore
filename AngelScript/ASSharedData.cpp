#include "ASSharedData.h"
#include "Log.h"
#include <algorithm>

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    SharedDataRegistry* SharedDataRegistry::instance()
    {
        static SharedDataRegistry instance;
        return &instance;
    }

    void SharedDataRegistry::Set(const std::string& key, const SharedDataEntry& value)
    {
        _data[key] = value;
        TC_LOG_DEBUG("server.angelscript", "SharedData: Set '{}'", key);
    }

    void SharedDataRegistry::SetInt(const std::string& key, int32 value)
    {
        SharedDataEntry entry;
        entry.SetInt(value);
        Set(key, entry);
    }

    void SharedDataRegistry::SetFloat(const std::string& key, float value)
    {
        SharedDataEntry entry;
        entry.SetFloat(value);
        Set(key, entry);
    }

    void SharedDataRegistry::SetString(const std::string& key, const std::string& value)
    {
        SharedDataEntry entry;
        entry.SetString(value);
        Set(key, entry);
    }

    void SharedDataRegistry::SetDB2File(const std::string& key, uint32 db2Handle)
    {
        SharedDataEntry entry;
        entry.SetDB2File(db2Handle);
        Set(key, entry);
    }

    SharedDataEntry SharedDataRegistry::Get(const std::string& key) const
    {
        auto it = _data.find(key);
        if (it != _data.end())
            return it->second;
        return SharedDataEntry(); // Null entry
    }

    int32 SharedDataRegistry::GetInt(const std::string& key, int32 defaultValue) const
    {
        auto it = _data.find(key);
        if (it != _data.end())
            return it->second.GetInt();
        return defaultValue;
    }

    float SharedDataRegistry::GetFloat(const std::string& key, float defaultValue) const
    {
        auto it = _data.find(key);
        if (it != _data.end())
            return it->second.GetFloat();
        return defaultValue;
    }

    std::string SharedDataRegistry::GetString(const std::string& key, const std::string& defaultValue) const
    {
        auto it = _data.find(key);
        if (it != _data.end())
            return it->second.GetString();
        return defaultValue;
    }

    uint32 SharedDataRegistry::GetDB2File(const std::string& key, uint32 defaultHandle) const
    {
        auto it = _data.find(key);
        if (it != _data.end())
            return it->second.GetDB2File();
        return defaultHandle;
    }

    bool SharedDataRegistry::Has(const std::string& key) const
    {
        return _data.find(key) != _data.end();
    }

    void SharedDataRegistry::Remove(const std::string& key)
    {
        _data.erase(key);
        TC_LOG_DEBUG("server.angelscript", "SharedData: Removed '{}'", key);
    }

    void SharedDataRegistry::Clear()
    {
        _data.clear();
        TC_LOG_INFO("server.angelscript", "SharedData: Cleared all data");
    }

    std::vector<std::string> SharedDataRegistry::GetKeys() const
    {
        std::vector<std::string> keys;
        keys.reserve(_data.size());
        for (const auto& pair : _data)
            keys.push_back(pair.first);
        return keys;
    }

    std::vector<std::string> SharedDataRegistry::GetKeysInNamespace(const std::string& ns) const
    {
        std::vector<std::string> keys;
        std::string prefix = ns + ":";
        
        for (const auto& pair : _data)
        {
            if (pair.first.find(prefix) == 0)
                keys.push_back(pair.first);
        }
        return keys;
    }

    void SharedDataRegistry::ClearNamespace(const std::string& ns)
    {
        std::string prefix = ns + ":";
        
        for (auto it = _data.begin(); it != _data.end();)
        {
            if (it->first.find(prefix) == 0)
            {
                TC_LOG_DEBUG("server.angelscript", "SharedData: Removed '{}'", it->first);
                it = _data.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

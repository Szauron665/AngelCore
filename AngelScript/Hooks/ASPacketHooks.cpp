#include "ASPacketHooks.h"
#include "angelscript.h"

#ifdef ANGELSCRIPT_INTEGRATION

namespace AngelScript
{
    ASPacketHooks* ASPacketHooks::instance()
    {
        static ASPacketHooks instance;
        return &instance;
    }

    void ASPacketHooks::RegisterScript(PacketHookType type, asIScriptFunction* func)
    {
        if (!func)
            return;

        size_t index = static_cast<size_t>(type);
        if (index >= static_cast<size_t>(PacketHookType::COUNT))
            return;

        if (_hooks.empty())
            _hooks.resize(static_cast<size_t>(PacketHookType::COUNT));

        _hooks[index].push_back(func);
    }

    const std::vector<asIScriptFunction*>& ASPacketHooks::GetHooks(PacketHookType type) const
    {
        static std::vector<asIScriptFunction*> empty;
        size_t index = static_cast<size_t>(type);
        if (index >= _hooks.size())
            return empty;
        return _hooks[index];
    }

    void ASPacketHooks::Clear()
    {
        _hooks.clear();
        _opcodeHandlers.clear();
    }

    // Opcode-specific handlers
    void ASPacketHooks::RegisterOpcodeHandler(uint32 opcode, asIScriptFunction* func, bool blockOriginal)
    {
        if (!func)
            return;

        OpcodeHandlerEntry entry;
        entry.opcode = opcode;
        entry.handler = func;
        entry.blockOriginal = blockOriginal;
        _opcodeHandlers[opcode] = entry;
    }

    void ASPacketHooks::UnregisterOpcodeHandler(uint32 opcode)
    {
        auto it = _opcodeHandlers.find(opcode);
        if (it != _opcodeHandlers.end())
            _opcodeHandlers.erase(it);
    }

    asIScriptFunction* ASPacketHooks::GetOpcodeHandler(uint32 opcode) const
    {
        auto it = _opcodeHandlers.find(opcode);
        if (it != _opcodeHandlers.end())
            return it->second.handler;
        return nullptr;
    }

    bool ASPacketHooks::ShouldBlockOriginal(uint32 opcode) const
    {
        auto it = _opcodeHandlers.find(opcode);
        if (it != _opcodeHandlers.end())
            return it->second.blockOriginal;
        return false;
    }

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION

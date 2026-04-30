#ifndef ASPACKETHOOKS_H
#define ASPACKETHOOKS_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ASHookTypes.h"
#include "Define.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include <vector>
#include <cstdint>
#include <map>
#include <string>
#include <functional>

// Forward declarations
class asIScriptFunction;
class WorldSession;
class ByteBuffer;
class Player;

namespace AngelScript
{
    // Packet hooks manager
    class ASPacketHooks
    {
    public:
        static ASPacketHooks* instance();

        void RegisterScript(PacketHookType type, asIScriptFunction* func);
        void RegisterOpcodeHandler(uint32 opcode, asIScriptFunction* func, bool blockOriginal = false);
        void UnregisterOpcodeHandler(uint32 opcode);
        
        const std::vector<asIScriptFunction*>& GetHooks(PacketHookType type) const;
        asIScriptFunction* GetOpcodeHandler(uint32 opcode) const;
        bool ShouldBlockOriginal(uint32 opcode) const;
        
        void Clear();
        
    private:
        struct OpcodeHandlerEntry
        {
            uint32 opcode;
            asIScriptFunction* handler;
            bool blockOriginal;
        };

        std::vector<std::vector<asIScriptFunction*>> _hooks;
        std::map<uint32, OpcodeHandlerEntry> _opcodeHandlers;
    };

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPACKETHOOKS_H

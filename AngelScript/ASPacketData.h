#ifndef ASPACKETDATA_H
#define ASPACKETDATA_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "Define.h"
#include <vector>
#include <string>

// Packet data structure for AngelScript
// Allows scripts to read/write raw packet data for handling unimplemented opcodes
struct PacketData
{
    uint32 opcode;
    uint32 size;
    std::vector<uint8> data;
    size_t _readPos = 0;
    size_t _writePos = 0;
    
    // Read methods
    uint8 ReadUInt8();
    uint16 ReadUInt16();
    uint32 ReadUInt32();
    uint64 ReadUInt64();
    int8 ReadInt8();
    int16 ReadInt16();
    int32 ReadInt32();
    int64 ReadInt64();
    float ReadFloat();
    double ReadDouble();
    std::string ReadString();
    std::string ReadCString();
    std::string ReadBytes(uint32 length);
    std::string ReadWoWString(uint32 length);
    uint64 ReadPackedUInt64();
    void ReadPackedGuid(uint64& low, uint64& high);
    
    // Write methods
    void WriteUInt8(uint8 val);
    void WriteUInt16(uint16 val);
    void WriteUInt32(uint32 val);
    void WriteUInt64(uint64 val);
    void WriteInt8(int8 val);
    void WriteInt16(int16 val);
    void WriteInt32(int32 val);
    void WriteInt64(int64 val);
    void WriteFloat(float val);
    void WriteDouble(double val);
    void WriteString(const std::string& val);
    void WriteCString(const std::string& val);
    void WritePackedGuid(uint64 low, uint64 high);
    
    // Position management
    void ResetReadPos();
    void SetReadPos(size_t pos);
    void ResetWritePos();
    size_t GetReadPos() const;
    size_t GetWritePos() const;
    bool HasMoreData() const;
    
    // Bit operations (for packed boolean fields in packets)
    bool ReadBit();
    uint32 ReadBits(uint32 bitCount);
    void WriteBit(bool bit);
    void WriteBits(uint32 value, uint32 bitCount);
    void FlushBits();
    void ResetBitReader();

    // TrinityCore ByteBuffer-compatible bit state.
    // _bitpos == 8 (InitialBitPos) means: no active bit buffer.
    static constexpr uint8 InitialBitPos = 8;
    uint8 _bitpos = InitialBitPos;
    uint8 _curbitval = 0;
};

// -----------------------------------------------------------------------------
// Bit ops — direct port of src/server/shared/Packets/ByteBuffer.h
// -----------------------------------------------------------------------------

inline bool PacketData::ReadBit()
{
    if (_bitpos >= 8)
    {
        if (_readPos >= data.size())
            return false;
        _curbitval = data[_readPos++];
        _bitpos = 0;
    }
    return ((_curbitval >> (8 - ++_bitpos)) & 1) != 0;
}

inline uint32 PacketData::ReadBits(uint32 bitCount)
{
    int32 bits = static_cast<int32>(bitCount);
    uint32 value = 0;

    if (bits > 8 - static_cast<int32>(_bitpos))
    {
        // whatever is left in the bit buffer
        int32 bitsInBuffer = 8 - static_cast<int32>(_bitpos);
        value = (_curbitval & ((1u << bitsInBuffer) - 1)) << (bits - bitsInBuffer);
        bits -= bitsInBuffer;

        while (bits >= 8)
        {
            if (_readPos >= data.size()) return value;
            uint8 b = data[_readPos++];
            bits -= 8;
            value |= static_cast<uint32>(b) << bits;
        }

        if (bits)
        {
            if (_readPos >= data.size()) return value;
            _curbitval = data[_readPos++];
            value |= (_curbitval >> (8 - bits)) & ((1u << bits) - 1);
            _bitpos = static_cast<uint8>(bits);
        }
        else
        {
            _bitpos = 8;
        }
    }
    else
    {
        value = (_curbitval >> (8 - _bitpos - bits)) & ((1u << bits) - 1);
        _bitpos += static_cast<uint8>(bits);
    }

    return value;
}

inline void PacketData::WriteBit(bool bit)
{
    if (_bitpos == InitialBitPos)
        _bitpos = 8;
    --_bitpos;
    if (bit)
        _curbitval |= (1u << _bitpos);

    if (_bitpos == 0)
    {
        data.push_back(_curbitval);
        _bitpos = InitialBitPos;
        _curbitval = 0;
    }
}

inline void PacketData::WriteBits(uint32 value, uint32 bitCount)
{
    int32 bits = static_cast<int32>(bitCount);
    value &= (1u << bits) - 1;

    if (_bitpos == InitialBitPos)
        _bitpos = 8;

    if (bits > static_cast<int32>(_bitpos))
    {
        _curbitval |= static_cast<uint8>(value >> (bits - _bitpos));
        bits -= _bitpos;
        _bitpos = 8;
        data.push_back(_curbitval);
        _curbitval = 0;

        while (bits >= 8)
        {
            bits -= 8;
            data.push_back(static_cast<uint8>((value >> bits) & 0xFF));
        }

        _bitpos = static_cast<uint8>(8 - bits);
        _curbitval = static_cast<uint8>((value & ((1u << bits) - 1)) << _bitpos);
    }
    else
    {
        _bitpos -= static_cast<uint8>(bits);
        _curbitval |= static_cast<uint8>(value << _bitpos);

        if (_bitpos == 0)
        {
            data.push_back(_curbitval);
            _bitpos = InitialBitPos;
            _curbitval = 0;
        }
    }
}

inline void PacketData::FlushBits()
{
    // Matches ByteBuffer::FlushBits. Emits the in-progress write byte if any.
    if (_bitpos == 8 || _bitpos == InitialBitPos)
    {
        _bitpos = InitialBitPos;
        _curbitval = 0;
        return;
    }
    data.push_back(_curbitval);
    _bitpos = InitialBitPos;
    _curbitval = 0;
}

inline void PacketData::ResetBitReader()
{
    // Matches ByteBuffer::ResetBitPos — discard partial bit buffer.
    _bitpos = InitialBitPos;
    _curbitval = 0;
}

// String read - fixed length bytes
inline std::string PacketData::ReadBytes(uint32 length)
{
    ResetBitReader(); // Ensure byte alignment
    if (_readPos + length > data.size()) return "";
    std::string s((char*)&data[_readPos], length);
    _readPos += length;
    return s;
}

// WoW string read - fixed length bytes with null bytes and invalid UTF-8 bytes stripped.
// Matches C# parser: Encoding.UTF8.GetString(bytes.Where(b => b != 0).ToArray())
inline std::string PacketData::ReadWoWString(uint32 length)
{
    ResetBitReader();
    if (_readPos + length > data.size()) return "";
    std::string raw;
    raw.reserve(length);
    for (uint32 i = 0; i < length; ++i)
    {
        uint8 b = data[_readPos++];
        if (b != 0)
            raw += static_cast<char>(b);
    }
    // Strip lone UTF-8 continuation bytes (0x80-0xBF) that have no valid leading byte,
    // which MySQL utf8mb4 rejects with errno 1366.
    std::string s;
    s.reserve(raw.size());
    for (size_t i = 0; i < raw.size(); )
    {
        uint8 b = static_cast<uint8>(raw[i]);
        if (b < 0x80)           // ASCII
            { s += raw[i++]; }
        else if (b < 0xC2)      // lone continuation or overlong — skip
            { ++i; }
        else if (b < 0xE0)      // 2-byte sequence
        {
            if (i + 1 < raw.size() && (static_cast<uint8>(raw[i+1]) & 0xC0) == 0x80)
                { s += raw[i]; s += raw[i+1]; i += 2; }
            else
                { ++i; }
        }
        else if (b < 0xF0)      // 3-byte sequence
        {
            if (i + 2 < raw.size() && (static_cast<uint8>(raw[i+1]) & 0xC0) == 0x80
                                   && (static_cast<uint8>(raw[i+2]) & 0xC0) == 0x80)
                { s += raw[i]; s += raw[i+1]; s += raw[i+2]; i += 3; }
            else
                { ++i; }
        }
        else if (b < 0xF5)      // 4-byte sequence
        {
            if (i + 3 < raw.size() && (static_cast<uint8>(raw[i+1]) & 0xC0) == 0x80
                                   && (static_cast<uint8>(raw[i+2]) & 0xC0) == 0x80
                                   && (static_cast<uint8>(raw[i+3]) & 0xC0) == 0x80)
                { s += raw[i]; s += raw[i+1]; s += raw[i+2]; s += raw[i+3]; i += 4; }
            else
                { ++i; }
        }
        else                    // > 0xF4 — invalid
            { ++i; }
    }
    return s;
}

// Packed GUID reading (WoW format: mask byte + non-zero bytes)
inline uint64 PacketData::ReadPackedUInt64()
{
    ResetBitReader(); // Ensure byte alignment
    uint8 mask = ReadUInt8();
    uint64 value = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        if (mask & (1 << i))
        {
            if (_readPos < data.size())
            {
                uint64 byteVal = data[_readPos++];
                value |= (byteVal << (i * 8));
            }
        }
    }
    return value;
}

inline void PacketData::ReadPackedGuid(uint64& low, uint64& high)
{
    // WoW ReadPackedGuid128: read low-mask byte, high-mask byte, then non-zero bytes for all 16 slots
    ResetBitReader();
    uint8 maskLow  = ReadUInt8();
    uint8 maskHigh = ReadUInt8();
    low = 0; high = 0;
    for (uint8 i = 0; i < 8; ++i)
        if (maskLow & (1u << i))
            low |= (static_cast<uint64>(ReadUInt8()) << (i * 8));
    for (uint8 i = 0; i < 8; ++i)
        if (maskHigh & (1u << i))
            high |= (static_cast<uint64>(ReadUInt8()) << (i * 8));
}

inline void PacketData::WritePackedGuid(uint64 low, uint64 high)
{
    FlushBits(); // Ensure byte alignment

    // WoW PackedGuid128 format: maskLow, maskHigh, then low non-zero bytes, then high non-zero bytes
    uint8 maskLow = 0, maskHigh = 0;
    uint8 bytesLow[8], bytesHigh[8];
    uint8 countLow = 0, countHigh = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        uint8 b = static_cast<uint8>(low >> (i * 8));
        if (b) { maskLow |= (1u << i); bytesLow[countLow++] = b; }
    }
    for (uint8 i = 0; i < 8; ++i)
    {
        uint8 b = static_cast<uint8>(high >> (i * 8));
        if (b) { maskHigh |= (1u << i); bytesHigh[countHigh++] = b; }
    }
    WriteUInt8(maskLow);
    WriteUInt8(maskHigh);
    for (uint8 i = 0; i < countLow; ++i) WriteUInt8(bytesLow[i]);
    for (uint8 i = 0; i < countHigh; ++i) WriteUInt8(bytesHigh[i]);
}

// Standard read methods with byte alignment
inline uint8 PacketData::ReadUInt8()
{
    ResetBitReader();
    if (_readPos >= data.size()) return 0;
    return data[_readPos++];
}

inline uint16 PacketData::ReadUInt16()
{
    ResetBitReader();
    if (_readPos + 2 > data.size()) return 0;
    uint16 val = *(uint16*)&data[_readPos];
    _readPos += 2;
    return val;
}

inline uint32 PacketData::ReadUInt32()
{
    ResetBitReader();
    if (_readPos + 4 > data.size()) return 0;
    uint32 val = *(uint32*)&data[_readPos];
    _readPos += 4;
    return val;
}

inline uint64 PacketData::ReadUInt64()
{
    ResetBitReader();
    if (_readPos + 8 > data.size()) return 0;
    uint64 val = *(uint64*)&data[_readPos];
    _readPos += 8;
    return val;
}

inline int8 PacketData::ReadInt8() { return static_cast<int8>(ReadUInt8()); }
inline int16 PacketData::ReadInt16() { return static_cast<int16>(ReadUInt16()); }
inline int32 PacketData::ReadInt32() { return static_cast<int32>(ReadUInt32()); }
inline int64 PacketData::ReadInt64() { return static_cast<int64>(ReadUInt64()); }

inline float PacketData::ReadFloat()
{
    ResetBitReader();
    if (_readPos + 4 > data.size()) return 0.0f;
    float val = *(float*)&data[_readPos];
    _readPos += 4;
    return val;
}

inline double PacketData::ReadDouble()
{
    ResetBitReader();
    if (_readPos + 8 > data.size()) return 0.0;
    double val = *(double*)&data[_readPos];
    _readPos += 8;
    return val;
}

// Write methods with byte alignment
inline void PacketData::WriteUInt8(uint8 val)
{
    FlushBits();
    data.push_back(val);
}

inline void PacketData::WriteUInt16(uint16 val)
{
    FlushBits();
    data.push_back(val & 0xFF);
    data.push_back((val >> 8) & 0xFF);
}

inline void PacketData::WriteUInt32(uint32 val)
{
    FlushBits();
    data.push_back(val & 0xFF);
    data.push_back((val >> 8) & 0xFF);
    data.push_back((val >> 16) & 0xFF);
    data.push_back((val >> 24) & 0xFF);
}

inline void PacketData::WriteUInt64(uint64 val)
{
    FlushBits();
    for (int i = 0; i < 8; ++i)
        data.push_back((val >> (i * 8)) & 0xFF);
}

inline void PacketData::WriteInt8(int8 val) { WriteUInt8(static_cast<uint8>(val)); }
inline void PacketData::WriteInt16(int16 val) { WriteUInt16(static_cast<uint16>(val)); }
inline void PacketData::WriteInt32(int32 val) { WriteUInt32(static_cast<uint32>(val)); }
inline void PacketData::WriteInt64(int64 val) { WriteUInt64(static_cast<uint64>(val)); }

inline void PacketData::WriteFloat(float val)
{
    FlushBits();
    union { float f; uint32 i; } u;
    u.f = val;
    WriteUInt32(u.i);
}

inline void PacketData::WriteDouble(double val)
{
    FlushBits();
    union { double d; uint64 i; } u;
    u.d = val;
    WriteUInt64(u.i);
}

// String methods
inline std::string PacketData::ReadString()
{
    ResetBitReader();
    std::string result;
    while (_readPos < data.size())
    {
        char c = static_cast<char>(data[_readPos++]);
        if (c == '\0')
            break;
        result += c;
    }
    return result;
}

inline std::string PacketData::ReadCString()
{
    return ReadString(); // Same as ReadString - null-terminated
}

inline void PacketData::WriteString(const std::string& val)
{
    FlushBits();
    for (char c : val)
        data.push_back(static_cast<uint8>(c));
    data.push_back(0); // Null terminator
}

inline void PacketData::WriteCString(const std::string& val)
{
    WriteString(val);
}

// Position management
inline void PacketData::ResetReadPos()
{
    _readPos = 0;
    _bitpos = InitialBitPos;
    _curbitval = 0;
}

inline void PacketData::SetReadPos(size_t pos)
{
    _readPos = (pos <= data.size()) ? pos : data.size();
    _bitpos = InitialBitPos;
    _curbitval = 0;
}

inline void PacketData::ResetWritePos()
{
    _writePos = 0;
    _bitpos = InitialBitPos;
    _curbitval = 0;
}

inline size_t PacketData::GetReadPos() const
{
    return _readPos;
}

inline size_t PacketData::GetWritePos() const
{
    return _writePos;
}

inline bool PacketData::HasMoreData() const
{
    return _readPos < data.size();
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPACKETDATA_H

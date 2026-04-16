/*
 * AngelScript PacketData Implementation
 */

#include "ASPacketData.h"
#include "ByteBuffer.h"
#include <cstring>

#ifdef ANGELSCRIPT_INTEGRATION

// Helper to check if we can read N bytes
static bool CanRead(const PacketData* pd, size_t bytes)
{
    return pd->_readPos + bytes <= pd->data.size();
}

// Read methods
uint8 PacketData::ReadUInt8()
{
    if (!CanRead(this, 1))
        return 0;
    return data[_readPos++];
}

uint16 PacketData::ReadUInt16()
{
    if (!CanRead(this, 2))
        return 0;
    uint16 val = data[_readPos] | (data[_readPos + 1] << 8);
    _readPos += 2;
    return val;
}

uint32 PacketData::ReadUInt32()
{
    if (!CanRead(this, 4))
        return 0;
    uint32 val = data[_readPos] | (data[_readPos + 1] << 8) | (data[_readPos + 2] << 16) | (data[_readPos + 3] << 24);
    _readPos += 4;
    return val;
}

uint64 PacketData::ReadUInt64()
{
    if (!CanRead(this, 8))
        return 0;
    uint64 val = static_cast<uint64>(data[_readPos]) |
                 (static_cast<uint64>(data[_readPos + 1]) << 8) |
                 (static_cast<uint64>(data[_readPos + 2]) << 16) |
                 (static_cast<uint64>(data[_readPos + 3]) << 24) |
                 (static_cast<uint64>(data[_readPos + 4]) << 32) |
                 (static_cast<uint64>(data[_readPos + 5]) << 40) |
                 (static_cast<uint64>(data[_readPos + 6]) << 48) |
                 (static_cast<uint64>(data[_readPos + 7]) << 56);
    _readPos += 8;
    return val;
}

int8 PacketData::ReadInt8()
{
    return static_cast<int8>(ReadUInt8());
}

int16 PacketData::ReadInt16()
{
    return static_cast<int16>(ReadUInt16());
}

int32 PacketData::ReadInt32()
{
    return static_cast<int32>(ReadUInt32());
}

int64 PacketData::ReadInt64()
{
    return static_cast<int64>(ReadUInt64());
}

float PacketData::ReadFloat()
{
    uint32 val = ReadUInt32();
    float result;
    static_assert(sizeof(val) == sizeof(result), "Size mismatch");
    std::memcpy(&result, &val, sizeof(result));
    return result;
}

double PacketData::ReadDouble()
{
    uint64 val = ReadUInt64();
    double result;
    static_assert(sizeof(val) == sizeof(result), "Size mismatch");
    std::memcpy(&result, &val, sizeof(result));
    return result;
}

std::string PacketData::ReadString()
{
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

std::string PacketData::ReadCString()
{
    // Same as ReadString - null-terminated string
    return ReadString();
}

// Write methods
void PacketData::WriteUInt8(uint8 val)
{
    if (_writePos < data.size())
        data[_writePos] = val;
    else
        data.push_back(val);
    _writePos++;
}

void PacketData::WriteUInt16(uint16 val)
{
    WriteUInt8(static_cast<uint8>(val & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 8) & 0xFF));
}

void PacketData::WriteUInt32(uint32 val)
{
    WriteUInt8(static_cast<uint8>(val & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 8) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 16) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 24) & 0xFF));
}

void PacketData::WriteUInt64(uint64 val)
{
    WriteUInt8(static_cast<uint8>(val & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 8) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 16) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 24) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 32) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 40) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 48) & 0xFF));
    WriteUInt8(static_cast<uint8>((val >> 56) & 0xFF));
}

void PacketData::WriteInt8(int8 val)
{
    WriteUInt8(static_cast<uint8>(val));
}

void PacketData::WriteInt16(int16 val)
{
    WriteUInt16(static_cast<uint16>(val));
}

void PacketData::WriteInt32(int32 val)
{
    WriteUInt32(static_cast<uint32>(val));
}

void PacketData::WriteInt64(int64 val)
{
    WriteUInt64(static_cast<uint64>(val));
}

void PacketData::WriteFloat(float val)
{
    uint32 uval;
    static_assert(sizeof(val) == sizeof(uval), "Size mismatch");
    std::memcpy(&uval, &val, sizeof(val));
    WriteUInt32(uval);
}

void PacketData::WriteDouble(double val)
{
    uint64 uval;
    static_assert(sizeof(val) == sizeof(uval), "Size mismatch");
    std::memcpy(&uval, &val, sizeof(val));
    WriteUInt64(uval);
}

void PacketData::WriteString(const std::string& val)
{
    for (char c : val)
    {
        WriteUInt8(static_cast<uint8>(c));
    }
    WriteUInt8(0); // Null terminator
}

void PacketData::WriteCString(const std::string& val)
{
    WriteString(val);
}

// Position management
void PacketData::ResetReadPos()
{
    _readPos = 0;
    _readingBits = false;
    _bitPos = 0;
}

void PacketData::ResetWritePos()
{
    _writePos = 0;
    _writingBits = false;
    _bitPos = 0;
}

size_t PacketData::GetReadPos() const
{
    return _readPos;
}

size_t PacketData::GetWritePos() const
{
    return _writePos;
}

bool PacketData::HasMoreData() const
{
    return _readPos < data.size();
}

#endif // ANGELSCRIPT_INTEGRATION

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
    void ResetWritePos();
    size_t GetReadPos() const;
    size_t GetWritePos() const;
    bool HasMoreData() const;
    
    // Bit operations (for packed boolean fields in packets)
    bool ReadBit();
    uint32 ReadBits(uint32 bitCount);
    void WriteBit(bool bit);
    void WriteBits(uint32 value, uint32 bitCount);
    void FlushBits();       // Flush pending write bits to byte boundary
    void ResetBitReader(); // Discard partial read byte, realign to byte boundary
    
    // Internal state for bit reading/writing
    uint8 _bitPos = 0;
    uint8 _currentByte = 0;
    bool _readingBits = false;
    bool _writingBits = false;
};

// Inline implementations for bit operations
inline bool PacketData::ReadBit()
{
    // Load new byte only if we don't have one being read
    if (!_readingBits)
    {
        if (_readPos >= data.size())
            return false;
        _currentByte = data[_readPos];
        _readPos++; // Advance immediately so standard reads don't double-dip
        _readingBits = true;
        _writingBits = false;
        _bitPos = 0;
    }

    bool bit = (_currentByte >> _bitPos) & 1;
    _bitPos++;
    
    // Clear reading state when byte is fully consumed
    if (_bitPos >= 8)
    {
        _readingBits = false;
        _bitPos = 0;
    }
    
    return bit;
}

inline uint32 PacketData::ReadBits(uint32 bitCount)
{
    uint32 value = 0;
    for (uint32 i = 0; i < bitCount; i++)
    {
        value |= (ReadBit() ? 1u : 0u) << i;  // LSB first
    }
    return value;
}

inline void PacketData::WriteBit(bool bit)
{
    if (!_writingBits)
    {
        _currentByte = 0;
        _bitPos = 0;
        _readingBits = false;
    }

    _writingBits = true;

    if (bit)
        _currentByte |= (1u << _bitPos);  // LSB first

    _bitPos++;

    if (_bitPos == 8)
    {
        data.push_back(_currentByte);
        _currentByte = 0;
        _bitPos = 0;
        _writingBits = false;
    }
}

inline void PacketData::WriteBits(uint32 value, uint32 bitCount)
{
    for (uint32 i = 0; i < bitCount; i++)
    {
        WriteBit((value >> i) & 1);  // LSB first
    }
}

inline void PacketData::FlushBits()
{
    if (_writingBits && _bitPos != 0)
    {
        data.push_back(_currentByte);
        _currentByte = 0;
        _bitPos = 0;
        _writingBits = false;
    }
    else if (_readingBits)
    {
        // Discard remaining bits in current byte — realign read cursor
        _readingBits = false;
        _bitPos = 0;
        _currentByte = 0;
    }
}

inline void PacketData::ResetBitReader()
{
    // Discard any partially-read byte and reset bit state — byte-aligns the read cursor
    // Note: _readPos is already at the next byte due to ReadBit advancing immediately
    _readingBits = false;
    _bitPos = 0;
    _currentByte = 0;
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
    low = ReadPackedUInt64();
    high = ReadPackedUInt64();
}

inline void PacketData::WritePackedGuid(uint64 low, uint64 high)
{
    FlushBits(); // Ensure byte alignment
    
    // Write low part
    uint8 maskLow = 0;
    uint8 bytesLow[8];
    uint8 countLow = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        uint8 byteVal = (low >> (i * 8)) & 0xFF;
        if (byteVal != 0)
        {
            maskLow |= (1 << i);
            bytesLow[countLow++] = byteVal;
        }
    }
    WriteUInt8(maskLow);
    for (uint8 i = 0; i < countLow; ++i)
        WriteUInt8(bytesLow[i]);
    
    // Write high part
    uint8 maskHigh = 0;
    uint8 bytesHigh[8];
    uint8 countHigh = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        uint8 byteVal = (high >> (i * 8)) & 0xFF;
        if (byteVal != 0)
        {
            maskHigh |= (1 << i);
            bytesHigh[countHigh++] = byteVal;
        }
    }
    WriteUInt8(maskHigh);
    for (uint8 i = 0; i < countHigh; ++i)
        WriteUInt8(bytesHigh[i]);
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
    _readingBits = false;
    _bitPos = 0;
    _currentByte = 0;
}

inline void PacketData::ResetWritePos()
{
    _writePos = 0;
    _writingBits = false;
    _bitPos = 0;
    _currentByte = 0;
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

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
    void FlushBits();  // Flush remaining bits to byte boundary
    
    // Internal state for bit reading/writing
    uint8 _bitPos = 0;
    uint8 _currentByte = 0;
    bool _readingBits = false;
    bool _writingBits = false;
};

// Inline implementations for bit operations
inline bool PacketData::ReadBit()
{
    if (_readPos >= data.size())
        return false;
    
    if (!_readingBits || _bitPos == 0)
    {
        _currentByte = data[_readPos++];
        _readingBits = true;
        _writingBits = false;
        _bitPos = 8;
    }
    
    _bitPos--;
    return (_currentByte >> _bitPos) & 1;
}

inline uint32 PacketData::ReadBits(uint32 bitCount)
{
    uint32 value = 0;
    for (uint32 i = 0; i < bitCount; i++)
    {
        value = (value << 1) | (ReadBit() ? 1 : 0);
    }
    return value;
}

inline void PacketData::WriteBit(bool bit)
{
    if (_writingBits)
    {
        // If we're switching from reading to writing, or vice versa
        if (_readingBits)
        {
            FlushBits();
            _readingBits = false;
        }
    }
    else if (!_readingBits && _bitPos == 0)
    {
        _currentByte = 0;
        _bitPos = 8;
    }
    
    _writingBits = true;
    _readingBits = false;
    _bitPos--;
    
    if (bit)
        _currentByte |= (1 << _bitPos);
    
    if (_bitPos == 0)
    {
        data.push_back(_currentByte);
        _currentByte = 0;
        _bitPos = 8;
    }
}

inline void PacketData::WriteBits(uint32 value, uint32 bitCount)
{
    for (int32 i = bitCount - 1; i >= 0; i--)
    {
        WriteBit((value >> i) & 1);
    }
}

inline void PacketData::FlushBits()
{
    if (_writingBits && _bitPos != 8 && _bitPos != 0)
    {
        data.push_back(_currentByte);
        _currentByte = 0;
        _bitPos = 0;
    }
    _writingBits = false;
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPACKETDATA_H

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
    if (_readPos >= data.size() && !_readingBits)
        return false;

    // Load new byte only if we don't have one being read
    if (!_readingBits)
    {
        if (_readPos >= data.size())
            return false;
        _currentByte = data[_readPos];
        _readingBits = true;
        _writingBits = false;
        _bitPos = 0;
    }

    bool bit = (_currentByte >> _bitPos) & 1;
    _bitPos++;
    
    // Only advance read position and clear reading state when byte is fully consumed
    if (_bitPos >= 8)
    {
        _readPos++;
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
    _readingBits = false;
    _bitPos = 0;
    _currentByte = 0;
}

#endif // ANGELSCRIPT_INTEGRATION
#endif // ASPACKETDATA_H

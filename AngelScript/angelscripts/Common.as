/*
 * Common.as
 * Shared utilities and type aliases for AngelScript scripting.
 */

// Type aliases for TC-compatible naming
typedef uint32 Entry;
typedef uint32 SpellId;
typedef uint32 QuestId;
typedef uint32 ItemId;
typedef uint32 GameObjectEntry;
typedef uint32 CreatureEntry;
typedef uint32 MapId;
typedef uint32 ZoneId;
typedef uint32 AreaId;
typedef uint32 FactionId;
typedef uint32 AccountId;
typedef uint64 GUID;

// Math constants
const float PI = 3.14159265358979323846;
const float HALF_PI = 1.57079632679489661923;
const float TWO_PI = 6.28318530717958647692;
const float DEG_TO_RAD = 0.01745329251994329577;
const float RAD_TO_DEG = 57.29577951308232087685;

// Utility: format gold string from copper amount
string FormatMoney(uint64 copper)
{
    uint64 gold = copper / 10000;
    uint64 silver = (copper % 10000) / 100;
    uint64 copperRemainder = copper % 100;
    return formatUInt(gold) + "g " + formatUInt(silver) + "s " + formatUInt(copperRemainder) + "c";
}

// Utility: format uint to string (AngelScript built-in)
string formatUInt(uint64 v) { return "" + v; }

// Utility: check if unit is a valid target
bool IsValidTarget(Unit@ unit)
{
    return unit !is null && unit.IsAlive();
}

// Utility: get distance between two positions
float GetDistance3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return Dist3D(x1, y1, z1, x2, y2, z2);
}

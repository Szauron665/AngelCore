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

// Math constants (PI and HALF_PI are registered by the C++ Math API)
const float TWO_PI = 6.28318530717958647692;
const float DEG_TO_RAD = 0.01745329251994329577;
const float RAD_TO_DEG = 57.29577951308232087685;

// Utility: format gold string from copper amount
string FormatMoney(uint64 copper)
{
    uint64 gold = copper / 10000;
    uint64 silver = (copper % 10000) / 100;
    uint64 copperRemainder = copper % 100;
    return gold + "g " + silver + "s " + copperRemainder + "c";
}

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

// Escape single quotes in a string for safe SQL insertion.
string EscapeString(const string& in s)
{
    string result = "";
    for (uint i = 0; i < s.length(); i++)
    {
        if (s[i] == 39) // single quote (0x27)
            result += "''";
        else
            result += s.substr(i, 1);
    }
    return result;
}

// Note: All race, class, and gender constants are now centralized in ScriptFramework.as

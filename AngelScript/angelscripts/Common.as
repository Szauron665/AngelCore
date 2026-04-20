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

// Race constants (from RaceMask.h)
const int RACE_NONE                = 0;
const int RACE_HUMAN               = 1;
const int RACE_ORC                 = 2;
const int RACE_DWARF               = 3;
const int RACE_NIGHTELF            = 4;
const int RACE_UNDEAD_PLAYER       = 5;
const int RACE_TAUREN              = 6;
const int RACE_GNOME               = 7;
const int RACE_TROLL               = 8;
const int RACE_GOBLIN              = 9;
const int RACE_BLOODELF            = 10;
const int RACE_DRAENEI             = 11;
const int RACE_WORGEN              = 22;
const int RACE_PANDAREN_NEUTRAL    = 24;
const int RACE_PANDAREN_ALLIANCE   = 25;
const int RACE_PANDAREN_HORDE      = 26;
const int RACE_NIGHTBORNE          = 27;
const int RACE_HIGHMOUNTAIN_TAUREN = 28;
const int RACE_VOID_ELF            = 29;
const int RACE_LIGHTFORGED_DRAENEI = 30;
const int RACE_ZANDALARI_TROLL     = 31;
const int RACE_KUL_TIRAN           = 32;
const int RACE_DARK_IRON_DWARF     = 34;
const int RACE_VULPERA             = 35;
const int RACE_MAGHAR_ORC          = 36;
const int RACE_MECHAGNOME          = 37;
const int RACE_DRACTHYR_ALLIANCE   = 52;
const int RACE_DRACTHYR_HORDE      = 70;
const int RACE_EARTHEN_DWARF_HORDE = 84;
const int RACE_EARTHEN_DWARF_ALLIANCE = 85;
const int RACE_HARANIR_HORDE       = 91;
const int RACE_HARANIR_ALLIANCE    = 86;

// Class constants (from SharedDefines.h)
const int CLASS_NONE           = 0;
const int CLASS_WARRIOR        = 1;
const int CLASS_PALADIN        = 2;
const int CLASS_HUNTER         = 3;
const int CLASS_ROGUE          = 4;
const int CLASS_PRIEST         = 5;
const int CLASS_DEATH_KNIGHT   = 6;
const int CLASS_SHAMAN         = 7;
const int CLASS_MAGE           = 8;
const int CLASS_WARLOCK        = 9;
const int CLASS_MONK           = 10;
const int CLASS_DRUID          = 11;
const int CLASS_DEMON_HUNTER   = 12;
const int CLASS_EVOKER         = 13;
const int CLASS_ADVENTURER     = 14;
const int CLASS_TRAVELER       = 15;

// Gender constants (from SharedDefines.h)
const int GENDER_NONE          = 2;
const int GENDER_MALE          = 0;
const int GENDER_FEMALE        = 1;

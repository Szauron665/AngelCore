/*
 * Dynamic DB2 Loading Example
 * 
 * This example shows how to load custom DB2 files that aren't loaded by TrinityCore.
 * You define the schema (structure) in AngelScript and load the DB2 file at runtime.
 * 
 * Usage:
 *   1. Create a DB2 schema defining the structure
 *   2. Load the DB2 file using the schema
 *   3. Access records by ID and read field values
 */

#include "../ScriptFramework.as"

// =============================================================================
// EXAMPLE 1: Custom Item Extension DB2
// Imagine you have a custom db2 file "ItemExtension.db2" with additional item data
// =============================================================================

void LoadCustomItemDB2()
{
    Print("Creating schema for ItemExtension DB2...");
    
    // Step 1: Create schema
    DB2Schema@ schema = CreateDB2Schema("ItemExtension");
    
    // Step 2: Define fields (must match the DB2 file structure!)
    // Field order matters - must match the DB2 column order
    schema.AddUInt32("ID");           // Field 0: ID (usually first)
    schema.AddUInt32("ItemID");       // Field 1: Reference to Item.db2
    schema.AddFloat("CustomQuality");  // Field 2: Custom quality multiplier
    schema.AddUInt32("BonusFlags");   // Field 3: Custom bonus flags
    schema.AddString("Description");  // Field 4: Custom description
    schema.AddBool("IsPremium");      // Field 5: Premium item flag
    
    // Step 3: Finalize the schema
    schema.Finalize();
    
    Print("Schema created with 6 fields");
    
    // Step 4: Load the DB2 file
    // The path is relative to the Data directory (where other DB2s are)
    uint32 handle = LoadDB2Storage("ItemExtension", "dbc/ItemExtension.db2", schema);
    
    if (handle == 0)
    {
        Print("ERROR: Failed to load ItemExtension.db2!");
        return;
    }
    
    Print("ItemExtension DB2 loaded successfully with handle: " + handle);
}

// =============================================================================
// EXAMPLE 2: Query the loaded DB2
// =============================================================================

void QueryItemExtension(uint32 itemId)
{
    // Get the loaded storage
    DB2Storage@ storage = GetDB2Storage("ItemExtension");
    
    if (storage is null)
    {
        Print("ItemExtension storage not found!");
        return;
    }
    
    // Check if record exists
    if (!storage.HasRecord(itemId))
    {
        Print("No ItemExtension data for item ID: " + itemId);
        return;
    }
    
    // Get the record
    DB2Record@ record = storage.GetRecord(itemId);
    if (record is null)
    {
        Print("Failed to get record for item ID: " + itemId);
        return;
    }
    
    // Read fields by name
    uint32 id = record.GetUInt32("ID");
    uint32 refItemId = record.GetUInt32("ItemID");
    float quality = record.GetFloat("CustomQuality");
    uint32 flags = record.GetUInt32("BonusFlags");
    string description = record.GetString("Description");
    bool isPremium = record.GetBool("IsPremium");
    
    Print("=== ItemExtension Data ===");
    Print("ID: " + id);
    Print("ItemID: " + refItemId);
    Print("CustomQuality: " + quality);
    Print("BonusFlags: " + flags);
    Print("Description: " + description);
    Print("IsPremium: " + (isPremium ? "Yes" : "No"));
}

// =============================================================================
// EXAMPLE 3: Custom Spell Modifiers DB2
// =============================================================================

void LoadSpellModifiersDB2()
{
    Print("Loading SpellModifiers DB2...");
    
    DB2Schema@ schema = CreateDB2Schema("SpellModifiers");
    
    schema.AddUInt32("ID");
    schema.AddUInt32("SpellID");        // Reference to Spell.db2
    schema.AddFloat("DamageMultiplier");
    schema.AddFloat("HealingMultiplier");
    schema.AddInt32("CooldownReduction"); // In milliseconds
    schema.AddUInt32("CastTimeReduction"); // In milliseconds
    schema.AddBool("IgnoreGCD");
    schema.AddUInt8("MaxTargets");       // Byte field
    
    schema.Finalize();
    
    uint32 handle = LoadDB2Storage("SpellModifiers", "dbc/SpellModifiers.db2", schema);
    
    if (handle == 0)
    {
        Print("Failed to load SpellModifiers.db2");
        return;
    }
    
    Print("SpellModifiers DB2 loaded with handle: " + handle);
}

// =============================================================================
// EXAMPLE 4: Using DB2 data in spell hooks
// =============================================================================

void OnSpellCastWithModifiers(Spell@ spell)
{
    if (spell is null) return;
    
    uint32 spellId = spell.GetSpellId();
    
    // Get the spell modifiers storage
    DB2Storage@ storage = GetDB2Storage("SpellModifiers");
    if (storage is null) return;
    
    // Check if this spell has custom modifiers
    if (!storage.HasRecord(spellId))
        return;
    
    DB2Record@ record = storage.GetRecord(spellId);
    if (record is null) return;
    
    // Apply custom modifiers
    float damageMult = record.GetFloat("DamageMultiplier");
    float healingMult = record.GetFloat("HealingMultiplier");
    bool ignoreGCD = record.GetBool("IgnoreGCD");
    
    Print("Spell " + spellId + " has custom modifiers:");
    Print("  Damage multiplier: " + damageMult);
    Print("  Healing multiplier: " + healingMult);
    Print("  Ignore GCD: " + (ignoreGCD ? "Yes" : "No"));
    
    // You can use these values to modify spell behavior
    // ... implement your custom logic here
}

// =============================================================================
// EXAMPLE 5: Iterate all records in a DB2
// =============================================================================

void ListAllItemExtensions()
{
    DB2Storage@ storage = GetDB2Storage("ItemExtension");
    if (storage is null)
    {
        Print("ItemExtension storage not available");
        return;
    }
    
    uint32 count = storage.GetNumRows();
    Print("ItemExtension has " + count + " records");
    
    // Note: In a real implementation, you'd iterate through all IDs
    // This is a simplified example showing the concept
    for (uint32 id = 1; id < 100000; ++id)
    {
        if (storage.HasRecord(id))
        {
            DB2Record@ record = storage.GetRecord(id);
            if (record !is null)
            {
                bool isPremium = record.GetBool("IsPremium");
                if (isPremium)
                {
                    Print("Premium item found: ID=" + id);
                }
            }
        }
    }
}

// =============================================================================
// EXAMPLE 6: Unload a DB2 when no longer needed
// =============================================================================

void CleanupCustomDB2s()
{
    Print("Unloading custom DB2 files...");
    
    // Unload by name
    UnloadDB2Storage("ItemExtension");
    
    // Or unload by handle
    DB2Storage@ storage = GetDB2Storage("SpellModifiers");
    if (storage !is null)
    {
        // Get handle and unload
        // (In real usage, you'd store the handle when loading)
    }
    
    Print("Custom DB2s unloaded");
}

// =============================================================================
// MAIN REGISTRATION
// =============================================================================

void main()
{
    Print("Dynamic DB2 Loading Example loaded!");
    Print("");
    Print("This example demonstrates:");
    Print("  1. Creating DB2 schemas in AngelScript");
    Print("  2. Loading custom DB2 files not loaded by TC");
    Print("  3. Accessing records and field values");
    Print("  4. Using DB2 data in spell hooks");
    Print("  5. Unloading DB2 files when done");
    Print("");
    Print("New API functions available:");
    Print("  - CreateDB2Schema(name) - Create a schema");
    Print("  - schema.AddInt32/UInt32/Float/String/Bool(name) - Add fields");
    Print("  - schema.Finalize() - Finalize schema");
    Print("  - LoadDB2Storage(name, path, schema) - Load DB2 file");
    Print("  - GetDB2Storage(name) - Get loaded storage");
    Print("  - storage.HasRecord(id) - Check if record exists");
    Print("  - storage.GetRecord(id) - Get record by ID");
    Print("  - record.GetUInt32/Float/String/Bool(field) - Get field values");
    Print("  - UnloadDB2Storage(name) - Unload a DB2");
    
    // Example: Load custom DB2s at startup
    // LoadCustomItemDB2();
    // LoadSpellModifiersDB2();
    
    // Register spell hook to use modifiers
    // RegisterSpellHook(SPELL_ON_CAST, @OnSpellCastWithModifiers);
}

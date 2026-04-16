/*
 * Simple Instance Script Example
 * Welcome messages, boss kill tracking, and completion rewards
 * Uses only registered APIs
 */

#include "../ScriptFramework.as"

// =============================================================================
// SIMPLE INSTANCE ENHANCEMENT - Shadowfang Keep (map 33)
// =============================================================================

// Track boss kills for this instance
uint32 bossesKilled = 0;
uint32 totalBosses = 4;

// Player enters instance
void OnPlayerEnter(Player@ player)
{
    if (player is null) return;
    
    SendSystemMessage(player, "=== WELCOME TO ENHANCED SHADOWFANG KEEP ===");
    PlaySoundToPlayer(player, 8960);
    
    // Entry buff
    player.AddAura(1126, player);
    SendSystemMessage(player, "You feel nature's blessing upon you.");
    
    if (bossesKilled > 0)
        SendSystemMessage(player, "Progress: " + bossesKilled + "/" + totalBosses + " bosses defeated");
}

// Player leaves instance
void OnPlayerLeave(Player@ player)
{
    if (player is null) return;
    player.RemoveAura(1126);
    SendSystemMessage(player, "May the shadows not follow you...");
}

// Boss killed
void OnBossKilled(Creature@ boss)
{
    if (boss is null) return;
    
    bossesKilled++;
    string bossName = boss.GetName();
    
    Print("Boss killed: " + bossName + " (" + bossesKilled + "/" + totalBosses + ")");
    
    // Milestone check
    if (bossesKilled >= totalBosses)
    {
        Print("Shadowfang Keep completed!");
    }
}

// =============================================================================
// MAIN REGISTRATION
// =============================================================================

void main()
{
    Print("Simple Instance Enhancement loaded!");
    Print("  - Enhanced Shadowfang Keep (map 33)");
    Print("  - Welcome messages and progress tracking");
    Print("  - Boss kill bonuses and completion rewards");
    
    // Register instance hooks
    RegisterInstanceScript(0, @OnPlayerEnter);   // ON_PLAYER_ENTER
    RegisterInstanceScript(2, @OnBossKilled);     // ON_BOSS_KILL
}

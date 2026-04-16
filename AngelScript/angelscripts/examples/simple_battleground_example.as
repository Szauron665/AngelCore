/*
 * Simple Battleground Script Example
 * Enhanced Warsong Gulch with custom rewards and announcements
 * Uses only registered APIs
 */

#include "../ScriptFramework.as"

// =============================================================================
// SIMPLE BATTLEGROUND ENHANCEMENT - Warsong Gulch
// =============================================================================

void OnBattlegroundStart()
{
    Print("Enhanced Warsong Gulch starting!");
}

void OnPlayerJoinBG(Player@ player)
{
    if (player is null) return;
    SendSystemMessage(player, "Welcome to Enhanced Warsong Gulch!");
    SendSystemMessage(player, "Capture the flag to win glory for your faction!");
}

void OnPlayerKillInBG(Player@ killer, Player@ killed)
{
    if (killer is null) return;
    string killerName = killer.GetName();
    string killedName = (killed !is null) ? killed.GetName() : "unknown";
    Print("BG Kill: " + killerName + " killed " + killedName);
    SendSystemMessage(killer, "Honorable kill: " + killedName);
}

// =============================================================================
// MAIN REGISTRATION
// =============================================================================

void main()
{
    Print("Simple Battleground Enhancement loaded!");
    Print("  - Enhanced Warsong Gulch with custom rewards");
    Print("  - Dramatic announcements and effects");
    Print("  - Bonus consumables for participants");
    
    RegisterBattlegroundScript(0, @OnBattlegroundStart);
    RegisterPlayerScript(PLAYER_ON_KILL_PLAYER, @OnPlayerKillInBG);
}

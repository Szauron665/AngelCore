/*
 * Simple Spell Script Example
 * Custom spell effects, damage modifications, and spell hooks
 * Uses only registered APIs from the AngelScript integration
 */

#include "../ScriptFramework.as"

// =============================================================================
// SPELL EFFECT HANDLER - Custom Fireball Enhancement
// =============================================================================

void OnFireballCast(Spell@ spell)
{
    if (spell is null) return;
    
    Unit@ caster = spell.GetCaster();
    if (caster is null) return;
    
    Player@ player = caster.ToPlayer();
    if (player is null) return;
    
    int32 castTime = spell.GetCastTime();
    
    // Visual feedback (uses SendSystemMessage as fallback)
    SendSystemMessage(player, "Enhanced Fireball!");
    PlaySoundToPlayer(player, 3078);
    
    // Level 20+ bonus: apply aura
    if (player.GetLevel() >= 20)
    {
        caster.AddAura(12042, caster);
        SendSystemMessage(player, "Your fireball burns with intensified power!");
    }
    
    Print("Player " + player.GetName() + " cast enhanced Fireball (cast time: " + castTime + "ms)");
}

// =============================================================================
// SPELL EFFECT HANDLER - Healing Touch Bonus
// =============================================================================

void OnHealingTouchCast(Spell@ spell)
{
    if (spell is null) return;
    
    Unit@ caster = spell.GetCaster();
    if (caster is null) return;
    
    Player@ player = caster.ToPlayer();
    if (player is null) return;
    
    float healthPct = player.GetHealthPct();
    
    // Full health: refund some mana
    if (healthPct >= 95.0f)
    {
        int32 currentMana = player.GetPower(POWER_MANA);
        player.SetPower(POWER_MANA, currentMana + 100);
        SendSystemMessage(player, "Full health - partial mana cost refunded.");
    }
    
    // Critical heal feedback
    if (healthPct < 50.0f)
    {
        SendSystemMessage(player, "Critical Heal!");
        PlaySoundToPlayer(player, 3230);
    }
}

// =============================================================================
// GLOBAL SPELL HOOK - Track All Spell Casts
// =============================================================================

void OnAnySpellCast(Spell@ spell)
{
    if (spell is null) return;
    
    Unit@ caster = spell.GetCaster();
    if (caster is null) return;
    
    uint32 spellId = spell.GetSpellId();
    string casterName = caster.GetName();
    
    Print("Spell cast: ID=" + spellId + " by " + casterName);
    
    Player@ player = caster.ToPlayer();
    if (player !is null)
    {
        if (spellId == 12042) // Arcane Power
        {
            SendSystemMessage(player, "ARCANE POWER ACTIVATED!");
            PlaySoundToPlayer(player, 3078);
        }
        else if (spellId == 12472) // Icy Veins
        {
            SendSystemMessage(player, "ICY VEINS - Casting speed increased!");
        }
    }
}

// =============================================================================
// UTILITY - Teleport by faction
// =============================================================================

void OnCustomTeleport(Spell@ spell)
{
    if (spell is null) return;
    
    Unit@ caster = spell.GetCaster();
    if (caster is null) return;
    
    Player@ player = caster.ToPlayer();
    if (player is null) return;
    
    int team = GetTeam(player);
    bool success = false;
    
    if (team == TEAM_ALLIANCE)
        success = player.TeleportTo(0, -8809.9f, 626.94f, 94.13f, 3.0f);
    else if (team == TEAM_HORDE)
        success = player.TeleportTo(1, 1601.0f, -4375.0f, 10.0f, 1.0f);
    
    if (success)
    {
        SendSystemMessage(player, "Teleported to your faction's capital!");
        PlaySoundToPlayer(player, 11745);
    }
    else
    {
        SendSystemMessage(player, "Teleport failed.");
        spell.Cancel();
    }
}

// =============================================================================
// MAIN REGISTRATION
// =============================================================================

void main()
{
    Print("Simple Spell Script loaded!");
    
    RegisterSpellEffectHandler(133, 0, @OnFireballCast);
    RegisterSpellEffectHandler(5185, 0, @OnHealingTouchCast);
    RegisterSpellHook(SPELL_ON_CAST, @OnAnySpellCast);
    
    Print("Spell handlers registered:");
    Print("  - Fireball (133): Enhanced damage + level bonus");
    Print("  - Healing Touch (5185): Mana refund when full health");
    Print("  - Global hook: Tracks all spell casts");
}

/*
 * AngelScript Dispatch Layer Implementation
 *
 * Each TC ScriptObject subclass routes its virtual method calls
 * into the AngelScript hook system via AngelScriptMgr.
 */

#ifndef ANGELSCRIPT_INTEGRATION
    #error "ANGELSCRIPT_INTEGRATION macro must be defined"
#endif

#include "ASDispatch.h"
#include "AngelScriptMgr.h"
#include "Player.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Unit.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "QuestDef.h"
#include "Map.h"
#include "InstanceScript.h"
#include "Log.h"

namespace AngelScript
{
    // ========================================================================
    // Player dispatch
    // ========================================================================

    ASPlayerScriptDispatch::ASPlayerScriptDispatch()
        : PlayerScript("AS_PlayerDispatch") {}

    void ASPlayerScriptDispatch::OnLogin(Player* player, bool /*firstLogin*/)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerHook(PlayerHookType::ON_LOGIN, player);
    }

    void ASPlayerScriptDispatch::OnLogout(Player* player)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerHook(PlayerHookType::ON_LOGOUT, player);
    }

    void ASPlayerScriptDispatch::OnChat(Player* player, uint32 type, uint32 lang, std::string& msg)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerChat(player, type, lang, msg);
    }

    void ASPlayerScriptDispatch::OnLevelChanged(Player* player, uint8 oldLevel)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerLevelUp(player, oldLevel);
    }

    void ASPlayerScriptDispatch::OnPVPKill(Player* killer, Player* killed)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerHook(PlayerHookType::ON_KILL_PLAYER, killer, killed);
    }

    void ASPlayerScriptDispatch::OnCreatureKill(Player* killer, Creature* killed)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerKill(killer, killed);
    }

    void ASPlayerScriptDispatch::OnPlayerKilledByCreature(Creature* killer, Player* killed)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerCreatureKillPlayer(killer, killed);
    }

    void ASPlayerScriptDispatch::OnDuelStart(Player* player1, Player* player2)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerDuelStart(player1, player2);
    }

    void ASPlayerScriptDispatch::OnDuelEnd(Player* winner, Player* loser, DuelCompleteType type)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerDuelEnd(winner, loser, type);
    }

    void ASPlayerScriptDispatch::OnSpellCast(Player* /*player*/, Spell* spell, bool /*skipCheck*/)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerSpellHook(SpellHookType::ON_CAST, spell);
    }

    void ASPlayerScriptDispatch::OnMoneyChanged(Player* player, int64& amount)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerMoneyChanged(player, amount);
    }

    void ASPlayerScriptDispatch::OnGiveXP(Player* player, uint32& amount, Unit* victim)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerGiveXP(player, amount, victim);
    }

    void ASPlayerScriptDispatch::OnReputationChange(Player* player, uint32 factionId, int32& standing, bool incremental)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerReputationChange(player, factionId, standing, incremental);
    }

    void ASPlayerScriptDispatch::OnQuestStatusChange(Player* player, uint32 questId)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerQuestStatusChange(player, questId);
    }

    void ASPlayerScriptDispatch::OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerPlayerUpdateZone(player, newZone, newArea);
    }

    void ASPlayerScriptDispatch::OnMapChanged(Player* /*player*/)
    {
        if (sAngelScriptMgr->IsEnabled())
        {
            // No corresponding AngelScript hook exists for ON_MAP_CHANGE yet
            // Could add to PlayerHookType enum if needed
        }
    }

    // ========================================================================
    // Unit dispatch
    // ========================================================================

    ASUnitScriptDispatch::ASUnitScriptDispatch()
        : UnitScript("AS_UnitDispatch") {}

    void ASUnitScriptDispatch::ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* /*spellInfo*/)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerUnitDamageModified(target, attacker, damage);
    }

    // ========================================================================
    // Creature dispatch (per-entry)
    // ========================================================================

    ASCreatureScriptDispatch::ASCreatureScriptDispatch()
        : CreatureScript("AS_CreatureDispatch") {}

    CreatureAI* ASCreatureScriptDispatch::GetAI(Creature* creature) const
    {
        if (!sAngelScriptMgr->IsEnabled())
            return nullptr;
        return sAngelScriptMgr->GetCreatureAI(creature);
    }

    // ========================================================================
    // GameObject dispatch (per-entry)
    // ========================================================================

    ASGameObjectScriptDispatch::ASGameObjectScriptDispatch()
        : GameObjectScript("AS_GameObjectDispatch") {}

    GameObjectAI* ASGameObjectScriptDispatch::GetAI(GameObject* go) const
    {
        if (!sAngelScriptMgr->IsEnabled())
            return nullptr;
        return sAngelScriptMgr->GetGameObjectAI(go);
    }

    // ========================================================================
    // World dispatch
    // ========================================================================

    ASWorldScriptDispatch::ASWorldScriptDispatch()
        : WorldScript("AS_WorldDispatch") {}

    void ASWorldScriptDispatch::OnStartup()
    {
        // Startup is handled by AngelScriptMgr::Initialize() called from World::Initialize()
    }

    void ASWorldScriptDispatch::OnShutdown()
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerWorldHook(WorldHookType::ON_SHUTDOWN);
    }

    void ASWorldScriptDispatch::OnUpdate(uint32 diff)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerWorldUpdate(diff);
    }

    void ASWorldScriptDispatch::OnConfigLoad(bool /*reload*/)
    {
        if (sAngelScriptMgr->IsEnabled())
            sAngelScriptMgr->TriggerWorldHook(WorldHookType::ON_CONFIG_LOAD);
    }

    // ========================================================================
    // Server dispatch (packet handling)
    // ========================================================================

    ASServerScriptDispatch::ASServerScriptDispatch()
        : ServerScript("AS_ServerDispatch") {}

    void ASServerScriptDispatch::OnPacketReceive(WorldSession* session, WorldPacket& packet)
    {
        if (!sAngelScriptMgr->IsEnabled()) return;
        sAngelScriptMgr->TriggerPacketReceive(session, packet, packet.GetOpcode());
    }

    void ASServerScriptDispatch::OnPacketSend(WorldSession* session, WorldPacket& packet)
    {
        if (!sAngelScriptMgr->IsEnabled()) return;
        sAngelScriptMgr->TriggerPacketSend(session, packet, packet.GetOpcode());
    }

    // ========================================================================
    // Spell dispatch
    // ========================================================================

    ASSpellScriptLoaderDispatch::ASSpellScriptLoaderDispatch()
        : SpellScriptLoader("AS_SpellDispatch") {}

    SpellScript* ASSpellScriptLoaderDispatch::GetSpellScript() const
    {
        // Spell scripts are handled via the effect handler in Spell.cpp
        // This loader is registered for the general spell hook dispatch
        return nullptr;
    }

    AuraScript* ASSpellScriptLoaderDispatch::GetAuraScript() const
    {
        return nullptr;
    }

    // ========================================================================
    // Instance dispatch
    // ========================================================================

    ASInstanceMapScriptDispatch::ASInstanceMapScriptDispatch()
        : InstanceMapScript("AS_InstanceDispatch", 0) {}

    InstanceScript* ASInstanceMapScriptDispatch::GetInstanceScript(InstanceMap* map) const
    {
        if (!sAngelScriptMgr->IsEnabled() || !map)
            return nullptr;
        return sAngelScriptMgr->GetInstanceScript(map);
    }

    // ========================================================================
    // Quest dispatch
    // ========================================================================

    ASQuestScriptDispatch::ASQuestScriptDispatch()
        : QuestScript("AS_QuestDispatch") {}

    void ASQuestScriptDispatch::OnQuestStatusChange(Player* player, Quest const* quest, QuestStatus /*oldStatus*/, QuestStatus /*newStatus*/)
    {
        if (sAngelScriptMgr->IsEnabled() && quest)
            sAngelScriptMgr->TriggerQuestStatusChange(player, quest->GetQuestId());
    }

    void ASQuestScriptDispatch::OnQuestObjectiveChange(Player* player, Quest const* quest, QuestObjective const& objective, int32 oldAmount, int32 newAmount)
    {
        if (sAngelScriptMgr->IsEnabled() && quest)
            sAngelScriptMgr->TriggerQuestObjectiveUpdate(player, quest->GetQuestId(), objective.ObjectID, oldAmount, newAmount);
    }

    // ========================================================================
    // Group dispatch
    // ========================================================================

    ASGroupScriptDispatch::ASGroupScriptDispatch()
        : GroupScript("AS_GroupDispatch") {}

    // ========================================================================
    // Guild dispatch
    // ========================================================================

    ASGuildScriptDispatch::ASGuildScriptDispatch()
        : GuildScript("AS_GuildDispatch") {}

    // ========================================================================
    // Item dispatch
    // ========================================================================

    ASItemScriptDispatch::ASItemScriptDispatch()
        : ItemScript("AS_ItemDispatch") {}

    // ========================================================================
    // Register all dispatch scripts
    // ========================================================================

    void RegisterDispatchScripts()
    {
        TC_LOG_INFO("server.angelscript", "Registering AngelScript dispatch scripts with ScriptMgr...");

        // TC's AddScript (called in base class constructors) requires a non-empty script context
        sScriptMgr->SetScriptContext("angelscript_dispatch");

        // Each constructor calls AddScript() internally via the TC base class
        new ASPlayerScriptDispatch();
        new ASUnitScriptDispatch();
        new ASCreatureScriptDispatch();
        new ASGameObjectScriptDispatch();
        new ASWorldScriptDispatch();
        new ASServerScriptDispatch();
        new ASQuestScriptDispatch();
        new ASGroupScriptDispatch();
        new ASGuildScriptDispatch();
        new ASItemScriptDispatch();

        // Reset context after registration
        sScriptMgr->SetScriptContext("");

        TC_LOG_INFO("server.angelscript", "AngelScript dispatch scripts registered (10 scripts)");
    }

} // namespace AngelScript

/*
 * AngelScript Dispatch Layer
 *
 * Routes AngelScript hooks through TrinityCore's existing ScriptMgr
 * virtual methods. This eliminates the need for direct core file patches
 * for Player/Unit/WorldSession/CharacterHandler events.
 *
 * Only World.cpp (Initialize) and Spell.cpp (effect override) need direct patches.
 */

#ifndef AS_DISPATCH_H
#define AS_DISPATCH_H

#ifdef ANGELSCRIPT_INTEGRATION

#include "ScriptMgr.h"
#include "ASHookTypes.h"
#include <string>

class Player;
class Creature;
class Unit;
class Spell;
class GameObject;
class WorldPacket;
class Group;
class Guild;
class Map;
class InstanceScript;
class Battleground;
class Quest;

namespace AngelScript
{
    // ---- Player dispatch ----
    class ASPlayerScriptDispatch : public PlayerScript
    {
    public:
        ASPlayerScriptDispatch();

        void OnLogin(Player* player, bool firstLogin) override;
        void OnLogout(Player* player) override;
        void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg) override;
        void OnLevelChanged(Player* player, uint8 oldLevel) override;
        void OnPVPKill(Player* killer, Player* killed) override;
        void OnCreatureKill(Player* killer, Creature* killed) override;
        void OnPlayerKilledByCreature(Creature* killer, Player* killed) override;
        void OnDuelStart(Player* player1, Player* player2) override;
        void OnDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override;
        void OnSpellCast(Player* player, Spell* spell, bool skipCheck) override;
        void OnMoneyChanged(Player* player, int64& amount) override;
        void OnGiveXP(Player* player, uint32& amount, Unit* victim) override;
        void OnReputationChange(Player* player, uint32 factionId, int32& standing, bool incremental) override;
        void OnQuestStatusChange(Player* player, uint32 questId) override;
        void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea) override;
        void OnMapChanged(Player* player) override;
    };

    // ---- Unit dispatch ----
    class ASUnitScriptDispatch : public UnitScript
    {
    public:
        ASUnitScriptDispatch();

        void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* spellInfo) override;
    };

    // ---- Creature dispatch (per-entry) ----
    class ASCreatureScriptDispatch : public CreatureScript
    {
    public:
        ASCreatureScriptDispatch();

        CreatureAI* GetAI(Creature* creature) const override;
    };

    // ---- GameObject dispatch (per-entry) ----
    class ASGameObjectScriptDispatch : public GameObjectScript
    {
    public:
        ASGameObjectScriptDispatch();

        GameObjectAI* GetAI(GameObject* go) const override;
    };

    // ---- World dispatch ----
    class ASWorldScriptDispatch : public WorldScript
    {
    public:
        ASWorldScriptDispatch();

        void OnStartup() override;
        void OnShutdown() override;
        void OnUpdate(uint32 diff) override;
        void OnConfigLoad(bool reload) override;
    };

    // ---- Server dispatch (packet handling) ----
    class ASServerScriptDispatch : public ServerScript
    {
    public:
        ASServerScriptDispatch();

        void OnPacketReceive(WorldSession* session, WorldPacket& packet) override;
        void OnPacketSend(WorldSession* session, WorldPacket& packet) override;
    };

    // ---- Spell dispatch ----
    class ASSpellScriptLoaderDispatch : public SpellScriptLoader
    {
    public:
        ASSpellScriptLoaderDispatch();

        SpellScript* GetSpellScript() const override;
        AuraScript* GetAuraScript() const override;
    };

    // ---- Instance dispatch ----
    class ASInstanceMapScriptDispatch : public InstanceMapScript
    {
    public:
        ASInstanceMapScriptDispatch();

        InstanceScript* GetInstanceScript(InstanceMap* map) const override;
    };

    // ---- Quest dispatch ----
    class ASQuestScriptDispatch : public QuestScript
    {
    public:
        ASQuestScriptDispatch();

        void OnQuestStatusChange(Player* player, Quest const* quest, QuestStatus oldStatus, QuestStatus newStatus) override;
        void OnQuestObjectiveChange(Player* player, Quest const* quest, QuestObjective const& objective, int32 oldAmount, int32 newAmount) override;
    };

    // ---- Group dispatch ----
    class ASGroupScriptDispatch : public GroupScript
    {
    public:
        ASGroupScriptDispatch();
    };

    // ---- Guild dispatch ----
    class ASGuildScriptDispatch : public GuildScript
    {
    public:
        ASGuildScriptDispatch();
    };

    // ---- Item dispatch ----
    class ASItemScriptDispatch : public ItemScript
    {
    public:
        ASItemScriptDispatch();
    };

    // ---- Initialize all dispatch scripts ----
    void RegisterDispatchScripts();

} // namespace AngelScript

#endif // ANGELSCRIPT_INTEGRATION
#endif // AS_DISPATCH_H

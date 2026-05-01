/*
 * AngelScript Gossip API
 * Dynamic gossip menu creation for NPC/GO interaction
 */

#ifndef ANGELSCRIPT_INTEGRATION
    #error "ANGELSCRIPT_INTEGRATION macro must be defined"
#endif

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#pragma push_macro("IN")
#pragma push_macro("OUT")
#pragma push_macro("OPTIONAL")
#undef IN
#undef OUT
#undef OPTIONAL

#include <angelscript.h>

#pragma pop_macro("OPTIONAL")
#pragma pop_macro("OUT")
#pragma pop_macro("IN")

#include "Player.h"
#include "Creature.h"
#include "GameObject.h"
#include "GossipDef.h"
#include "NPCPackets.h"
#include "QuestDef.h"
#include "ObjectMgr.h"
#include "Log.h"

namespace AngelScript
{
    // Gossip icon constant values
    static uint32 GOSSIP_ICON_CHAT_CONST = 0;
    static uint32 GOSSIP_ICON_VENDOR_CONST = 1;
    static uint32 GOSSIP_ICON_TAXI_CONST = 2;
    static uint32 GOSSIP_ICON_TRAINER_CONST = 3;
    static uint32 GOSSIP_ICON_QUEST_CONST = 4;

    // ---- Gossip wrapper functions ----

    static void Gossip_StartMenu(Player* player, uint32 /*sender*/, uint32 actionMenu)
    {
        if (!player) return;
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(actionMenu);
    }

    static void Gossip_AddMenuItem(Player* player, uint32 icon, const std::string& /*text*/, uint32 sender, uint32 action)
    {
        if (!player) return;
        // Use simplified AddMenuItem(menuId, menuItemId, sender, action)
        player->PlayerTalkClass->GetGossipMenu().AddMenuItem(0, icon, sender, action);
    }

    static void Gossip_AddMenuItemWithCode(Player* player, uint32 icon, const std::string& /*text*/, uint32 sender, uint32 action, const std::string& /*codeText*/)
    {
        if (!player) return;
        // Use simplified AddMenuItem(menuId, menuItemId, sender, action)
        player->PlayerTalkClass->GetGossipMenu().AddMenuItem(0, icon, sender, action);
    }

    static void Gossip_SendMenu(Player* player, uint32 menuId, Creature* creature)
    {
        if (!player) return;
        player->PlayerTalkClass->SendGossipMenu(menuId, creature ? creature->GetGUID() : ObjectGuid::Empty);
    }

    static void Gossip_SendMenuGO(Player* player, uint32 menuId, GameObject* go)
    {
        if (!player) return;
        player->PlayerTalkClass->SendGossipMenu(menuId, go ? go->GetGUID() : ObjectGuid::Empty);
    }

    static void Gossip_SendClose(Player* player)
    {
        if (!player) return;
        player->PlayerTalkClass->SendCloseGossip();
    }

    static void Gossip_AddQuestMenu(Player* /*player*/, Creature* /*creature*/)
    {
        // QuestGiverStatusInfo and AddGossipMenuItemData no longer exist in this TC version
        // Quest menu is handled automatically by the core
    }

    static void Gossip_SendPOI(Player* player, uint32 poiId)
    {
        if (!player) return;
        player->PlayerTalkClass->SendPointOfInterest(poiId);
    }

    void RegisterGossipAPI(asIScriptEngine* _scriptEngine)
    {
        int r;
        r = _scriptEngine->RegisterGlobalFunction("void GossipStartMenu(Player@, uint32, uint32)", asFUNCTION(Gossip_StartMenu), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipAddMenuItem(Player@, uint32, const string& in, uint32, uint32)", asFUNCTION(Gossip_AddMenuItem), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipAddMenuItemWithCode(Player@, uint32, const string& in, uint32, uint32, const string& in)", asFUNCTION(Gossip_AddMenuItemWithCode), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipSendMenu(Player@, uint32, Creature@)", asFUNCTION(Gossip_SendMenu), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipSendMenuGO(Player@, uint32, GameObject@)", asFUNCTION(Gossip_SendMenuGO), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipSendClose(Player@)", asFUNCTION(Gossip_SendClose), asCALL_CDECL);
        r = _scriptEngine->RegisterGlobalFunction("void GossipSendPOI(Player@, uint32)", asFUNCTION(Gossip_SendPOI), asCALL_CDECL);

        // Gossip icon constants
        r = _scriptEngine->RegisterGlobalProperty("const uint32 GOSSIP_ICON_CHAT", const_cast<uint32*>(&GOSSIP_ICON_CHAT_CONST));
        r = _scriptEngine->RegisterGlobalProperty("const uint32 GOSSIP_ICON_VENDOR", const_cast<uint32*>(&GOSSIP_ICON_VENDOR_CONST));
        r = _scriptEngine->RegisterGlobalProperty("const uint32 GOSSIP_ICON_TAXI", const_cast<uint32*>(&GOSSIP_ICON_TAXI_CONST));
        r = _scriptEngine->RegisterGlobalProperty("const uint32 GOSSIP_ICON_TRAINER", const_cast<uint32*>(&GOSSIP_ICON_TRAINER_CONST));
        r = _scriptEngine->RegisterGlobalProperty("const uint32 GOSSIP_ICON_QUEST", const_cast<uint32*>(&GOSSIP_ICON_QUEST_CONST));

        TC_LOG_INFO("server.angelscript", "Gossip API registered");
    }

} // namespace AngelScript

/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"

/*
   Recuperador de anillos perdidos.
*/

#define GMSG 537006

class npc_icc_rep_rings : public CreatureScript
{
public:
    npc_icc_rep_rings() : CreatureScript("npc_icc_rep_rings") { }

    enum AshenRings
    {
        ASHEN_VERDICT = 1156,
        C_F = 50377,
        H_F = 50378,
        MS_F = 52569,
        MA_F = 50376,
        T_F = 50375,
        C_H = 50384,
        H_H = 50386,
        MS_H = 52570,
        MA_H = 50387,
        T_H = 50388,
        C_R = 50397,
        H_R = 50399,
        MS_R = 52571,
        MA_R = 50401,
        T_R = 50403,
        C_E = 50398,
        H_E = 50400,
        MS_E = 52572,
        MA_E = 50402,
        T_E = 50404,
        QUEST_CHOOSE_YOUR_PATH = 24815
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        uint32 friendly = 3000;
        uint32 honored = 9000;
        uint32 revered = 21000;
        uint32 exalted = 42000;
        uint32 value = player->GetReputation(ASHEN_VERDICT);

        if (value >= friendly && value < honored)
        {
            if (!player->HasItemCount(C_F, 1, true) && !player->HasItemCount(H_F, 1, true) && !player->HasItemCount(MS_F, 1, true) && !player->HasItemCount(MA_F, 1, true) &&
                !player->HasItemCount(T_F, 1, true) && player->GetQuestStatus(QUEST_CHOOSE_YOUR_PATH) == QUEST_STATUS_COMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster (Friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (Friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - strength (Friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+3);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agility (Friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (Friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= honored && value < revered)
        {
            if (!player->HasItemCount(C_F, 1, true) && !player->HasItemCount(H_F, 1, true) && !player->HasItemCount(MS_F, 1, true) && !player->HasItemCount(MA_F, 1, true) &&
                !player->HasItemCount(T_F, 1, true) && !player->HasItemCount(C_H, 1, true) && !player->HasItemCount(H_H, 1, true) && !player->HasItemCount(MS_H, 1, true) &&
                !player->HasItemCount(MA_H, 1, true) && !player->HasItemCount(T_H, 1, true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster (Honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (Honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - strength (Honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agility (Honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (Honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+10);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= revered && value < exalted)
        {
            if (!player->HasItemCount(C_F, 1, true) && !player->HasItemCount(H_F, 1, true) && !player->HasItemCount(MS_F, 1, true) && !player->HasItemCount(MA_F, 1, true) &&
                !player->HasItemCount(T_F, 1, true) && !player->HasItemCount(C_H, 1, true) && !player->HasItemCount(H_H, 1, true) && !player->HasItemCount(MS_H, 1, true) &&
                !player->HasItemCount(MA_H, 1, true) && !player->HasItemCount(T_H, 1, true) && !player->HasItemCount(C_R, 1, true) && !player->HasItemCount(H_R, 1, true) &&
                !player->HasItemCount(MS_R, 1, true) && !player->HasItemCount(MA_R, 1, true) && !player->HasItemCount(T_R, 1, true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster (Revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (Revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - strength (Revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agility (Revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (Revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+15);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= exalted)
        {
            if (!player->HasItemCount(C_F, 1, true) && !player->HasItemCount(H_F, 1, true) && !player->HasItemCount(MS_F, 1, true) && !player->HasItemCount(MA_F, 1, true) &&
                !player->HasItemCount(T_F, 1, true) && !player->HasItemCount(C_H, 1, true) && !player->HasItemCount(H_H, 1, true) && !player->HasItemCount(MS_H, 1, true) &&
                !player->HasItemCount(MA_H, 1, true) && !player->HasItemCount(T_H, 1, true) && !player->HasItemCount(C_R, 1, true) && !player->HasItemCount(H_R, 1, true) &&
                !player->HasItemCount(MS_R, 1, true) && !player->HasItemCount(MA_R, 1, true) && !player->HasItemCount(T_R, 1, true) && !player->HasItemCount(C_E, 1, true) &&
                !player->HasItemCount(H_E, 1, true) && !player->HasItemCount(MS_E, 1, true) && !player->HasItemCount(MA_E, 1, true) && !player->HasItemCount(T_E, 1, true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster (Exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (Exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - strength (Exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agility (Exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (Exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+20);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
            }
        }
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->AddItem(C_F, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->AddItem(H_F, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->AddItem(MS_F, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->AddItem(MA_F, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->AddItem(T_F, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                player->AddItem(C_H, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                player->AddItem(H_H, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+8:
                player->AddItem(MS_H, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+9:
                player->AddItem(MA_H, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+10:
                player->AddItem(T_H, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                player->AddItem(C_R, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
                player->AddItem(H_R, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+13:
                player->AddItem(MS_R, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+14:
                player->AddItem(MA_R, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+15:
                player->AddItem(T_R, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+16:
                player->AddItem(C_E, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+17:
                player->AddItem(H_E, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+18:
                player->AddItem(MS_E, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+19:
                player->AddItem(MA_E, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+20:
                player->AddItem(T_E, 1);
                break;
        }
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

void AddSC_icc_rings()
{
    new npc_icc_rep_rings();
}

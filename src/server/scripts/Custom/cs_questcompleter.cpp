/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

/* ScriptData
Name: custom_Commandscript
%Complete: 90 will never complete
Comment: commandscript for custom commands
Category: Scripts
EndScriptData */

#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "MapManager.h"
#include "Chat.h"

class questcompleter_commandscript : public CommandScript
{
    public:
        questcompleter_commandscript() : CommandScript("questcompleter_commandscript") { }

        ChatCommand* GetCommands() const override
        {
            static ChatCommand questcompleterCommandTable[] =
            {
                { "add",              rbac::RBAC_PERM_COMMAND_QUESTCOMPLETER_ADD,      true, &HandleQuestCompleterAddCommand,      "", NULL },
                { "del",              rbac::RBAC_PERM_COMMAND_QUESTCOMPLETER_DEL,      true, &HandleQuestCompleterDelCommand,      "", NULL },
                { "",                 rbac::RBAC_PERM_COMMAND_QUESTCOMPLETER_COMPLETE, true, &HandleQuestCompleterCompleteCommand, "", NULL },
                { NULL,               0,                                               false, NULL,                                "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "qc",              rbac::RBAC_PERM_COMMAND_QUESTCOMPLETER,           true,  NULL,                                "", questcompleterCommandTable },
                { NULL,              0,                                                false, NULL,                                "", NULL }
            };
            return commandTable;
        }

        static bool HandleQuestCompleterCompleteCommand(ChatHandler* handler, char const* args)
        {
            char* cId = handler->extractKeyFromLink((char*)args, "Hquest");

            if(!cId)
            {
                handler->PSendSysMessage("Syntax: .qc complete $quest\n\nCompletes quest if $quest is bugged.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 entry = atol(cId);
            Quest const* quest = sObjectMgr->GetQuestTemplate(entry);
            if (!quest)
            {
                handler->PSendSysMessage("Please enter a quest link.");
                handler->SetSentErrorMessage(true);
                return false;
            }
            else
            {
                if(entry)
                {
                    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_QUESTCOMPLETER);
                    stmt->setUInt32(0, entry);
                    PreparedQueryResult resultCheck = LoginDatabase.Query(stmt);

                    if (resultCheck && (*resultCheck)[0].GetUInt32())
                    {
                        Player* player = handler->GetSession()->GetPlayer();
                        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST_COMPLETE, entry, player))
                        {
                            handler->PSendSysMessage(LANG_COMMAND_QUEST_COMPLETE_DISABLED);
                            handler->SetSentErrorMessage(true);
                            return false;
                        }

                        // If player doesn't have the quest
                        if (!quest || player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
                        {
                            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
                            handler->SetSentErrorMessage(true);
                            return false;
                        }

                        // Add quest items for quests that require items
                        for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
                        {
                            uint32 id = quest->RequiredItemId[x];
                            uint32 count = quest->RequiredItemCount[x];
                            if (!id || !count)
                                continue;

                            uint32 curItemCount = player->GetItemCount(id, true);

                            ItemPosCountVec dest;
                            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count-curItemCount);
                            if (msg == EQUIP_ERR_OK)
                            {
                                Item* item = player->StoreNewItem(dest, id, true);
                                player->SendNewItem(item, count-curItemCount, true, false);
                            }
                        }

                        // All creature/GO slain/cast (not required, but otherwise it will display "Creature slain 0/10")
                        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                        {
                            int32 creature = quest->RequiredNpcOrGo[i];
                            uint32 creatureCount = quest->RequiredNpcOrGoCount[i];

                            if (creature > 0)
                            {
                                if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creature))
                                    for (uint16 z = 0; z < creatureCount; ++z)
                                        player->KilledMonster(creatureInfo, ObjectGuid::Empty);
                            }
                            else if (creature < 0)
                                for (uint16 z = 0; z < creatureCount; ++z)
                                    player->KillCreditGO(creature);
                        }

                        // If the quest requires reputation to complete
                        if (uint32 repFaction = quest->GetRepObjectiveFaction())
                        {
                            uint32 repValue = quest->GetRepObjectiveValue();
                            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
                            if (curRep < repValue)
                                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                                    player->GetReputationMgr().SetReputation(factionEntry, repValue);
                        }

                        // If the quest requires a SECOND reputation to complete
                        if (uint32 repFaction = quest->GetRepObjectiveFaction2())
                        {
                            uint32 repValue2 = quest->GetRepObjectiveValue2();
                            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
                            if (curRep < repValue2)
                                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                                    player->GetReputationMgr().SetReputation(factionEntry, repValue2);
                        }

                        // If the quest requires money
                        int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
                        if (ReqOrRewMoney < 0)
                            player->ModifyMoney(-ReqOrRewMoney);

                        player->CompleteQuest(entry);
                    }
                    else
                        handler->PSendSysMessage("Quest is not bugged!");
                }
                else
                {
                    handler->PSendSysMessage("Please enter a quest link.");
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            return true;
        }

        static bool HandleQuestCompleterAddCommand(ChatHandler* handler, char const* args)
        {
            char* cId = handler->extractKeyFromLink((char*)args, "Hquest");

            if(!cId)
            {
                handler->PSendSysMessage("Syntax: .qc add $quest\n\nAdds $quest to the quest completer.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 entry = atol(cId);
            Quest const* quest = sObjectMgr->GetQuestTemplate(entry);
            if (!quest)
            {
                handler->PSendSysMessage("Please enter a quest link.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_QUESTCOMPLETER);
            stmt->setUInt32(0, entry);
            PreparedQueryResult resultCheck = LoginDatabase.Query(stmt);

            if (resultCheck && (*resultCheck)[0].GetUInt32())
                handler->PSendSysMessage("Quest is already added!");
            else if (entry)
            {
                PreparedStatement* stmt = NULL;
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_QUESTCOMPLETER);
                stmt->setUInt32(0, entry);
                LoginDatabase.Execute(stmt);
                handler->PSendSysMessage("Quest %u was added!", entry);
            }
            else
                handler->PSendSysMessage("There was a error with your request.");

            return true;
        }

        static bool HandleQuestCompleterDelCommand(ChatHandler* handler, char const* args)
        {
            char* cId = handler->extractKeyFromLink((char*)args, "Hquest");

            if(!cId)
            {
                handler->PSendSysMessage("Syntax: .qc del $quest\n\nDeletes $quest from the quest completer.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 entry = atol(cId);
            Quest const* quest = sObjectMgr->GetQuestTemplate(entry);
            if (!quest)
            {
                handler->PSendSysMessage("Please enter a quest link.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_QUESTCOMPLETER);
            stmt->setUInt32(0, entry);
            PreparedQueryResult resultCheck = LoginDatabase.Query(stmt);

            if (resultCheck && ((*resultCheck)[0].GetUInt32() == 0))
                handler->PSendSysMessage("Quest not in list!");
            else if (entry)
            {
                PreparedStatement* stmt = NULL;
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_QUESTCOMPLETER);
                stmt->setUInt32(0, entry);
                LoginDatabase.Execute(stmt);
                handler->PSendSysMessage("Quest %u was removed!", entry);
            }
            else
                handler->PSendSysMessage("There was a error with your request.");

            return true;
        }
};

void AddSC_questcompleter_commandscript()
{
    new questcompleter_commandscript();
}

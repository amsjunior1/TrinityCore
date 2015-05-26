/*
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

/*
 * The Ninja Inquisitor is an anti-ninja solution for MMO servers. It collects
 * the necessary information and provides the means for a GM to rapidly solve
 * any looting disagreements, take action against ninjas and recover ninjed
 * items upon inquiry requested by players via website.
 *
 * The rest of this project is a website interface in PHP
 *
 * The Ninja Inquisitor is a project by Havenard <havenard@hotmail.com>
 */

#include "NinjaInquisitor.h"

NinjaInquisitor::NinjaInquisitor()
{
    // nothing to initialize
}

NinjaInquisitor::~NinjaInquisitor()
{
    m_players_last_instance.clear();
    m_players_expire_time.clear();
}

uint32 NinjaInquisitor::GetLastInstanceId(uint32 playerGUID)
{
    if (GetMSTimeDiffToNow(m_players_expire_time[playerGUID]) < 300000)
        return m_players_last_instance[playerGUID];
    return 0;
}

uint32 NinjaInquisitor::GetLastInstanceId(Player* player)
{
    return GetLastInstanceId(player->GetGUIDLow());
}

void NinjaInquisitor::SetLastInstanceId(uint32 playerGUID, uint32 instanceId)
{
    m_players_last_instance[playerGUID] = instanceId;
    m_players_expire_time[playerGUID]   = getMSTime();
}

void NinjaInquisitor::SetLastInstanceId(Player* player, uint32 instanceId)
{
    SetLastInstanceId(player->GetGUIDLow(), instanceId);
}

uint32 NinjaInquisitor::GetInstanceId(ObjectGuid playerGUID)
{
    if (Player* player = ObjectAccessor::FindPlayer(playerGUID))
        return GetInstanceId(player);
    return GetLastInstanceId(playerGUID.GetEntry());
}

uint32 NinjaInquisitor::GetCurrentMapInstanceId(Player* player)
{
    Map* map = player->GetMap();
    if (map && map->GetInstanceId())
        return map->GetInstanceId();
    return 0;
}

uint32 NinjaInquisitor::GetInstanceId(Player* player)
{
    uint32 instanceId = GetCurrentMapInstanceId(player);

    if (!instanceId)
        instanceId = GetLastInstanceId(player);

    if (!instanceId)
        if (Group* group = player->GetGroup())
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (instanceId = GetLastInstanceId(member))
                        break;

    if (instanceId)
        SetLastInstanceId(player, instanceId);

    return instanceId;
}

void NinjaInquisitor::Log(uint32 instanceId, uint32 playerGUID, const char *str, ...) {
    va_list ap;
    char text[MAX_QUERY_LEN];

    if (!instanceId)
        return;

    va_start(ap, str);
    vsnprintf(text, MAX_QUERY_LEN, str, ap);
    va_end(ap);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_NINJAINQUISITOR_LOG);
    stmt->setUInt32(0, instanceId);
    stmt->setUInt32(1, playerGUID);
    stmt->setString(2, text);
    CharacterDatabase.Execute(stmt);
}

void NinjaInquisitor::InitializeInstance(uint32 instanceId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_NINJAINQUISITOR_LOG);
    stmt->setUInt32(0, instanceId);
    CharacterDatabase.Execute(stmt);
}

void NinjaInquisitor::LogRandomRoll(Player* player, uint32 minimum, uint32 maximum, uint32 roll)
{
    if (!player)
        return;

    if (uint32 instanceId = GetInstanceId(player))
        Log(instanceId, player->GetGUIDLow(), "randomroll %d %d %d", minimum, maximum, roll);
}

void NinjaInquisitor::LogItemRoll(ObjectGuid itemGUID, uint32 itemEntry, ObjectGuid playerGUID, uint8 rollNumber, uint8 rollType)
{
    if (!itemGUID)
        return;

    if (uint32 instanceId = GetInstanceId(playerGUID))
        Log(instanceId, playerGUID.GetEntry(), "itemroll %d %d:%d %d", rollType, itemEntry, itemGUID.GetEntry(), rollNumber);
}

void NinjaInquisitor::LogItemRollWon(ObjectGuid itemGUID, uint32 itemEntry, ObjectGuid playerGUID, uint8 rollNumber, uint8 rollType)
{
    if (!itemGUID)
        return;

    if (uint32 instanceId = GetInstanceId(playerGUID))
        Log(instanceId, playerGUID.GetEntry(), "itemwon %d %d:%d %d", rollType, itemEntry, itemGUID.GetEntry(), rollNumber);
}

void NinjaInquisitor::LogMessage(Player* player, uint32 type, uint32 lang, std::string const& to, std::string const& channel, std::string const& message)
{
    if (!player)
        return;

    if (lang == LANG_ADDON)
        return;

    uint32 instanceId = GetInstanceId(player);

    if (type == CHAT_MSG_WHISPER)
        if (Player* receiver = sObjectAccessor->FindPlayerByName(to))
        {
            uint32 instanceId2 = GetInstanceId(receiver);

            if (instanceId)
                Log(instanceId,  player->GetGUIDLow(), "whisper %d %s", receiver->GetGUIDLow(), message.c_str());
            if (instanceId2 && (instanceId != instanceId2))
                Log(instanceId2, player->GetGUIDLow(), "whisper %d %s", receiver->GetGUIDLow(), message.c_str());
        }

    if (!instanceId)
        return;

    uint32 guildId = 0;
    if (Guild* guild = player->GetGuild())
        guildId = guild->GetId();

    Map* map = player->GetMap();
    uint32 inInstance = (map && map->GetInstanceId());

    switch (type)
    {
        case CHAT_MSG_SAY:
            Log(instanceId, player->GetGUIDLow(), "say %d %s", inInstance, message.c_str());
            break;
        case CHAT_MSG_YELL:
            Log(instanceId, player->GetGUIDLow(), "yell %d %s", inInstance, message.c_str());
            break;
        case CHAT_MSG_GUILD:
            Log(instanceId, player->GetGUIDLow(), "guild %d %s", guildId, message.c_str());
            break;
        case CHAT_MSG_OFFICER:
            Log(instanceId, player->GetGUIDLow(), "officer %d %s", guildId, message.c_str());
            break;
        case CHAT_MSG_PARTY:
            Log(instanceId, player->GetGUIDLow(), "party %s", message.c_str());
            break;
        case CHAT_MSG_RAID:
            Log(instanceId, player->GetGUIDLow(), "raid %s", message.c_str());
            break;
        case CHAT_MSG_PARTY_LEADER:
            Log(instanceId, player->GetGUIDLow(), "party_leader %s", message.c_str());
            break;
        case CHAT_MSG_RAID_LEADER:
            Log(instanceId, player->GetGUIDLow(), "raid_leader %s", message.c_str());
            break;
        case CHAT_MSG_RAID_WARNING:
            Log(instanceId, player->GetGUIDLow(), "raid_warning %s", message.c_str());
            break;
        case CHAT_MSG_CHANNEL:
            Log(instanceId, player->GetGUIDLow(), "channel %s %s", channel.c_str(), message.c_str());
            break;
    }
}

void NinjaInquisitor::LogLootMoney(Player* player)
{
    if (!player)
        return;

    uint32 instanceId = GetCurrentMapInstanceId(player);
    if (!instanceId)
        return;

    std::vector<Player*> playersNear;
    ObjectGuid lootGUID = player->GetLootGUID();

    if (!lootGUID)
        return;

    if (Group* group = player->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member)
                continue;

            if (player->IsWithinDistInMap(member, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                playersNear.push_back(member);
        }
    }
    else
        playersNear.push_back(player);

    switch (lootGUID.GetHigh())
    {
        case HIGHGUID_GAMEOBJECT:
        {
            GameObject* go = player->GetMap()->GetGameObject(lootGUID);
            if (go && (go->GetOwnerGUID() == player->GetGUID() || go->IsWithinDistInMap(player, INTERACTION_DISTANCE)))
                if (Loot* loot = &go->loot)
                {
                    uint32 goldPerPlayer = uint32(loot->gold / playersNear.size());
                    for (std::vector<Player*>::const_iterator i = playersNear.begin(); i != playersNear.end(); ++i)
                        Log(instanceId, (*i)->GetGUIDLow(), "loot_gold gameobject:%d %d", go->GetEntry(), goldPerPlayer);
                }
            break;
        }
        case HIGHGUID_UNIT:
        case HIGHGUID_VEHICLE:
        {
            Creature* creature = player->GetMap()->GetCreature(lootGUID);
            if (creature && !creature->IsAlive() && creature->IsWithinDistInMap(player, INTERACTION_DISTANCE))
                if (Loot* loot = &creature->loot)
                {
                    uint32 goldPerPlayer = uint32(loot->gold / playersNear.size());
                    for (std::vector<Player*>::const_iterator i = playersNear.begin(); i != playersNear.end(); ++i)
                        Log(instanceId, (*i)->GetGUIDLow(), "loot_gold creature:%d %d", creature->GetEntry(), goldPerPlayer);
                }
            break;
        }
    }
}

void NinjaInquisitor::LogLootMasterGiveItem(Player* player, Player* receiver, Item* item)
{
    if (!player || !receiver || !item)
        return;

    uint32 instanceId = GetCurrentMapInstanceId(player);
    if (!instanceId)
        return;

    ObjectGuid lootGUID = player->GetLootGUID();
    bool isCreature, isGameObject;
    uint32 sourceEntry = 0;

    if (isCreature = lootGUID.IsCreatureOrVehicle())
    {
        if (Creature* creature = player->GetMap()->GetCreature(lootGUID))
            sourceEntry = creature->GetEntry();
    }
    else if (isGameObject = lootGUID.IsGameObject())
    {
        if (GameObject* go = player->GetMap()->GetGameObject(lootGUID))
            sourceEntry = go->GetEntry();
    }

    if (sourceEntry && (isCreature || isGameObject))
    {
        if (player == receiver)
            Log(instanceId, player->GetGUIDLow(), "loot_self %s:%d %d:%d", isCreature ? "creature" : "gameobject", sourceEntry, item->GetEntry(), item->GetGUIDLow());
        else
            Log(instanceId, player->GetGUIDLow(), "loot_give %d %s:%d %d:%d", receiver->GetGUIDLow(), isCreature ? "creature" : "gameobject", sourceEntry, item->GetEntry(), item->GetGUIDLow());
    }
}

void NinjaInquisitor::LogAutostoreLootItem(Player* player, Item* item)
{
    LogLootMasterGiveItem(player, player, item);
}

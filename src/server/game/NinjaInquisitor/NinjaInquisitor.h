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

#ifndef NINJAINQUISITOR_H
#define NINJAINQUISITOR_H

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "SharedDefines.h"
#include "Player.h"
#include "Group.h"
#include "Guild.h"
#include "ObjectDefines.h"

class NinjaInquisitor
{
    NinjaInquisitor();
    ~NinjaInquisitor();
    public:
        static NinjaInquisitor* instance()
        {
            static NinjaInquisitor* instance = new NinjaInquisitor();
            return instance;
        }
        void LogRandomRoll(Player* player, uint32 minimum, uint32 maximum, uint32 roll);
        void LogItemRoll(ObjectGuid itemGUID, uint32 itemEntry, ObjectGuid playerGUID, uint8 rollNumber, uint8 rollType);
        void LogItemRollWon(ObjectGuid itemGUID, uint32 itemEntry, ObjectGuid playerGUID, uint8 rollNumber, uint8 rollType);
        void LogLootMoney(Player* player);
        void LogMessage(Player* player, uint32 type, uint32 lang, std::string const& to, std::string const& channel, std::string const& message);
        void LogLootMasterGiveItem(Player* player, Player* receiver, Item* item);
        void LogAutostoreLootItem(Player* player, Item* item);
        void InitializeInstance(uint32 instanceId);
    private:
        std::map<uint32, uint32> m_players_last_instance, m_players_expire_time;
        uint32 GetLastInstanceId(uint32 playerGUID);
        uint32 GetLastInstanceId(Player* player);
        uint32 GetCurrentMapInstanceId(Player* player);
        void SetLastInstanceId(uint32 playerGUID, uint32 instanceId);
        void SetLastInstanceId(Player* player, uint32 instanceId);
        uint32 GetInstanceId(ObjectGuid playerGUID);
        uint32 GetInstanceId(Player* player);
        void Log(uint32 instanceId, uint32 playerGUID, const char *str, ...);
};

#define sNinjaInquisitor NinjaInquisitor::instance()

#endif

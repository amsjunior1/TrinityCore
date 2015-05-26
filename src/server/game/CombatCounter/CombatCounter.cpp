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

#include "CombatCounter.h"
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Player.h"
#include "Map.h"

CombatCounter::CombatCounter()
{
    in_combat = false;
    is_healing_counter = false;
}

CombatCounter::~CombatCounter()
{
    ValueTable.clear();
}

void CombatCounter::CombatBegin(Unit* unit, bool in_zone_counter /*= false*/)
{
    in_combat = true;
    begin_time = getMSTime();

    entry = unit->GetEntry();
    ObjectGuid guid = unit->GetGUID();

    Map::PlayerList const &PlayerList = unit->GetMap()->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
            {
                if (is_healing_counter)
                    player->SetHealingCounterGUID(in_zone_counter ? guid : ObjectGuid::Empty);
                else
                    player->SetDamageCounterGUID(in_zone_counter ? guid : ObjectGuid::Empty);
            }

    if (Map* map = unit->GetMap())
    {
        if (map->IsRaid())
        {
            if (map->Is25ManRaid())
                mode = map->IsHeroic() ? 5 : 3;
            else
                mode = map->IsHeroic() ? 4 : 2;
        }
        else
            mode = map->IsHeroic() ? 1 : 0;
    }
    else
        mode = 0;

    if (is_healing_counter)
        mode += 6;

    ValueTable.clear();
}

void CombatCounter::InputValue(Unit* attacker, uint32 value)
{
    if (!attacker || !in_combat)
        return;

    if (Player* player = attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
        ValueTable[player->GetGUIDLow()] += value;
}

void CombatCounter::CombatComplete()
{
    if (ValueTable.empty() || !in_combat)
        return;

    in_combat = false;

    float delta_time = float(GetMSTimeDiffToNow(begin_time)) / 1000.0f;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    for (std::map<uint32, uint32>::const_iterator itr = ValueTable.begin(); itr != ValueTable.end(); ++itr)
        trans->PAppend("INSERT INTO `dps_counters` (`entry`, `mode`, `guid`, `dps`) VALUES (%d, %d, %d, %f) ON DUPLICATE KEY UPDATE `dps` = GREATEST(`dps`, VALUES(`dps`))", entry, mode, itr->first, float(itr->second) / delta_time);

    CharacterDatabase.CommitTransaction(trans);

    ValueTable.clear();    
}

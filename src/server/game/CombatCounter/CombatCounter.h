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

#ifndef COMBATCOUNTER_H
#define COMBATCOUNTER_H

#include <map>

class Unit;

class CombatCounter
{
    public:
        CombatCounter();
        ~CombatCounter();
        void CombatBegin(Unit* unit, bool in_zone_counter = false);
        void InputValue(Unit* attacker, uint32 value);
        void CombatComplete();
        void SetForHealing(bool for_healing = true) { is_healing_counter = for_healing; }
    private:
        bool in_combat;
        bool is_healing_counter;
        uint32 entry;
        uint32 mode;
        uint32 begin_time;
        std::map<uint32, uint32> ValueTable;
};

#endif

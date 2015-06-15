SET @NPC = 50000;

DELETE FROM `creature_template` WHERE `entry` = @NPC;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES ('@NPC', '0', '0', '0', '0', '0', '16112', '0', '0', '0', 'Eye', 'Arena watcher', '', '0', '80', '80', '0', '35', '1', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', 'npc_arena_watcher', '0')

DELETE FROM `creature_text` WHERE `entry` = @NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `text`, `type`, `comment`) VALUES
(@NPC, '0', 'Não a batalhas em andamento neste momento.', '15', 'ArenaWatcher'),
(@NPC, '1', 'A Batalha acabou', '15', 'ArenaWatcher'),
(@NPC, '2', 'O Alvo nao esta logado', '15', 'ArenaWatcher'),
(@NPC, '3', 'O Alvo nao esta na Arena', '15', 'ArenaWatcher'),
(@NPC, '4', 'O Alvo e GameMaster', '15', 'ArenaWatcher');

DELETE FROM `locales_creature_text` WHERE `entry` = @NPC;
INSERT INTO `locales_creature_text` (`entry`, `Groupid`, `id`, `text_loc8`) VALUES
(@NPC, '0', '0', '? ?????? ?????? ?????? ???'),
(@NPC, '1', '0', '???? ??? ????????'),
(@NPC, '2', '0', '????? ?? ? ????'),
(@NPC, '3', '0', '????? ?? ?? ?????'),
(@NPC, '4', '0', '?????? ??????? ?? ??????? ????????');

DELETE FROM `trinity_string` WHERE `entry` BETWEEN 11200 AND 11203;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc8`) VALUES
('11200', 'Acompanhar partidas 2x2 (partidas agora: %u)', '???????? ???? 2?2 (?????? ???: %u)'),
('11201', 'Acompanhar partidas 3x3 (partidas agora: %u)', '???????? ???? 3?3 (?????? ???: %u)'),
('11202', 'Acompanhar partidas 5x5 (partidas agora: %u)', '???????? ???? 5?5 (?????? ???: %u)'),
('11203', 'Acompanhar o Jogador de perto', '??????? ?? ???????');
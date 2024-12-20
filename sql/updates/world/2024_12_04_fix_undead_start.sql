--
SET @ENTRY := 49044;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,24959,0,0,0,11,73524,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Agatha - On quest 24959 - cast spell'),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,28,73523,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Agatha - Remove aura from the player');

UPDATE `creature_template_addon` SET `auras` = '49414' WHERE `entry` = @ENTRY;

DELETE FROM `spell_area` WHERE spell=65051 and area=5692;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `racemask`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(65051, 5692, 0, 28608, 16, 1, 0, 3);

-- Add .reload angelscript command to command table
DELETE FROM `command` WHERE `name` = 'reload angelscript';
INSERT INTO `command` (`name`, `help`) VALUES
('reload angelscript', 'Reloads all AngelScript scripts');
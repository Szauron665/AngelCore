-- AngelCore Custom RBAC Permission for AngelScript Reload
-- Adds the permission for .reload angelscript command

-- Add the permission to rbac_permissions table
DELETE FROM `rbac_permissions` WHERE `id` = 1000;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1000, 'Command: reload angelscript');

-- Link to Administrator Commands (id=196) and Gamemaster Commands (id=197)
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` = 1000;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(196, 1000),
(197, 1000);
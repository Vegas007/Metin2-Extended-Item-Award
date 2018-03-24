ALTER TABLE player.item_award ADD
(
	`attrtype0` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue0` smallint(6) NOT NULL DEFAULT 0,
	`attrtype1` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue1` smallint(6) NOT NULL DEFAULT 0,
	`attrtype2` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue2` smallint(6) NOT NULL DEFAULT 0,
	`attrtype3` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue3` smallint(6) NOT NULL DEFAULT 0,
	`attrtype4` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue4` smallint(6) NOT NULL DEFAULT 0,
	`attrtype5` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue5` smallint(6) NOT NULL DEFAULT 0,
	`attrtype6` tinyint(4) NOT NULL DEFAULT 0,
	`attrvalue6` smallint(6) NOT NULL DEFAULT 0
);

ALTER TABLE player.item_award MODIFY `mall` tinyint(1) NOT NULL DEFAULT 0 AFTER `attrvalue6`;
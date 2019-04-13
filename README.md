
# Metin2 Extended Item Award
- You can store all bonuses and stones for items.

**02.04.2018**
- Fixed problem with save bonus after reload <cache> items.
- Fixed problem with unknown values.
- Correction for socket real time and more.
- Fixed unknown average/skill damage bonus value.
player.item_proto.addon_type = -1 (Eg. 189, 199, 299, 1139, 1179, 2159, 2179, 3169, 3219, 5119, 5129, 6019, 6069, 6079, 7169)[+0 - +9]
That's for the items which have addon type (-1) and you added them in item shop without bonuses like skill damage or hit damage,
value x, y as default, so they'll will be without bonuses and get 'bugged'.
Now when the item will be inserted there'll be a check if item doesn't have those bonuses (from query) add a random average/skill damage bonus value.
```sql
INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 189, 1, 1); 
```

**12.04.2019**
- Fixed unique items based on the real time.
- Fixed unstackable items.
- Fixed if item count overflow occured, then set it to maximum.
- Added support for books. (check skill types, unknown skill), skill vnum need to be saved into socket0, (4=Aura of the Sword < player.skill_proto), if the skill vnum is unknown, there will be a random book based on pc races, excluded skills PASSIVE, GUILD, SUPPORT.
- Added a to-do for ITEM_BLEND, check if apply_type exists in bonuses, check if apply_value/apply_duration is equal with grades (1/2/3/4/5) from settings, blend.txt
- Added auto query.
 
```sql
# Random book
INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 50300, 1, 1); 
# Specific book by skill vnum
INSERT INTO player.item_award(`login`, `vnum`, `count`, `socket0`, `mall`) VALUES ('account', 50300, 1, 4, 1);
```

![screenshot_196](https://user-images.githubusercontent.com/17238102/37860642-581cde84-2f32-11e8-9f1e-36cc2eef695d.png) ![screenshot_48](https://user-images.githubusercontent.com/17238102/38222896-611fc684-36f0-11e8-8535-26bf5fcef867.png)
![28367df426cae8e7c1d54cd01d9cf6a2](https://user-images.githubusercontent.com/17238102/38222908-7074939e-36f0-11e8-8d09-1027396185f9.gif) ![enter image description here](https://i.gyazo.com/5cf81572ee8fb081e9515634a3d65566.gif)

###

> Data information

| login		| vnum		| count		| given_time			| why			| socket0	| socket1	| socket2	| attrtype0	| attrvalue0	| attrtype1	| attrvalue1	| attrtype2	| attrvalue2	| attrtype3	| attrvalue3	| attrtype4	| attrvalue4	| attrtype5	| attrvalue5	| attrtype6	| attrvalue6	| mall	|
| ---      	| ---		| ---       | ------------------	| ---       	| ---		| ---		| ---		| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---	|
| test		| 12029		| 1 		| '2018-03-25 05:53:17'	| 'ITEM_SHOP' 	| 28442 	| 28441 	| 28438 	| 1 		| 1500 			| 29		| 10 			| 30 		| 10 			| 31		| 10			| 32		| 10			| 0			| 0				| 0			| 0 			| 1 	|

> MySQL Command Test
```sql
# Sockets & attrs
INSERT INTO `player`.`item_award`(`login`, `vnum`, `count`, `given_time`, `why`, `socket0`, `socket1`, `socket2`, `attrtype0`, `attrvalue0`, `attrtype1`, `attrvalue1`, `attrtype2`, `attrvalue2`, `attrtype3`, `attrvalue3`, `attrtype4`, `attrvalue4`, `mall`) VALUES (
	'test',                 -- ACCOUNT_NAME
	12029,                  -- ITEM_VNUM
	1,                      -- ITEM_COUNT
	'2018-03-25 05:53:17',  -- GIVEN_TIME
	'ITEM_SHOP',            -- REASON
	28442, 28441, 28438,    -- SOCKET 1 & 2 & 3
	1, 1500,                -- APPLY_MAX_HP
	29, 10,                 -- APPLY_RESIST_SWORD,
	30, 10,                 -- APPLY_RESIST_TWOHAND
	31, 10,                 -- APPLY_RESIST_DAGGER
	32, 10,                 -- APPLY_RESIST_BELL
	1                       -- MALL
);
```

# Metin2 Extended Item Award
>  You can store all bonuses and stones for items.

**2018-04-02 14:02:11 Monday**
- Fixed problem with save bonus after reload <cache> items.
- Fixed problem with unknown values.
- Correction for socket real time and more.
- Fixed unknown average/skill damage bonus value.
player.item_proto.addon_type = -1 (Eg. 189, 199, 299, 1139, 1179, 2159, 2179, 3169, 3219, 5119, 5129, 6019, 6069, 6079, 7169)[+0 - +9]
That's for the items which have addon type (-1) and you added them in item shop without bonuses like skill damage or hit damage,
value x, y as default, so they'll will be without bonuses and get bugged.
Now when the item will be inserted there'll be a check if item doesn't have those bonuses (from query) add a random average/skill damage bonus value.
```sql
INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 189, 1, 1); 
```
------------
**2019-04-12 02:31:18 Friday**
- Fixed unique items based on the real time.
- Fixed unstackable items.
- Fixed if item count overflow occured, then set it to maximum.
- Added support for books. (check skill types, unknown skill), skill vnum need to be saved into socket0, (**4=Aura of the Sword** < player.skill_proto), if the skill vnum is unknown, there will be a random book based on pc races, excluded skills PASSIVE, GUILD, SUPPORT.
- Added a to-do for **ITEM_BLEND**, check if apply_type exists in bonuses, check if apply_value/apply_duration is equal with grades (1/2/3/4/5) from settings, **blend.txt**
- Added auto query.
```sql
# Random book
INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 50300, 1, 1); 
# Specific book by skill vnum
INSERT INTO player.item_award(`login`, `vnum`, `count`, `socket0`, `mall`) VALUES ('account', 50300, 1, 4, 1);
```
------------
**2019-04-16 14:54:48 Tuesday** [(Video - Click)](https://i.gyazo.com/403fd912ef53eb55d0b8ea21fadf484e.mp4)
- Added a check for attr types and values **min** - **max**.
- You can't insert wrong bonuses into a specific item.
- Eg. Add 2000 *MAX_HP* on your **Sword+9**, was possible, now not.
- Eg. Add +500 *INT* to your shield, now there's a check for min-max value of player.item_attr Lv.1 - Lv.5
and your **500** *INT*   value will be replaced with max value from lvl5 of bonus, like **12** (**lv5**), that happen with all the bonuses, same thing with the values lower than lvl1, like 5 *HP_REGEN* on your neck, when the minimum (**lv1**) is **10**, the value will be replaced with **10**.
- If the bonus type can't be added into a specific item, the bonus will be ignored > deleted. (example: critical pct to armor)
- Refactorized all the code and moved all features into [ItemAwardManager.cpp](https://github.com/Vegas007/Metin2-Extended-Item-Award/blob/master/01.%20Svn/Server/db/ItemAwardManager.cpp).
- C++11 or higher is required for checking attributes.
```sql
# Test unknown types + higher and lower values in game.
INSERT INTO `player`.`item_award`(`login`, `vnum`, `count`, `attrtype0`, `attrvalue0`, `attrtype1`, `attrvalue1`, `attrtype2`, `attrvalue2`, `attrtype3`, `attrvalue3`, `attrtype4`, `attrvalue4`, `mall`) VALUES (
	'test',
	149,
	1,
	17, 25,   -- ATTBONUS_HUMAN
	22, 35,   -- ATTBONUS_DEVIL,
	32, 175,  -- RESIST_BELL
	33, -150, -- RESIST_FAN
	48, 1,    -- IMMUNE_STUN
	1
);

# See the min-max values for all the bonuses from weapon.
SELECT apply+0 AS `index`, apply AS `name`, lv1 as `min_value`, lv5 as `max_value` 
	FROM `item_attr` WHERE weapon > 0;
	
# See if a specific bonus is included in bonuses of weapon.
SELECT apply, apply+0 FROM `item_attr` WHERE weapon > 0
	AND apply in ('ATTBONUS_HUMAN', 'ATTBONUS_DEVIL', 'RESIST_BELL', 'RESIST_FAN', 'IMMUNE_STUN');
```
------------
<p align="left"> 
<img src="https://i.gyazo.com/403fd912ef53eb55d0b8ea21fadf484e.gif">
</p>

------------
<p align="center"><img src="https://user-images.githubusercontent.com/17238102/37860642-581cde84-2f32-11e8-9f1e-36cc2eef695d.png"><img src="https://user-images.githubusercontent.com/17238102/38222896-611fc684-36f0-11e8-8535-26bf5fcef867.png"></p>

<p align="center"><img src="https://user-images.githubusercontent.com/17238102/38222908-7074939e-36f0-11e8-8d09-1027396185f9.gif"><img src="https://i.gyazo.com/5cf81572ee8fb081e9515634a3d65566.gif"></p>

------------

# Sockets & attrs
| login		| vnum		| count		| given_time			| why			| socket0	| socket1	| socket2	| attrtype0	| attrvalue0	| attrtype1	| attrvalue1	| attrtype2	| attrvalue2	| attrtype3	| attrvalue3	| attrtype4	| attrvalue4	| attrtype5	| attrvalue5	| attrtype6	| attrvalue6	| mall	|
| ---      	| ---		| ---       | ------------------	| ---       	| ---		| ---		| ---		| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---		| ---			| ---	|
| test		| 12029		| 1 		| '2018-03-25 05:53:17'	| 'ITEM_SHOP' 	| 28442 	| 28441 	| 28438 	| 1 		| 1500 			| 29		| 10 			| 30 		| 10 			| 31		| 10			| 32		| 10			| 0			| 0				| 0			| 0 			| 1 	|

```sql
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

# Metin2 Extended Item Award

![screenshot_196](https://user-images.githubusercontent.com/17238102/37860642-581cde84-2f32-11e8-9f1e-36cc2eef695d.png)


- Now you can store all bonus and stones for items.

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

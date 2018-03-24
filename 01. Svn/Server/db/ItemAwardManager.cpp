//1.1 Search for:
	snprintf(szQuery, sizeof(szQuery), "SELECT id,login,vnum,count,socket0,socket1,socket2,mall,why FROM item_award WHERE taken_time IS NULL and id > %d", g_dwLastCachedItemAwardID);
//1.1 Replace with:
#if defined(ENABLE_EXTEND_ITEM_AWARD)	
	snprintf(szQuery, sizeof(szQuery), "SELECT id, login, vnum, count, socket0, socket1, socket2, "
			"attrtype0, attrvalue0, "
			"attrtype1, attrvalue1, "
			"attrtype2, attrvalue2, "
			"attrtype3, attrvalue3, "
			"attrtype4, attrvalue4, "
			"attrtype5, attrvalue5, "
			"attrtype6, attrvalue6, "
			"mall, why "
			"FROM item_award WHERE taken_time IS NULL and id > %d", g_dwLastCachedItemAwardID);
#else
	snprintf(szQuery, sizeof(szQuery), "SELECT id,login,vnum,count,socket0,socket1,socket2,mall,why FROM item_award WHERE taken_time IS NULL and id > %d", g_dwLastCachedItemAwardID);
#endif

//1.2 Search for:
		str_to_number(kData->dwSocket2, row[col++]);
//1.2 Add after:
#if defined(ENABLE_EXTEND_ITEM_AWARD)	
		for (size_t kIndex = 0; kIndex < ITEM_ATTRIBUTE_MAX_NUM; kIndex++)
		{
			str_to_number(kData->aAttr[kIndex].bType, row[col++]);
			str_to_number(kData->aAttr[kIndex].sValue, row[col++]);
		}
#endif
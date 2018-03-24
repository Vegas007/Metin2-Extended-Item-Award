//1.1 Search for:
					item.alSockets[2] = dwSocket2;
//1.1 Replace with:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
					item.alSockets[2] = pItemAward->dwSocket2;
					thecore_memcpy(&item.aAttr, pItemAward->aAttr, sizeof(item.aAttr));
#else
					item.alSockets[2] = dwSocket2;
#endif
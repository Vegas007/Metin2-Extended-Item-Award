//1.1) Search for:
					DWORD dwSocket2 = 0;
//1.1) Replace with:
					#ifdef ENABLE_EXTEND_ITEM_AWARD
					DWORD dwSocket2 = pItemAward->dwSocket2;
					#else
					DWORD dwSocket2 = 0;
					#endif

//1.2) Search for:
						DWORD dwSkillIdx;
						DWORD dwSkillVnum;

						do
						{
							dwSkillIdx = number(0, m_vec_skillTable.size()-1);
							dwSkillVnum = m_vec_skillTable[dwSkillIdx].dwVnum;
							break;
						} while (1);

						pItemAward->dwSocket0 = dwSkillVnum;
//1.2) Replace with:
#ifndef ENABLE_EXTEND_ITEM_AWARD
						DWORD dwSkillIdx;
						DWORD dwSkillVnum;

						do
						{
							dwSkillIdx = number(0, m_vec_skillTable.size()-1);
							dwSkillVnum = m_vec_skillTable[dwSkillIdx].dwVnum;
							break;
						} while (1);

						pItemAward->dwSocket0 = dwSkillVnum;
#endif

//1.3) Search for:
					if (pItemTable->bType == ITEM_UNIQUE)
					{
						if (pItemAward->dwSocket2 != 0)
							dwSocket2 = pItemAward->dwSocket2;
						else
							dwSocket2 = pItemTable->alValues[0];
					}
//1.3) Replace with:
					if (pItemTable->bType == ITEM_UNIQUE)
					{
#ifdef ENABLE_EXTEND_ITEM_AWARD
						// 12.04.2019 - Correction for unique items based on the real time.
						const long lValue0 = pItemTable->alValues[ITEM_SOCKET_REMAIN_SEC];
						const long lValue2 = pItemTable->alValues[ITEM_SOCKET_UNIQUE_REMAIN_TIME];
						const time_t tNow = CClientManager::instance().GetCurrentTime();
						dwSocket2 = (lValue2 == 0) ? static_cast<DWORD>(lValue0) : static_cast<DWORD>(tNow + lValue0);
#else
						if (pItemAward->dwSocket2 != 0)
							dwSocket2 = pItemAward->dwSocket2;
						else
							dwSocket2 = pItemTable->alValues[0];
#endif
					}
					
//1.4) Search for:
						snprintf(szQuery, sizeof(szQuery),
								"INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2) "
								"VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u)",
								GetTablePostfix(),
								GainItemID(),
								pi->account_id,
								pi->ip[0] == 0 ? "SAFEBOX" : "MALL",
								iPos,
								pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);
//1.4) Replace with:
#ifdef ENABLE_EXTEND_ITEM_AWARD
						ItemAwardManager::instance().CheckItemCount(*pItemAward, *pItemTable);
						ItemAwardManager::instance().CheckItemBlend(*pItemAward, *pItemTable);
						ItemAwardManager::instance().CheckItemAddonType(*pItemAward, *pItemTable);
						ItemAwardManager::instance().CheckItemSkillBook(*pItemAward, m_vec_skillTable);
						#ifdef USE_ITEM_AWARD_CHECK_ATTRIBUTES
						ItemAwardManager::instance().CheckItemAttributes(*pItemAward, *pItemTable, m_vec_itemAttrTable);
						#endif
						
						// START_OF_AUTO_QUERY
						char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];
						
						int	iLen = snprintf(szColumns, sizeof(szColumns), "id, owner_id, window, pos, vnum, count");
						int	iValueLen = snprintf(szValues, sizeof(szValues), "%u, %u, '%s', %d, %u, %u", GainItemID(), pi->account_id, (pi->ip[0] == 0) ? "SAFEBOX" : "MALL", iPos, pItemAward->dwVnum, pItemAward->dwCount);

						iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", socket0, socket1, socket2");
						iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen, ", %u, %u, %u", pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);

						for (size_t i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
						{
							iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", attrtype%d, attrvalue%d", i, i);
							iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen, ", %d, %d", pItemAward->aAttr[i].bType, pItemAward->aAttr[i].sValue);
						}
						// END_OF_AUTO_QUERY
						
						snprintf(szQuery, sizeof(szQuery), "INSERT INTO item%s (%s) VALUES(%s)", GetTablePostfix(), szColumns, szValues);
#else
						snprintf(szQuery, sizeof(szQuery),
								"INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2) "
								"VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u)",
								GetTablePostfix(),
								GainItemID(),
								pi->account_id,
								pi->ip[0] == 0 ? "SAFEBOX" : "MALL",
								iPos,
								pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);
#endif

//1.5) Search for:
					item.alSockets[2] = dwSocket2;
//1.5) Add after:
#ifdef ENABLE_EXTEND_ITEM_AWARD
					thecore_memcpy(&item.aAttr, pItemAward->aAttr, sizeof(item.aAttr));
#endif

//1.6) If you use martysama0134 source:
#define ENABLE_ITEMAWARD_REFRESH // Should be enabled

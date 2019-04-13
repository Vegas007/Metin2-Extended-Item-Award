//1.1) Search for:
extern int g_iPlayerCacheFlushSeconds;
//1.1) Add before:
#ifdef ENABLE_EXTEND_ITEM_AWARD
#include <float.h>

enum EItemSkillBook
{
	ITEM_SKILL_VNUM = 50300,
	ITEM_SKILLFORGET_VNUM = 70037,
};

inline const double uniform_random(const double a, const double b)
{
	return thecore_random() / (RAND_MAX + 1.f) * (b - a) + a;
}

inline const float GetRandomGaussian(const float fAverage, const float sigma)
{
	static bool bHasNextGaussian = false;
	static float fNextGaussian = 0.0f;

	if (bHasNextGaussian)
	{
		bHasNextGaussian = false;
		return (fNextGaussian * sigma) + fAverage;
	}
	else
	{
		double v1(0), v2(0), s(0);
		do
		{
			v1 = uniform_random(-1.f, 1.f);
			v2 = uniform_random(-1.f, 1.f);
			s = v1 * v1 + v2 * v2;
		} while (s >= 1.f || fabs(s) < FLT_EPSILON);

		const double multiplier = sqrtf(-2 * logf(s) / s);
		fNextGaussian = v2 * multiplier;
		bHasNextGaussian = true;
		return (v1 * multiplier) * sigma + fAverage;
	}
}
#endif

//1.2) Search for:
					DWORD dwSocket2 = 0;
//1.2) Replace with:
					#ifdef ENABLE_EXTEND_ITEM_AWARD
					DWORD dwSocket2 = pItemAward->dwSocket2;
					#else
					DWORD dwSocket2 = 0;
					#endif

//1.3) Search for:
						pItemAward->dwSocket0 = dwSkillVnum;
//1.3) Replace with:
						#ifndef ENABLE_EXTEND_ITEM_AWARD
						pItemAward->dwSocket0 = dwSkillVnum;
						#endif

//1.4) Search for:
					if (pItemTable->bType == ITEM_UNIQUE)
					{
						if (pItemAward->dwSocket2 != 0)
							dwSocket2 = pItemAward->dwSocket2;
						else
							dwSocket2 = pItemTable->alValues[0];
					}
//1.4) Replace with:
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
					
//1.5) Search for:
						snprintf(szQuery, sizeof(szQuery),
								"INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2) "
								"VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u)",
								GetTablePostfix(),
								GainItemID(),
								pi->account_id,
								pi->ip[0] == 0 ? "SAFEBOX" : "MALL",
								iPos,
								pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);
//1.5) Replace with:
#ifdef ENABLE_EXTEND_ITEM_AWARD
						// 11.04.2019 - Count fix for unstackable items.
						// Check if item count overflow occured, then set it to maximum
						const bool bIsStackableItem = (pItemTable->dwFlags & ITEM_FLAG_STACKABLE);
						pItemAward->dwCount = MINMAX(1, (pItemAward->dwCount > 1 && !bIsStackableItem) ? 1 : pItemAward->dwCount, ITEM_MAX_COUNT);
						
						// TODO-UNFINISHED: Check if apply_type exists in bonuses.
						// Check if apply_value/apply_duration is equal with grades (1/2/3/4/5) from settings, blend.txt
						const bool bIsBlendItem = (pItemTable->bType == ITEM_BLEND);
						if (bIsBlendItem)
						{
							const DWORD apply_type = pItemAward->dwSocket0;
							const DWORD apply_value = pItemAward->dwSocket1;
							const DWORD apply_duration = pItemAward->dwSocket2;
							
							if (apply_type == 0 || apply_value == 0 || apply_duration == 0)
								sys_err("ItemAward - Unknown settings for ITEM_BLEND.");
						}

						// 02.04.2018 - Fixed unknown average/skill damage bonus value.
						// player.item_proto.addon_type = -1 (Eg. 189, 199, 299, 1139, 1179, 2159, 2179, 3169, 3219, 5119, 5129, 6019, 6069, 6079, 7169)[+0 - +9]
						// That's for the items which have addon type (-1) and you added them in item shop without bonuses like skill damage or hit damage,
						// value x, y as default, so they'll will be without bonuses and get 'bugged'.
						// Now when the item will be inserted there'll be a check if item doesn't have those bonuses (from query) add a random average/skill damage bonus value.
						// INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 189, 1, 1); 
						const bool bIsAddonTypeItem = (pItemTable->sAddonType == -1);
						if (bIsAddonTypeItem)
						{
							bool bHasBonus = false;
							for (size_t i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
							{
								const BYTE bType = pItemAward->aAttr[i].bType;
								const short sValue = pItemAward->aAttr[i].sValue;

								if ((bType == APPLY_SKILL_DAMAGE_BONUS || bType == APPLY_NORMAL_HIT_DAMAGE_BONUS) && sValue) 
									bHasBonus = true; 
							}

							if (!bHasBonus)
							{
								const short sApplySkillDamageValue = MINMAX(-30, static_cast<short>((GetRandomGaussian(0, 5) + 0.5f)), 30);
								const short sApplyNormalHitValue = abs(sApplySkillDamageValue) <= 20 ? (-2 * sApplySkillDamageValue) + abs(number(-8, 8) + number(-8, 8)) + number(1, 4) : (-2 * sApplySkillDamageValue) + number(1, 5);
								
								pItemAward->aAttr[0].bType = APPLY_NORMAL_HIT_DAMAGE_BONUS;
								pItemAward->aAttr[0].sValue = sApplyNormalHitValue;	
								pItemAward->aAttr[1].bType = APPLY_SKILL_DAMAGE_BONUS;
								pItemAward->aAttr[1].sValue = sApplySkillDamageValue;
							}
						}

						// 12.04.2019 - Added support for books. (check skill types, unknown skill)
						// Skill vnum need to be saved into socket0.
						// Random book - INSERT INTO player.item_award(`login`, `vnum`, `count`, `mall`) VALUES ('account', 50300, 1, 1); 
						// Specific book by skill index - INSERT INTO player.item_award(`login`, `vnum`, `count`, `socket0`, `mall`) VALUES ('account', 50300, 1, 4, 1);
						// 	(4=Aura of the Sword), if the skill index is unknown, there will be a random book.
						const bool bIsBookItem = (pItemAward->dwVnum == ITEM_SKILL_VNUM || pItemAward->dwVnum == ITEM_SKILLFORGET_VNUM);
						if (bIsBookItem)
						{
							DWORD dwSocket0SkillVnum = pItemAward->dwSocket0;
							
							// Check if skill index exists in skill table.
							bool bHasSkill = false;
							for (size_t i = 0; i < m_vec_skillTable.size(); ++i)
							{
								if (m_vec_skillTable[i].dwVnum == dwSocket0SkillVnum)
								{
									bHasSkill = true;
									break;
								}
							}

							if (!bHasSkill)
							{
								do
								{
									const TSkillTable * pkSkillTable = &m_vec_skillTable[number(0, m_vec_skillTable.size() - 1)];
									const DWORD dwSkillVnum = pkSkillTable->dwVnum;
									const BYTE bSkillType = pkSkillTable->bType;
									
									// Ignore skills who are PASSIVE, GUILD, SUPPORT.
									// Check if skills types are 1 >> 4 & 7. (WARRIOR, ASSASSIN, SHAMAN, SURA, WOLFMAN)
									const bool bIsPCSkill = (JOB_WARRIOR + 1 <= bSkillType && bSkillType <= JOB_SHAMAN + 1) || bSkillType == 7;
									if (!bIsPCSkill)
										continue;
									
									dwSocket0SkillVnum = dwSkillVnum;
									break;
								}
								while (true);
					
								pItemAward->dwSocket0 = dwSocket0SkillVnum;
							}
						}
						
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

//1.6) Search for:
					item.alSockets[2] = dwSocket2;
//1.6) Add after:
#ifdef ENABLE_EXTEND_ITEM_AWARD
					thecore_memcpy(&item.aAttr, pItemAward->aAttr, sizeof(item.aAttr));
#endif

//1.7) If you use martysama0134 source:
#define ENABLE_ITEMAWARD_REFRESH﻿ // Should be enabled﻿﻿﻿﻿.
//1.1 Search for:
extern int g_iPlayerCacheFlushSeconds;
//1.1 Add before:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
#include <float.h>
inline const double uniform_random(const double a, const double b)
{
	return thecore_random() / (RAND_MAX + 1.f) * (b - a) + a;
}

inline const float GetRandomGaussian(const float avg, const float sigma)
{
	static bool bHasNextGaussian = false;
	static float fNextGaussian = 0.0f;

	if (bHasNextGaussian)
	{
		bHasNextGaussian = false;
		return fNextGaussian * sigma + avg;
	}
	else
	{
		double v1, v2, s;
		do
		{
			v1 = uniform_random(-1.f, 1.f);
			v2 = uniform_random(-1.f, 1.f);
			s = v1 * v1 + v2 * v2;
		} while (s >= 1.f || fabs(s) < FLT_EPSILON);

		const double multiplier = sqrtf(-2 * logf(s) / s);
		fNextGaussian = v2 * multiplier;
		bHasNextGaussian = true;
		return v1 * multiplier * sigma + avg;
	}
}
#endif

//1.2 Search for:
						snprintf(szQuery, sizeof(szQuery),
								"INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2) "
								"VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u)",
								GetTablePostfix(),
								GainItemID(),
								pi->account_id,
								pi->ip[0] == 0 ? "SAFEBOX" : "MALL",
								iPos,
								pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);
//1.2 Replace with:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
						if (pItemTable) 
						{
							// Correction for socket real time and more.
							if (pItemTable->bType == ITEM_UNIQUE || pItemTable->bType == ITEM_USE)
							{
								if (pItemTable->alValues[2] == 0)
									pItemAward->dwSocket2 = pItemTable->alValues[0];
								else
									pItemAward->dwSocket2 = CClientManager::instance().GetCurrentTime() + pItemTable->alValues[0];
							}
							
							// Random average/skill damage bonus for empty values.
							if (pItemTable->sAddonType == - 1) // player.item_proto.addon_type = -1 (Eg. 189, 199, 299, 1139, 1179, 2159, 2179, 3169, 3219, 5119, 5129, 6019, 6069, 6079, 7169)[+0 - +9]
							{
								bool bResultAdd = true;
								for (size_t j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
								{
									const BYTE bType = pItemAward->aAttr[j].bType;
									const short sValue = pItemAward->aAttr[j].sValue;

									if ((bType == APPLY_SKILL_DAMAGE_BONUS || bType == APPLY_NORMAL_HIT_DAMAGE_BONUS) && sValue) 
										bResultAdd = false; 
								}

								if (bResultAdd)
								{
									const int bApplySkillDamageValue = MINMAX(-30, static_cast<int>((GetRandomGaussian(0, 5) + 0.5f)), 30);
									const int sApplyNormalHitValue = abs(bApplySkillDamageValue) <= 20 ? -2 * bApplySkillDamageValue + abs(number(-8, 8) + number(-8, 8)) + number(1, 4) : -2 * bApplySkillDamageValue + number(1, 5);
									
									pItemAward->aAttr[0].bType = APPLY_NORMAL_HIT_DAMAGE_BONUS;
									pItemAward->aAttr[0].sValue = sApplyNormalHitValue;	
									pItemAward->aAttr[1].bType = APPLY_SKILL_DAMAGE_BONUS;
									pItemAward->aAttr[1].sValue = bApplySkillDamageValue;
								}
							}
						}

						snprintf(szQuery, sizeof(szQuery),
								"INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2, "
								"attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6) "
								"VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u, %u, %d, %u, %d, %u, %d, %u, %d, %u, %d, %u, %d, %u, %d)",
						GetTablePostfix(), GainItemID(), pi->account_id, pi->ip[0] == 0 ? "SAFEBOX" : "MALL", iPos, pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2,
							pItemAward->aAttr[0].bType, pItemAward->aAttr[0].sValue, pItemAward->aAttr[1].bType, pItemAward->aAttr[1].sValue, pItemAward->aAttr[2].bType, pItemAward->aAttr[2].sValue,
							pItemAward->aAttr[3].bType, pItemAward->aAttr[3].sValue, pItemAward->aAttr[4].bType, pItemAward->aAttr[4].sValue, pItemAward->aAttr[5].bType, pItemAward->aAttr[5].sValue,
							pItemAward->aAttr[6].bType, pItemAward->aAttr[6].sValue);
#else
						snprintf(szQuery, sizeof(szQuery), "INSERT INTO item%s (id, owner_id, window, pos, vnum, count, socket0, socket1, socket2) VALUES(%u, %u, '%s', %d, %u, %u, %u, %u, %u)"
						GetTablePostfix(), GainItemID(), pi->account_id, pi->ip[0] == 0 ? "SAFEBOX" : "MALL", iPos, pItemAward->dwVnum, pItemAward->dwCount, pItemAward->dwSocket0, pItemAward->dwSocket1, dwSocket2);
#endif

//1.3 Search for:
					item.alSockets[2] = dwSocket2;
//1.3 Replace with:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
					item.alSockets[2] = pItemAward->dwSocket2;
					thecore_memcpy(&item.aAttr, pItemAward->aAttr, sizeof(item.aAttr));
#else
					item.alSockets[2] = dwSocket2;
#endif
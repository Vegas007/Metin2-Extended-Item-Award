//1.1 Search for:
extern int g_iPlayerCacheFlushSeconds;
//1.1 Add before:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
#include <float.h>
inline const double uniform_random(double a, double b)
{
	return thecore_random() / (RAND_MAX + 1.f) * (b - a) + a;
}

inline const float GetRandomGaussian(float avg, float sigma)
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

		double multiplier = sqrtf(-2 * logf(s) / s);
		fNextGaussian = v2 * multiplier;
		bHasNextGaussian = true;
		return v1 * multiplier * sigma + avg;
	}
}
#endif

//1.2 Search for:
					item.alSockets[2] = dwSocket2;
//1.2 Replace with:
#if defined(ENABLE_EXTEND_ITEM_AWARD)
					item.alSockets[2] = pItemAward->dwSocket2;
					thecore_memcpy(&item.aAttr, pItemAward->aAttr, sizeof(item.aAttr));

					// Random hit/skill damage bonus for empty weapons.
					if (pItemTable && pItemTable->sAddonType == - 1) // player.item_proto.addon_type = -1 (Eg. 189, 199, 299, 1139, 1179, 2159, 2179, 3169, 3219, 5119, 5129, 6019, 6069, 6079, 7169)[+0 - +9]
					{
						bool bResultAdd = true;
						for (size_t j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
						{
							const BYTE bType = item.aAttr[j].bType;
							const short sValue = item.aAttr[j].sValue;

							if ((bType == APPLY_SKILL_DAMAGE_BONUS || bType == APPLY_NORMAL_HIT_DAMAGE_BONUS) && sValue) 
								bResultAdd = false; 
						}
						
						int bApplySkillDamageValue = MINMAX(-30, (int)(GetRandomGaussian(0, 5) + 0.5f), 30);
						int sApplyNormalHitValue = abs(bApplySkillDamageValue) <= 20 ? -2 * bApplySkillDamageValue + abs(number(-8, 8) + number(-8, 8)) + number(1, 4) : -2 * bApplySkillDamageValue + number(1, 5);

						if (bResultAdd)
						{
							item.aAttr[0].bType = APPLY_SKILL_DAMAGE_BONUS;
							item.aAttr[0].sValue = bApplySkillDamageValue;
							item.aAttr[1].bType = APPLY_NORMAL_HIT_DAMAGE_BONUS;
							item.aAttr[1].sValue = sApplyNormalHitValue;	
						}
					}
#else
					item.alSockets[2] = dwSocket2;
#endif
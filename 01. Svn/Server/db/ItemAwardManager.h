//1.1) Search for:
#include "Peer.h"
//1.1) Add after:
#ifdef ENABLE_EXTEND_ITEM_AWARD
enum
{
	ITEM_ATTRIBUTE_NONE = -1,
	ITEM_SKILL_VNUM = 50300,
	ITEM_SKILLFORGET_VNUM = 70037,
};
#endif

//1.2) Search for:
    bool	bMall;
//1.2) Add after:
#ifdef ENABLE_EXTEND_ITEM_AWARD
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#endif

//1.3) Search for:
		void				Taken(DWORD dwAwardID, DWORD dwItemID);
//1.3) Add after:
#ifdef ENABLE_EXTEND_ITEM_AWARD
		void 				CheckItemAddonType(TItemAward & pkItemAward, const TItemTable & pkItemTable);
		void				CheckItemCount(TItemAward & pkItemAward, const TItemTable & pkItemTable);
		void				CheckItemBlend(TItemAward & pkItemAward, const TItemTable & pkItemTable);
		void				CheckItemSkillBook(TItemAward & pkItemAward, const std::vector<TSkillTable> vec_skillTable);
#endif
#ifdef USE_ITEM_AWARD_CHECK_ATTRIBUTES
		void				CheckItemAttributes(TItemAward & pkItemAward, const TItemTable & pkItemTable, const std::vector<TItemAttrTable> vec_itemAttrTable);
		int8_t				GetItemAttributeSetIndex(const uint8_t bItemType, const uint8_t bItemSubType) const;
#endif
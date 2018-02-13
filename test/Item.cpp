
#include "Item.h"

namespace driver
{
	ItemData::ItemData()
	{
	}

	ItemData::~ItemData()
	{
	}


	void ItemData::SetItemMask(ItemMaskType eType,bool bFlag)
	{
		switch(eType)
		{
		case ITEM_MASK_INVALID:
		case ITEM_MASK_BIND:
			{
				if(bFlag)
					m_nItemMask |= eType;
				else
					m_nItemMask &= ~eType;
			}
			break;
		default:
			{
				//AssertEx(0,"��Ʒϵͳ���ش������û�ȡ������[%d]",eItemDataMaskType);
			}
		}

		return;
	}
	bool ItemData::GetItemMask(ItemMaskType eType)
	{
		switch(eType)
		{
		case ITEM_MASK_INVALID:
		case ITEM_MASK_BIND:
			return ( m_nItemMask & eType ) != 0 ;
		default:
			{
				//AssertEx(0,"��Ʒϵͳ���ش������û�ȡ������[%d]",eItemDataMaskType);
			}
		}
		return false;
	}
}
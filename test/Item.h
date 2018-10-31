#ifndef _ITEM_H_
#define _ITEM_H_

#include "common/driver.h"

namespace driver
{
//物品动态存储区大小
#define ITEM_DYNAMIC_SIZE		10

	struct ItemStruct
	{
		void Clear()
		{
			m_nID = invalid_id;
		}

		tuint32 m_nID;
		tuint32 m_nMask;
	};

	class ItemData
	{
	private:
		enum ItemMaskType
		{
			ITEM_MASK_INVALID	= 0x00000001,	//是否有效
			ITEM_MASK_BIND		= 0x00000002,	//是否绑定

			ITEM_MASK_MAX		= 0x80000000,	//
		};

	public:
		ItemData();
		virtual ~ItemData();

	public:
		void	SetID(tuint32 nID) {m_nID = nID;};
		tuint32 GetID(){return m_nID;};

		void	SetItemMask(ItemMaskType eType,bool bFlag);
		bool	GetItemMask(ItemMaskType eType);

	private:
		tuint32 m_nID;
		tuint32 m_nDataID;
		tuint32 m_nItemMask;
		tuint8	m_nDataVersion;
		tuint32 m_nDynamicData[ITEM_DYNAMIC_SIZE];
	};

	class ItemInterfaceConst
	{
	public:
		explicit ItemInterfaceConst(const ItemData& rkItem):m_rkItemData(rkItem)
		{
		}

	public:

	private:
		const ItemData& m_rkItemData;
	};

	class ItemInterface : public ItemInterfaceConst
	{
	public:
		explicit ItemInterface(ItemData& rkItem):ItemInterfaceConst(rkItem),m_rkItemData(rkItem)
		{
		}

	public:

	private:
		ItemData& m_rkItemData;
	};

}


#endif //_ITEM_H_
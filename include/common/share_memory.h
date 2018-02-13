#ifndef __SHARE_MEMEORY_H__
#define __SHARE_MEMEORY_H__

#include "share_memory_api.h"

namespace driver { 

	enum Enm_Sharemem_Mode
	{
		ESM_INIT	= 0,	//内存模式：初始化
		ESM_RESUME	= 1,	//内存模式：还原
	};

	class Share_Memory
	{
	public:
		Share_Memory (void);
		~Share_Memory (void);

		//加载共享内存
		bool attach_share_memory (tuint32 uiKey, size_t uiSize);

		//卸载共享内存
		void detach_share_memory();

		// 分配大小为size的内存块，失败返回NULL
		char* create_segment(size_t uiSize);

		inline Enm_Sharemem_Mode get_mode() const
		{
			return m_enmMode;
		}

		// 得到可用空间大小，单位:byte
		inline size_t get_free_size (void) const
		{
			return (m_uiSize - m_uiOffset);
		}

		// 得到已用空间大小，单位:byte
		inline size_t get_used_size (void) const
		{
			return m_uiOffset;
		}

	private:
		tuint32		m_uiKey;
		size_t		m_uiSize;
		size_t		m_uiOffset;

		SMHandle			m_hShareMemHandle;
		char*				m_pMemBase;

		Enm_Sharemem_Mode	m_enmMode;
	};
}

#endif // __SHARE_MEMEORY_H__

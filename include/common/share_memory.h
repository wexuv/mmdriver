#ifndef __SHARE_MEMEORY_H__
#define __SHARE_MEMEORY_H__

#include "share_memory_api.h"

namespace driver { 

	enum Enm_Sharemem_Mode
	{
		ESM_INIT	= 0,	//�ڴ�ģʽ����ʼ��
		ESM_RESUME	= 1,	//�ڴ�ģʽ����ԭ
	};

	class Share_Memory
	{
	public:
		Share_Memory (void);
		~Share_Memory (void);

		//���ع����ڴ�
		bool attach_share_memory (tuint32 uiKey, size_t uiSize);

		//ж�ع����ڴ�
		void detach_share_memory();

		// �����СΪsize���ڴ�飬ʧ�ܷ���NULL
		char* create_segment(size_t uiSize);

		inline Enm_Sharemem_Mode get_mode() const
		{
			return m_enmMode;
		}

		// �õ����ÿռ��С����λ:byte
		inline size_t get_free_size (void) const
		{
			return (m_uiSize - m_uiOffset);
		}

		// �õ����ÿռ��С����λ:byte
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

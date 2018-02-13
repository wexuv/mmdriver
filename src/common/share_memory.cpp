#include "share_memory.h"

namespace driver {

	Share_Memory::Share_Memory (void)
	{
		m_uiKey = 0;
		m_uiSize = 0;
		m_uiOffset = sizeof(*this);

		m_hShareMemHandle = INVALID_SM_HANDLE;
		m_pMemBase = 0;

		m_enmMode = ESM_INIT;
	}

	Share_Memory::~Share_Memory (void)
	{
		// do nothing
	}

	bool Share_Memory::attach_share_memory (tuint32 uiKey, size_t uiSize)
	{
		if (uiSize <= 0)
		{
			return false;
		}

		size_t uiRealSize = uiSize + sizeof(Share_Memory);

		Enm_Sharemem_Mode enmMode = ESM_INIT;

		// 首先去尝试attach指定大小的内存
		SMHandle handle = sharememory_api::OpenShareMem(uiKey, uiRealSize);
		if (INVALID_SM_HANDLE == handle)
		{
			// 创建新共享内存
			handle = sharememory_api::CreateShareMem(uiKey, uiRealSize);
			if (INVALID_SM_HANDLE == handle)
			{
				return false;
			}
		}
		else
		{
			// attach到已有内存区域，并没有真正创建新的内存
			enmMode = ESM_RESUME;
		}

		if(uiKey != m_uiKey || uiRealSize != m_uiSize)
		{
			enmMode = ESM_INIT;
		}

		// attach到自己家先
		m_pMemBase = (char*)sharememory_api::MapShareMem(handle);
		if (0 == m_pMemBase)
		{
			return false;
		}

		m_hShareMemHandle = handle;

		m_uiKey  = uiKey;
		m_uiSize = uiRealSize;

		m_enmMode = enmMode;

		m_uiOffset = sizeof(*this);

		return true;
	}

	void Share_Memory::detach_share_memory()
	{
		if(m_pMemBase != 0)
		{
			sharememory_api::UnMapShareMem(m_pMemBase);
			m_pMemBase = 0;
		}

		if(m_hShareMemHandle != INVALID_SM_HANDLE)
		{
			sharememory_api::CloseShareMem(m_hShareMemHandle);
			m_hShareMemHandle = INVALID_SM_HANDLE;
		}
	}

	char* Share_Memory::create_segment (size_t uiSize)
	{
		if(m_pMemBase == 0)
			return 0;

		if (uiSize <= 0)
		{
			return 0;
		}

		if (uiSize > get_free_size())
		{
			return 0;
		}

		char* mem = m_pMemBase + m_uiOffset;
		m_uiOffset += uiSize;

		return mem;
	}
}

#ifndef _POOL_H_
#define _POOL_H_

#include "types.h"
#include "lib.h"
#include "define.h"
#include "exception.h"

#include <list>
#include <string>

namespace driver
{
	class IPool
	{
	public:
		typedef std::list<IPool*> PoolManager;
	public:
		virtual tint32 alloc_acount() = 0;
		virtual tint32 pool_size() = 0;
		virtual tint32 pool_used() = 0;
		virtual tint32 pool_obj_size() = 0;
		virtual const tchar* pool_name() = 0;
	public:

		static PoolManager& instance()
		{
			static PoolManager m_PoolManager;
			return m_PoolManager;
		}
	protected:
		IPool()
		{
			instance().push_back(this);
		};

		~IPool() 
		{
			int a = 0;
		};
	};

	template <typename Obj>
	class PoolObj;

	template <typename Obj>
	class Pool : public IPool
	{
		friend class PoolObj<Obj>;

		typedef std::vector<void*>	MemCache;
		typedef MemCache::iterator	MemCacheIter;

	public:
		static Pool<Obj>& instance();

		virtual tint32 alloc_acount();

		virtual tint32 pool_size();

		virtual tint32 pool_used();

		virtual tint32 pool_obj_size();

		virtual const tchar* pool_name();

	private:
		Pool();
		~Pool();

	protected:
		static void* alloc(size_t size);
		static void free(void* p);

	private:
		void* _alloc(size_t size);
		void _free(void* p);

	private:
		MemCache	m_MemCache;
		tint32		m_nAllocCount;
		tint32		m_nPoolSize;
		tint32		m_nPoolUsed;
		std::string m_ObjClassName;
	};

	template <typename Obj>
	class PoolObj
	{
	public:
		void* operator new(size_t size)
		{
			return Pool<Obj>::alloc(size);
		}

		void operator delete(void* p)
		{
			Pool<Obj>::free(p);
		}
	};

}

#endif //_POOL_H_
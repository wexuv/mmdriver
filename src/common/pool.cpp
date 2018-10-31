
#include "common/pool.h"
#include "common/lib.h"

namespace driver
{
	template <typename Obj>
	Pool<Obj>::Pool()
	{
		m_ObjClassName = typeid(Obj).name();
		m_nAllocCount = 0;
		m_nPoolSize = 0;
		m_nPoolUsed = 0;

		m_MemCache.clear();
	};

	template <typename Obj>
	Pool<Obj>::~Pool() 
	{
		if(m_MemCache.size() != m_nPoolSize)
		{
			//
		}

		for(MemCacheIter iter = m_MemCache.begin(); iter != m_MemCache.end(); ++ iter)
		{
			::operator delete(*iter);
		}

		m_MemCache.clear();
	};

	template <typename Obj>
	Pool<Obj>& Pool<Obj>::instance()
	{
		static Pool<Obj> s_Pool;

		return s_Pool;
	}

	template <typename Obj>
	void* Pool<Obj>::alloc(size_t size)
	{
		return Pool<Obj>::instance()._alloc(size);
	}

	template <typename Obj>
	void Pool<Obj>::free(void* p)
	{
		return Pool<Obj>::instance()._free(p);
	}

	template <typename Obj>
	void* Pool<Obj>::_alloc(size_t size)
	{
		//不允许多继承，会导致分配大小不同
		AssertEx(pool_obj_size() == size,"");

		++m_nAllocCount;

		if(!m_MemCache.empty())
		{
			void* p = m_MemCache.back();
			m_MemCache.pop_back();

			++m_nPoolUsed;

			return p;
		}

		void *p = ::operator new(size);

		++m_nPoolUsed;
		++m_nPoolSize;

		return p;
	}

	template <typename Obj>
	void Pool<Obj>::_free(void* p)
	{
		m_MemCache.push_back(p);

		--m_nPoolUsed;
	}

	template <typename Obj>
	tint32 Pool<Obj>::alloc_acount()
	{
		return m_nAllocCount;
	}

	template <typename Obj>
	tint32 Pool<Obj>::pool_size()
	{
		return m_nPoolSize;
	};

	template <typename Obj>
	tint32 Pool<Obj>::pool_used()
	{
		return m_nPoolUsed;
	}

	template <typename Obj>
	tint32 Pool<Obj>::pool_obj_size()
	{
		return sizeof(Obj);
	}

	template <typename Obj>
	const tchar* Pool<Obj>::pool_name()
	{
		return m_ObjClassName.c_str();
	}

	//class Test : public PoolObj<Test>
	//{
	//public:
	//	Test()
	//	{
	//		a++;
	//	}
	//public:
	//	static int a;
	//	int b;
	//};
	//int Test::a = 0;

	//class TestObj : public Test,PoolObj<TestObj>
	//{
	//};

	//class Test2 {};
	//class Test2Obj : public Test2,public PoolObj<Test2Obj> {};

	//class Test3 : public Test2
	//{
	//	int a;
	//	int b;
	//	int c;
	//	int d;
	//};
	//class Test3Obj : public Test3,public PoolObj<Test3Obj> {};

	//class Test4
	//{
	//public:
	//	int a;
	//	int b;
	//	int c;
	//	int d;
	//};

	//class B
	//{
	//public:
	//	B()
	//	{
	//		Test* p = new Test;
	//		delete p;

	//		//PoolObj *p1 = new PoolObj;
	//		//delete p1;

	//		std::shared_ptr<Test> p1(new Test);

	//		std::shared_ptr<Test3> p3(new Test3);

	//		Test2* p2 = new Test2;
	//		delete p2;

	//		Test3Obj* t3 = NULL;
	//		DWORD now = GetTickCount();
	//		for(int i = 0; i < 2000000; ++i)
	//		{
	//			t3 = new Test3Obj();

	//			Test3Obj* t33 = new Test3Obj();

	//			//delete t3;
	//			//delete t33;
	//		}
	//		printf("%d\n",GetTickCount()-now);

	//		Test4* t4 = NULL;
	//		now = GetTickCount();
	//		for(int i = 0; i < 2000000; ++i)
	//		{
	//			t4 = new Test4();
	//			Test4* t44 = new Test4();

	//			delete t4;
	//			delete t44;
	//		}
	//		printf("%d\n",GetTickCount()-now);

	//		now = GetTickCount();
	//		for(int i = 0; i < 2000000; ++i)
	//		{
	//			t3 = new Test3Obj();
	//			Test3Obj* t33 = new Test3Obj();

	//			delete t3;
	//			delete t33;
	//		}
	//		printf("%d\n",GetTickCount()-now);


	//		IPool* pPool = &(Pool<Test3>::instance());

	//		printf("%s,alloc_count:%d,pool size:%d,pool used:%d,pool obj size:%d,construct:%d\n",
	//			pPool->pool_name(),pPool->alloc_acount(),pPool->pool_size(),pPool->pool_used(),pPool->pool_obj_size(),p1->a);

	//		IPool::PoolManager& rManager = IPool::instance();

	//		for(IPool::PoolManager::iterator iter = rManager.begin(); iter != rManager.end(); ++ iter)
	//		{
	//			printf("%s,alloc_count:%d,pool size:%d,pool used:%d,pool obj size:%d,construct:%d\n",
	//				(*iter)->pool_name(),(*iter)->alloc_acount(),(*iter)->pool_size(),(*iter)->pool_used(),(*iter)->pool_obj_size(),p1->a);
	//		}
	//		int a = 0;
	//	}
	//};

	//class C
	//{
	//public:
	//	C()
	//	{
	//		B* b = new B();
	//		delete b;
	//	}
	//	~C()
	//	{
	//		B* b = new B();
	//		delete b;
	//	}
	//};

	//C c;
}
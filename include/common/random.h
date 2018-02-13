#ifndef _RANDOM_H_
#define _RANDOM_H_

namespace driver 
{
	//实现按照权重随机
	templete<class TKEY,class TWEIGHT>
	class random_with_weight
	{
	public:
		random_with_weight();
		~random_with_weight();

	public:
		void push(TKEY key,TWEIGHT weight)
		{
			m_container.push_back(key,weight);
		}

		TKEY random()
		{
			tint32 nRandom = Gen::Random(0,weight);
			std::map::iterator = m_container.begin();
		}

	private:
		std::map<TKEY,TWEIGHT>	m_container;
	};

}

#endif //_RANDOM_H_
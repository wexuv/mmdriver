#ifndef _STD_EX_H_
#define _STD_EX_H_

#include "types.h"
#include "exception.h"

namespace driver
{

	//boundary safe vector
	template<typename _ElemType>
	class bsvector : public std::vector<_ElemType>
	{
	public:
		typedef std::vector<_ElemType>	_BaseClass;
		typedef bsvector<_ElemType>		_MyselfClass;

		typedef _ElemType&				_ElemRef;
		typedef const _ElemType&		_ConstElemRef;

		typedef typename _BaseClass::size_type	size_type;

	public:
		bsvector<_ElemType>(void) : _BaseClass() {}
		bsvector<_ElemType>(const _MyselfClass& rSrc) : _BaseClass(static_cast<const _BaseClass&>(rSrc)) {}
		~bsvector<_ElemType>(void) {}
		_MyselfClass& operator = (const _MyselfClass& rSrc)
		{
			_BaseClass::operator = (static_cast<const _BaseClass&>(rSrc));
			return (*this);
		}

	public:
		_ElemRef operator[](size_type _i)
		{
			AssertEx(_i >= 0 && _i < _BaseClass::size(), "bsvector");
			return _BaseClass::operator [](_i);
		}

		_ConstElemRef operator[](size_type _i) const
		{
			AssertEx(_i >= 0 && _i < _BaseClass::size(), "bsvector");
			return _BaseClass::operator [](_i);
		}
	};
}
#endif //_STD_EX_H_
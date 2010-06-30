/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Framework::Entity

Classes:
	Linear										������ֵ
***********************************************************************/

#ifndef VCZH_ENTITY_LINEAR
#define VCZH_ENTITY_LINEAR

#include "..\Collections\Dictionary.h"
#include "..\Stream\StreamSerialization.h"
#include "..\Collections\OperationCopyFrom.h"
#include "..\Collections\OperationEnumerable.h"

namespace vl
{
	template<typename K, typename V>
	class Linear
	{
	protected:
		collections::Dictionary<K, V>		factors;
		V									constant;
	public:
		Linear()
		{
		}

		Linear(const V& value)
			:constant(value)
		{
		}

		Linear(const Linear<K, V>& value)
			:constant(value.constant)
		{
			collections::CopyFrom(factors.Wrap(), value.factors.Wrap());
		}

		Linear& operator=(const V& value)
		{
			factors.Clear();
			constant=value;
			return *this;
		}

		Linear& operator=(const Linear<K, V>& value)
		{
			collections::CopyFrom(factors.Wrap(), value.factors.Wrap());
			constant=value.constant;
			return *this;
		}

		Linear<K, V>& operator()(const K& k, const V& v)
		{
			factors.Set(k, v);
			return *this;
		}

		Linear<K, V>& operator()(const V& v)
		{
			constant=v;
			return *this;
		}

		const V& Constant()
		{
			return constant;
		}

		const V& Factor(const K& k)
		{
			return factors[k];
		}

		const V& Factor(const K& k, const V& v)
		{
			vint index=factors.Keys().IndexOf(k);
			return index==-1?v:factors.Values()[index];
		}

/***********************************************************************
�ӷ�
***********************************************************************/

		friend Linear operator+(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}

		friend Linear operator+(const Linear<K, V>& a, const V& b)
		{
		}

		friend Linear operator+(const V& a, const Linear<K, V>& b)
		{
		}

		Linear& operator+=(const Linear<K, V>& value)
		{
		}

		Linear& operator+=(const V& value)
		{
		}

/***********************************************************************
����
***********************************************************************/

		friend Linear operator-(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}

		friend Linear operator-(const Linear<K, V>& a, const V& b)
		{
		}

		friend Linear operator-(const V& a, const Linear<K, V>& b)
		{
		}

		Linear& operator-=(const Linear<K, V>& value)
		{
		}

		Linear& operator-=(const V& value)
		{
		}

/***********************************************************************
�˷�
***********************************************************************/

		friend Linear operator*(const Linear<K, V>& a, const V& b)
		{
		}

		friend Linear operator*(const V& a, const Linear<K, V>& b)
		{
		}

		Linear& operator*=(const V& value)
		{
		}

/***********************************************************************
����
***********************************************************************/

		friend V operator/(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}

		friend Linear operator/(const Linear<K, V>& a, const V& b)
		{
		}

		Linear& operator/=(const V& value)
		{
		}

/***********************************************************************
����
***********************************************************************/

		friend Linear operator%(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}

		friend Linear operator%(const Linear<K, V>& a, const V& b)
		{
		}

		Linear& operator%=(const Linear<K, V>& value)
		{
		}

		Linear& operator%=(const V& value)
		{
		}

/***********************************************************************
�Ƚ�
***********************************************************************/

		friend bool operator==(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return a.constant==b.constant && collections::CompareEnumerable(a.factors.Wrap(), b.factors.Wrap())==0;
		}

		friend bool operator==(const Linear<K, V>& a, const V& b)
		{
			return a.constant==b && a.factors.Count()==0;
		}

		friend bool operator==(const V& a, const Linear<K, V>& b)
		{
			return b.constant==a && b.factors.Count()==0;
		}

		friend bool operator!=(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return a.constant!=b.constant || collections::CompareEnumerable(a.factors.Wrap(), b.factors.Wrap())!=0;
		}

		friend bool operator!=(const Linear<K, V>& a, const V& b)
		{
			return a.constant!=b || a.factors.Count()!=0;
		}

		friend bool operator!=(const V& a, const Linear<K, V>& b)
		{
			return b.constant!=a || b.factors.Count()!=0;
		}
	};
}

#endif
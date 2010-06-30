/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Entity

Classes:
	Linear										：线性值
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
加法
***********************************************************************/

		template<typename K, typename V>
		friend Linear<K, V> operator+(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return Linear<K, V>(a)+=b;
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator+(const Linear<K, V>& a, const V& b)
		{
			return Linear<K, V>(a)+=b;
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator+(const V& a, const Linear<K, V>& b)
		{
			return Linear<K, V>(b)+=a;
		}

		Linear<K, V>& operator+=(const Linear<K, V>& value)
		{
			for(vint i=0;i<value.factors.Count();i++)
			{
				const K& k=value.factors.Keys()[i];
				const V& v=value.factors.Values()[i];
				vint index=factors.Keys().IndexOf(k);
				if(index==-1)
				{
					factors.Add(k, v);
				}
				else
				{
					factors.Set(k, v+factors.Values()[index]);
				}
			}
			constant+=value.constant;
			return *this;
		}

		Linear<K, V>& operator+=(const V& value)
		{
			constant+=value;
			return *this;
		}

/***********************************************************************
减法
***********************************************************************/

		template<typename K, typename V>
		friend Linear<K, V> operator-(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return Linear<K, V>(a)-=b;
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator-(const Linear<K, V>& a, const V& b)
		{
			return Linear<K, V>(a)-=b;
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator-(const V& a, const Linear<K, V>& b)
		{
			return (-Linear<K, V>(b))(a-b.constant);
		}

		Linear<K, V>& operator-=(const Linear<K, V>& value)
		{
			for(vint i=0;i<value.factors.Count();i++)
			{
				const K& k=value.factors.Keys()[i];
				const V& v=value.factors.Values()[i];
				vint index=factors.Keys().IndexOf(k);
				if(index==-1)
				{
					factors.Add(k, -v);
				}
				else
				{
					factors.Set(k, factors.Values()[index]-v);
				}
			}
			constant-=value.constant;
			return *this;
		}

		Linear<K, V>& operator-=(const V& value)
		{
			constant-=value;
			return *this;
		}

		Linear<K, V> operator-()
		{
			Linear<K, V> result(-constant);
			for(vint i=0;i<factors.Count();i++)
			{
				result.factors.Add(factors.Keys()[i], -factors.Values()[i]);
			}
			return result;
		}

/***********************************************************************
乘法
***********************************************************************/
		
		template<typename K, typename V>
		friend Linear<K, V> operator*(const Linear<K, V>& a, const V& b)
		{
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator*(const V& a, const Linear<K, V>& b)
		{
		}

		Linear<K, V>& operator*=(const V& value)
		{
		}

/***********************************************************************
除法
***********************************************************************/
		
		template<typename K, typename V>
		friend V operator/(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator/(const Linear<K, V>& a, const V& b)
		{
		}

		Linear<K, V>& operator/=(const V& value)
		{
		}

/***********************************************************************
求余
***********************************************************************/
		
		template<typename K, typename V>
		friend Linear<K, V> operator%(const Linear<K, V>& a, const Linear<K, V>& b)
		{
		}
		
		template<typename K, typename V>
		friend Linear<K, V> operator%(const Linear<K, V>& a, const V& b)
		{
		}

		Linear<K, V>& operator%=(const Linear<K, V>& value)
		{
		}

		Linear<K, V>& operator%=(const V& value)
		{
		}

/***********************************************************************
比较
***********************************************************************/
		
		template<typename K, typename V>
		friend bool operator==(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return a.constant==b.constant && collections::CompareEnumerable(a.factors.Wrap(), b.factors.Wrap())==0;
		}
		
		template<typename K, typename V>
		friend bool operator==(const Linear<K, V>& a, const V& b)
		{
			return a.constant==b && a.factors.Count()==0;
		}
		
		template<typename K, typename V>
		friend bool operator==(const V& a, const Linear<K, V>& b)
		{
			return b.constant==a && b.factors.Count()==0;
		}
		
		template<typename K, typename V>
		friend bool operator!=(const Linear<K, V>& a, const Linear<K, V>& b)
		{
			return a.constant!=b.constant || collections::CompareEnumerable(a.factors.Wrap(), b.factors.Wrap())!=0;
		}
		
		template<typename K, typename V>
		friend bool operator!=(const Linear<K, V>& a, const V& b)
		{
			return a.constant!=b || a.factors.Count()!=0;
		}
		
		template<typename K, typename V>
		friend bool operator!=(const V& a, const Linear<K, V>& b)
		{
			return b.constant!=a || b.factors.Count()!=0;
		}
	};
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Combinator::Input

Classes:
	StringInput<T>							：字符串输入
	EnumerableInput<T>						：迭代器输入
	TokenInput<T>							：记号列表输入

Parser Input Schema:
	class Input<T>
	{
	public:
		typedef T ElementType;

		const T& Current()const;
		bool Available()const;
		bool Next();
		vint Index()const;

		Input(const Input<T>& input);
		bool operator > < == >= <= != (const Input<T>& input)const;
		Input<T>& operator=(const Input<T>& input);
	};

	Input只跟自己复制出来的其他Input比较
	Available()==false之后不会再调用Next()
***********************************************************************/

#ifndef VCZH_COMBINATOR_PARSERINPUT
#define VCZH_COMBINATOR_PARSERINPUT

#include "..\Collections\Interfaces.h"
#include "..\Pointer.h"

namespace vl
{
	namespace combinator
	{
		template<typename T>
		class StringInput : public Object
		{
		public:
			typedef T		ElementType;
		protected:
			ObjectString<T>	input;
			const T*		start;
			const T*		current;
			vint				length;
		public:
			StringInput(const T* _input)
				:input(_input)
			{
				start=input.Buffer();
				current=input.Buffer();
				length=input.Length();
			}

			StringInput(const ObjectString<T>& _input)
				:input(_input)
				,start(_input.Buffer())
				,current(_input.Buffer())
				,length(_input.Length())
			{
			}

			StringInput(const StringInput<T>& _input)
				:input(_input.input)
				,start(_input.start)
				,current(_input.current)
				,length(_input.length)
			{
			}

			const T* Buffer()const
			{
				return current;
			}

			vint Length()const
			{
				return length;
			}

			void Step(vint count)
			{
				current+=count;
				if(current-start>length)
				{
					current=start+length;
				}
			}

			ObjectString<T> CurrentString()const
			{
				if(*current)
				{
					return input.Right(length-(current-start));
				}
				else
				{
					return ObjectString<T>();
				}
			}

			const T& Current()const
			{
				return *current;
			}

			bool Available()const
			{
				return *current!=0;
			}

			bool Next()
			{
				if(Available())
				{
					current++;
				}
				return Available();
			}

			vint Index()const
			{
				return current-start;
			}

			StringInput<T>& operator=(const StringInput<T>& _input)
			{
				input=_input.input;
				start=_input.start;
				current=_input.current;
				length=_input.length;
				return *this;
			}

			bool operator>(const StringInput<T>& input)const
			{
				return current>input.current;
			}

			bool operator>=(const StringInput<T>& input)const
			{
				return current>=input.current;
			}

			bool operator<(const StringInput<T>& input)const
			{
				return current<input.current;
			}

			bool operator<=(const StringInput<T>& input)const
			{
				return current<=input.current;
			}

			bool operator==(const StringInput<T>& input)const
			{
				return current==input.current;
			}

			bool operator!=(const StringInput<T>& input)const
			{
				return current!=input.current;
			}
		};
		
		template<typename T>
		class EnumerableInput : public Object
		{
		public:
			typedef T		ElementType;
		protected:
			Ptr<collections::IEnumerator<T>>	enumerator;
		public:
			EnumerableInput(const collections::IEnumerable<T>& enumerable)
				:enumerator(enumerable.CreateEnumerator())
			{
			}

			EnumerableInput(const EnumerableInput<T>& input)
				:enumerator(input.enumerator->Clone())
			{
			}

			const T& Current()const
			{
				return enumerator->Current();
			}

			bool Available()const
			{
				return enumerator->Available();
			}

			bool Next()
			{
				return enumerator->Next();
			}

			vint Index()const
			{
				return enumerator->Index();
			}

			EnumerableInput<T>& operator=(const EnumerableInput<T>& input)
			{
				enumerator=input.enumerator->Clone();
				return *this;
			}

			bool operator>(const EnumerableInput<T>& input)const
			{
				return Index()>input.Index();
			}

			bool operator>=(const EnumerableInput<T>& input)const
			{
				return Index()>=input.Index();
			}

			bool operator<(const EnumerableInput<T>& input)const
			{
				return Index()<input.Index();
			}

			bool operator<=(const EnumerableInput<T>& input)const
			{
				return Index()<=input.Index();
			}

			bool operator==(const EnumerableInput<T>& input)const
			{
				return Index()==input.Index();
			}

			bool operator!=(const EnumerableInput<T>& input)const
			{
				return Index()!=input.Index();
			}
		};
		
		template<typename T>
		class TokenInput : public Object
		{
		protected:
			T*				tokens;
			vint				current;
			vint				count;
		public:
			TokenInput(T* _tokens, vint _count)
				:tokens(_tokens)
				,current(0)
				,count(_count)
			{
			}

			TokenInput(const TokenInput& input)
				:tokens(input.tokens)
				,current(input.current)
				,count(input.count)
			{
			}

			T Current()const
			{
				return tokens[current];
			}

			bool Available()const
			{
				return current<count;
			}

			bool Next()
			{
				current++;
				return Available();
			}

			vint Index()const
			{
				return current;
			}

			TokenInput& operator=(const TokenInput& input)
			{
				tokens=input.tokens;
				current=input.current;
				count=input.count;
				return *this;
			}

			bool operator>(const TokenInput& input)const
			{
				return current>input.current;
			}

			bool operator>=(const TokenInput& input)const
			{
				return current>=input.current;
			}

			bool operator<(const TokenInput& input)const
			{
				return current<input.current;
			}

			bool operator<=(const TokenInput& input)const
			{
				return current<=input.current;
			}

			bool operator==(const TokenInput& input)const
			{
				return current==input.current;
			}

			bool operator!=(const TokenInput& input)const
			{
				return current!=input.current;
			}
		};
	}
}

#endif
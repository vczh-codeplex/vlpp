/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Combinator::Framework

Classes:
	Types<I>								：各种类型常量
	Types<I>::GlobalInfo					：全局数据基类
	CombinatorError<I>						：组合子错误
	ParsingResult<O>						：组合子返回结果
	Combinator<I, O>						：组合子基类

***********************************************************************/

#ifndef VCZH_COMBINATOR_EXPRESSION
#define VCZH_COMBINATOR_EXPRESSION

#include "..\Collections\List.h"
#include "..\Exception.h"
#include "..\Pointer.h"
#include "..\Function.h"
#include "..\Collections\OperationCopyFrom.h"
#include "..\Collections\OperationAggregate.h"
#include "..\Collections\OperationSelect.h"

namespace vl
{
	namespace combinator
	{
		template<typename I>
		class CombinatorError;

		template<typename I>
		class Types
		{
		private:
			Types()
			{
			}
		public:
			typedef I									Input;
			typedef CombinatorError<I>					Error;
			typedef Ptr<Error>							ErrorRef;
			typedef collections::List<ErrorRef>			ErrorList;

			class GlobalInfo : public Object
			{
			protected:
				static vint GetInputFromError(ErrorRef error)
				{
					return error->GetPosition().Index();
				}

				vint GetMaxErrorPosition(const ErrorList& errors)
				{
					return errors.Wrap()>>collections::Select(GetInputFromError)>>collections::Max();
				}
			public:
				typedef Ptr<GlobalInfo>					Ref;

				ErrorList					errors;
				ErrorList					candidateErrors;

				void Append(const GlobalInfo& info)
				{
					collections::CopyFrom(errors.Wrap(), info.errors.Wrap(), true);
					if(info.candidateErrors.Count()!=0)
					{
						if(candidateErrors.Count()==0)
						{
							collections::CopyFrom(candidateErrors.Wrap(), info.candidateErrors.Wrap(), false);
						}
						else if(GetMaxErrorPosition(candidateErrors) < GetMaxErrorPosition(info.candidateErrors))
						{
							collections::CopyFrom(candidateErrors.Wrap(), info.candidateErrors.Wrap(), false);
						}
					}
				}

				void CandidateAppend(const GlobalInfo& info)
				{
					if(info.errors.Count()==0)
					{
						collections::CopyFrom(candidateErrors.Wrap(), info.candidateErrors.Wrap(), false);
					}
					else if(info.candidateErrors.Count()==0)
					{
						collections::CopyFrom(candidateErrors.Wrap(), info.errors.Wrap(), false);
					}
					else if(GetMaxErrorPosition(info.errors) < GetMaxErrorPosition(info.candidateErrors))
					{
						collections::CopyFrom(candidateErrors.Wrap(), info.candidateErrors.Wrap(), false);
					}
					else
					{
						collections::CopyFrom(candidateErrors.Wrap(), info.errors.Wrap(), false);
					}
				}
			};
		};

		template<typename I>
		class CombinatorError : public Exception
		{
		public:
			typedef typename Types<I>::Input			InputType;
		protected:
			InputType						position;

		public:
			CombinatorError(const WString& _message, InputType _position)
				:Exception(_message)
				,position(_position)
			{
			}

			const InputType& GetPosition()
			{
				return position;
			}
		};

		template<typename O>
		class ParsingResult
		{
		protected:
			O								value;
			bool							isNull;
		public:
			ParsingResult(const O& _value)
				:value(_value)
				,isNull(false)
			{
			}

			ParsingResult(const ParsingResult<O>& result)
				:value(result.value)
				,isNull(result.isNull)
			{
			}

			ParsingResult()
				:isNull(true)
			{
			}

			ParsingResult<O>& operator=(const ParsingResult<O>& result)
			{
				value=result.value;
				isNull=result.isNull;
				return *this;
			}

			const O& Value()const
			{
				return value;
			}

			operator bool()const
			{
				return !isNull;
			}
		};

		template<typename T1, typename T2>
		class ParsingPair
		{
		protected:
			T1								first;
			T2								second;
		public:
			ParsingPair()
			{
			}

			ParsingPair(const T1& _first, const T2& _second)
				:first(_first)
				,second(_second)
			{
			}

			const T1& First()const
			{
				return first;
			}

			const T2& Second()const
			{
				return second;
			}

			bool operator==(const ParsingPair<T1, T2>& pair)const
			{
				return first==pair.first && second==pair.second;
			}

			bool operator!=(const ParsingPair<T1, T2>& pair)const
			{
				return first!=pair.first || second!=pair.second;
			}
		};

		template<typename T>
		class ParsingList : public Object, public collections::IEnumerable<T>
		{
		public:
			class Node
			{
				friend class ParsingList<T>;
			public:
				typedef Ptr<Node>		Ref;
			protected:
				T							value;
				Ref							next;
			public:
				Node(const T& _value)
					:value(_value)
				{
				}

				const T& Value()const
				{
					return value;
				}

				Ref Next()const
				{
					return next;
				}
			};
		protected:
			class Enumerator : public Object, public collections::IEnumerator<T>
			{
			protected:
				typename Node::Ref			head;
				typename Node::Ref			current;
				vint							index;

				Enumerator(const typename Node::Ref& _head, const typename Node::Ref& _current, vint _index)
					:head(_head)
					,current(_current)
					,index(_index)
				{
				}
			public:
				Enumerator(const typename Node::Ref& _head)
					:head(_head)
					,current(_head)
					,index(0)
				{
				}

				~Enumerator()
				{
					current=0;
					while(head)
					{
						head=head->Next();
					}
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(head, current, index);
				}

				const T& Current()const
				{
					return current->Value();
				}

				vint Index()const
				{
					return index;
				}

				bool Next()
				{
					if(current)
					{
						current=current->Next();
						index++;
					}
					return current;
				}

				bool Available()const
				{
					return current;
				}

				void Reset()
				{
					current=head;
					index=0;
				}
			};

			typename Node::Ref				head;
			typename Node::Ref				tail;
			vint								count;
		public:
			ParsingList()
				:count(0)
			{
			}

			ParsingList(const ParsingList<T>& list)
				:head(list.head)
				,tail(list.tail)
				,count(list.count)
			{
			}

			~ParsingList()
			{
				tail=0;
				while(head)
				{
					head=head->Next();
				}
			}

			ParsingList<T>& operator=(const ParsingList<T>& list)
			{
				head=list.head;
				tail=list.tail;
				count=list.count;
				return *this;
			}

			const typename Node::Ref& Head()const
			{
				return head;
			}

			const typename Node::Ref& Tail()const
			{
				return tail;
			}

			vint Count()const
			{
				return count;
			}

			void Append(const T& value)
			{
				typename Node::Ref node=new Node(value);
				if(head)
				{
					tail->next=node;
					tail=node;
				}
				else
				{
					head=tail=node;
				}
				count++;
			}

			collections::IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(head);
			}
		};

		template<typename I, typename O>
		class Combinator : public Object, private NotCopyable
		{
		public:
			typedef typename Types<I>::Input			InputType;
			typedef typename Types<I>::Error			ErrorType;
			typedef typename Types<I>::ErrorRef			ErrorRefType;
			typedef typename Types<I>::ErrorList		ErrorListType;
			typedef typename Types<I>::GlobalInfo		GlobalInfoType;
			typedef O									OutputType;
			typedef Ptr<Combinator<I, O>>				Ref;

			virtual ParsingResult<OutputType>			Parse(InputType& input, GlobalInfoType& globalInfo)const=0;
		};
	}
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Combinator::Combinator

Classes:
	CombinatorException<I>		：分析器语法分析错误
	Node<I, O>					：分析器节点
	Rule<I, O>					：分析器规则

	+a							：一个或多个重复
	*a							：零个或多个重复
	a | b						：选择匹配
	a + b						：连续匹配
	a >> b						：连续匹配并忽略左操作数结果
	a << b						：连续匹配并忽略右操作数结果
	a(f)						：错误可恢复匹配
	a[f]						：结果转换匹配
	opt(a)						：零个或一个重复
	unit<I>(v)					：记号值匹配
	pred<I>(f)					：记号函数匹配
***********************************************************************/

#ifndef VCZH_COMBINATOR_COMBINATOR
#define VCZH_COMBINATOR_COMBINATOR

#include "Expression.h"
#include "ParserInput.h"
#include "_Unit.h"
#include "_Pred.h"
#include "_Seq.h"
#include "_Alt.h"
#include "_Loop.h"
#include "_Using.h"
#include "_Error.h"
#include "_Ref.h"

namespace vl
{
	namespace combinator
	{

/***********************************************************************
Exception
***********************************************************************/

		template<typename I>
		class CombinatorException : public Exception
		{
		protected:
			I									input;
			typename Types<I>::GlobalInfo		globalInfo;
		public:
			CombinatorException(const I& _input, const typename Types<I>::GlobalInfo& _globalInfo)
				:Exception(L"发生Combinator语法分析错误。")
				,input(_input)
			{
				globalInfo.Append(_globalInfo);
			}

			CombinatorException(const CombinatorException<I>& exception)
				:input(exception.input)
			{
				globalInfo.Append(exception.globalInfo);
			}

			const I& GetInput()const
			{
				return input;
			}

			const typename Types<I>::GlobalInfo& GetGlobalInfo()const
			{
				return globalInfo;
			}
		};

/***********************************************************************
Node
***********************************************************************/

		template<typename I, typename O>
		class Node : public Object
		{
		protected:
			typename Combinator<I, O>::Ref		element;
		public:
			Node(const typename Combinator<I, O>::Ref _element)
				:element(_element)
			{
			}

			Node(const Node<I, O>& node)
				:element(node.element)
			{
			}

			Node()
			{
			}

			Node<I, O>& operator=(const Node<I, O>& node)
			{
				element=node.element;
				return *this;
			}

			virtual typename Combinator<I, O>::Ref GetCombinator()const
			{
				return element;
			}

			ParsingResult<O> Parse(I& input, typename Types<I>::GlobalInfo& globalInfo)const
			{
				return element->Parse(input, globalInfo);
			}

			O Parse(const I& input, bool allowError, I* remain=0)const
			{
				Types<I>::GlobalInfo globalInfo;
				I trackingInput=input;
				ParsingResult<O> result=Parse(trackingInput, globalInfo);
				if(result && (allowError || globalInfo.errors.Count()==0))
				{
					if(remain)
					{
						*remain=trackingInput;
					}
					return result.Value();
				}
				else
				{
					throw CombinatorException<I>(trackingInput, globalInfo);
				}
			}

			O ParseFull(const I& input, bool allowError)const
			{
				Types<I>::GlobalInfo globalInfo;
				I trackingInput=input;
				ParsingResult<O> result=Parse(trackingInput, globalInfo);
				if(result && !trackingInput.Available() && (allowError || globalInfo.errors.Count()==0))
				{
					return result.Value();
				}
				else
				{
					if(trackingInput.Available() && globalInfo.candidateErrors.Count())
					{
						CopyFrom(globalInfo.errors.Wrap(), globalInfo.candidateErrors.Wrap());
					}
					throw CombinatorException<I>(trackingInput, globalInfo);
				}
			}

			Node<I, ParsingList<O>> operator+()const
			{
				return new _Loop<I, O>(GetCombinator(), 1);
			}

			Node<I, ParsingList<O>> operator*()const
			{
				return new _Loop<I, O>(GetCombinator(), 0);
			}

			Node<I, O> operator|(const Node<I, O>& second)const
			{
				return new _Alt<I, O>(GetCombinator(), second.GetCombinator());
			}

			template<typename O2>
			Node<I, ParsingPair<O, O2>> operator+(const Node<I, O2>& second)const
			{
				return new _Seq<I, O, O2>(GetCombinator(), second.GetCombinator());
			}

			template<typename O2>
			Node<I, O2> operator>>(const Node<I, O2>& second)const
			{
				return new _Right<I, O, O2>(GetCombinator(), second.GetCombinator());
			}

			template<typename O2>
			Node<I, O> operator<<(const Node<I, O2>& second)const
			{
				return new _Left<I, O, O2>(GetCombinator(), second.GetCombinator());
			}

			Node<I, O> operator()(ParsingResult<O>(*recoverer)(I&, typename Types<I>::GlobalInfo&))const
			{
				return new _Error<I, O>(GetCombinator(), recoverer);
			}

			Node<I, O> operator()(const Func<ParsingResult<O>(I&, typename Types<I>::GlobalInfo&)>& recoverer)const
			{
				return new _Error<I, O>(GetCombinator(), recoverer);
			}

			template<typename O2>
			Node<I, O2> operator[](O2(*converter)(const O&))const
			{
				return new _Using<I, O, O2>(GetCombinator(), converter);
			}

			template<typename O2>
			Node<I, O2> operator[](const Func<O2(const O&)>& converter)const
			{
				return new _Using<I, O, O2>(GetCombinator(), converter);
			}
		};

		template<typename I, typename O>
		Node<I, ParsingList<O>> opt(const Node<I, O>& node)
		{
			return new _Loop<I, O>(node.GetCombinator(), 0, 1);
		}

		template<typename I, typename O>
		Node<I, ParsingList<O>> loop(const Node<I, O>& node, vint min, vint max=-1)
		{
			return new _Loop<I, O>(node.GetCombinator(), min, max);
		}

		template<typename I>
		Node<I, typename I::ElementType> unit(const typename I::ElementType& value)
		{
			return new _Unit<I>(value, L"遇到预期外的输入。");
		}

		template<typename I>
		Node<I, typename I::ElementType> pred(const Func<bool(const typename I::ElementType&)>& predicate)
		{
			return new _Pred<I>(predicate, L"遇到预期外的输入。");
		}

/***********************************************************************
List Constructor (T,T*)?
***********************************************************************/

		template<typename T>
		static Ptr<collections::List<T>> ToList(const ParsingList<ParsingPair<T, ParsingList<T>>>& input)
		{
			Ptr<collections::List<T>> result=new collections::List<T>;
			if(input.Head())
			{
				result->Add(input.Head()->Value().First());
				ParsingList<T>::Node::Ref current=input.Head()->Value().Second().Head();
				while(current)
				{
					result->Add(current->Value());
					current=current->Next();
				}
			}
			return result;
		}

		template<typename I, typename T>
		static Node<I, Ptr<collections::List<T>>> list(const Node<I, ParsingList<ParsingPair<T, ParsingList<T>>>>& node)
		{
			Ptr<collections::List<T>>(*converter)(const ParsingList<ParsingPair<T, ParsingList<T>>>& input)=&ToList<T>;
			return node[converter];
		}

		template<typename T>
		static ParsingList<T> ToPList(const ParsingList<ParsingPair<T, ParsingList<T>>>& input)
		{
			ParsingList<T> result;
			if(input.Head())
			{
				result.Append(input.Head()->Value().First());
				ParsingList<T>::Node::Ref current=input.Head()->Value().Second().Head();
				while(current)
				{
					result.Append(current->Value());
					current=current->Next();
				}
			}
			return result;
		}

		template<typename I, typename T>
		static Node<I, ParsingList<T>> plist(const Node<I, ParsingList<ParsingPair<T, ParsingList<T>>>>& node)
		{
			ParsingList<T>(*converter)(const ParsingList<ParsingPair<T, ParsingList<T>>>& input)=&ToPList<T>;
			return node[converter];
		}

/***********************************************************************
List Constructor (T,T*)
***********************************************************************/

		template<typename T>
		static Ptr<collections::List<T>> ToList(const ParsingPair<T, ParsingList<T>>& input)
		{
			Ptr<collections::List<T>> result=new collections::List<T>;
			result->Add(input.First());
			ParsingList<T>::Node::Ref current=input.Second().Head();
			while(current)
			{
				result->Add(current->Value());
				current=current->Next();
			}
			return result;
		}

		template<typename I, typename T>
		static Node<I, Ptr<collections::List<T>>> list(const Node<I, ParsingPair<T, ParsingList<T>>>& node)
		{
			Ptr<collections::List<T>>(*converter)(const ParsingPair<T, ParsingList<T>>& input)=&ToList<T>;
			return node[converter];
		}

		template<typename T>
		static ParsingList<T> ToPList(const ParsingPair<T, ParsingList<T>>& input)
		{
			ParsingList<T> result;
			result.Append(input.First());
			ParsingList<T>::Node::Ref current=input.Second().Head();
			while(current)
			{
				result.Append(current->Value());
				current=current->Next();
			}
			return result;
		}

		template<typename I, typename T>
		static Node<I, ParsingList<T>> plist(const Node<I, ParsingPair<T, ParsingList<T>>>& node)
		{
			ParsingList<T>(*converter)(const ParsingPair<T, ParsingList<T>>& input)=&ToPList<T>;
			return node[converter];
		}

/***********************************************************************
List Constructor T*
***********************************************************************/

		template<typename T>
		static Ptr<collections::List<T>> ToList(const ParsingList<T>& input)
		{
			Ptr<collections::List<T>> result=new collections::List<T>;
			ParsingList<T>::Node::Ref current=input.Head();
			while(current)
			{
				result->Add(current->Value());
				current=current->Next();
			}
			return result;
		}

		template<typename I, typename T>
		static Node<I, Ptr<collections::List<T>>> list(const Node<I, ParsingList<T>>& node)
		{
			Ptr<collections::List<T>>(*converter)(const ParsingList<T>& input)=&ToList<T>;
			return node[converter];
		}

/***********************************************************************
Left Recursive Constructor
***********************************************************************/

		template<typename T, typename U>
		class _lrec_converter
		{
		protected:
			Func<T(const T&, const U&)>		converter;
		public:
			_lrec_converter(const Func<T(const T&, const U&)>& _converter)
				:converter(_converter)
			{
			}

			T operator()(const ParsingPair<T, ParsingList<U>>& input)const
			{
				T result=input.First();
				ParsingList<U>::Node::Ref current=input.Second().Head();
				while(current)
				{
					result=converter(result, current->Value());
					current=current->Next();
				}
				return result;
			}
		};

		template<typename I, typename T, typename U>
		static Node<I, T> lrec(const Node<I, ParsingPair<T, ParsingList<U>>>& node, const Func<T(const T&, const U&)>& converter)
		{
			return node[Func<T(const ParsingPair<T, ParsingList<U>>&)>(_lrec_converter<T, U>(converter))];
		}

		template<typename I, typename T, typename U>
		static Node<I, T> lrec(const Node<I, ParsingPair<T, ParsingList<U>>>& node, T(*converter)(const T&, const U&))
		{
			return node[Func<T(const ParsingPair<T, ParsingList<U>>&)>(_lrec_converter<T, U>(converter))];
		}

/***********************************************************************
Rule
***********************************************************************/

		template<typename I, typename O>
		class Rule : public Node<I, O>
		{
		public:
			Rule()
			{
			}

			Rule(const Node<I, O>& _element)
				:element(_element.GetCombinator())
			{
			}

			typename Combinator<I, O>::Ref GetCombinator()const
			{
				return new _Ref<I, O>(element);
			}

			Rule<I, O>& operator=(const Node<I, O>& _element)
			{
				element=_element.GetCombinator();
				return *this;
			}

			Rule<I, O>& operator=(const Rule<I, O>& _element)
			{
				element=_element.element;
				return *this;
			}

			Node<I, O> GetContent()
			{
				return element;
			}
		};
	}
}

#endif
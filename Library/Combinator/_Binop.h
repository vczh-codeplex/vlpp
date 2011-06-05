/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_BINOP
#define VCZH_COMBINATOR_BINOP

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O>
		class _Binop : public Combinator<I, O>
		{
			struct PrecedenceRecord;
		protected:
			class OperatorRecord : public Object
			{
			public:
				virtual ParsingResult<OutputType>			ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)=0;
			};

			class PrecedenceRecord
			{
			public:
				_Binop<I, O>*								binop;
				PrecedenceRecord*							previous;
				OperatorRecord*								first;
				OperatorRecord*								last;

				template<typename T>
				void AddOperatorRecord(T* newRecord)
				{
					if(last==0)
					{
						first=newRecord;
						last=newRecord;
					}
					else
					{
						T* record=dynamic_cast<T*>(last);
						CHECK_ERROR(record, L"_Binop<I, O>::PrecedenceRecord::LastOperatorRecord<T>()#��������¼���Ͳ�ƥ�䡣");
						record->last=newRecord;
						last=newRecord;
					}
				}
			};

			ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)const
			{
				return precedence
					?precedence->first->ParsePrecedence(precedence, input, globalInfo)
					:expression->Parse(input, globalInfo)
					;
			}
		protected:
			template<typename T>
			class PreUnaryRecord : public OperatorRecord
			{
			public:
				PreUnaryRecord*								last;
				typename Combinator<I, T>::Ref				op;
				Func<O(T, O)>								handler;

				PreUnaryRecord()
					:last(0)
				{
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					return ParsingResult<OutputType>();
				}
			};
			
			template<typename T>
			class PostUnaryRecord : public OperatorRecord
			{
			public:
				PostUnaryRecord*							last;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T)>								handler;

				PostUnaryRecord()
					:last(0)
				{
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					return ParsingResult<OutputType>();
				}
			};
			
			template<typename T>
			class LeftBinaryRecord : public OperatorRecord
			{
			public:
				LeftBinaryRecord*							last;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T, O)>							handler;

				LeftBinaryRecord()
					:last(0)
				{
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					return ParsingResult<OutputType>();
				}
			};
			
			template<typename T>
			class RightBinaryRecord : public OperatorRecord
			{
			public:
				RightBinaryRecord*							last;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T, O)>							handler;

				RightBinaryRecord()
					:last(0)
				{
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					return ParsingResult<OutputType>();
				}
			};
		protected:
			typename Combinator<I, O>::Ref					expression;
			PrecedenceRecord*								lowestPrecedence;
			bool											needNewPrecedence;

			PrecedenceRecord* CurrentPrecedence()
			{
				if(needNewPrecedence)
				{
					PrecedenceRecord* precedence=new PrecedenceRecord;
					precedence->binop=this;
					precedence->previous=lowestPrecedence;
					precedence->first=0;
					precedence->last=0;

					lowestPrecedence=precedence;
					needNewPrecedence=false;
				}
				return lowestPrecedence;
			}
		public:
			_Binop(typename Combinator<I, O>::Ref _expression)
				:expression(_expression)
				,lowestPrecedence(0)
				,needNewPrecedence(true)
			{
			}

			~_Binop()
			{
				while(lowestPrecedence)
				{
					PrecedenceRecord* previous=lowestPrecedence->previous;
					delete lowestPrecedence;
					lowestPrecedence=previous;
				}
			}

			template<typename T>
			void PreUnary(typename Combinator<I, T>::Ref op, Func<O(T, O)> handler)
			{
				PreUnaryRecord<T>* record=new PreUnaryRecord<T>;
				record->op=op;
				record->handler=handler;
				CurrentPrecedence()->AddOperatorRecord(record);
			}

			template<typename T>
			void PostUnary(typename Combinator<I, T>::Ref op, Func<O(O, T)> handler)
			{
				PostUnaryRecord<T>* record=new PostUnaryRecord<T>;
				record->op=op;
				record->handler=handler;
				CurrentPrecedence()->AddOperatorRecord(record);
			}

			template<typename T>
			void LeftBinary(typename Combinator<I, T>::Ref op, Func<O(O, T, O)> handler)
			{
				LeftBinaryRecord<T>* record=new LeftBinaryRecord<T>;
				record->op=op;
				record->handler=handler;
				CurrentPrecedence()->AddOperatorRecord(record);
			}

			template<typename T>
			void RightBinary(typename Combinator<I, T>::Ref op, Func<O(O, T, O)> handler)
			{
				RightBinaryRecord<T>* record=new RightBinaryRecord<T>;
				record->op=op;
				record->handler=handler;
				CurrentPrecedence()->AddOperatorRecord(record);
			}

			void Precedence()
			{
				needNewPrecedence=true;
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				return ParsePrecedence(lowestPrecedence, input, globalInfo);
			}
		};
	}
}

#endif
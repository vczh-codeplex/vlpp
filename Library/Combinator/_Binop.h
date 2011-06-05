/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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

				PrecedenceRecord(_Binop<I, O>* _binop)
					:binop(_binop)
					,previous(0)
					,first(0)
					,last(0)
				{
				}

				~PrecedenceRecord()
				{
					if(first) delete first;
				}

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
						CHECK_ERROR(record, L"_Binop<I, O>::PrecedenceRecord::LastOperatorRecord<T>()#²Ù×÷·û¼ÇÂ¼ÀàÐÍ²»Æ¥Åä¡£");
						record->next=newRecord;
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
				PreUnaryRecord*								next;
				typename Combinator<I, T>::Ref				op;
				Func<O(T, O)>								handler;

				PreUnaryRecord()
					:next(0)
				{
				}

				~PreUnaryRecord()
				{
					if(next) delete next;
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					PreUnaryRecord<T>* current=this;
					while(current)
					{
						InputType newInput=input;
						GlobalInfoType newGlobalInfo;
						if(ParsingResult<T> opResult=current->op->Parse(newInput, newGlobalInfo))
						{
							input=newInput;
							globalInfo.Append(newGlobalInfo);
								
							ParsingResult<O> result=precedence->binop->ParsePrecedence(precedence, input, globalInfo);
							if(result)
							{
								result=ParsingResult<O>(current->handler(opResult.Value(), result.Value()));
							}
							return result;
						}
						current=current->next;
					}
					return precedence->binop->ParsePrecedence(precedence->previous, input, globalInfo);
				}
			};
			
			template<typename T>
			class PostUnaryRecord : public OperatorRecord
			{
			public:
				PostUnaryRecord*							next;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T)>								handler;

				PostUnaryRecord()
					:next(0)
				{
				}

				~PostUnaryRecord()
				{
					if(next) delete next;
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					ParsingResult<O> result=precedence->binop->ParsePrecedence(precedence->previous, input, globalInfo);
					if(result)
					{
						O exp=result.Value();
						while(true)
						{
							PostUnaryRecord<T>* current=this;
							while(current)
							{
								InputType newInput=input;
								GlobalInfoType newGlobalInfo;
								if(ParsingResult<T> opResult=current->op->Parse(newInput, newGlobalInfo))
								{
									input=newInput;
									globalInfo.Append(newGlobalInfo);
									exp=current->handler(exp, opResult.Value());
									break;
								}
								current=current->next;
							}
							if(!current) break;
						}
						result=ParsingResult<O>(exp);
					}
					return result;
				}
			};
			
			template<typename T>
			class LeftBinaryRecord : public OperatorRecord
			{
			public:
				LeftBinaryRecord*							next;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T, O)>							handler;

				LeftBinaryRecord()
					:next(0)
				{
				}

				~LeftBinaryRecord()
				{
					if(next) delete next;
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					ParsingResult<O> result=precedence->binop->ParsePrecedence(precedence->previous, input, globalInfo);
					if(result)
					{
						O exp=result.Value();
						while(true)
						{
							LeftBinaryRecord<T>* current=this;
							while(current)
							{
								InputType newInput=input;
								GlobalInfoType newGlobalInfo;
								if(ParsingResult<T> opResult=current->op->Parse(newInput, newGlobalInfo))
								{
									if(ParsingResult<O> right=precedence->binop->ParsePrecedence(precedence->previous, newInput, newGlobalInfo))
									{
										input=newInput;
										globalInfo.Append(newGlobalInfo);
										exp=current->handler(exp, opResult.Value(), right.Value());
										break;
									}
								}
								current=current->next;
							}
							if(!current) break;
						}
						result=ParsingResult<O>(exp);
					}
					return result;
				}
			};
			
			template<typename T>
			class RightBinaryRecord : public OperatorRecord
			{
			public:
				RightBinaryRecord*							next;
				typename Combinator<I, T>::Ref				op;
				Func<O(O, T, O)>							handler;

				RightBinaryRecord()
					:next(0)
				{
				}

				~RightBinaryRecord()
				{
					if(next) delete next;
				}

				ParsingResult<OutputType> ParsePrecedence(PrecedenceRecord* precedence, InputType& input, GlobalInfoType& globalInfo)
				{
					ParsingResult<O> result=precedence->binop->ParsePrecedence(precedence->previous, input, globalInfo);
					if(result)
					{
						RightBinaryRecord<T>* current=this;
						while(current)
						{
							InputType newInput=input;
							GlobalInfoType newGlobalInfo;
							if(ParsingResult<T> opResult=current->op->Parse(newInput, newGlobalInfo))
							{
								if(ParsingResult<O> right=precedence->binop->ParsePrecedence(precedence, newInput, newGlobalInfo))
								{
									input=newInput;
									globalInfo.Append(newGlobalInfo);
									O exp=current->handler(result.Value(), opResult.Value(), right.Value());
									return ParsingResult<O>(exp);
								}
							}
							current=current->next;
						}
					}
					return result;
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
					PrecedenceRecord* precedence=new PrecedenceRecord(this);
					precedence->previous=lowestPrecedence;
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
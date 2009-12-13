#include "RegexPure.h"

namespace vl
{
	namespace regex_internal
	{

/***********************************************************************
PureInterpretor
***********************************************************************/

		PureInterpretor::PureInterpretor(Automaton::Ref dfa, CharRange::List& subsets)
		{
			stateCount=dfa->states.Count();
			charSetCount=subsets.Count()+1;
			startState=dfa->states.IndexOf(dfa->startState);

			//填充字符映射表
			for(int i=0;i<sizeof(charMap)/sizeof(*charMap);i++)
			{
				charMap[i]=charSetCount-1;
			}
			for(int i=0;i<subsets.Count();i++)
			{
				CharRange range=subsets[i];
				for(int j=range.begin;j<=range.end;j++)
				{
					charMap[j]=i;
				}
			}
			
			//构造状态转换表
			transition=new int*[stateCount];
			for(int i=0;i<stateCount;i++)
			{
				transition[i]=new int[charSetCount];
				for(int j=0;j<charSetCount;j++)
				{
					transition[i][j]=-1;
				}

				State* state=dfa->states[i].Obj();
				for(int j=0;j<state->transitions.Count();j++)
				{
					Transition* dfaTransition=state->transitions[j];
					switch(dfaTransition->type)
					{
					case Transition::Chars:
						{
							int index=subsets.IndexOf(dfaTransition->range);
							if(index==-1)
							{
								CHECK_ERROR(false, L"PureInterpretor::PureInterpretor(Automaton::Ref, CharRange::List&)#指定的字符集转换没有出现在正规化的字符集结果上。");
							}
							transition[i][index]=dfa->states.IndexOf(dfaTransition->target);
						}
						break;
					default:
						CHECK_ERROR(false, L"PureInterpretor::PureInterpretor(Automaton::Ref, CharRange::List&)#PureInterpretor只接受Transition::Chars转换。");
					}
				}
			}

			//填充终结状态表
			finalState=new bool[stateCount];
			for(int i=0;i<stateCount;i++)
			{
				finalState[i]=dfa->states[i]->finalState;
			}
		}

		PureInterpretor::~PureInterpretor()
		{
			delete[] finalState;
			for(int i=0;i<stateCount;i++)
			{
				delete[] transition[i];
			}
			delete[] transition;
		}

		bool PureInterpretor::MatchHead(const wchar_t* input, const wchar_t* start, PureResult& result)
		{
			result.start=input-start;
			result.length=-1;
			result.finalState=-1;

			int currentState=startState;
			const wchar_t* read=input;
			while(currentState!=-1)
			{
				if(finalState[currentState])
				{
					result.length=read-input;
					result.finalState=currentState;
				}
				if(!*read)break;
				int charIndex=charMap[*read++];
				currentState=transition[currentState][charIndex];
			}

			return result.finalState!=-1;
		}

		bool PureInterpretor::Match(const wchar_t* input, const wchar_t* start, PureResult& result)
		{
			const wchar_t* read=input;
			while(*read)
			{
				if(MatchHead(read, start, result))
				{
					return true;
				}
				read++;
			}
			return false;
		}
	}
}
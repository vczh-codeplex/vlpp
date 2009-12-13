#include "Regex.h"
#include "RegexExpression.h"
#include "RegexPure.h"
#include "RegexRich.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace regex
	{
		using namespace collections;
		using namespace regex_internal;

/***********************************************************************
RegexString
***********************************************************************/

		RegexString::RegexString(int _start)
			:start(_start)
			,length(0)
		{
		}

		RegexString::RegexString(const WString& _string, int _start, int _length)
			:value(_length==0?L"":_string.Sub(_start, _length))
			,start(_start)
			,length(_length)
		{
		}

		int RegexString::Start()const
		{
			return start;
		}

		int RegexString::Length()const
		{
			return length;
		}

		const WString& RegexString::Value()const
		{
			return value;
		}

		bool RegexString::operator==(const RegexString& string)const
		{
			return start==string.start && length==string.length && value==string.value;
		}

/***********************************************************************
RegexMatch
***********************************************************************/
		
		RegexMatch::RegexMatch(const WString& _string, PureResult* _result)
			:success(true)
			,result(_string, _result->start, _result->length)
		{
		}

		RegexMatch::RegexMatch(const WString& _string, RichResult* _result, RichInterpretor* _rich)
			:success(true)
			,result(_string, _result->start, _result->length)
		{
			for(int i=0;i<_result->captures.Count();i++)
			{
				CaptureRecord& capture=_result->captures[i];
				if(capture.capture==-1)
				{
					captures.Add(RegexString(_string, capture.start, capture.length));
				}
				else
				{
					groups.Add(_rich->CaptureNames()[capture.capture], RegexString(_string, capture.start, capture.length));
				}
			}
		}

		RegexMatch::RegexMatch(const RegexString& _result)
			:success(false)
			,result(_result)
		{
		}
			
		bool RegexMatch::Success()const
		{
			return success;
		}

		const RegexString& RegexMatch::Result()const
		{
			return result;
		}

		const RegexMatch::CaptureList& RegexMatch::Captures()const
		{
			return captures.Wrap();
		}

		const RegexMatch::CaptureGroup& RegexMatch::Groups()const
		{
			return groups.Wrap();
		}

/***********************************************************************
Regex
***********************************************************************/

		void Regex::Process(const WString& text, bool keepEmpty, bool keepSuccess, bool keepFail, RegexMatch::List& matches)const
		{
			if(rich)
			{
				const wchar_t* start=text.Buffer();
				const wchar_t* input=start;
				RichResult result;
				while(rich->Match(input, start, result))
				{
					int offset=input-start;
					if(keepFail)
					{
						if(result.start>offset || keepEmpty)
						{
							matches.Add(new RegexMatch(RegexString(text, offset, result.start-offset)));
						}
					}
					if(keepSuccess)
					{
						matches.Add(new RegexMatch(text, &result, rich));
					}
					input=start+result.start+result.length;
				}
				if(keepFail)
				{
					int remain=input-start;
					int length=text.Length()-remain;
					if(length || keepEmpty)
					{
						matches.Add(new RegexMatch(RegexString(text, remain, length)));
					}
				}
			}
			else
			{
				const wchar_t* start=text.Buffer();
				const wchar_t* input=start;
				PureResult result;
				while(pure->Match(input, start, result))
				{
					int offset=input-start;
					if(keepFail)
					{
						if(result.start>offset || keepEmpty)
						{
							matches.Add(new RegexMatch(RegexString(text, offset, result.start-offset)));
						}
					}
					if(keepSuccess)
					{
						matches.Add(new RegexMatch(text, &result));
					}
					input=start+result.start+result.length;
				}
				if(keepFail)
				{
					int remain=input-start;
					int length=text.Length()-remain;
					if(length || keepEmpty)
					{
						matches.Add(new RegexMatch(RegexString(text, remain, length)));
					}
				}
			}
		}
		
		Regex::Regex(const WString& code, bool preferPure)
			:pure(0)
			,rich(0)
		{
			CharRange::List subsets;
			RegexExpression::Ref regex=ParseRegexExpression(code);
			Expression::Ref expression=regex->Merge();
			expression->NormalizeCharSet(subsets);

			bool pureRequired=false;
			bool richRequired=false;
			if(preferPure)
			{
				if(expression->HasNoExtension())
				{
					pureRequired=true;
				}
				else
				{
					if(expression->CanTreatAsPure())
					{
						pureRequired=true;
						richRequired=true;
					}
					else
					{
						richRequired=true;
					}
				}
			}
			else
			{
				richRequired=true;
			}

			try
			{
				if(pureRequired)
				{
					Dictionary<State*, State*> nfaStateMap;
					Group<State*, State*> dfaStateMap;
					Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
					Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, PureEpsilonChecker, nfaStateMap.Wrap());
					Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());
					pure=new PureInterpretor(dfa, subsets);
				}
				if(richRequired)
				{
					Dictionary<State*, State*> nfaStateMap;
					Group<State*, State*> dfaStateMap;
					Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
					Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, RichEpsilonChecker, nfaStateMap.Wrap());
					Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());
					rich=new RichInterpretor(dfa);
				}
			}
			catch(...)
			{
				if(pure)delete pure;
				if(rich)delete rich;
				throw;
			}
		}

		Regex::~Regex()
		{
			if(pure)delete pure;
			if(rich)delete rich;
		}

		bool Regex::IsPureMatch()const
		{
			return rich?false:true;
		}

		bool Regex::IsPureTest()const
		{
			return pure?true:false;
		}

		RegexMatch::Ref Regex::MatchHead(const WString& text)const
		{
			if(rich)
			{
				RichResult result;
				if(rich->MatchHead(text.Buffer(), text.Buffer(), result))
				{
					return new RegexMatch(text, &result, rich);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				PureResult result;
				if(pure->MatchHead(text.Buffer(), text.Buffer(), result))
				{
					return new RegexMatch(text, &result);
				}
				else
				{
					return 0;
				}
			}
		}

		RegexMatch::Ref Regex::Match(const WString& text)const
		{
			if(rich)
			{
				RichResult result;
				if(rich->Match(text.Buffer(), text.Buffer(), result))
				{
					return new RegexMatch(text, &result, rich);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				PureResult result;
				if(pure->Match(text.Buffer(), text.Buffer(), result))
				{
					return new RegexMatch(text, &result);
				}
				else
				{
					return 0;
				}
			}
		}

		bool Regex::TestHead(const WString& text)const
		{
			if(pure)
			{
				PureResult result;
				return pure->MatchHead(text.Buffer(), text.Buffer(), result);
			}
			else
			{
				RichResult result;
				return rich->MatchHead(text.Buffer(), text.Buffer(), result);
			}
		}

		bool Regex::Test(const WString& text)const
		{
			if(pure)
			{
				PureResult result;
				return pure->Match(text.Buffer(), text.Buffer(), result);
			}
			else
			{
				RichResult result;
				return rich->Match(text.Buffer(), text.Buffer(), result);
			}
		}

		void Regex::Search(const WString& text, RegexMatch::List& matches)const
		{
			Process(text, false, true, false, matches);
		}

		void Regex::Split(const WString& text, bool keepEmptyMatch, RegexMatch::List& matches)const
		{
			Process(text, keepEmptyMatch, false, true, matches);
		}

		void Regex::Cut(const WString& text, bool keepEmptyMatch, RegexMatch::List& matches)const
		{
			Process(text, keepEmptyMatch, true, true, matches);
		}

/***********************************************************************
RegexTokens
***********************************************************************/

		bool RegexToken::operator==(const RegexToken& _token)const
		{
			return start==_token.start && length==_token.length && token==_token.token && reading==_token.reading;
		}

		class RegexTokenEnumerator : public Object, public IEnumerator<RegexToken>
		{
		protected:
			bool				available;
			RegexToken			token;
			int					index;
			PureInterpretor*	pure;
			Array<int>&			stateTokens;
			const wchar_t*		reading;
			const wchar_t*		start;
			bool				cacheAvailable;
			RegexToken			cacheToken;

			void Read()
			{
				if(cacheAvailable || *reading)
				{
					if(cacheAvailable)
					{
						token=cacheToken;
						cacheAvailable=false;
					}
					else
					{
						token.reading=reading;
						token.start=0;
						token.length=0;
						token.token=-2;
					}

					PureResult result;
					while(*reading)
					{
						int id=-1;
						if(!pure->MatchHead(reading, start, result))
						{
							result.start=reading-start;
							result.length=1;
						}
						else
						{
							id=stateTokens[result.finalState];
						}
						if(token.token==-2)
						{
							token.start=result.start;
							token.length=result.length;
							token.token=id;
						}
						else if(token.token==id)
						{
							token.length+=result.length;
						}
						else
						{
							cacheAvailable=true;
							cacheToken.reading=reading;
							cacheToken.start=result.start;
							cacheToken.length=result.length;
							cacheToken.token=id;
						}
						reading+=result.length;
						if(cacheAvailable)
						{
							break;
						}
					}

					index++;
					available=true;
				}
				else
				{
					available=false;
				}
			}
		public:
			RegexTokenEnumerator(const RegexTokenEnumerator& enumerator)
				:available(enumerator.available)
				,token(enumerator.token)
				,index(enumerator.index)
				,pure(enumerator.pure)
				,stateTokens(enumerator.stateTokens)
				,reading(enumerator.reading)
				,start(enumerator.start)
				,cacheAvailable(enumerator.cacheAvailable)
				,cacheToken(enumerator.cacheToken)
			{
			}

			RegexTokenEnumerator(PureInterpretor* _pure, Array<int>& _stateTokens, const wchar_t* _start)
				:available(true)
				,index(-1)
				,pure(_pure)
				,stateTokens(_stateTokens)
				,reading(_start)
				,start(_start)
				,cacheAvailable(false)
			{
				Read();
			}

			IEnumerator<RegexToken>* Clone()const
			{
				return new RegexTokenEnumerator(*this);
			}

			const RegexToken& Current()const
			{
				return token;
			}

			int Index()const
			{
				return index;
			}

			bool Next()
			{
				Read();
				return available;
			}

			bool Available()const
			{
				return available;
			}

			void Reset()
			{
				index=-1;
				reading=start;
				Read();
			}
		};

		RegexTokens::RegexTokens(PureInterpretor* _pure, Array<int>& _stateTokens, const WString& _code)
			:pure(_pure)
			,stateTokens(_stateTokens)
			,code(_code)
		{
		}

		IEnumerator<RegexToken>* RegexTokens::CreateEnumerator()const
		{
			return new RegexTokenEnumerator(pure, stateTokens, code.Buffer());
		}

/***********************************************************************
RegexLexer
***********************************************************************/

		RegexLexer::RegexLexer(const collections::IEnumerable<WString>& tokens)
			:pure(0)
		{
			//构造所有DFA
			List<Expression::Ref> expressions;
			List<Automaton::Ref> dfas;
			CharRange::List subsets;
			Ptr<IEnumerator<WString>> enumerator=tokens.CreateEnumerator();
			while(enumerator->Available())
			{
				const WString& code=enumerator->Current();
				enumerator->Next();

				RegexExpression::Ref regex=ParseRegexExpression(code);
				Expression::Ref expression=regex->Merge();
				expression->CollectCharSet(subsets);
				expressions.Add(expression);
			}
			for(int i=0;i<expressions.Count();i++)
			{
				Dictionary<State*, State*> nfaStateMap;
				Group<State*, State*> dfaStateMap;
				Expression::Ref expression=expressions[i];
				expression->ApplyCharSet(subsets);
				Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
				Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, PureEpsilonChecker, nfaStateMap.Wrap());
				Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());
				dfas.Add(dfa);
			}

			//为每一个DFA设置标记
			ids.Resize(dfas.Count());
			for(int i=0;i<ids.Count();i++)
			{
				ids[i]=i;
			}
			for(int i=0;i<dfas.Count();i++)
			{
				Automaton::Ref dfa=dfas[i];
				for(int j=0;j<dfa->states.Count();j++)
				{
					dfa->states[j]->userData=&ids[i];
				}
			}

			//将DFA组合成大的e-NFA
			Automaton::Ref bigEnfa=new Automaton;
			for(int i=0;i<dfas.Count();i++)
			{
				CopyFrom(bigEnfa->states.Wrap(), dfas[i]->states.Wrap());
				CopyFrom(bigEnfa->transitions.Wrap(), dfas[i]->transitions.Wrap());
			}
			bigEnfa->startState=bigEnfa->NewState();
			for(int i=0;i<dfas.Count();i++)
			{
				bigEnfa->NewEpsilon(bigEnfa->startState, dfas[i]->startState);
			}

			//转换成DFA
			Dictionary<State*, State*> nfaStateMap;
			Group<State*, State*> dfaStateMap;
			Automaton::Ref bigNfa=EpsilonNfaToNfa(bigEnfa, PureEpsilonChecker, nfaStateMap.Wrap());
			for(int i=0;i<nfaStateMap.Keys().Count();i++)
			{
				void* userData=nfaStateMap.Values()[i]->userData;
				nfaStateMap.Keys()[i]->userData=userData;
			}
			Automaton::Ref bigDfa=NfaToDfa(bigNfa, dfaStateMap.Wrap());
			for(int i=0;i<dfaStateMap.Keys().Count();i++)
			{
				void* userData=dfaStateMap.GetByIndex(i)[0]->userData;
				dfaStateMap.Keys()[i]->userData=userData;
			}

			//构造状态机
			pure=new PureInterpretor(bigDfa, subsets);
			stateTokens.Resize(bigDfa->states.Count());
			for(int i=0;i<stateTokens.Count();i++)
			{
				void* userData=bigDfa->states[i]->userData;
				stateTokens[i]=userData?(*(int*)userData):-1;
			}
		}

		RegexLexer::~RegexLexer()
		{
			if(pure)delete pure;
		}

		RegexTokens RegexLexer::Parse(const WString& code)
		{
			return RegexTokens(pure, stateTokens, code);
		}
	}
}
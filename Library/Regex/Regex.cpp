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

		RegexString::RegexString(vint _start)
			:start(_start)
			,length(0)
		{
		}

		RegexString::RegexString(const WString& _string, vint _start, vint _length)
			:value(_length==0?L"":_string.Sub(_start, _length))
			,start(_start)
			,length(_length)
		{
		}

		vint RegexString::Start()const
		{
			return start;
		}

		vint RegexString::Length()const
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
			for(vint i=0;i<_result->captures.Count();i++)
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
					vint offset=input-start;
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
					vint remain=input-start;
					vint length=text.Length()-remain;
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
					vint offset=input-start;
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
					vint remain=input-start;
					vint length=text.Length()-remain;
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
					Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, PureEpsilonChecker, nfaStateMap);
					Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap);
					pure=new PureInterpretor(dfa, subsets);
				}
				if(richRequired)
				{
					Dictionary<State*, State*> nfaStateMap;
					Group<State*, State*> dfaStateMap;
					Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
					Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, RichEpsilonChecker, nfaStateMap);
					Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap);
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
			return length==_token.length && token==_token.token && reading==_token.reading;
		}
		
		bool RegexToken::operator==(const wchar_t* _token)const
		{
			return wcslen(_token)==length && wcsncmp(reading, _token, length)==0;
		}

		class RegexTokenEnumerator : public Object, public IEnumerator<RegexToken>
		{
		protected:
			bool				available;
			RegexToken			token;
			vint					index;
			PureInterpretor*	pure;
			Array<vint>&			stateTokens;
			const wchar_t*		reading;
			const wchar_t*		start;
			vint					lineIndex;
			vint					lineStart;
			vint					codeIndex;
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
					token.lineIndex=lineIndex;
					token.lineStart=lineStart;
					token.codeIndex=codeIndex;

					PureResult result;
					while(*reading)
					{
						vint id=-1;
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
						else if(token.token==id && id==-1)
						{
							token.length+=result.length;
						}
						else
						{
							cacheAvailable=true;
							cacheToken.reading=reading;
							cacheToken.start=result.start;
							cacheToken.length=result.length;
							cacheToken.codeIndex=codeIndex;
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

					for(vint i=0;i<token.length;i++)
					{
						if(token.reading[i]==L'\n')
						{
							lineIndex++;
							lineStart=0;
						}
						else
						{
							lineStart++;
						}
					}
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
				,lineIndex(enumerator.lineIndex)
				,lineStart(enumerator.lineStart)
				,codeIndex(enumerator.codeIndex)
				,cacheAvailable(enumerator.cacheAvailable)
				,cacheToken(enumerator.cacheToken)
			{
			}

			RegexTokenEnumerator(PureInterpretor* _pure, Array<vint>& _stateTokens, const wchar_t* _start, vint _codeIndex)
				:available(true)
				,index(-1)
				,pure(_pure)
				,stateTokens(_stateTokens)
				,reading(_start)
				,start(_start)
				,lineIndex(0)
				,lineStart(0)
				,codeIndex(_codeIndex)
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

			vint Index()const
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
				cacheAvailable=false;
				Read();
			}

			void ReadToEnd(List<RegexToken>& tokens, bool(*discard)(vint))
			{
				while(available)
				{
					if(!discard(token.token))
					{
						tokens.Add(token);
					}
					Read();
				}
			}
		};

		RegexTokens::RegexTokens(PureInterpretor* _pure, Array<vint>& _stateTokens, const WString& _code, vint _codeIndex)
			:pure(_pure)
			,stateTokens(_stateTokens)
			,code(_code)
			,codeIndex(_codeIndex)
		{
		}

		RegexTokens::RegexTokens(const RegexTokens& regexTokens)
			:pure(regexTokens.pure)
			, stateTokens(regexTokens.stateTokens)
			, code(regexTokens.code)
			, codeIndex(regexTokens.codeIndex)
		{

		}

		IEnumerator<RegexToken>* RegexTokens::CreateEnumerator()const
		{
			return new RegexTokenEnumerator(pure, stateTokens, code.Buffer(), codeIndex);
		}

		bool DefaultDiscard(vint token)
		{
			return false;
		}

		void RegexTokens::ReadToEnd(collections::List<RegexToken>& tokens, bool(*discard)(vint))const
		{
			if(discard==0)
			{
				discard=&DefaultDiscard;
			}
			RegexTokenEnumerator(pure, stateTokens, code.Buffer(), codeIndex).ReadToEnd(tokens, discard);
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
			for(vint i=0;i<expressions.Count();i++)
			{
				Dictionary<State*, State*> nfaStateMap;
				Group<State*, State*> dfaStateMap;
				Expression::Ref expression=expressions[i];
				expression->ApplyCharSet(subsets);
				Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
				Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, PureEpsilonChecker, nfaStateMap);
				Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap);
				dfas.Add(dfa);
			}

			//为每一个DFA设置标记
			for(vint i=0;i<dfas.Count();i++)
			{
				Automaton::Ref dfa=dfas[i];
				for(vint j=0;j<dfa->states.Count();j++)
				{
					if(dfa->states[j]->finalState)
					{
						dfa->states[j]->userData=(void*)i;
					}
					else
					{
						dfa->states[j]->userData=(void*)dfas.Count();
					}
				}
			}

			//将DFA组合成大的e-NFA
			Automaton::Ref bigEnfa=new Automaton;
			for(vint i=0;i<dfas.Count();i++)
			{
				CopyFrom(bigEnfa->states.Wrap(), dfas[i]->states.Wrap());
				CopyFrom(bigEnfa->transitions.Wrap(), dfas[i]->transitions.Wrap());
			}
			bigEnfa->startState=bigEnfa->NewState();
			for(vint i=0;i<dfas.Count();i++)
			{
				bigEnfa->NewEpsilon(bigEnfa->startState, dfas[i]->startState);
			}

			//转换成DFA
			Dictionary<State*, State*> nfaStateMap;
			Group<State*, State*> dfaStateMap;
			Automaton::Ref bigNfa=EpsilonNfaToNfa(bigEnfa, PureEpsilonChecker, nfaStateMap);
			for(vint i=0;i<nfaStateMap.Keys().Count();i++)
			{
				void* userData=nfaStateMap.Values()[i]->userData;
				nfaStateMap.Keys()[i]->userData=userData;
			}
			Automaton::Ref bigDfa=NfaToDfa(bigNfa, dfaStateMap);
			for(vint i=0;i<dfaStateMap.Keys().Count();i++)
			{
				void* userData=dfaStateMap.GetByIndex(i)[0]->userData;
				for(vint j=1;j<dfaStateMap.GetByIndex(i).Count();j++)
				{
					void* newData=dfaStateMap.GetByIndex(i)[j]->userData;
					if(userData>newData)
					{
						userData=newData;
					}
				}
				dfaStateMap.Keys()[i]->userData=userData;
			}

			//构造状态机
			pure=new PureInterpretor(bigDfa, subsets);
			stateTokens.Resize(bigDfa->states.Count());
			for(vint i=0;i<stateTokens.Count();i++)
			{
				void* userData=bigDfa->states[i]->userData;
				stateTokens[i]=(vint)userData;
			}
		}

		RegexLexer::~RegexLexer()
		{
			if(pure)delete pure;
		}

		RegexTokens RegexLexer::Parse(const WString& code, vint codeIndex)
		{
			return RegexTokens(pure, stateTokens, code, codeIndex);
		}
	}
}
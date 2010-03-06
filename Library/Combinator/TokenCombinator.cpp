#include "TokenCombinator.h"

namespace vl
{
	namespace combinator
	{
		using namespace regex;

/***********************************************************************
tchs
***********************************************************************/

		class _tchs : public Combinator<EnumerableInput<RegexToken>, wchar_t>
		{
		protected:
			WString				chars;
			const wchar_t*		buffer;
		public:
			_tchs(const WString& _chars)
				:chars(_chars)
				,buffer(_chars.Buffer())
			{
			}

			ParsingResult<wchar_t> Parse(EnumerableInput<RegexToken>& input, Types<EnumerableInput<RegexToken>>::GlobalInfo& globalInfo)const
			{
				const wchar_t* reading=buffer;
				if(input.Available() && input.Current().length==1)
				{
					while(*reading)
					{
						if(*input.Current().reading==*reading)
						{
							input.Next();
							return ParsingResult<wchar_t>(*reading);
						}
						else
						{
							reading++;
						}
					}
				}
				//globalInfo.errors.Add(new CombinatorError<EnumerableInput<RegexToken>>(L"这里缺少字符\""+chars+L"\"。", input));
				return ParsingResult<wchar_t>();
			}
		};

/***********************************************************************
tstr
***********************************************************************/

		class _tstr : public Combinator<EnumerableInput<RegexToken>, WString>
		{
		protected:
			WString				chars;
			const wchar_t*		buffer;
		public:
			_tstr(const WString& _chars)
				:chars(_chars)
				,buffer(_chars.Buffer())
			{
			}

			ParsingResult<WString> Parse(EnumerableInput<RegexToken>& input, Types<EnumerableInput<RegexToken>>::GlobalInfo& globalInfo)const
			{
				if(input.Available() && input.Current().length==chars.Length())
				{
					if(wcsncmp(buffer, input.Current().reading, chars.Length())==0)
					{
						input.Next();
						return ParsingResult<WString>(chars);
					}
				}
				//globalInfo.errors.Add(new CombinatorError<EnumerableInput<RegexToken>>(L"这里缺少字符串\""+chars+L"\"。", input));
				return ParsingResult<WString>();
			}
		};

/***********************************************************************
trgx
***********************************************************************/

		class _trgx : public Combinator<EnumerableInput<RegexToken>, WString>
		{
		protected:
			Regex				regex;
			WString				code;
		public:
			_trgx(const WString& _regex)
				:regex(_regex)
				,code(_regex)
			{
			}

			ParsingResult<WString> Parse(EnumerableInput<RegexToken>& input, Types<EnumerableInput<RegexToken>>::GlobalInfo& globalInfo)const
			{
				if(input.Available())
				{
					WString token(input.Current().reading, input.Current().length);
					RegexMatch::Ref result=regex.MatchHead(token);
					if(result && result->Result().Length()==token.Length())
					{
						input.Next();
						return ParsingResult<WString>(result->Result().Value());
					}
				}
				//globalInfo.errors.Add(new CombinatorError<EnumerableInput<RegexToken>>(L"这里缺少字符串\""+code+L"\"。", input));
				return ParsingResult<WString>();
			}
		};

/***********************************************************************
辅助函数
***********************************************************************/

		Node<EnumerableInput<RegexToken>, wchar_t> tch(wchar_t c)
		{
			return new _tchs(c);
		}

		Node<EnumerableInput<RegexToken>, wchar_t> tchs(const WString& chars)
		{
			return new _tchs(chars);
		}

		Node<EnumerableInput<RegexToken>, WString> tstr(const WString& string)
		{
			return new _tstr(string);
		}

		Node<EnumerableInput<RegexToken>, WString> trgx(const WString& code)
		{
			return new _trgx(code);
		}
	}
}
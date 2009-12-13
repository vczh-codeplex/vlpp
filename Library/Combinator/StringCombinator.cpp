#include "StringCombinator.h"
#include "..\Regex\Regex.h"

namespace vl
{
	namespace combinator
	{
		using namespace regex;

/***********************************************************************
chs
***********************************************************************/

		class _chs : public Combinator<StringInput<wchar_t>, wchar_t>
		{
		protected:
			WString				chars;
			const wchar_t*		buffer;
		public:
			_chs(const WString& _chars)
				:chars(_chars)
				,buffer(_chars.Buffer())
			{
			}

			ParsingResult<wchar_t> Parse(StringInput<wchar_t>& input, Types<StringInput<wchar_t>>::GlobalInfo& globalInfo)const
			{
				const wchar_t* reading=buffer;
				if(input.Available())
				{
					while(*reading)
					{
						if(input.Current()==*reading)
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
				globalInfo.errors.Add(new CombinatorError<StringInput<wchar_t>>(L"这里缺少字符\""+chars+L"\"。", input));
				return ParsingResult<wchar_t>();
			}
		};

/***********************************************************************
str
***********************************************************************/

		class _str : public Combinator<StringInput<wchar_t>, WString>
		{
		protected:
			WString				chars;
			const wchar_t*		buffer;
		public:
			_str(const WString& _chars)
				:chars(_chars)
				,buffer(_chars.Buffer())
			{
			}

			ParsingResult<WString> Parse(StringInput<wchar_t>& input, Types<StringInput<wchar_t>>::GlobalInfo& globalInfo)const
			{
				if(input.Available())
				{
					const wchar_t* inputBuffer=&input.Current();
					if(wcsncmp(buffer, inputBuffer, chars.Length())==0)
					{
						input.Step(chars.Length());
						return ParsingResult<WString>(chars);
					}
				}
				globalInfo.errors.Add(new CombinatorError<StringInput<wchar_t>>(L"这里缺少字符串\""+chars+L"\"。", input));
				return ParsingResult<WString>();
			}
		};

/***********************************************************************
rgx
***********************************************************************/

		class _rgx : public Combinator<StringInput<wchar_t>, WString>
		{
		protected:
			Regex				regex;
			WString				code;
		public:
			_rgx(const WString& _regex)
				:regex(_regex)
				,code(_regex)
			{
			}

			ParsingResult<WString> Parse(StringInput<wchar_t>& input, Types<StringInput<wchar_t>>::GlobalInfo& globalInfo)const
			{
				RegexMatch::Ref result=regex.MatchHead(input.CurrentString());
				if(result)
				{
					input.Step(result->Result().Length());
					return ParsingResult<WString>(result->Result().Value());
				}
				else
				{
					globalInfo.errors.Add(new CombinatorError<StringInput<wchar_t>>(L"这里缺少字符串\""+code+L"\"。", input));
					return ParsingResult<WString>();
				}
			}
		};

/***********************************************************************
辅助函数
***********************************************************************/

		Node<StringInput<wchar_t>, wchar_t> ch(wchar_t c)
		{
			return unit<StringInput<wchar_t>>(c);
		}

		Node<StringInput<wchar_t>, wchar_t> chs(const WString& chars)
		{
			return new _chs(chars);
		}

		Node<StringInput<wchar_t>, WString> str(const WString& string)
		{
			return new _str(string);
		}

		Node<StringInput<wchar_t>, WString> rgx(const WString& code)
		{
			return new _rgx(code);
		}
	}
}
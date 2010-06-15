#include "TokenCombinator.h"

namespace vl
{
	namespace combinator
	{
		using namespace regex;

/***********************************************************************
tk(vint)
***********************************************************************/

		class _tk_i : public Combinator<TokenInput<RegexToken>, RegexToken>
		{
		protected:
			vint					token;
		public:
			_tk_i(vint _token)
				:token(_token)
			{
			}

			ParsingResult<RegexToken> Parse(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& globalInfo)const
			{
				if(input.Available() && input.Current().token==token)
				{
					RegexToken result=input.Current();
					input.Next();
					return ParsingResult<RegexToken>(result);
				}
				else
				{
					globalInfo.errors.Add(new CombinatorError<TokenInput<RegexToken>>(L"这里缺少相应记号。", input));
					return ParsingResult<RegexToken>();
				}
			}
		};

/***********************************************************************
tk(WString)
***********************************************************************/

		class _tk_s : public Combinator<TokenInput<RegexToken>, RegexToken>
		{
		protected:
			WString				token;
		public:
			_tk_s(WString _token)
				:token(_token)
			{
			}

			ParsingResult<RegexToken> Parse(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& globalInfo)const
			{
				if(input.Available() && input.Current().length==token.Length() && wcsncmp(input.Current().reading, token.Buffer(), token.Length())==0)
				{
					RegexToken result=input.Current();
					input.Next();
					return ParsingResult<RegexToken>(result);
				}
				else
				{
					globalInfo.errors.Add(new CombinatorError<TokenInput<RegexToken>>(L"这里缺少相应记号。", input));
					return ParsingResult<RegexToken>();
				}
			}
		};

/***********************************************************************
辅助函数
***********************************************************************/

		Node<TokenInput<RegexToken>, RegexToken> tk(vint token)
		{
			return new _tk_i(token);
		}

		Node<TokenInput<RegexToken>, RegexToken> tk(const WString& token)
		{
			return new _tk_s(token);
		}
	}
}
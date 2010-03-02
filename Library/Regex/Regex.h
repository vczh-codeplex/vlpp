/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Regex::Regular Expression

Classes:
	RegexString						：字符串匹配结果
	RegexMatch						：匹配结果
	Regex							：正则表达式引擎
	RegexToken						：词法记号
	RegexTokens						：词法记号表
	RegexLexer						：词法分析器
***********************************************************************/

#ifndef VCZH_REGEX_REGEX
#define VCZH_REGEX_REGEX

#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace regex_internal
	{
		class PureResult;
		class PureInterpretor;
		class RichResult;
		class RichInterpretor;
	}

	namespace regex
	{

/***********************************************************************
正则表达式引擎数据结构
***********************************************************************/

		class RegexString : public Object
		{
		protected:
			WString										value;
			int											start;
			int											length;

		public:
			RegexString(int _start=0);
			RegexString(const WString& _string, int _start, int _length);

			int											Start()const;
			int											Length()const;
			const WString&								Value()const;
			bool										operator==(const RegexString& string)const;
		};

		class RegexMatch : public Object, private NotCopyable
		{
			friend class Regex;
		public:
			typedef Ptr<RegexMatch>										Ref;
			typedef collections::List<Ref>								List;
			typedef collections::IReadonlyList<RegexString>				CaptureList;
			typedef collections::IReadonlyGroup<WString, RegexString>	CaptureGroup;
		protected:
			collections::List<RegexString>				captures;
			collections::Group<WString, RegexString>	groups;
			bool										success;
			RegexString									result;

			RegexMatch(const WString& _string, regex_internal::PureResult* _result);
			RegexMatch(const WString& _string, regex_internal::RichResult* _result, regex_internal::RichInterpretor* _rich);
			RegexMatch(const RegexString& _result);
		public:
			
			bool										Success()const;
			const RegexString&							Result()const;
			const CaptureList&							Captures()const;
			const CaptureGroup&							Groups()const;
		};

/***********************************************************************
正则表达式引擎
***********************************************************************/

		class Regex : public Object, private NotCopyable
		{
		protected:
			regex_internal::PureInterpretor*			pure;
			regex_internal::RichInterpretor*			rich;

			void										Process(const WString& text, bool keepEmpty, bool keepSuccess, bool keepFail, RegexMatch::List& matches)const;
		public:
			Regex(const WString& code, bool preferPure=true);
			~Regex();

			bool										IsPureMatch()const;
			bool										IsPureTest()const;

			RegexMatch::Ref								MatchHead(const WString& text)const;
			RegexMatch::Ref								Match(const WString& text)const;
			bool										TestHead(const WString& text)const;
			bool										Test(const WString& text)const;
			void										Search(const WString& text, RegexMatch::List& matches)const;
			void										Split(const WString& text, bool keepEmptyMatch, RegexMatch::List& matches)const;
			void										Cut(const WString& text, bool keepEmptyMatch, RegexMatch::List& matches)const;
		};

/***********************************************************************
正则表达式词法分析器
***********************************************************************/

		class RegexToken
		{
		public:
			int											start;
			int											length;
			int											token;
			const wchar_t*								reading;

			int											lineIndex;
			int											lineStart;

			bool										operator==(const RegexToken& _token)const;
		};

		class RegexTokens : public Object, public collections::IEnumerable<RegexToken>
		{
			friend class RegexLexer;
		protected:
			regex_internal::PureInterpretor*			pure;
			collections::Array<int>&					stateTokens;
			WString										code;
			
			RegexTokens(regex_internal::PureInterpretor* _pure, collections::Array<int>& _stateTokens, const WString& _code);
		public:

			collections::IEnumerator<RegexToken>*		CreateEnumerator()const;
		};

		class RegexLexer : public Object, private NotCopyable
		{
		protected:
			regex_internal::PureInterpretor*			pure;
			collections::Array<int>						ids;
			collections::Array<int>						stateTokens;
		public:
			RegexLexer(const collections::IEnumerable<WString>& tokens);
			~RegexLexer();

			RegexTokens									Parse(const WString& code);
		};
	}
}

#endif
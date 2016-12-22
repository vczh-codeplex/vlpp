/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Regex::Regular Expression

Classes:
	RegexString						���ַ���ƥ����
	RegexMatch						��ƥ����
	Regex							��������ʽ����
	RegexToken						���ʷ��Ǻ�
	RegexTokens						���ʷ��Ǻű�
	RegexLexer						���ʷ�������
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
������ʽ�������ݽṹ
***********************************************************************/

		class RegexString : public Object
		{
		protected:
			WString										value;
			vint											start;
			vint											length;

		public:
			RegexString(vint _start=0);
			RegexString(const WString& _string, vint _start, vint _length);

			vint											Start()const;
			vint											Length()const;
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
������ʽ����
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
������ʽ�ʷ�������
***********************************************************************/

		class RegexToken
		{
		public:
			vint										start;
			vint										length;
			vint										token;
			const wchar_t*								reading;

			vint										lineIndex;
			vint										lineStart;
			vint										codeIndex;

			bool										operator==(const RegexToken& _token)const;
			bool										operator==(const wchar_t* _token)const;
		};

		class RegexTokens : public Object, public collections::IEnumerable<RegexToken>
		{
			friend class RegexLexer;
		protected:
			regex_internal::PureInterpretor*			pure;
			collections::Array<vint>&					stateTokens;
			WString										code;
			vint											codeIndex;
			
			RegexTokens(regex_internal::PureInterpretor* _pure, collections::Array<vint>& _stateTokens, const WString& _code, vint _codeIndex);
		public:
			RegexTokens(const RegexTokens& regexTokens);

			collections::IEnumerator<RegexToken>*		CreateEnumerator()const;
			void										ReadToEnd(collections::List<RegexToken>& tokens, bool(*discard)(vint)=0)const;
		};

		class RegexLexer : public Object, private NotCopyable
		{
		protected:
			regex_internal::PureInterpretor*			pure;
			collections::Array<vint>						ids;
			collections::Array<vint>						stateTokens;
		public:
			RegexLexer(const collections::IEnumerable<WString>& tokens);
			~RegexLexer();

			RegexTokens									Parse(const WString& code, vint codeIndex=-1);
		};
	}
}

#endif
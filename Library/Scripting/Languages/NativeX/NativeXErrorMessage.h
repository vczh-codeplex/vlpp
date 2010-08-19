/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
StringResource::NativeXErrorMessage

Classes:
	NativeXErrorMessage									：字符串资源类
	
本文件使用Vczh String Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_NATIVEX_NATIVEXERRORMESSAGE
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_NATIVEX_NATIVEXERRORMESSAGE

#include "..\..\..\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			class NativeXErrorMessage
			{
			public:
				static WString UnrecognizedToken(const WString& token);
				static WString UnitAlreadyExists(const WString& name);
				static WString UnitNotExists(const WString& name);
				static WString UnitCircularReferenced(const WString& name);
				static WString NeedExpression();
				static WString NeedType();
				static WString NeedStatement();
				static WString NeedDeclaration();
				static WString NeedID();
				static WString NeedTypeExpression();
				static WString NeedLt();
				static WString NeedGt();
				static WString NeedOpenBrace();
				static WString NeedCloseBrace();
				static WString NeedCloseArray();
				static WString NeedComma();
				static WString NeedSemicolon();
				static WString NeedColon();
				static WString NeedCloseStat();
				static WString NeedAssign();
				static WString NeedOpenStruct();
				static WString NeedCloseStruct();
				static WString NeedOpenConcept();
				static WString NeedCloseConcept();
				static WString NeedWhile();
				static WString NeedWhen();
				static WString NeedWith();
				static WString NeedDo();
				static WString NeedUnit();
				static WString OperatorShouldNotBe(const WString& right, const WString& wrong);
			};
		}
	}
}

#endif
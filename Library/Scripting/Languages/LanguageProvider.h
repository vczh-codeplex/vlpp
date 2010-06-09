/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Language Provider

Classes:
  LanguageException						：错误信息

Interfaces:
  ILanguageProvider						：程序语言服务提供者
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDER
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDER

#include "..\..\Pointer.h"
#include "..\..\Exception.h"
#include "..\BasicIL\BasicILDefinition.h"
#include "LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		class LanguageException : public Exception
		{
		private:
			int									lineIndex;
			int									lineStart;
			int									textStart;
			int									codeIndex;
		public:
			LanguageException(const WString& message, int _lineIndex, int _lineStart, int _textStart, int _codeIndex);
			~LanguageException();

			int									LineIndex()const;
			int									LineStart()const;
			int									TextStart()const;
			int									CodeIndex()const;
		};

		class ILanguageProvider : public Interface
		{
		public:
			enum LanguageTypeEnum
			{
				Native,
				Managed,
				Dynamic,
			};

			virtual WString						LanguageName()=0;
			virtual LanguageTypeEnum			LanguageType()=0;
			virtual Ptr<LanguageAssembly>		Compile(
													const WString& assemblyName,
													collections::IReadonlyList<Ptr<LanguageAssembly>>& references,
													collections::IReadonlyList<WString>& codes, 
													collections::IList<Ptr<LanguageException>>& errors
													)=0;
		};
	}
}

#endif
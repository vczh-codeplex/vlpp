/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Classes:
  LanguageException						��������Ϣ

Interfaces:
  ILanguageProvider						���������Է����ṩ��
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
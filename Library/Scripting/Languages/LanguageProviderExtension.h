/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Interfaces:
  IBasicLanguageProvider						���������Է����ṩ��
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION

#include "LanguageProvider.h"
#include "..\BasicLanguage\BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		class IBasicLanguageProvider : public Interface
		{
		public:
			virtual Ptr<basiclanguage::BasicProgram>	ParseProgram(
															const WString& code, 
															Ptr<Object>& outputExtra,
															collections::IList<Ptr<LanguageException>>& errors
															)=0;

			virtual void								GenerateCode(
															Ptr<basiclanguage::BasicProgram> program,
															Ptr<Object> inputExtra,
															stream::TextWriter& writer
															)=0;

			virtual bool								GenerateHeader(
															Ptr<LanguageAssembly> assembly,
															Ptr<Object> inputExtra,
															stream::TextWriter& writer
															)=0;
		};
	}
}

#endif
#include "ManagedX.h"
#include "ManagedXErrorMessage.h"
#include "ManagedXCodeGenerator.h"
#include "ManagedXParser.h"

#include "..\LanguageProviderManagedExtension.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
/***********************************************************************
½Ó¿Ú
***********************************************************************/

			class ManagedXProvider : public Object, public ILanguageProvider, public IManagedLanguageProvider
			{
			protected:
				Ptr<ManagedXParser>						parser;
			public:
				ManagedXProvider()
				{
					parser=ManagedXParser::Create();
				}

				WString LanguageName()
				{
					return L"ManagedX";
				}

				LanguageTypeEnum LanguageType()
				{
					return ILanguageProvider::Managed;
				}

				Ptr<LanguageAssembly> Compile(
					const WString& assemblyName,
					collections::IReadonlyList<Ptr<LanguageAssembly>>& references,
					collections::IReadonlyList<WString>& codes, 
					collections::IList<Ptr<LanguageException>>& errors,
					stream::TextWriter* assemblyLogTextWriter=0,
					basicil::BasicIL::ICommentProvider* assemblyLogCommentProvider=0
					)
				{
					return 0;
				}

				Ptr<managedlanguage::ManagedProgram> ParseProgram(
					const WString& code, 
					Ptr<Object>& outputExtra,
					collections::IList<Ptr<LanguageException>>& errors
					)
				{
					Ptr<ManagedXUnit> unit=parser->Parse(code, 0, errors);
					if(unit)
					{
						return unit->program;
					}
					else
					{
						return 0;
					}
				}

				Ptr<basiclanguage::BasicProgram> CompileToBasicProgram(
					collections::IReadonlyList<Ptr<LanguageAssembly>>& references,
					collections::IReadonlyList<WString>& codes, 
					collections::IList<Ptr<LanguageException>>& errors
					)
				{
					return 0;
				}

				void GenerateCode(
					Ptr<managedlanguage::ManagedProgram> program,
					Ptr<Object> inputExtra,
					stream::TextWriter& writer
					)
				{
					ManagedX_GenerateCode_Program(program, writer);
				}

				bool GenerateHeader(
					Ptr<LanguageAssembly> assembly,
					Ptr<Object> inputExtra,
					stream::TextWriter& writer
					)
				{
					return false;
				}
			};
		}

		Ptr<ILanguageProvider> CreateManagedXLanguageProvider()
		{
			return new language_managedx::ManagedXProvider;
		}
	}
}
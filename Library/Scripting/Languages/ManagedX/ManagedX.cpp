#include "ManagedX.h"
#include "ManagedXErrorMessage.h"
#include "ManagedXCodeGenerator.h"

#include "..\LanguageProviderExtension.h"

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
			public:

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
					return 0;
				}

				void GenerateCode(
					Ptr<managedlanguage::ManagedProgram> program,
					Ptr<Object> inputExtra,
					stream::TextWriter& writer
					)
				{
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
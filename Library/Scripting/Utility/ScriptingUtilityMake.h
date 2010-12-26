/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ScriptingUtility::Make

Classes:
	LanguageMaker								£º±àÒë½Å±¾½âÊÍÆ÷
***********************************************************************/

#ifndef VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYMAKE
#define VCZH_SCRIPTING_UTILITY_SCRIPTINGUTILITYMAKE

#include "..\Languages\LanguageProvider.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			class LanguageMakerException : public Exception
			{
			public:
				Ptr<collections::List<Ptr<LanguageException>>> errors;

				LanguageMakerException(const WString& message);
				LanguageMakerException(const WString& message, const collections::IReadonlyList<Ptr<LanguageException>>& _errors);
			};

			class LanguageMakeFile : public Object
			{
			public:
				WString								baseLocation;
				WString								language;
				WString								assemblyName;
				WString								assemblyOutput;
				WString								headerLanguage;
				WString								headerOutput;
				WString								debugAssemblyOutput;
				collections::List<WString>			compiles;

				LanguageMakeFile();
				
				void								Load(const WString& _baseLocation, stream::TextReader& reader);
			};

			class LanguageMaker : public Object
			{
				typedef collections::Dictionary<WString, Ptr<ILanguageProvider>>		_ProviderMap;
				typedef collections::Dictionary<WString, Ptr<ILanguageProvider>(*)()>		_ProviderFactoryMap;
			protected:
				_ProviderMap						providers;
				_ProviderFactoryMap					providerFactories;

				Ptr<ILanguageProvider>				GetProvider(const WString& name);
			public:
				LanguageMaker();

				void								Execute(const LanguageMakeFile& makeFile);
			};
		}
	}
}

#endif
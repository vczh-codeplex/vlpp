/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Classes:
  LanguageAssembly						������
  LanguageHost							����������ʱ
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME

#include "..\BasicIL\BasicILInterpretor.h"
#include "LanguageMetadata.h"
#include "BasicLanguageMetadata.h"

namespace vl
{
	namespace scripting
	{
		class LanguageAssembly;
		class LanguageHost;

		class LanguageAssembly : public Object, public IMetadataProvider
		{
			friend class LanguageHost;
			typedef collections::IReadonlyDictionary<WString, Ptr<ResourceStream>>	_ResourceMap;
		protected:
			Ptr<basicil::BasicIL>						il;
			LanguageHost*								host;
			int											instructionKey;
			Ptr<BasicLanguageMetadata>					basicLanguageMetadata;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);

			LanguageHost*								GetHost();
			const _ResourceMap&							GetResources();
			Ptr<ResourceStream>							GetResource(const WString& name);
			int											GetInstructionKey();

			BasicLanguageMetadata*						GetBasicLanguageMetadata();
		};

		class LanguageHost : public Object
		{
		protected:
			Ptr<basicil::BasicILInterpretor>			interpretor;
			collections::List<Ptr<LanguageAssembly>>	loadedAssemblies;
		public:
			LanguageHost(int stackSize);

			bool										LoadAssembly(Ptr<LanguageAssembly> assembly);
		};
	}
}

#endif
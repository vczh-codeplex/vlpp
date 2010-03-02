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
#include "..\..\Pointer.h"

namespace vl
{
	namespace scripting
	{
		class LanguageHost;

		class LanguageAssembly : public Object
		{
			friend class LanguageHost;
		protected:
			Ptr<basicil::BasicIL>						il;
			LanguageHost*								host;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);

			Ptr<basicil::BasicIL>						GetIL();
			LanguageHost*								GetHost();
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
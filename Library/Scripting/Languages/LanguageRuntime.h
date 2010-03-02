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
		class LanguageAssembly : public Object
		{
		protected:
			Ptr<basicil::BasicIL>				il;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);

			Ptr<basicil::BasicIL>				GetIL();
		};

		class LanguageHost : public Object
		{
		protected:
			Ptr<basicil::BasicILInterpretor>	interpretor;
		public:
			LanguageHost(int stackSize);
		};
	}
}

#endif
#include "NativeX.h"
#include "..\..\BasicLanguage\BasicLanguageExpression.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace basicil;
			using namespace basiclanguage;
			using namespace regex;
			using namespace combinator;
			using namespace collections;

/***********************************************************************
�﷨������
***********************************************************************/

/***********************************************************************
�ӿ�
***********************************************************************/

			Ptr<ILanguageProvider> CreateNativeXLanguageProvider(NativeXMessageTranslator* messageTranslator)
			{
				return 0;
			}
		}
	}
}
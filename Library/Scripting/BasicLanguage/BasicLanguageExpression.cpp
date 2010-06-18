#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_DECLARATION_TARGETS)

			BasicPosition::BasicPosition()
				:start(0)
				,lineStart(0)
				,lineIndex(0)
				,codeIndex(0)
			{
			}

			bool BasicLinking::HasLink()const
			{
				return assemblyName!=L"" || symbolName!=L"";
			}

			bool BasicLinking::operator==(const BasicLinking& linking)const
			{
				return assemblyName==linking.assemblyName && symbolName==linking.symbolName;
			}

			bool BasicLinking::operator!=(const BasicLinking& linking)const
			{
				return assemblyName!=linking.assemblyName || symbolName!=linking.symbolName;
			}

			BasicBinaryExpression::BasicBinaryExpression()
			{
				shortcut=true;
			}

			bool BasicGeneric::HasGeneric()const
			{
				return arguments.Count()>0;
			}
		}
	}
}
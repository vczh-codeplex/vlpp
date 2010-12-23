#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

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

/***********************************************************************
Helper Functions
***********************************************************************/

			Ptr<BasicAttribute> BasicLanguage_FindFirstAttribute(const ICollection<Ptr<BasicAttribute>>& attributes, const WString& name)
			{
				for(vint i=0;i<attributes.Count();i++)
				{
					if(attributes[i]->attributeName==name)
					{
						return attributes[i];
					}
				}
				return 0;
			}

			bool BasicLanguage_FindAllAttributes(const ICollection<Ptr<BasicAttribute>>& attributes, const WString& name, ICollection<Ptr<BasicAttribute>>& result)
			{
				bool found=false;
				for(vint i=0;i<attributes.Count();i++)
				{
					if(attributes[i]->attributeName==name)
					{
						result.Add(attributes[i]);
						found=true;
					}
				}
				return found;
			}
		}
	}
}
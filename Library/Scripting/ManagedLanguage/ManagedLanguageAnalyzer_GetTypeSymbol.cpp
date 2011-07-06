#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			class ManagedAnalyzerGetTypeSymbolParameter : public Object
			{
			public:
				const MAP&						context;
				List<ManagedTypeSymbol*>&		result;

				ManagedAnalyzerGetTypeSymbolParameter(const MAP& _context, List<ManagedTypeSymbol*>& _result)
					:context(_context)
					,result(_result)
				{
				}
			};
			typedef ManagedAnalyzerGetTypeSymbolParameter MAGTSP;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAGTSP)

/***********************************************************************
ManagedLanguage_GetTypeSymbol_Type
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedGenericType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedType)
				{
					ManagedLanguage_GetTypeSymbol_ExtendedType(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type)

/***********************************************************************
ManagedLanguage_GetTypeSymbol_ExtendedType
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAGTSP)

				ALGORITHM_PROCEDURE_MATCH(ManagedArrayType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedEventType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAutoReferType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDynamicType)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
GetTypeSymbol
***********************************************************************/

			ManagedTypeSymbol* GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument)
			{
				List<ManagedTypeSymbol*> result;
				ManagedLanguage_GetTypeSymbol_Type(type, MAGTSP(argument, result));
				if(result.Count()==0)
				{
					//argument.errors.Add(ManagedLanguageCodeException::GetTypeNotExists(type.Obj()));
					return 0;
				}
				else if(result.Count()>1)
				{
					//argument.errors.Add(ManagedLanguageCodeException::GetTypeDuplicated(type.Obj()));
					return 0;
				}
				else
				{
					return result[0];
				}
			}
		}
	}
}
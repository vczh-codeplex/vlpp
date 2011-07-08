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
				List<ManagedSymbolItem*>&		temporaryResult;
				List<ManagedTypeSymbol*>&		result;

				ManagedAnalyzerGetTypeSymbolParameter(const MAP& _context, List<ManagedSymbolItem*>& _temporaryResult, List<ManagedTypeSymbol*>& _result)
					:context(_context)
					,temporaryResult(_temporaryResult)
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
					argument.result.Clear();
					ManagedTypeSymbol* arrayDecl=GetSystemType(node, L"Array"+itow(node->dimensionCount), argument.context, 1);
					ManagedTypeSymbol* element=GetTypeSymbol(node->elementType, argument.context);
					if(arrayDecl && element)
					{
						List<ManagedTypeSymbol*> genericArguments;
						genericArguments.Add(element);
						argument.result.Add(argument.context.symbolManager->GetType(arrayDecl->GetSymbol(), genericArguments.Wrap()));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionType)
				{
					argument.result.Clear();
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(Ptr<ManagedType>, parameter, node->parameterTypes.Wrap())
					{
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context);
						if(parameterType)
						{
							genericArguments.Add(parameterType);
						}
					}
					ManagedTypeSymbol* returnType=GetTypeSymbol(node->returnType, argument.context);
					ManagedTypeSymbol* voidType=GetSystemType(node, L"Void", argument.context);

					if(returnType && voidType && genericArguments.Count()==node->parameterTypes.Count())
					{
						if(returnType==voidType)
						{
							ManagedTypeSymbol* procedureDecl=GetSystemType(node, L"Procedure", argument.context, genericArguments.Count());
							if(procedureDecl)
							{
								if(genericArguments.Count()==0)
								{
									argument.result.Add(procedureDecl);
								}
								else
								{
									argument.result.Add(argument.context.symbolManager->GetType(procedureDecl->GetSymbol(), genericArguments.Wrap()));
								}
							}
						}
						else
						{
							genericArguments.Insert(0, returnType);
							ManagedTypeSymbol* functionDecl=GetSystemType(node, L"Function", argument.context, genericArguments.Count());
							if(functionDecl)
							{
								argument.result.Add(argument.context.symbolManager->GetType(functionDecl->GetSymbol(), genericArguments.Wrap()));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedEventType)
				{
					argument.result.Clear();
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(Ptr<ManagedType>, parameter, node->parameterTypes.Wrap())
					{
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context);
						if(parameterType)
						{
							genericArguments.Add(parameterType);
						}
					}

					if(genericArguments.Count()==node->parameterTypes.Count())
					{
						ManagedTypeSymbol* eventDecl=GetSystemType(node, L"Event", argument.context, genericArguments.Count());
						if(eventDecl)
						{
							if(genericArguments.Count()==0)
							{
								argument.result.Add(eventDecl);
							}
							else
							{
								argument.result.Add(argument.context.symbolManager->GetType(eventDecl->GetSymbol(), genericArguments.Wrap()));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAutoReferType)
				{
					argument.result.Clear();
					argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalAutoRefer(node));
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDynamicType)
				{
					argument.result.Clear();
					ManagedTypeSymbol* dynamicDecl=GetSystemType(node, L"IDynamic", argument.context);
					if(dynamicDecl)
					{
						argument.result.Add(dynamicDecl);
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
GetTypeSymbol
***********************************************************************/

			ManagedTypeSymbol* GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument)
			{
				List<ManagedSymbolItem*> temporaryResult;
				List<ManagedTypeSymbol*> result;
				ManagedLanguage_GetTypeSymbol_Type(type, MAGTSP(argument, temporaryResult, result));
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
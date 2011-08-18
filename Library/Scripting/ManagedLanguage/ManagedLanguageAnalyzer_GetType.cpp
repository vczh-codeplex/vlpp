#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
GetType
***********************************************************************/

			bool CanImplicitlyConvertTo(ManagedTypeSymbol* from, ManagedTypeSymbol* to, const MAP& argument)
			{
				return from==to;
			}

			ManagedTypeSymbol* GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument)
			{
				MAP newArgument(argument, argument.currentSymbol, expectedType);
				ManagedTypeSymbol* result=ManagedLanguage_GetTypeInternal_Expression(node, newArgument);
				argument.contextManager->SetExpression(node, result, newArgument.currentSymbol);
				if(!result || newArgument.expectedType && !CanImplicitlyConvertTo(result, newArgument.expectedType, argument))
				{
					argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, argument.expectedType));
				}
				return result;
			}

/***********************************************************************
ManagedLanguage_GetTypeInternal_Expression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedNullExpression)
				{
					ManagedSymbolItem* symbol=GetRealSymbol(argument.expectedType->GetSymbol());
					switch(symbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Interface:
						return argument.expectedType;
					default:
						return 0;
					}
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIntegerExpression)
				{
					if(
						argument.expectedType==argument.contextManager->predefinedTypes.sint8 ||
						argument.expectedType==argument.contextManager->predefinedTypes.sint16 ||
						argument.expectedType==argument.contextManager->predefinedTypes.sint32 ||
						argument.expectedType==argument.contextManager->predefinedTypes.sint64)
					{
						return argument.expectedType;
					}
					else if(!node->sign && (
						argument.expectedType==argument.contextManager->predefinedTypes.uint8 ||
						argument.expectedType==argument.contextManager->predefinedTypes.uint16 ||
						argument.expectedType==argument.contextManager->predefinedTypes.uint32 ||
						argument.expectedType==argument.contextManager->predefinedTypes.uint64))
					{
						return argument.expectedType;
					}
					else if(
						argument.expectedType==argument.contextManager->predefinedTypes.singleType ||
						argument.expectedType==argument.contextManager->predefinedTypes.doubleType)
					{
						return argument.expectedType;
					}
					else
					{
						return argument.contextManager->predefinedTypes.sint32;
					}
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFloatExpression)
				{
					if(argument.expectedType==argument.contextManager->predefinedTypes.singleType)
					{
						return argument.contextManager->predefinedTypes.singleType;
					}
					else
					{
						return argument.contextManager->predefinedTypes.doubleType;
					}
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBooleanExpression)
				{
					return argument.contextManager->predefinedTypes.boolType;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCharExpression)
				{
					return argument.contextManager->predefinedTypes.charType;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedStringExpression)
				{
					return argument.contextManager->predefinedTypes.stringType;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedReferenceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedMemberExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInstantiatedExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInvokeExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewObjectExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFunctionResultExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedThisExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBaseExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedAssignmentExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedExtendedExpression)
				{
					throw ManagedLanguage_GetTypeInternal_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_Expression)

/***********************************************************************
ManagedLanguage_GetTypeInternal_ExtendedExpression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedLambdaExpression)
				{
					// TODO: push lambda expression
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNullChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedTypeofExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedDefaultExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedUnaryExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBinaryExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewArrayExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIsTypeExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCastingExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIndexExpression)
				{
					throw 0;
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_ExtendedExpression)
		}
	}
}
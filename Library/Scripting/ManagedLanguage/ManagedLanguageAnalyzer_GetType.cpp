#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			class ManagedAnalyzerGetExpressionTypeParameter : public Object
			{
			public:
				struct Choice
				{
					ManagedTypeSymbol*					type;
					ManagedSymbolItem*					symbol;

					Choice(ManagedTypeSymbol* _type=0, ManagedSymbolItem* _symbol=0)
						:type(_type)
						,symbol(_symbol)
					{
					}

					bool operator==(const Choice& value){return false;}
					bool operator!=(const Choice& value){return true;}
				};
			public:
				const MAP&								context;
				List<Choice>&							choices;

				ManagedAnalyzerGetExpressionTypeParameter(const MAP& _context, List<Choice>& _choices)
					:context(_context)
					,choices(_choices)
				{
				}
			};
			typedef ManagedAnalyzerGetExpressionTypeParameter MAGETP;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAGETP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAGETP)

/***********************************************************************
GetType
***********************************************************************/

			bool CanImplicitlyConvertTo(ManagedTypeSymbol* from, ManagedTypeSymbol* to, const MAP& argument)
			{
				return from==to;
			}

			ManagedTypeSymbol* GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument)
			{
				List<MAGETP::Choice> choices;
				MAP a1(argument, argument.currentSymbol, expectedType);
				MAGETP a2(a1, choices);
				ManagedLanguage_GetTypeInternal_Expression(node, a2);

				if(expectedType)
				{
					for(int i=choices.Count()-1;i>=0;i--)
					{
						if(!CanImplicitlyConvertTo(choices[i].type, expectedType, argument))
						{
							choices.RemoveAt(i);
						}
					}
				}

				if(choices.Count()>1)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, expectedType));
				}
				else if(choices.Count()==1)
				{
					ManagedTypeSymbol* result=a2.choices[0].type;
					if(expectedType && !CanImplicitlyConvertTo(result, expectedType, argument))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, expectedType));
					}
					return result;
				}
				return 0;
			}

/***********************************************************************
ManagedLanguage_GetTypeInternal_Expression
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAGETP)

				ALGORITHM_PROCEDURE_MATCH(ManagedNullExpression)
				{
					ManagedSymbolItem* symbol=GetRealSymbol(argument.context.expectedType->GetSymbol());
					switch(symbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Interface:
						argument.choices.Add(argument.context.expectedType);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIntegerExpression)
				{
					if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint64)
					{
						argument.choices.Add(argument.context.expectedType);
					}
					else if(!node->sign && (
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint64))
					{
						argument.choices.Add(argument.context.expectedType);
					}
					else if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.doubleType)
					{
						argument.choices.Add(argument.context.expectedType);
					}
					else
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.sint32);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
					if(argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType)
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.singleType);
					}
					else
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.doubleType);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.boolType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.charType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.stringType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInvokeExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewObjectExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionResultExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAssignmentExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedExpression)
				{
					ManagedLanguage_GetTypeInternal_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression)

/***********************************************************************
ManagedLanguage_GetTypeInternal_ExtendedExpression
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAGETP)

				ALGORITHM_PROCEDURE_MATCH(ManagedLambdaExpression)
				{
					// TODO: push lambda expression
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNullChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeofExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDefaultExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
					// TODO: do this first
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewArrayExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIsTypeExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCastingExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIndexExpression)
				{
					throw 0;
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression)
		}
	}
}
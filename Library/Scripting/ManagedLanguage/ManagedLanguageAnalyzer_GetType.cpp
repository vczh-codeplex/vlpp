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
					for(vint i=choices.Count()-1;i>=0;i--)
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
					ManagedSymbolItem* symbol=a2.choices[0].symbol;
					ManagedTypeSymbol* type=a2.choices[0].type;
					// TODO: check non-expression symbol
					// TODO: check incomplete type
					if(expectedType && !CanImplicitlyConvertTo(type, expectedType, argument))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, expectedType));
					}
					return type;
				}
				return 0;
			}

			ManagedTypeSymbol* GetThisType(const MAP& argument)
			{
				List<ManagedSymbolDeclaration*> types;
				{
					ManagedSymbolItem* symbol=argument.currentSymbol;
					while(symbol)
					{
						switch(symbol->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							types.Add(dynamic_cast<ManagedSymbolDeclaration*>(symbol));
							break;
						}
						symbol=symbol->GetParentItem();
					}
				}

				ManagedTypeSymbol* type=0;
				for(vint i=types.Count()-1;i<=0;i--)
				{
					ManagedSymbolDeclaration* declaration=types[i];
					ManagedTypeSymbol* declarationType=argument.symbolManager->GetType(declaration, type);
					if(declaration->orderedGenericParameterNames.Count()>0)
					{
						List<ManagedTypeSymbol*> genericArguments;
						FOREACH(WString, genericParameterName, declaration->orderedGenericParameterNames.Wrap())
						{
							ManagedSymbolItem* genericParameter=declaration->ItemGroup(genericParameterName)->Items()[0];
							ManagedTypeSymbol* genericParameterType=argument.symbolManager->GetType(genericParameter);
							genericArguments.Add(genericParameterType);
						}
						declarationType=argument.symbolManager->GetInstiantiatedType(declarationType, genericArguments.Wrap());
					}
					type=declarationType;
				}
				return type;
			}

			ManagedTypeSymbol* GetBaseType(ManagedTypeSymbol* type, const MAP& argument)
			{
				ManagedSymbolDeclaration* declaration=dynamic_cast<ManagedSymbolDeclaration*>(GetRealSymbol(type->GetSymbol()));
				ManagedTypeSymbol* firstBaseType=0;
				FOREACH(ManagedTypeSymbol*, baseType, declaration->baseTypes.Wrap())
				{
					if(firstBaseType) break;
					switch(GetRealSymbol(baseType->GetSymbol())->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Interface:
						firstBaseType=baseType;
						break;
					}
				}

				if(firstBaseType)
				{
					return argument.symbolManager->ReplaceGenericArguments(firstBaseType, type);
				}
				else
				{
					return 0;
				}
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
					bool fail=true;
					ManagedMember* target=argument.context.contextManager->GetThisTargetMember();
					if(target)
					{
						ManagedSymbolItem* targetSymbol=argument.context.symbolManager->GetSymbol(target);
						switch(targetSymbol->GetSymbolType())
						{
						case ManagedSymbolItem::Method:
							{
								ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(targetSymbol);
								if(method->memberType==declatt::Instance)
								{
									fail=false;
								}
							}
							break;
						case ManagedSymbolItem::Constructor:
							fail=false;
							break;
						case ManagedSymbolItem::ConverterOperator:
							{
								ManagedSymbolConverterOperator* converterOperator=dynamic_cast<ManagedSymbolConverterOperator*>(targetSymbol);
								if(converterOperator->memberType==declatt::Instance)
								{
									fail=false;
								}
							}
							break;
						}
					}

					if(fail)
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalThis(node));
					}
					else
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						argument.choices.Add(MAGETP::Choice(thisType, thisType->GetSymbol()));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					bool fail=true;
					ManagedMember* target=argument.context.contextManager->GetThisTargetMember();
					if(target)
					{
						ManagedSymbolItem* targetSymbol=argument.context.symbolManager->GetSymbol(target);
						ManagedSymbolItem* definitionType=targetSymbol->GetParentItem();
						bool containsBaseClassOrStructure=false;

						switch(definitionType->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								ManagedSymbolDeclaration* declaration=dynamic_cast<ManagedSymbolDeclaration*>(definitionType);
								FOREACH(ManagedTypeSymbol*, baseType, declaration->baseTypes.Wrap())
								{
									switch(GetRealSymbol(baseType->GetSymbol())->GetSymbolType())
									{
									case ManagedSymbolItem::Class:
									case ManagedSymbolItem::Structure:
										containsBaseClassOrStructure=true;
										break;
									}
								}
							}
							break;
						}

						if(containsBaseClassOrStructure)
						{
							switch(targetSymbol->GetSymbolType())
							{
							case ManagedSymbolItem::Method:
								{
									ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(targetSymbol);
									if(method->memberType==declatt::Instance)
									{
										fail=false;
									}
								}
								break;
							case ManagedSymbolItem::Constructor:
								fail=false;
								break;
							case ManagedSymbolItem::ConverterOperator:
								{
									ManagedSymbolConverterOperator* converterOperator=dynamic_cast<ManagedSymbolConverterOperator*>(targetSymbol);
									if(converterOperator->memberType==declatt::Instance)
									{
										fail=false;
									}
								}
								break;
							}
						}
					}

					if(fail)
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalBase(node));
					}
					else
					{
						ManagedTypeSymbol* thisType=GetBaseType(GetThisType(argument.context), argument.context);
						argument.choices.Add(MAGETP::Choice(thisType, thisType->GetSymbol()));
					}
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
#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			struct ExpressionChoice
			{
				ManagedTypeSymbol*					type;	// symbol type if exists. for method and generic parameter, it this the owner type's type.
				ManagedSymbolItem*					symbol;

				ExpressionChoice(ManagedTypeSymbol* _type=0, ManagedSymbolItem* _symbol=0)
					:type(_type)
					,symbol(_symbol)
				{
				}

				bool operator==(const ExpressionChoice& value){return false;}
				bool operator!=(const ExpressionChoice& value){return true;}
			};

			struct ExpressionResolvingResult
			{

				ManagedTypeSymbol*						expressionResult;
				ManagedSymbolNamespace*					namespaceResult;
				List<ManagedTypeSymbol*>					typeResults;
				List<ExpressionChoice>					methodResults;

				ExpressionResolvingResult()
					:expressionResult(0)
					,namespaceResult(0)
				{
				}

				bool IsEmpty()
				{
					return expressionResult==0 && namespaceResult==0 && typeResults.Count()==0 && methodResults.Count()==0;
				}

				void Clear()
				{
					expressionResult=0;
					namespaceResult=0;
					typeResults.Clear();
					methodResults.Clear();
				}
			};

			class ManagedAnalyzerGetExpressionTypeParameter : public Object
			{
			public:
			public:
				const MAP&								context;
				ExpressionResolvingResult&				result;

				ManagedAnalyzerGetExpressionTypeParameter(const MAP& _context, ExpressionResolvingResult& _result)
					:context(_context)
					,result(_result)
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
				return from==to || IsInheritedFrom(from, to, argument);
			}

			bool IsValue(ManagedSymbolItem* symbol)
			{
				switch(symbol->GetSymbolType())
				{
				case ManagedSymbolItem::Variable:
				case ManagedSymbolItem::Lambda:
				case ManagedSymbolItem::LambdaParameter:
				case ManagedSymbolItem::Field:
				case ManagedSymbolItem::Property:
				case ManagedSymbolItem::PropertySetterValue:
				case ManagedSymbolItem::MethodParameter:
					return true;
				default:
					return false;
				}
			}

			ManagedTypeSymbol* GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument)
			{
				ExpressionResolvingResult expressionResolvingResult;
				MAP a1(argument, argument.currentSymbol, expectedType);
				MAGETP a2(a1, expressionResolvingResult);
				ManagedLanguage_GetTypeInternal_Expression(node, a2);

				if(expressionResolvingResult.expressionResult)
				{
					if(expectedType)
					{
						CheckTypeInMethod(node, expressionResolvingResult.expressionResult, argument);
						if(!CanImplicitlyConvertTo(expressionResolvingResult.expressionResult, expectedType, argument))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, expectedType));
						}
					}
				}
				else if(!expressionResolvingResult.IsEmpty())
				{
					argument.errors.Add(ManagedLanguageCodeException::GetExpressionIsNotValue(node));
				}

				return expressionResolvingResult.expressionResult;
			}

/***********************************************************************
Helper Functions
***********************************************************************/

			bool IsInStaticMethod(const MAP& argument)
			{
				ManagedMember* target=argument.contextManager->GetThisTargetMember();
				if(target)
				{
					ManagedSymbolItem* targetSymbol=argument.symbolManager->GetSymbol(target);
					switch(targetSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Method:
						{
							ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(targetSymbol);
							if(method->memberType==declatt::Instance)
							{
								return false;
							}
						}
						break;
					case ManagedSymbolItem::Constructor:
						return false;
					case ManagedSymbolItem::ConverterOperator:
						{
							ManagedSymbolConverterOperator* converterOperator=dynamic_cast<ManagedSymbolConverterOperator*>(targetSymbol);
							if(converterOperator->memberType==declatt::Instance)
							{
								return false;
							}
						}
						break;
					}
				}
				return true;
			}

			ManagedTypeSymbol* GetTypeFromInsideScope(ManagedSymbolItem* symbol, const MAP& argument)
			{
				List<ManagedSymbolDeclaration*> types;
				{
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
				for(vint i=types.Count()-1;i>=0;i--)
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

			ManagedTypeSymbol* GetThisType(const MAP& argument)
			{
				ManagedSymbolItem* currentSymbol=argument.currentSymbol;
				ManagedSymbolItem* thisSymbol=0;
				while(currentSymbol && !thisSymbol)
				{
					switch(currentSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						thisSymbol=currentSymbol;
						break;
					}
					currentSymbol=currentSymbol->GetParentItem();
				}
				return thisSymbol==0?0:GetTypeFromInsideScope(thisSymbol, argument);
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
					case ManagedSymbolItem::Structure:
						firstBaseType=baseType;
						break;
					}
				}

				if(firstBaseType)
				{
					return argument.symbolManager->GetBaseType(firstBaseType, type);
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
SearchMember
***********************************************************************/

			void SearchMemberOfNamespace(ManagedExpression* node, ManagedSymbolItem* namespaceItem, const WString& name, const MAGETP& argument)
			{
				if(ManagedSymbolItemGroup* group=namespaceItem->ItemGroup(name))
				{
					FOREACH(ManagedSymbolItem*, item, group->Items())
					{
						switch(item->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								argument.result.typeResults.Add(argument.context.symbolManager->GetType(item));
							}
							break;
						case ManagedSymbolItem::Namespace:
							{
								if(argument.result.namespaceResult==0)
								{
									argument.result.Clear();
									argument.result.namespaceResult=dynamic_cast<ManagedSymbolNamespace*>(item);
								}
								else
								{
									argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
								}
							}
							return;
						}
					}
				}
			}

			bool IsVisible(bool staticOnly, declatt::MemberType memberType)
			{
				return memberType==declatt::Static || !staticOnly;
			}

			bool IsVisible(ManagedTypeSymbol* thisType, ManagedTypeSymbol* containerType, declatt::Accessor accessor, const MAP& argument)
			{
				if(thisType==containerType)
				{
					return true;
				}
				else if(IsInheritedFrom(thisType, containerType, argument))
				{
					return accessor!=declatt::Private;
				}
				else
				{
					return accessor!=declatt::Private && accessor!=declatt::Protected;
				}
			}

			void SearchMemberOfType(ManagedExpression* node, ManagedTypeSymbol* thisType, ManagedTypeSymbol* containerType, bool staticOnly, const WString& name, const MAGETP& argument, bool searchForMethodOnly=false, bool searchForOperator=false)
			{
				// TODO: Search methods in base types
				ManagedSymbolItem* containerScope=GetRealSymbol(containerType->GetSymbol());
				switch(containerScope->GetSymbolType())
				{
				case ManagedSymbolItem::GenericParameter:
					// TODO:
					throw 0;
					break;
				case ManagedSymbolItem::Class:
				case ManagedSymbolItem::Structure:
				case ManagedSymbolItem::Interface:
					if(ManagedSymbolItemGroup* group=containerScope->ItemGroup(name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							switch(item->GetSymbolType())
							{
							case ManagedSymbolItem::Field:
								if(!searchForMethodOnly)
								{
									ManagedSymbolField* symbol=dynamic_cast<ManagedSymbolField*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context))
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										if(argument.result.IsEmpty())
										{
											argument.result.expressionResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							case ManagedSymbolItem::Property:
								if(!searchForMethodOnly)
								{
									ManagedSymbolProperty* symbol=dynamic_cast<ManagedSymbolProperty*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context) && !symbol->implementedInterfaceType)
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										if(argument.result.IsEmpty())
										{
											argument.result.expressionResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							case ManagedSymbolItem::Method:
								{
									ManagedSymbolMethod* symbol=dynamic_cast<ManagedSymbolMethod*>(item);
									if(symbol->inheritation!=declatt::Override)
									{
										if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context) && !symbol->implementedInterfaceType)
										{
											if(!argument.result.IsEmpty() && argument.result.methodResults.Count()==0)
											{
												argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
											}
											else
											{
												argument.result.methodResults.Add(ExpressionChoice(containerType, item));
											}
										}
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								if(!searchForMethodOnly)
								{
									if(IsVisible(thisType, containerType, dynamic_cast<ManagedSymbolDeclaration*>(item)->accessor, argument.context))
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->GetType(item, containerType);
										argument.result.typeResults.Add(symbolType);
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								if(!searchForMethodOnly)
								{
									ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(item);
									if(IsVisible(thisType, containerType, symbol->accessor, argument.context))
									{
										// TODO: this is wrong
										ManagedTypeSymbol* symbolType=GetRealType(symbol->type, argument.context);
										argument.result.typeResults.Add(symbolType);
									}
								}
								break;
							}
						}
					}

					if(argument.result.IsEmpty() || (argument.result.methodResults.Count()>0 && !searchForOperator))
					{
						FOREACH(ManagedTypeSymbol*, baseType, dynamic_cast<ManagedSymbolDeclaration*>(containerScope)->baseTypes.Wrap())
						{
							if(containerScope->GetSymbolType()==ManagedSymbolItem::Interface || GetRealSymbol(baseType->GetSymbol())->GetSymbolType()!=ManagedSymbolItem::Interface)
							{
								SearchMemberOfType(node, thisType, argument.context.symbolManager->GetBaseType(baseType, containerType), staticOnly, name, argument, (argument.result.methodResults.Count()>0), searchForOperator);
							}
						}
					}
					break;
				}
			}

/***********************************************************************
Method Overloading
***********************************************************************/

			struct MethodArgument
			{
				ManagedArgument::ArgumentType		argumentType;
				ExpressionResolvingResult*			value;
				ManagedTypeSymbol*					valueType;

				MethodArgument()
					:argumentType(ManagedArgument::Normal)
					,value(0)
					,valueType(0)
				{
				}

				MethodArgument(ManagedTypeSymbol* _valueType)
					:argumentType(ManagedArgument::Normal)
					,value(0)
					,valueType(_valueType)
				{
				}

				MethodArgument(ExpressionResolvingResult* _value)
					:argumentType(ManagedArgument::Normal)
					,value(_value)
					,valueType(0)
				{
				}

				MethodArgument(ManagedArgument::ArgumentType _parameterType, ExpressionResolvingResult* _value)
					:argumentType(_parameterType)
					,value(_value)
					,valueType(0)
				{
				}

				bool operator==(const MethodArgument& argument){return false;}
				bool operator!=(const MethodArgument& argument){return true;}
			};

			vint GetOverloadingScore(MethodArgument methodArgument, ManagedTypeSymbol* parameterType, ManagedParameter::ParameterType parameterAttribute, const MAP& argument, bool forOperator)
			{
				switch(parameterAttribute)
				{
				case ManagedParameter::Ref:
					if(methodArgument.argumentType!=ManagedArgument::Ref && !forOperator)
					{
						return -1;
					}
					break;
				case ManagedParameter::Out:
					if(methodArgument.argumentType!=ManagedArgument::Out)
					{
						return -1;
					}
					break;
				default:
					if(methodArgument.argumentType!=ManagedArgument::Normal)
					{
						return -1;
					}
				}
				ManagedTypeSymbol* expressionType=0;
				if(methodArgument.valueType)
				{
					expressionType=methodArgument.valueType;
				}
				else
				{
					expressionType=methodArgument.value->expressionResult;
				}
				if(expressionType)
				{
					if(expressionType==parameterType)
					{
						return 3;
					}
					else if(IsInheritedFrom(expressionType, parameterType, argument))
					{
						return 2;
					}
					else if(CanImplicitlyConvertTo(expressionType, parameterType, argument))
					{
						return 1;
					}
					else
					{
						return  -1;
					}
				}
				else
				{
					return -1;
				}
			}
			
			// perfect = 3
			// need type implicit conversion = 2
			// need generic parameter in-out conversion = 1
			// don't params expansion = +10
			// failed = -1
			vint GetOverloadingScore(ManagedSymbolItem* parameterContainer, List<WString>& parameterNames, ManagedTypeSymbol* contextType, List<MethodArgument>& methodArguments, const MAP& argument, bool forOperator)
			{
				vint score=3;
				bool enableParamsExpansion=false;
				bool needParamsExpansion=false;
				for(vint i=0;i<parameterNames.Count();i++)
				{
					ManagedSymbolMethodParameter* parameter=dynamic_cast<ManagedSymbolMethodParameter*>(parameterContainer->ItemGroup(parameterNames[i])->Items()[0]);
					if(i==parameterNames.Count()-1 && parameter->parameterType==ManagedParameter::Params)
					{
						enableParamsExpansion=true;
					}
					ManagedTypeSymbol* parameterType=argument.symbolManager->ReplaceGenericArguments(parameter->type, contextType);

					if(enableParamsExpansion)
					{
						vint currentScore=-1;
						if(parameterNames.Count()==methodArguments.Count())
						{
							currentScore=GetOverloadingScore(methodArguments[i], parameterType, parameter->parameterType, argument, forOperator);
							if(score>currentScore && currentScore!=-1)
							{
								score=currentScore;
							}
						}

						if(currentScore==-1)
						{
							ManagedTypeSymbol* paramsElementType=0;
							if(parameterType->GetGenericDeclaration()==argument.contextManager->predefinedTypes.array1OfTType)
							{
								paramsElementType=parameterType->GetGenericArguments()[0];
							}
							if(paramsElementType==0)
							{
								score=-1;
							}
							else
							{
								needParamsExpansion=true;
								if(i<methodArguments.Count())
								{
									for(vint j=i;j<methodArguments.Count();j++)
									{
										currentScore=GetOverloadingScore(methodArguments[j], paramsElementType, ManagedParameter::Normal, argument, forOperator)==-1;
										if(score>currentScore) score=currentScore;
									}
								}
							}
						}
					}
					else
					{
						if(i<methodArguments.Count())
						{
							vint currentScore=GetOverloadingScore(methodArguments[i], parameterType, parameter->parameterType, argument, forOperator);
							if(score>currentScore) score=currentScore;
						}
						else
						{
							score=-1;
						}
					}
				}

				if(enableParamsExpansion)
				{
					if(parameterNames.Count()>=methodArguments.Count()-1) return -1;
				}
				else if(parameterNames.Count()!=methodArguments.Count())
				{
					return -1;
				}
				return score==-1?-1:(score+(needParamsExpansion?0:10));
			}

			template<typename T>
			ExpressionChoice SearchMatchedMethods(ManagedExpression* node, List<ExpressionChoice>& methodChoices, List<MethodArgument>& methodArguments, ManagedTypeSymbol* thisType, const MAP& argument, bool addErrorWhenNothingMatched, bool forOperator)
			{
				vint methodOverloadingScore=-1;
				List<ExpressionChoice> methodOverloadingResult;

				FOREACH(ExpressionChoice, methodChoice, methodChoices.Wrap())
				{
					T* method=dynamic_cast<T*>(methodChoice.symbol);
					if(method && IsVisible(thisType, methodChoice.type, method->accessor, argument))
					{
						vint score=GetOverloadingScore(method, method->orderedMethodParameterNames, methodChoice.type, methodArguments, argument, forOperator);
						if(score!=-1 && score>=methodOverloadingScore)
						{
							if(score>methodOverloadingScore)
							{
								methodOverloadingResult.Clear();
							}
							methodOverloadingResult.Add(methodChoice);
						}
					}
				}

				switch(methodOverloadingResult.Count())
				{
				case 0:
					if(addErrorWhenNothingMatched)
					{
						argument.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
					}
					break;
				case 1:
					return methodOverloadingResult[0];
				default:
					argument.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
				}
				return ExpressionChoice();
			}

			bool ConvertArguments(List<Ptr<ExpressionResolvingResult>>& methodArgumentResults, List<MethodArgument>& methodArguments, List<Ptr<ManagedArgument>>& arguments, const MAGETP& argument)
			{
				FOREACH(Ptr<ManagedArgument>, methodArgument, arguments.Wrap())
				{
					// TODO: check defaultParameterName
					// TODO: check out/ref right value
					Ptr<ExpressionResolvingResult> result=new ExpressionResolvingResult;
					methodArgumentResults.Add(result);
					MAGETP newArgument(argument.context, *result.Obj());
					ManagedLanguage_GetTypeInternal_Expression(methodArgument->value, newArgument);
					if(result->IsEmpty())
					{
						return false;
					}
				}

				for(vint i=0;i<arguments.Count();i++)
				{
					methodArguments.Add(MethodArgument(arguments[i]->argumentType, methodArgumentResults[i].Obj()));
				}
				return true;
			}

			bool ConvertArguments(List<Ptr<ExpressionResolvingResult>>& methodArgumentResults, List<MethodArgument>& methodArguments, List<Ptr<ManagedExpression>>& arguments, const MAGETP& argument)
			{
				FOREACH(Ptr<ManagedExpression>, methodArgument, arguments.Wrap())
				{
					Ptr<ExpressionResolvingResult> result=new ExpressionResolvingResult;
					methodArgumentResults.Add(result);
					MAGETP newArgument(argument.context, *result.Obj());
					ManagedLanguage_GetTypeInternal_Expression(methodArgument, newArgument);
					if(result->IsEmpty())
					{
						return false;
					}
				}

				for(vint i=0;i<arguments.Count();i++)
				{
					methodArguments.Add(MethodArgument(ManagedArgument::Normal, methodArgumentResults[i].Obj()));
				}
				return true;
			}

/***********************************************************************
ManagedLanguage_GetTypeInternal_Expression
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAGETP)

				ALGORITHM_PROCEDURE_MATCH(ManagedNullExpression)
				{
					if(argument.context.expectedType)
					{
						ManagedSymbolItem* symbol=GetRealSymbol(argument.context.expectedType->GetSymbol());
						switch(symbol->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Interface:
							argument.result.expressionResult=argument.context.expectedType;
						}
					}
					if(!argument.result.expressionResult)
					{
						//argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalNull(node));
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
						argument.result.expressionResult=argument.context.expectedType;
					}
					else if(!node->sign && (
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint64))
					{
						argument.result.expressionResult=argument.context.expectedType;
					}
					else if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.doubleType)
					{
						argument.result.expressionResult=argument.context.expectedType;
					}
					else
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.sint32;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
					if(argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType)
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.singleType;
					}
					else
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.doubleType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.boolType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.charType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.stringType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
					bool inStaticMethod=IsInStaticMethod(argument.context);
					ManagedSymbolItem* currentScope=argument.context.currentSymbol;
					ManagedTypeSymbol* thisType=GetThisType(argument.context);
					bool firstType=true;

					while(argument.result.IsEmpty() && currentScope!=0)
					{
						switch(currentScope->GetSymbolType())
						{
						case ManagedSymbolItem::Block:
						case ManagedSymbolItem::Lambda:
						case ManagedSymbolItem::Property:
						case ManagedSymbolItem::Method:
						case ManagedSymbolItem::Constructor:
						case ManagedSymbolItem::ConverterOperator:
							{
								if(ManagedSymbolItemGroup* group=currentScope->ItemGroup(node->name))
								{
									FOREACH(ManagedSymbolItem*, item, group->Items())
									{
										switch(item->GetSymbolType())
										{
										case ManagedSymbolItem::Variable:
											{
												ManagedSymbolVariable* symbol=dynamic_cast<ManagedSymbolVariable*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::LambdaParameter:
											{
												ManagedSymbolLambdaParameter* symbol=dynamic_cast<ManagedSymbolLambdaParameter*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::GenericParameter:
											{
												argument.result.typeResults.Add(argument.context.symbolManager->GetType(item));
											}
											return;
										case ManagedSymbolItem::MethodParameter:
											{
												ManagedSymbolMethodParameter* symbol=dynamic_cast<ManagedSymbolMethodParameter*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::PropertySetterValue:
											{
												ManagedSymbolPropertySetterValue* symbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(item);
												ManagedSymbolProperty* propertySymbol=dynamic_cast<ManagedSymbolProperty*>(item->GetParentItem());
												argument.result.expressionResult=propertySymbol->type;
											}
											return;
										}
									}
								}
							}
							break;
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								if(ManagedSymbolItemGroup* group=currentScope->ItemGroup(node->name))
								{
									FOREACH(ManagedSymbolItem*, item, group->Items())
									{
										switch(item->GetSymbolType())
										{
										case ManagedSymbolItem::GenericParameter:
											{
												argument.result.typeResults.Add(argument.context.symbolManager->GetType(item));
											}
											return;
										}
									}
								}
								ManagedTypeSymbol* containerType=GetTypeFromInsideScope(currentScope, argument.context);
								SearchMemberOfType(node, thisType, containerType, (!firstType || inStaticMethod), node->name, argument);

								if(argument.result.IsEmpty())
								{
									ManagedSymbolDeclaration* currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(currentScope);
									FOREACH(ManagedSymbolNamespace*, namespaceSymbol, currentDeclaration->_availableUsingNamespaces.Wrap())
									{
										SearchMemberOfNamespace(node, namespaceSymbol, node->name, argument);
									}
								}
								firstType=false;
							}
							break;
						case ManagedSymbolItem::Global:
						case ManagedSymbolItem::Namespace:
							{
								SearchMemberOfNamespace(node, currentScope, node->name, argument);
							}
							break;
						}
						currentScope=currentScope->GetParentItem();
					}

					if(argument.result.IsEmpty())
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
					if(node->operand)
					{
						ManagedLanguage_GetTypeInternal_Expression(node->operand, argument);
						if(!argument.result.IsEmpty())
						{
							if(argument.result.methodResults.Count()>0)
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionIsNotValue(node));
							}
							else
							{
								bool inStaticMethod=IsInStaticMethod(argument.context);
								ManagedTypeSymbol* thisType=GetThisType(argument.context);
							
								if(argument.result.expressionResult)
								{
									ManagedTypeSymbol* type=argument.result.expressionResult;
									argument.result.Clear();
									SearchMemberOfType(node, thisType, type, inStaticMethod, node->member, argument);
								}
								else if(argument.result.typeResults.Count()==0)
								{
									ManagedTypeSymbol* type=0;
									FOREACH(ManagedTypeSymbol*, typeResult, argument.result.typeResults.Wrap())
									{
										ManagedSymbolItem* typeSymbol=GetRealSymbol(typeResult->GetSymbol());
										switch(typeSymbol->GetSymbolType())
										{
										case ManagedSymbolItem::Class:
										case ManagedSymbolItem::Structure:
										case ManagedSymbolItem::Interface:
											{
												ManagedSymbolDeclaration* typeDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(typeSymbol);
												if(typeDeclaration->orderedGenericParameterNames.Count()>0)
												{
													break;
												}
											}
										case ManagedSymbolItem::GenericParameter:
											if(type==0)
											{
												type=typeResult;
											}
											else
											{
												argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node->operand.Obj(), 0));
											}
											break;
										}
									}
									if(type)
									{
										SearchMemberOfType(node, thisType, type, inStaticMethod, node->member, argument);
									}
								}
								else if(argument.result.namespaceResult)
								{
									ManagedSymbolNamespace* namespaceResult=argument.result.namespaceResult;
									argument.result.Clear();
									SearchMemberOfNamespace(node, namespaceResult, node->member, argument);
								}

								if(argument.result.IsEmpty())
								{
									argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
								}
							}
						}
					}
					else
					{
						SearchMemberOfNamespace(node, argument.context.symbolManager->Global(), node->member, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedExpression)
				{
					List<ManagedTypeSymbol*> argumentTypes;
					FOREACH(Ptr<ManagedType>, argumentType, node->argumentTypes.Wrap())
					{
						ManagedTypeSymbol* type=GetTypeSymbolInMethod(argumentType, argument.context);
						if(type)
						{
							argumentTypes.Add(type);
						}
					}

					ExpressionResolvingResult operandResult;
					MAGETP newArgument(argument.context, operandResult);
					ManagedLanguage_GetTypeInternal_Expression(node->operand, newArgument);

					if(argumentTypes.Count()==node->argumentTypes.Count())
					{
						if(operandResult.typeResults.Count()>0)
						{
							ManagedTypeSymbol* type=0;
							FOREACH(ManagedTypeSymbol*, typeResult, argument.result.typeResults.Wrap())
							{
								ManagedSymbolItem* typeSymbol=GetRealSymbol(typeResult->GetSymbol());
								switch(typeSymbol->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
									{
										ManagedSymbolDeclaration* typeDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(typeSymbol);
										if(typeDeclaration->orderedGenericParameterNames.Count()==argumentTypes.Count())
										{
											if(type==0)
											{
												type=typeResult;
											}
											else
											{
												argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node->operand.Obj(), 0));
											}
										}
									}
									break;
								}
							}

							if(type)
							{
								ManagedTypeSymbol* instanciatedType=argument.context.symbolManager->GetInstiantiatedType(type, argumentTypes.Wrap());
								CheckTypeInMethod(node, instanciatedType, argument.context);
								argument.result.typeResults.Add(instanciatedType);
							}
						}
						else if(operandResult.methodResults.Count()>0)
						{
							throw 0;
						}
						else
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalInstanciation(node));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInvokeExpression)
				{
					ExpressionResolvingResult methodResults;
					MAGETP newArgument(argument.context, methodResults);
					ManagedLanguage_GetTypeInternal_Expression(node->function, newArgument);

					if(!methodResults.IsEmpty())
					{
						if(methodResults.methodResults.Count()>0)
						{
							List<Ptr<ExpressionResolvingResult>> methodArgumentResults;
							List<MethodArgument> methodArguments;
							if(ConvertArguments(methodArgumentResults, methodArguments, node->arguments, argument))
							{
								ManagedTypeSymbol* thisType=GetThisType(argument.context);
								SearchMatchedMethods<ManagedSymbolMethod>(node, methodResults.methodResults, methodArguments, thisType, argument.context, true, false);
							}
						}
						else
						{
							// TODO: check op_invoke
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewObjectExpression)
				{
					ManagedTypeSymbol* objectType=GetTypeSymbolInMethod(node->objectType, argument.context);
					if(objectType)
					{
						List<Ptr<ExpressionResolvingResult>> methodArgumentResults;
						List<MethodArgument> methodArguments;
						ManagedSymbolItem* objectSymbol=GetRealSymbol(objectType->GetSymbol());
						if(ConvertArguments(methodArgumentResults, methodArguments, node->arguments, argument))
						{
							if(ManagedSymbolItemGroup* group=objectSymbol->ItemGroup(ManagedSymbolConstructor::SymbolName))
							{
								List<ExpressionChoice> constructorChoices;
								FOREACH(ManagedSymbolItem*, constructorItem, group->Items())
								{
									constructorChoices.Add(ExpressionChoice(objectType, constructorItem));
								}

								ManagedTypeSymbol* thisType=GetThisType(argument.context);
								SearchMatchedMethods<ManagedSymbolConstructor>(node, constructorChoices, methodArguments, thisType, argument.context, true, false);
							}
							else if(node->arguments.Count()>0)
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
							}
						}
						// TODO: check property setters
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionResultExpression)
				{
					ManagedMember* member=0;
					ManagedLambdaExpression* lambda=0;
					ManagedTypeSymbol* returnType=argument.context.contextManager->GetResultTarget(member, lambda);
					if(member)
					{
						argument.result.expressionResult=returnType;
					}
					else
					{
						// TODO: check lambda expression
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalFunctionResult(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
					if(IsInStaticMethod(argument.context))
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalThis(node));
					}
					else
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						argument.result.expressionResult=thisType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					bool fail=true;
					if(!IsInStaticMethod(argument.context))
					{
						ManagedMember* target=argument.context.contextManager->GetThisTargetMember();
						if(target)
						{
							ManagedSymbolItem* targetSymbol=argument.context.symbolManager->GetSymbol(target);
							ManagedSymbolItem* definitionType=targetSymbol->GetParentItem();

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
											fail=false;
											break;
										}
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
						ManagedTypeSymbol* baseType=GetBaseType(GetThisType(argument.context), argument.context);
						argument.result.expressionResult=baseType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAssignmentExpression)
				{
					// TODO: check right value
					ManagedTypeSymbol* toType=GetType(node->leftOperand.Obj(), 0, argument.context);
					GetType(node->rightOperand.Obj(), toType, argument.context);
					if(toType)
					{
						argument.result.expressionResult=toType;
					}
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
					ManagedTypeSymbol* operandType=GetType(node->operand.Obj(), 0, argument.context);
					if(operandType)
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						ExpressionResolvingResult operandResult;
						MAGETP operandArgument(argument.context, operandResult);
						SearchMemberOfType(node, thisType, operandType, true, node->operatorName, operandArgument, false, true);

						if(!operandResult.IsEmpty() && operandResult.methodResults.Count()==0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
						}
						else
						{
							List<MethodArgument> methodArguments;
							methodArguments.Add(MethodArgument(operandType));
							ExpressionChoice matchedOperator=SearchMatchedMethods<ManagedSymbolMethod>(node, operandResult.methodResults, methodArguments, thisType, argument.context, true, true);

							if(matchedOperator.symbol)
							{
								ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(matchedOperator.symbol);
								argument.result.expressionResult=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, matchedOperator.type);
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
					ManagedTypeSymbol* leftType=GetType(node->leftOperand.Obj(), 0, argument.context);
					ManagedTypeSymbol* rightType=GetType(node->rightOperand.Obj(), 0, argument.context);
					if(leftType && rightType)
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						ExpressionResolvingResult leftResult, rightResult;
						MAGETP leftArgument(argument.context, leftResult), rightArgument(argument.context, rightResult);
						SearchMemberOfType(node, thisType, leftType, true, node->operatorName, leftArgument, false, true);
						SearchMemberOfType(node, thisType, rightType, true, node->operatorName, rightArgument, false, true);

						List<MethodArgument> methodArguments;
						methodArguments.Add(MethodArgument(leftType));
						methodArguments.Add(MethodArgument(rightType));
						
						if(!leftResult.IsEmpty() && leftResult.methodResults.Count()==0 && !rightResult.IsEmpty() && rightResult.methodResults.Count()==0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
						}
						else
						{
							ExpressionChoice matchedOperator=SearchMatchedMethods<ManagedSymbolMethod>(node, leftArgument.result.methodResults, methodArguments, thisType, argument.context, false, true);
							if(matchedOperator.symbol)
							{
								ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(matchedOperator.symbol);
								argument.result.expressionResult=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, matchedOperator.type);
							}
							else
							{
								matchedOperator=SearchMatchedMethods<ManagedSymbolMethod>(node, rightArgument.result.methodResults, methodArguments, thisType, argument.context, true, true);
								if(matchedOperator.symbol)
								{
									ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(matchedOperator.symbol);
									argument.result.expressionResult=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, matchedOperator.type);
								}
							}
						}
					}
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
					ManagedTypeSymbol* fromType=GetType(node->operand.Obj(), 0, argument.context);
					ManagedTypeSymbol* toType=GetTypeSymbolInMethod(node->type, argument.context);
					if(fromType && toType)
					{
						if(!CanImplicitlyConvertTo(fromType, toType, argument.context) && !IsInheritedFrom(toType, fromType, argument.context))
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionCannotCastToType(node, toType));
						}
						argument.result.expressionResult=toType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIndexExpression)
				{
					List<Ptr<ExpressionResolvingResult>> methodArgumentResults;
					List<MethodArgument> methodArguments;
					if(ConvertArguments(methodArgumentResults, methodArguments, node->indices, argument))
					{
						WString operatorName=node->forRead?L"op_index_get":L"op_index_set";
						ManagedTypeSymbol* operandType=GetType(node->operand.Obj(), 0, argument.context);
						if(operandType)
						{
							ManagedTypeSymbol* thisType=GetThisType(argument.context);
							ExpressionResolvingResult operandResult;
							MAGETP operandArgument(argument.context, operandResult);
							SearchMemberOfType(node, thisType, operandType, true, operatorName, operandArgument, false, true);
							
							if(!operandResult.IsEmpty() && operandResult.methodResults.Count()==0)
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
							}
							else
							{
								SearchMatchedMethods<ManagedSymbolMethod>(node, operandResult.methodResults, methodArguments, thisType, argument.context, true, true);
							}
						}
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression)
		}
	}
}
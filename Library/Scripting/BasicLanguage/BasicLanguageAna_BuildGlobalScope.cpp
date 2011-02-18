#include "BasicLanguageAnalyzer.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
Helper Functions
***********************************************************************/

			BP BuildBasicGenericScope(BasicDeclaration* declaration, const BP& argument)
			{
				if(declaration->genericDeclaration.HasGeneric())
				{
					BasicScope* scope=0;
					BasicFunctionDeclaration* functionDeclaration=dynamic_cast<BasicFunctionDeclaration*>(declaration);
					if(functionDeclaration)
					{
						scope=argument.env->CreateFunctionScope(argument.scope, functionDeclaration);
					}
					else
					{
						scope=argument.env->CreateScope(argument.scope);
					}
					for(vint i=0;i<declaration->genericDeclaration.arguments.Count();i++)
					{
						const WString& name=declaration->genericDeclaration.arguments[i];
						if(scope->types.Items().Keys().Contains(name))
						{
							argument.errors.Add(BasicLanguageCodeException::GetGenericArgumentAlreadyExists(declaration, name));
						}
						else
						{
							scope->types.Add(name, argument.typeManager->GetGenericArgumentType(name));
						}
					}
					for(vint i=0;i<declaration->genericDeclaration.constraints.Count();i++)
					{
						Ptr<BasicGeneric::Constraint> constraint=declaration->genericDeclaration.constraints[i];
						if(!scope->types.Items().Keys().Contains(constraint->argumentName))
						{
							argument.errors.Add(BasicLanguageCodeException::GetGenericArgumentNotExists(declaration, constraint->argumentName));
						}
						else
						{
							Ptr<BasicScope::Concept> concept=scope->concepts.Find(constraint->conceptName);
							if(!concept)
							{
								argument.errors.Add(BasicLanguageCodeException::GetConceptNotExists(declaration, constraint->conceptName));
							}
							else
							{
								scope->genericConstraints.Add(argument.typeManager->GetGenericArgumentType(constraint->argumentName), constraint->conceptName);
							}
						}
					}
					BP newArgument(argument, scope);
					return BP(argument, scope);
				}
				else
				{
					return argument;
				}
			}

			BasicTypeRecord* BuildBasicGenericType(BasicTypeRecord* type, BasicDeclaration* declaration, const BP& argument)
			{
				if(declaration->genericDeclaration.HasGeneric())
				{
					BasicTypeRecord* genericType=argument.typeManager->CreateGenericType(dynamic_cast<BasicStructureDeclaration*>(declaration));
					List<BasicTypeRecord*> genericArguments;
					for(vint i=0;i<declaration->genericDeclaration.arguments.Count();i++)
					{
						BasicTypeRecord* genericArgument=argument.typeManager->GetGenericArgumentType(declaration->genericDeclaration.arguments[i]);
						if(!genericArguments.Contains(genericArgument))
						{
							genericArguments.Add(genericArgument);
						}
					}
					argument.typeManager->UpdateGenericType(genericType, type, genericArguments.Wrap());
					return genericType;
				}
				else
				{
					return type;
				}
			}

			bool BasicLanguage_EnsureNothingExists(BasicDeclaration* declaration, const BP& argument, const WString& name)
			{
				if(argument.scope->functions.Items().Keys().Contains(name))
				{
					argument.errors.Add(BasicLanguageCodeException::GetFunctionAlreadyExists(declaration));
					return false;
				}
				else if(argument.scope->variables.Items().Keys().Contains(name))
				{
					argument.errors.Add(BasicLanguageCodeException::GetVariableAlreadyExists(declaration));
					return false;
				}
				else if(argument.scope->types.Items().Keys().Contains(name))
				{
					argument.errors.Add(BasicLanguageCodeException::GetTypeAlreadyExists(declaration));
					return false;
				}
				else if(argument.scope->concepts.Items().Keys().Contains(name))
				{
					argument.errors.Add(BasicLanguageCodeException::GetConceptAlreadyExists(declaration));
					return false;
				}
				else
				{
					return true;
				}
			}

/***********************************************************************
BasicLanguage_BuildGlobalScopePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(BasicLanguage_EnsureNothingExists(node, argument, node->name))
					{
						BP internalArgument=BuildBasicGenericScope(node, argument);
						argument.scope->functions.Add(node->name, node);
						BasicTypeRecord* type=0;
						try
						{
							type=BasicLanguage_GetTypeRecord(node->signatureType, internalArgument, false);
							type=BuildBasicGenericType(type, node, internalArgument);
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
						if(type)
						{
							argument.env->RegisterFunctionType(node, type);
						}
					}
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					vint forward=argument.forwardStructures.IndexOf(node->name);
					if(forward==-1 && !BasicLanguage_EnsureNothingExists(node, argument, node->name))
					{
						return;
					}
					if(!node->defined)
					{
						if(forward==-1)
						{
							argument.forwardStructures.Add(node->name);
							BasicTypeRecord* structure=argument.typeManager->CreateStructureType(node);
							structure=BuildBasicGenericType(structure, node, argument);
							argument.scope->types.Add(node->name, structure);
						}
					}
					else
					{
						BasicTypeRecord* structure=0;
						if(forward==-1)
						{
							structure=argument.typeManager->CreateStructureType(node);
							BasicTypeRecord* genericType=BuildBasicGenericType(structure, node, argument);
							argument.scope->types.Add(node->name, genericType);
						}
						else
						{
							argument.forwardStructures.RemoveAt(forward);
							BasicTypeRecord* type=argument.scope->types.Items()[node->name];
							if(type->GetType()==BasicTypeRecord::Generic)
							{
								type=type->ElementType();
							}
							structure=dynamic_cast<BasicStructureTypeRecord*>(type);
						}

						List<WString> names;
						List<BasicTypeRecord*> types;
						BP internalArgument=BuildBasicGenericScope(node, argument);
						for(vint i=0;i<node->memberNames.Count();i++)
						{
							if(node->memberNames.IndexOf(node->memberNames[i])==i)
							{
								try
								{
									BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->memberTypes[i], internalArgument, false);
									if(type->GetType()==BasicTypeRecord::Structure)
									{
										if(!type->Defined())
										{
											argument.errors.Add(BasicLanguageCodeException::GetStructureMemberCannotBeUndefinedType(node, i));
										}
									}
									types.Add(type);
									names.Add(node->memberNames[i]);
								}
								catch(Ptr<BasicLanguageCodeException> e)
								{
									argument.errors.Add(e);
								}
							}
							else
							{
								argument.errors.Add(BasicLanguageCodeException::GetStructureMemberAlreadyExists(node, i));
							}
						}
						argument.typeManager->UpdateStructureType(structure, names.Wrap(), types.Wrap());
						argument.env->RegisterStructureType(node, structure);
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(BasicLanguage_EnsureNothingExists(node, argument, node->name))
					{
						try
						{
							BP internalArgument=BuildBasicGenericScope(node, argument);
							BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->type, internalArgument, false);
							type=BuildBasicGenericType(type, node, internalArgument);
							argument.scope->variables.Add(node->name, BasicScope::Variable(node, type));
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
					if(BasicLanguage_EnsureNothingExists(node, argument, node->name))
					{
						try
						{
							BP internalArgument=BuildBasicGenericScope(node, argument);
							BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->type, internalArgument, false);
							type=BuildBasicGenericType(type, node, internalArgument);
							argument.scope->types.Add(node->name, type);
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptBaseDeclaration)
				{
					if(BasicLanguage_EnsureNothingExists(node, argument, node->name))
					{
						BasicScope* conceptScope=argument.env->CreateScope(argument.scope);
						conceptScope->types.Add(node->conceptType, argument.typeManager->GetGenericArgumentType(node->conceptType));
						BP internalArgument(argument, conceptScope);

						Ptr<BasicScope::Concept> conceptObject=new BasicScope::Concept;
						conceptObject->conceptScope=conceptScope;
						conceptObject->conceptType=argument.typeManager->GetGenericArgumentType(node->conceptType);
						conceptObject->conceptDeclaration=node;
						argument.scope->concepts.Add(node->name, conceptObject);

						for(vint i=0;i<node->functions.Count();i++)
						{
							BasicConceptBaseDeclaration::FunctionConcept* functionConcept=node->functions[i].Obj();
							if(conceptObject->functions.Keys().Contains(functionConcept->name))
							{
								argument.errors.Add(BasicLanguageCodeException::GetConceptFunctionAlreadyExists(node, functionConcept->name));
							}
							else
							{
								try
								{
									BasicTypeRecord* functionType=BasicLanguage_GetTypeRecord(functionConcept->signatureType, internalArgument, false);
									conceptObject->functions.Add(functionConcept->name, functionType);
								}
								catch(Ptr<BasicLanguageCodeException> e)
								{
									argument.errors.Add(e);
								}
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptInstanceDeclaration)
				{
					Ptr<BasicScope::Concept> conceptObject=argument.scope->concepts.Find(node->name);
					if(!conceptObject)
					{
						argument.errors.Add(BasicLanguageCodeException::GetConceptNotExists(node));
						return;
					}

					BP internalArgument=BuildBasicGenericScope(node, argument);
					BasicTypeRecord* instanceType=BasicLanguage_GetTypeRecord(node->instanceType, argument, true);
					switch(instanceType->GetType())
					{
					case BasicTypeRecord::Primitive:
						if(instanceType->PrimitiveType()==void_type)
						{
							argument.errors.Add(BasicLanguageCodeException::GetInstanceTypeNotCorrect(node));
						}
						break;
					case BasicTypeRecord::Generic:
						if(instanceType->ParameterCount()!=node->genericDeclaration.arguments.Count())
						{
							argument.errors.Add(BasicLanguageCodeException::GetInstanceTypeNotCorrect(node));
						}
						break;
					default:
						argument.errors.Add(BasicLanguageCodeException::GetInstanceTypeNotCorrect(node));
					}
					
					Ptr<BasicScope::Instance> instanceObject=new BasicScope::Instance;
					instanceObject->targetConcept=conceptObject.Obj();
					instanceObject->instanceDeclaration=node;
					instanceObject->instanceType=instanceType;
					instanceObject->instanceScope=internalArgument.scope;
					argument.scope->instances.Add(instanceObject);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildGlobalScopePass1(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1)

/***********************************************************************
BasicLanguage_BuildGlobalScopePass2
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					for(vint i=0;i<node->attributes.Count();i++)
					{
						Ptr<BasicAttribute> attribute=node->attributes[i];
						if(attribute->attributeName==L"public")
						{
							BasicTypeRecord* type=argument.env->GetFunctionType(node, true);
							if(type)
							{
								if(!IsPublicType(type->ReturnType(), argument))
								{
									argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, type->ReturnType()));
								}
								for(vint i=0;i<type->ParameterCount();i++)
								{
									if(!IsPublicType(type->ParameterType(i), argument))
									{
										argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, type->ParameterType(i)));
									}
								}
							}
						}
						else if(attribute->attributeName==L"assembly_initialization")
						{
							BasicTypeRecord* type=argument.env->GetFunctionType(node, true);
							if(type)
							{
								if(type->ReturnType()!=argument.typeManager->GetPrimitiveType(void_type)
									|| type->ParameterCount()>0
									|| node->genericDeclaration.HasGeneric()
									|| node->linking.HasLink()
									)
								{
									argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnFunctionDeclaration(node, attribute->attributeName));
								}
							}
						}
						else
						{
							argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnFunctionDeclaration(node, attribute->attributeName));
						}
					}
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					if(!node->defined)
					{
						BasicStructureTypeRecord* structure=dynamic_cast<BasicStructureTypeRecord*>(argument.scope->types.Items()[node->name]);
						if(!structure->Defined())
						{
							argument.errors.Add(BasicLanguageCodeException::GetPredeclaredStructureShouldBeDefined(node));
						}

						for(vint i=0;i<node->attributes.Count();i++)
						{
							Ptr<BasicAttribute> attribute=node->attributes[i];
							argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnStructureDeclaration(node, attribute->attributeName));
						}
					}
					else
					{
						for(vint i=0;i<node->attributes.Count();i++)
						{
							Ptr<BasicAttribute> attribute=node->attributes[i];
							if(attribute->attributeName==L"public")
							{
								BasicTypeRecord* type=argument.env->GetStructureType(node);
								if(type)
								{
									for(vint i=0;i<type->MemberCount();i++)
									{
										if(!IsPublicType(type->MemberType(i), argument))
										{
											argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, type->MemberType(i)));
										}
									}
								}
							}
							else
							{
								argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnStructureDeclaration(node, attribute->attributeName));
							}
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					for(vint i=0;i<node->attributes.Count();i++)
					{
						Ptr<BasicAttribute> attribute=node->attributes[i];
						if(attribute->attributeName==L"public")
						{
							BasicScope::Variable variable=argument.scope->variables.Find(node->name);
							if(variable.globalVariable && variable.type)
							{
								if(!IsPublicType(variable.type, argument))
								{
									argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, variable.type));
								}
							}
						}
						else
						{
							argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnVariableDeclaration(node, attribute->attributeName));
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
					for(vint i=0;i<node->attributes.Count();i++)
					{
						Ptr<BasicAttribute> attribute=node->attributes[i];
						argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnTypeRenameDeclaration(node, attribute->attributeName));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptBaseDeclaration)
				{
					for(vint i=0;i<node->attributes.Count();i++)
					{
						Ptr<BasicAttribute> attribute=node->attributes[i];
						if(attribute->attributeName==L"public")
						{
							Ptr<BasicScope::Concept> concept=argument.scope->concepts.Find(node->name);
							if(concept)
							{
								for(vint i=0;i<concept->functions.Count();i++)
								{
									BasicTypeRecord* type=concept->functions.Values()[i];
									if(!IsPublicType(type, argument))
									{
										argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, type));
									}
								}
							}
						}
						else
						{
							argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnConceptDeclaration(node, attribute->attributeName));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptInstanceDeclaration)
				{
					if(!node->defined)
					{
						for(vint i=0;i<node->attributes.Count();i++)
						{
							Ptr<BasicAttribute> attribute=node->attributes[i];
							argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnInstanceDeclaration(node, attribute->attributeName));
						}
					}
					else
					{
						BasicTypeRecord* instanceType=BasicLanguage_GetTypeRecord(node->instanceType, argument, true);
						Ptr<BasicScope::Instance> instanceObject=argument.scope->FindInstance(instanceType, node->name);
						bool publicInstanceType=true;
						bool publicConcept=false;
						bool publicInstance=false;
						{
							if(!IsPublicType(instanceType, argument))
							{
								publicInstanceType=false;
							}
							Ptr<BasicScope::Concept> concept=argument.scope->concepts.Find(node->name);
							if(concept && concept->conceptDeclaration)
							{
								if(concept->conceptDeclaration->linking.HasLink())
								{
									publicConcept=true;
								}
								else
								{
									publicConcept=BasicLanguage_FindFirstAttribute(concept->conceptDeclaration->attributes.Wrap(), L"public");
								}
							}
						}

						for(vint i=0;i<node->attributes.Count();i++)
						{
							Ptr<BasicAttribute> attribute=node->attributes[i];
							if(attribute->attributeName==L"public")
							{
								publicInstance=true;
								if(!publicInstanceType)
								{
									argument.errors.Add(BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(node, instanceType));
								}
								if(!publicConcept)
								{
									argument.errors.Add(BasicLanguageCodeException::GetInstanceDeclaredOnNonPublicConceptShouldBeNonPublic(node));
								}
							}
							else
							{
								argument.errors.Add(BasicLanguageCodeException::GetAttributeCannotApplyOnInstanceDeclaration(node, attribute->attributeName));
							}
						}
						if(!publicInstance && publicConcept)
						{
							argument.errors.Add(BasicLanguageCodeException::GetInstanceDeclaredOnPublicConceptShouldBePublic(node));
						}

						List<WString> conceptFunctions;
						CopyFrom(conceptFunctions.Wrap(), instanceObject->targetConcept->functions.Keys());
						BP newArgument(argument, instanceObject->instanceScope);

						Dictionary<BasicTypeRecord*, BasicTypeRecord*> table;
						if(node->genericDeclaration.HasGeneric())
						{
							Dictionary<BasicTypeRecord*, BasicTypeRecord*> internalTable;
							for(vint i=0;i<node->genericDeclaration.arguments.Count();i++)
							{
								internalTable.Add(instanceObject->instanceType->ParameterType(i), argument.typeManager->GetGenericArgumentType(node->genericDeclaration.arguments[i]));
							}
							BasicTypeRecord* instanciatedInstanceType=argument.typeManager->Instanciate(instanceObject->instanceType, internalTable.Wrap());
							table.Add(instanceObject->targetConcept->conceptType, instanciatedInstanceType);
						}
						else
						{
							table.Add(instanceObject->targetConcept->conceptType, instanceObject->instanceType);
						}

						for(vint i=0;i<node->functions.Count();i++)
						{
							BasicConceptInstanceDeclaration::FunctionInstance* functionInstance=node->functions[i].Obj();
							if(conceptFunctions.Contains(functionInstance->name))
							{
								conceptFunctions.Remove(functionInstance->name);
								BasicTypeRecord* functionType=0;
								BasicFunctionDeclaration* functionDeclaration=0;
								Ptr<BasicScope::Instance::FunctionInstance> functionInstanceObject=new BasicScope::Instance::FunctionInstance;

								Ptr<BasicExpression> functionExpression;
								Ptr<BasicReferenceExpression> referenceExpression;
								if(functionInstance->normalFunction)
								{
									functionExpression=functionInstance->normalFunction;
									referenceExpression=functionInstance->normalFunction;
								}
								else
								{
									functionExpression=functionInstance->genericFunction;
									referenceExpression=functionInstance->genericFunction->reference;
									for(vint j=0;j<functionInstance->genericFunction->argumentTypes.Count();j++)
									{
										functionInstanceObject->genericParameters.Add(BasicLanguage_GetTypeRecord(functionInstance->genericFunction->argumentTypes[j], newArgument, false));
									}
								}
								
								functionType=BasicLanguage_GetExpressionType(functionExpression, newArgument);
								if(functionType)
								{
									BasicEnv::Reference reference=argument.env->GetReference(referenceExpression.Obj());
									if(reference.isVariable)
									{
										argument.errors.Add(BasicLanguageCodeException::GetConceptFunctionTypeNotMatches(node, functionInstance->name));
									}
									else
									{
										functionDeclaration=reference.function;
										BasicTypeRecord* conceptFunctionType=instanceObject->targetConcept->functions[functionInstance->name];
										conceptFunctionType=argument.typeManager->Instanciate(conceptFunctionType, table.Wrap());
										if(conceptFunctionType!=functionType)
										{
											argument.errors.Add(BasicLanguageCodeException::GetConceptFunctionTypeNotMatches(node, functionInstance->name));
										}
									}
								}

								functionInstanceObject->functionDeclaration=functionDeclaration;
								instanceObject->functions.Add(functionInstance->name, functionInstanceObject);
							}
							else
							{
								argument.errors.Add(BasicLanguageCodeException::GetInstanceShouldNotHaveFunction(node, functionInstance->name));
							}
						}

						for(vint i=0;i<conceptFunctions.Count();i++)
						{
							argument.errors.Add(BasicLanguageCodeException::GetInstanceShouldHaveFunction(node, conceptFunctions[i]));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildGlobalScopePass2(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2)

/***********************************************************************
BasicLanguage_BuildGlobalScope
***********************************************************************/

			void BasicLanguage_BuildGlobalScope(Ptr<BasicProgram> program, BP& argument)
			{
				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildGlobalScopePass1(program->declarations[i], argument);
				}
				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildGlobalScopePass2(program->declarations[i], argument);
				}
			}
		}
	}
}
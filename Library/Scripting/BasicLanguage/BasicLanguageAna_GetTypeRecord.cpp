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
BasicLanguage_GetTypeRecord
***********************************************************************/

			void CheckRequiredInstance(BasicTypeRecord* type, BasicType* typeExpression, const WString& conceptName, const BP& argument)
			{
				Ptr<BasicScope::Instance> instanceObject;
				if(argument.scope->RequiredInstanceExists(type, conceptName, instanceObject))
				{
					BasicGenericStructureProxyTypeRecord* structureProxyType=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
					if(structureProxyType)
					{
						BasicConceptInstanceDeclaration* instance=instanceObject->instanceDeclaration;
						BasicDeclaration* structure=structureProxyType->UninstanciatedStructureType()->Declaration();
						for(vint i=0;i<instance->genericDeclaration.constraints.Count();i++)
						{
							Ptr<BasicGeneric::Constraint> constraint=instance->genericDeclaration.constraints[i];
							vint argumentIndex=instance->genericDeclaration.arguments.IndexOf(constraint->argumentName);
							BasicTypeRecord* structureGenericArgumentType=argument.typeManager->GetGenericArgumentType(structure->genericDeclaration.arguments[argumentIndex]);
							BasicTypeRecord* parameterType=structureProxyType->GenericArgumentMap()[structureGenericArgumentType];
							CheckRequiredInstance(parameterType, typeExpression, constraint->conceptName, argument);
						}
					}
				}
				else
				{
					argument.errors.Add(BasicLanguageCodeException::GetInstanceShouldBeDeclaredOnType(typeExpression, type, conceptName));
				}
			}

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecordInternal, BasicType, BP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicPrimitiveType)
				{
					return argument.typeManager->GetPrimitiveType(node->type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicPointerType)
				{
					return argument.typeManager->GetPointerType(BasicLanguage_GetTypeRecord(node->elementType, argument, false));
				}

				ALGORITHM_FUNCTION_MATCH(BasicArrayType)
				{
					return argument.typeManager->GetArrayType(BasicLanguage_GetTypeRecord(node->elementType, argument, false), node->size);
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceType)
				{
					BasicTypeRecord* type=argument.scope->types.Find(node->name);
					if(type)
					{
						return type;
					}
					else
					{
						throw BasicLanguageCodeException::GetTypeNameNotExists(node);
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionType)
				{
					List<BasicTypeRecord*> parameterTypes;
					for(vint i=0;i<node->parameterTypes.Count();i++)
					{
						parameterTypes.Add(BasicLanguage_GetTypeRecord(node->parameterTypes[i], argument, false));
					}
					return argument.typeManager->GetFunctionType(
						BasicLanguage_GetTypeRecord(node->returnType, argument, false),
						parameterTypes.Wrap()
						);
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanciatedGenericType)
				{
					BasicTypeRecord* genericType=BasicLanguage_GetTypeRecord(node->elementType, argument, true);
					if(genericType->GetType()!=BasicTypeRecord::Generic)
					{
						throw BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(node);
					}
					else
					{
						if(node->argumentTypes.Count()!=genericType->ParameterCount())
						{
							throw BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(node);
						}
						Dictionary<BasicTypeRecord*, BasicTypeRecord*> argumentTypes;
						for(vint i=0;i<genericType->ParameterCount();i++)
						{
							argumentTypes.Add(genericType->ParameterType(i), BasicLanguage_GetTypeRecord(node->argumentTypes[i], argument, false));
						}
						
						BasicDeclaration* genericDeclaration=genericType->Declaration();
						if(genericDeclaration)
						{
							for(vint i=0;i<genericDeclaration->genericDeclaration.constraints.Count();i++)
							{
								BasicGeneric::Constraint* constraint=genericDeclaration->genericDeclaration.constraints[i].Obj();
								vint argumentIndex=genericDeclaration->genericDeclaration.arguments.IndexOf(constraint->argumentName);
								BasicTypeRecord* argumentType=argumentTypes[genericType->ParameterType(argumentIndex)];
								CheckRequiredInstance(argumentType, node->argumentTypes[argumentIndex].Obj(), constraint->conceptName, argument);
							}
						}

						return argument.typeManager->Instanciate(genericType, argumentTypes.Wrap());
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicTypeofExpressionType)
				{
					return BasicLanguage_GetExpressionType(node->expression, argument);
				}

				ALGORITHM_FUNCTION_MATCH(BasicTypeofMemberType)
				{
					BasicTypeRecord* structure=BasicLanguage_GetTypeRecord(node->type, argument, false);
					if(structure && structure->GetType()==BasicTypeRecord::Structure)
					{
						BasicTypeRecord* member=structure->MemberType(node->member);
						if(!member)
						{
							throw BasicLanguageCodeException::GetStructureMemberNotExists(node);
						}
						else
						{
							return member;
						}
					}
					else
					{
						throw BasicLanguageCodeException::GetLeftOperandShouldBeStructure(node);
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedType)
				{
					return argument.semanticExtension->GetTypeRecord(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecordInternal)

			BasicTypeRecord* BasicLanguage_GetTypeRecord(BasicType* type, const BP& argument, bool acceptRawGenericType)
			{
				BasicTypeRecord* typeRecord=BasicLanguage_GetTypeRecordInternal(type, argument);
				if(typeRecord->GetType()==BasicTypeRecord::Generic && !acceptRawGenericType)
				{
					argument.errors.Add(BasicLanguageCodeException::GetCannotUseUninstanciatedGenericType(type));
				}
				return typeRecord;
			}

			BasicTypeRecord* BasicLanguage_GetTypeRecord(Ptr<BasicType> type, const BP& argument, bool acceptRawGenericType)
			{
				return BasicLanguage_GetTypeRecord(type.Obj(), argument, acceptRawGenericType);
			}

/***********************************************************************
IsPublicType
***********************************************************************/
			
			bool IsPublicType(BasicTypeRecord* type, const BP& argument)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Primitive:
					return true;
				case BasicTypeRecord::Pointer:
					return IsPublicType(type->ElementType(), argument);
				case BasicTypeRecord::Array:
					return IsPublicType(type->ElementType(), argument);
				case BasicTypeRecord::Function:
					{
						if(!IsPublicType(type->ReturnType(), argument))return false;
						for(vint i=0;i<type->ParameterCount();i++)
						{
							if(!IsPublicType(type->ParameterType(i), argument))return false;
						}
						return true;
					}
				case BasicTypeRecord::Structure:
					{
						BasicDeclaration* declaration=type->Declaration();
						if(!declaration->linking.HasLink())
						{
							if(!BasicLanguage_FindFirstAttribute(declaration->attributes.Wrap(), L"public"))return false;
							BasicGenericStructureProxyTypeRecord* proxyType=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
							if(proxyType)
							{
								const IReadonlyList<BasicTypeRecord*>& arguments=proxyType->GenericArgumentMap().Values();
								for(vint i=0;i<arguments.Count();i++)
								{
									if(!IsPublicType(arguments[i], argument))return false;
								}
							}
						}
						return true;
					}
				case BasicTypeRecord::GenericArgument:
					return true;
				case BasicTypeRecord::Generic:
					return IsPublicType(type->ElementType(), argument);
				default:
					return false;
				}
			}
		}
	}
}
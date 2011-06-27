#include "BasicLanguageTypeManager.h"
#include "..\..\Collections\OperationEnumerable.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicTypeRecord
***********************************************************************/

			BasicTypeRecord::BasicTypeRecord()
			{
			}

			BasicTypeRecord::TypeRecordType BasicTypeRecord::GetType()
			{
				CHECK_FAIL(L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicPrimitiveTypeEnum BasicTypeRecord::PrimitiveType()
			{
				CHECK_FAIL(L"BasicTypeRecord::PrimitiveType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ElementType()
			{
				CHECK_FAIL(L"BasicTypeRecord::ElementType()#不支持此操作。");
			}

			vint BasicTypeRecord::ElementCount()
			{
				CHECK_FAIL(L"BasicTypeRecord::ElementCount()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ReturnType()
			{
				CHECK_FAIL(L"BasicTypeRecord::ReturnType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ParameterType(vint index)
			{
				CHECK_FAIL(L"BasicTypeRecord::ParameterType()#不支持此操作。");
			}

			vint BasicTypeRecord::ParameterCount()
			{
				CHECK_FAIL(L"BasicTypeRecord::ParameterCount()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(vint index)
			{
				CHECK_FAIL(L"BasicTypeRecord::MemberType(vint)#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(const WString& name)
			{
				CHECK_FAIL(L"BasicTypeRecord::MemberType(const WString&)#不支持此操作。");
			}

			const WString& BasicTypeRecord::MemberName(vint index)
			{
				CHECK_FAIL(L"BasicTypeRecord::MemberName(vint)#不支持此操作。");
			}

			vint BasicTypeRecord::MemberNameIndex(const WString& name)
			{
				CHECK_FAIL(L"BasicTypeRecord::MemberName(vint)#不支持此操作。");
			}

			vint BasicTypeRecord::MemberCount()
			{
				CHECK_FAIL(L"BasicTypeRecord::MemberCount()#不支持此操作。");
			}

			bool BasicTypeRecord::Defined()
			{
				CHECK_FAIL(L"BasicTypeRecord::Defined()#不支持此操作。");
			}

			BasicDeclaration* BasicTypeRecord::Declaration()
			{
				CHECK_FAIL(L"BasicTypeRecord::Declaration()#不支持此操作。");
			}
			
			WString BasicTypeRecord::ArgumentName()
			{
				CHECK_FAIL(L"BasicTypeRecord::Defined()#不支持此操作。");
			}

/***********************************************************************
BasicPrimitiveTypeRecord
***********************************************************************/

			BasicPrimitiveTypeRecord::BasicPrimitiveTypeRecord(BasicPrimitiveTypeEnum primitiveType)
				:CommonFlagTypeRecord<BasicTypeRecord, BasicPrimitiveTypeEnum>(primitiveType)
			{
			}

			BasicTypeRecord::TypeRecordType BasicPrimitiveTypeRecord::GetType()
			{
				return BasicTypeRecord::Primitive;
			}

			BasicPrimitiveTypeEnum BasicPrimitiveTypeRecord::PrimitiveType()
			{
				return InternalGetFlag();
			}

/***********************************************************************
BasicPointerTypeRecord
***********************************************************************/

			BasicPointerTypeRecord::BasicPointerTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType)
				:CommonDecoratorTypeRecord<BasicTypeRecord, 0>(elementType)
			{
			}

			BasicTypeRecord::TypeRecordType BasicPointerTypeRecord::GetType()
			{
				return BasicTypeRecord::Pointer;
			}

			BasicTypeRecord* BasicPointerTypeRecord::ElementType()
			{
				return InternalGetElementType();
			}

/***********************************************************************
BasicArrayTypeRecord
***********************************************************************/

			BasicArrayTypeRecord::BasicArrayTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType, vint elementCount)
				:CommonParameterizedTypeRecord<BasicTypeRecord, vint>(elementType, elementCount)
			{
			}

			BasicTypeRecord::TypeRecordType BasicArrayTypeRecord::GetType()
			{
				return BasicTypeRecord::Array;
			}

			BasicTypeRecord* BasicArrayTypeRecord::ElementType()
			{
				return InternalGetElementType();
			}

			vint BasicArrayTypeRecord::ElementCount()
			{
				return InternalGetParameter();
			}

/***********************************************************************
BasicFunctionTypeRecord
***********************************************************************/

			BasicFunctionTypeRecord::BasicFunctionTypeRecord(P& argument)
				:returnType(argument.returnType)
			{
				CopyFrom(parameterTypes.Wrap(), argument.parameterTypes);
			}

			BasicTypeRecord::TypeRecordType BasicFunctionTypeRecord::GetType()
			{
				return BasicTypeRecord::Function;
			}

			BasicTypeRecord* BasicFunctionTypeRecord::ReturnType()
			{
				return returnType;
			}

			BasicTypeRecord* BasicFunctionTypeRecord::ParameterType(vint index)
			{
				return parameterTypes[index];
			}

			vint BasicFunctionTypeRecord::ParameterCount()
			{
				return parameterTypes.Count();
			}

/***********************************************************************
BasicStructureTypeRecord
***********************************************************************/

			vint BasicStructureTypeRecord::P::Compare(const P& p)const
			{
				if(typeTable==p.typeTable)
				{
					return 0;
				}
				else
				{
					return CompareEnumerable(typeTable->Wrap(), p.typeTable->Wrap());
				}
			}

			bool BasicStructureTypeRecord::P::operator==(const P& p)const
			{
				return Compare(p)==0;
			}

			bool BasicStructureTypeRecord::P::operator!=(const P& p)const
			{
				return Compare(p)!=0;
			}

			bool BasicStructureTypeRecord::P::operator<(const P& p)const
			{
				return Compare(p)<0;
			}

			bool BasicStructureTypeRecord::P::operator<=(const P& p)const
			{
				return Compare(p)<=0;
			}

			bool BasicStructureTypeRecord::P::operator>(const P& p)const
			{
				return Compare(p)>0;
			}

			bool BasicStructureTypeRecord::P::operator>=(const P& p)const
			{
				return Compare(p)>=0;
			}

			BasicStructureTypeRecord::BasicStructureTypeRecord(BasicDeclaration* _declaration)
				:declaration(_declaration)
				,referencedGenericType(0)
			{
			}

			BasicTypeRecord::TypeRecordType BasicStructureTypeRecord::GetType()
			{
				return BasicTypeRecord::Structure;
			}

			BasicTypeRecord* BasicStructureTypeRecord::ElementType()
			{
				return referencedGenericType;
			}

			BasicTypeRecord* BasicStructureTypeRecord::MemberType(vint index)
			{
				return types[index];
			}

			BasicTypeRecord* BasicStructureTypeRecord::MemberType(const WString& name)
			{
				vint index=names.IndexOf(name);
				return index==-1?0:types[index];
			}

			const WString& BasicStructureTypeRecord::MemberName(vint index)
			{
				return names[index];
			}

			vint BasicStructureTypeRecord::MemberNameIndex(const WString& name)
			{
				return names.IndexOf(name);
			}

			vint BasicStructureTypeRecord::MemberCount()
			{
				return types.Count();
			}

			bool BasicStructureTypeRecord::Defined()
			{
				return defined;
			}

			BasicDeclaration* BasicStructureTypeRecord::Declaration()
			{
				return declaration;
			}

/***********************************************************************
BasicGenericArgumentTypeRecord
***********************************************************************/

			BasicGenericArgumentTypeRecord::BasicGenericArgumentTypeRecord(const WString& name)
				:CommonFlagTypeRecord<BasicTypeRecord, WString>(name)
			{
			}

			BasicTypeRecord::TypeRecordType BasicGenericArgumentTypeRecord::GetType()
			{
				return BasicTypeRecord::GenericArgument;
			}

			WString BasicGenericArgumentTypeRecord::ArgumentName()
			{
				return InternalGetFlag();
			}

/***********************************************************************
BasicTypeManager辅助函数
***********************************************************************/

			CommonFlagTypeRecord<BasicTypeRecord, BasicPrimitiveTypeEnum>* BasicPrimitiveTypeRecordAllocator(BasicPrimitiveTypeEnum type)
			{
				return new BasicPrimitiveTypeRecord(type);
			}

			CommonDecoratorTypeRecord<BasicTypeRecord, 0>* BasicPointerTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType)
			{
				return new BasicPointerTypeRecord(elementType);
			}

			CommonParameterizedTypeRecord<BasicTypeRecord, vint>* BasicArrayTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType, vint elementCount)
			{
				return new BasicArrayTypeRecord(elementType, elementCount);
			}

			CommonTypeRecord<BasicTypeRecord>* BasicFunctionTypeRecordAllocator(BasicFunctionTypeRecord::P& argument)
			{
				return new BasicFunctionTypeRecord(argument);
			}

			bool BasicFunctionTypeRecordComparer(BasicFunctionTypeRecord::P& argument, CommonTypeRecord<BasicTypeRecord>* type)
			{
				if(type->ReturnType()!=argument.returnType)return false;
				if(type->ParameterCount()!=argument.parameterTypes.Count())return false;
				for(vint j=0;j<type->ParameterCount();j++)
				{
					if(type->ParameterType(j)!=argument.parameterTypes[j])
					{
						return false;
					}
				}
				return true;
			}

			CommonFlagTypeRecord<BasicTypeRecord, WString>* BasicGenericArgumentRecordAllocator(WString name)
			{
				return new BasicGenericArgumentTypeRecord(name);
			}

/***********************************************************************
BasicGenericStructureProxyTypeRecord
***********************************************************************/

			BasicGenericStructureProxyTypeRecord::BasicGenericStructureProxyTypeRecord(BasicTypeRecord* _structureType, BasicTypeManager* _manager, Ptr<_GenericInstanciatingTypeTable> _typeTable)
				:structureType(_structureType)
				,manager(_manager)
				,typeTable(_typeTable)
			{
			}

			BasicTypeRecord::TypeRecordType BasicGenericStructureProxyTypeRecord::GetType()
			{
				return structureType->GetType();
			}

			BasicTypeRecord* BasicGenericStructureProxyTypeRecord::MemberType(vint index)
			{
				BasicTypeRecord* type=structureType->MemberType(index);
				if(type)
				{
					return manager->Instanciate(type, typeTable);
				}
				else
				{
					return 0;
				}
			}

			BasicTypeRecord* BasicGenericStructureProxyTypeRecord::MemberType(const WString& name)
			{
				BasicTypeRecord* type=structureType->MemberType(name);
				if(type)
				{
					return manager->Instanciate(type, typeTable);
				}
				else
				{
					return 0;
				}
			}

			const WString& BasicGenericStructureProxyTypeRecord::MemberName(vint index)
			{
				return structureType->MemberName(index);
			}

			vint BasicGenericStructureProxyTypeRecord::MemberNameIndex(const WString& name)
			{
				return structureType->MemberNameIndex(name);
			}

			vint BasicGenericStructureProxyTypeRecord::MemberCount()
			{
				return structureType->MemberCount();
			}

			bool BasicGenericStructureProxyTypeRecord::Defined()
			{
				return structureType->Defined();
			}

			BasicDeclaration* BasicGenericStructureProxyTypeRecord::Declaration()
			{
				return structureType->Declaration();
			}

			BasicTypeRecord* BasicGenericStructureProxyTypeRecord::UninstantiatedStructureType()
			{
				return structureType;
			}

			BasicGenericStructureProxyTypeRecord::_IGenericInstanciatingTypeTable& BasicGenericStructureProxyTypeRecord::GenericArgumentMap()
			{
				return typeTable->Wrap();
			}

/***********************************************************************
BasicGenericTypeRecord
***********************************************************************/

			BasicGenericTypeRecord::BasicGenericTypeRecord(BasicDeclaration* _declaration)
				:elementType(0)
				,declaration(_declaration)
			{
			}

			BasicTypeRecord::TypeRecordType BasicGenericTypeRecord::GetType()
			{
				return BasicTypeRecord::Generic;
			}

			BasicTypeRecord* BasicGenericTypeRecord::ElementType()
			{
				return elementType;
			}

			BasicTypeRecord* BasicGenericTypeRecord::ParameterType(vint index)
			{
				return parameterTypes[index];
			}

			vint BasicGenericTypeRecord::ParameterCount()
			{
				return parameterTypes.Count();
			}

			BasicDeclaration* BasicGenericTypeRecord::Declaration()
			{
				return declaration;
			}

/***********************************************************************
BasicTypeManager
***********************************************************************/

			BasicTypeRecord* BasicTypeManager::Instanciate(BasicTypeRecord* genericType, Ptr<_GenericInstanciatingTypeTable> parameters)
			{
				switch(genericType->GetType())
				{
				case BasicTypeRecord::Primitive:
					{
						return genericType;
					}
					break;
				case BasicTypeRecord::Pointer:
					{
						return GetPointerType(Instanciate(genericType->ElementType(), parameters));
					}
					break;
				case BasicTypeRecord::Array:
					{
						return GetArrayType(Instanciate(genericType->ElementType(), parameters), genericType->ElementCount());
					}
					break;
				case BasicTypeRecord::Function:
					{
						BasicTypeRecord* returnType=Instanciate(genericType->ReturnType(), parameters);
						List<BasicTypeRecord*> parameterTypes;
						for(vint i=0;i<genericType->ParameterCount();i++)
						{
							parameterTypes.Add(Instanciate(genericType->ParameterType(i), parameters));
						}
						return GetFunctionType(returnType, parameterTypes.Wrap());
					}
					break;
				case BasicTypeRecord::GenericArgument:
					{
						vint index=parameters->Keys().IndexOf(genericType);
						return index==-1?genericType:parameters->Values()[index];
					}
					break;
				case BasicTypeRecord::Structure:
					{
						if(BasicStructureTypeRecord* structureType=dynamic_cast<BasicStructureTypeRecord*>(genericType))
						{
							BasicStructureTypeRecord::P p;
							p.typeTable=parameters;
							vint index=structureType->proxyTable.Keys().IndexOf(p);
							if(index==-1)
							{
								BasicGenericStructureProxyTypeRecord* proxyType=new BasicGenericStructureProxyTypeRecord(genericType, this, parameters);
								CommonTypeManager<BasicTypeRecord>::RegisterTypeRecord(proxyType);
								structureType->proxyTable.Add(p, proxyType);
								return proxyType;
							}
							else
							{
								return structureType->proxyTable.Values()[index];
							}
						}
						else if(BasicGenericStructureProxyTypeRecord* proxyType=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(genericType))
						{
							Ptr<_GenericInstanciatingTypeTable> newParameters=new _GenericInstanciatingTypeTable;
							for(vint i=0;i<parameters->Count();i++)
							{
								vint index=proxyType->typeTable->Values().IndexOf(parameters->Keys()[i]);
								if(index==-1)
								{
									newParameters->Add(parameters->Keys()[i], parameters->Values()[i]);
								}
								else
								{
									newParameters->Add(proxyType->typeTable->Keys()[i], parameters->Values()[i]);
								}
							}
							return Instanciate(proxyType->structureType, newParameters);
						}
						else
						{
							return genericType;
						}
					}
					break;
				case BasicTypeRecord::Generic:
					{
						return Instanciate(genericType->ElementType(), parameters);
					}
					break;
				default:
					return genericType;
				}
			}
			
			BasicTypeManager::BasicTypeManager()
			{
			}

			BasicTypeManager::~BasicTypeManager()
			{
			}

			BasicTypeRecord* BasicTypeManager::GetPrimitiveType(BasicPrimitiveTypeEnum primitiveType)
			{
				return CommonTypeManager<BasicTypeRecord>::GetFlagTypeRecord(
					primitiveType,
					primitiveTypes.Wrap(),
					BasicPrimitiveTypeRecordAllocator
					);
			}

			BasicTypeRecord* BasicTypeManager::GetPointerType(BasicTypeRecord* elementType)
			{
				return CommonTypeManager<BasicTypeRecord>::GetDecoratorTypeRecord(
					dynamic_cast<CommonTypeRecord<BasicTypeRecord>*>(elementType),
					BasicPointerTypeRecordAllocator
					);
			}

			BasicTypeRecord* BasicTypeManager::GetArrayType(BasicTypeRecord* elementType, vint elementCount)
			{
				return CommonTypeManager<BasicTypeRecord>::GetCommonParameterizedTypeRecord(
					dynamic_cast<CommonTypeRecord<BasicTypeRecord>*>(elementType),
					elementCount,
					elementType->arrayTypes.Wrap(),
					BasicArrayTypeRecordAllocator
					);
			}

			BasicTypeRecord* BasicTypeManager::GetFunctionType(BasicTypeRecord* returnType, const collections::IReadonlyList<BasicTypeRecord*>& parameterTypes)
			{
				BasicFunctionTypeRecord::P parameter={returnType, parameterTypes};
				return CommonTypeManager<BasicTypeRecord>::GetComplexTypeRecord(
					parameter,
					functionTypes.Wrap(),
					BasicFunctionTypeRecordAllocator,
					BasicFunctionTypeRecordComparer
					);
			}

			BasicTypeRecord* BasicTypeManager::CreateStructureType(BasicStructureDeclaration* declaration)
			{
				BasicStructureTypeRecord* type=new BasicStructureTypeRecord(declaration);
				type->defined=false;
				CommonTypeManager<BasicTypeRecord>::RegisterTypeRecord(type);
				return type;
			}

			void BasicTypeManager::UpdateStructureType(BasicStructureDeclaration* declaration, BasicTypeRecord* structureType, const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types)
			{
				BasicStructureTypeRecord* type=dynamic_cast<BasicStructureTypeRecord*>(structureType);
				if(type && !type->defined)
				{
					if(declaration)type->declaration=declaration;
					CopyFrom(type->names.Wrap(), names);
					CopyFrom(type->types.Wrap(), types);
					type->defined=true;
				}
			}

			BasicTypeRecord* BasicTypeManager::GetGenericArgumentType(const WString& name)
			{
				return CommonTypeManager<BasicTypeRecord>::GetFlagTypeRecord(
					name,
					genericArgumentTypes.Wrap(),
					BasicGenericArgumentRecordAllocator
					);
			}

			BasicTypeRecord* BasicTypeManager::CreateGenericType(BasicStructureDeclaration* declaration)
			{
				BasicGenericTypeRecord* type=new BasicGenericTypeRecord(declaration);
				CommonTypeManager<BasicTypeRecord>::RegisterTypeRecord(type);
				return type;
			}

			void BasicTypeManager::UpdateGenericType(BasicTypeRecord* genericType, BasicTypeRecord* elementType, const collections::IReadonlyList<BasicTypeRecord*>& parameters)
			{
				BasicGenericTypeRecord* type=dynamic_cast<BasicGenericTypeRecord*>(genericType);
				if(type && !type->elementType)
				{
					type->elementType=elementType;
					CopyFrom(type->parameterTypes.Wrap(), parameters);
					BasicStructureTypeRecord* structureType=dynamic_cast<BasicStructureTypeRecord*>(elementType);
					if(structureType)
					{
						structureType->referencedGenericType=genericType;
					}
				}
			}

			BasicTypeRecord* BasicTypeManager::Instanciate(BasicTypeRecord* genericType, const _IGenericInstanciatingTypeTable& parameters)
			{
				Ptr<_GenericInstanciatingTypeTable> typeTable=new _GenericInstanciatingTypeTable;
				CopyFrom(typeTable->Wrap(), parameters);
				return Instanciate(genericType, typeTable);
			}
		}
	}
}
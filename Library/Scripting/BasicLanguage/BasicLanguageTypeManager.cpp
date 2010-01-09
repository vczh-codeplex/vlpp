#include "BasicLanguageTypeManager.h"
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
				:manager(0)
				,pointerType(0)
			{
			}

			BasicTypeManager* BasicTypeRecord::GetTypeManager()
			{
				return manager;
			}

			BasicTypeRecord::TypeRecordType BasicTypeRecord::GetType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicPrimitiveTypeEnum BasicTypeRecord::PrimitiveType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ElementType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			int BasicTypeRecord::ElementCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ReturnType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ParameterType(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			int BasicTypeRecord::ParameterCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(const WString& name)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			const WString& BasicTypeRecord::MemberName(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			int BasicTypeRecord::MemberCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

/***********************************************************************
BasicPrimitiveTypeRecord
***********************************************************************/

			BasicPrimitiveTypeRecord::BasicPrimitiveTypeRecord(BasicTypeManager* _manager, BasicPrimitiveTypeEnum _primitiveType)
				:primitiveType(_primitiveType)
			{
				manager=_manager;
			}

			BasicTypeRecord::TypeRecordType BasicPrimitiveTypeRecord::GetType()
			{
				return BasicTypeRecord::Primitive;
			}

			BasicPrimitiveTypeEnum BasicPrimitiveTypeRecord::PrimitiveType()
			{
				return primitiveType;
			}

/***********************************************************************
BasicPointerTypeRecord
***********************************************************************/

			BasicPointerTypeRecord::BasicPointerTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _elementType)
				:elementType(_elementType)
			{
				manager=_manager;
			}

			BasicTypeRecord::TypeRecordType BasicPointerTypeRecord::GetType()
			{
				return BasicTypeRecord::Pointer;
			}

			BasicTypeRecord* BasicPointerTypeRecord::ElementType()
			{
				return elementType;
			}

/***********************************************************************
BasicArrayTypeRecord
***********************************************************************/

			BasicArrayTypeRecord::BasicArrayTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _elementType, int _elementCount)
				:elementType(_elementType)
				,elementCount(_elementCount)
			{
				manager=_manager;
			}

			BasicTypeRecord::TypeRecordType BasicArrayTypeRecord::GetType()
			{
				return BasicTypeRecord::Array;
			}

			BasicTypeRecord* BasicArrayTypeRecord::ElementType()
			{
				return elementType;
			}

			int BasicArrayTypeRecord::ElementCount()
			{
				return elementCount;
			}

/***********************************************************************
BasicFunctionTypeRecord
***********************************************************************/

			BasicFunctionTypeRecord::BasicFunctionTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _returnType, const collections::IReadonlyList<BasicTypeRecord*>& _parameterTypes)
				:returnType(_returnType)
			{
				manager=_manager;
				CopyFrom(parameterTypes.Wrap(), _parameterTypes);
			}

			BasicTypeRecord::TypeRecordType BasicFunctionTypeRecord::GetType()
			{
				return BasicTypeRecord::Function;
			}

			BasicTypeRecord* BasicFunctionTypeRecord::ReturnType()
			{
				return returnType;
			}

			BasicTypeRecord* BasicFunctionTypeRecord::ParameterType(int index)
			{
				return parameterTypes[index];
			}

			int BasicFunctionTypeRecord::ParameterCount()
			{
				return parameterTypes.Count();
			}

/***********************************************************************
BasicStructureTypeRecord
***********************************************************************/

			BasicStructureTypeRecord::BasicStructureTypeRecord(BasicTypeManager* _manager, const collections::IReadonlyList<WString>& _names, const collections::IReadonlyList<BasicTypeRecord*>& _types)
			{
				CHECK_ERROR(_names.Count()==_types.Count(), L"BasicStructureTypeRecord::BasicStructureTypeRecord()#名字类型数量必须一致。");
				manager=_manager;
				CopyFrom(names.Wrap(), _names);
				CopyFrom(types.Wrap(), _types);
			}

			BasicTypeRecord::TypeRecordType BasicStructureTypeRecord::GetType()
			{
				return BasicTypeRecord::Structure;
			}

			BasicTypeRecord* BasicStructureTypeRecord::MemberType(int index)
			{
				return types[index];
			}

			BasicTypeRecord* BasicStructureTypeRecord::MemberType(const WString& name)
			{
				return types[names.IndexOf(name)];
			}

			const WString& BasicStructureTypeRecord::MemberName(int index)
			{
				return names[index];
			}

			int BasicStructureTypeRecord::MemberCount()
			{
				return types.Count();
			}

/***********************************************************************
BasicTypeManager
***********************************************************************/
			
			BasicTypeManager::BasicTypeManager()
			{
			}

			BasicTypeManager::~BasicTypeManager()
			{
			}

			BasicTypeRecord* BasicTypeManager::GetPrimitiveType(BasicPrimitiveTypeEnum primitiveType)
			{
				int index=primitiveTypes.Keys().IndexOf(primitiveType);
				if(index==-1)
				{
					BasicPrimitiveTypeRecord* type=new BasicPrimitiveTypeRecord(this, primitiveType);
					allocatedTypes.Add(type);
					primitiveTypes.Add(primitiveType, type);
					return type;
				}
				else
				{
					return primitiveTypes.Values()[index];
				}
			}

			BasicTypeRecord* BasicTypeManager::GetPointerType(BasicTypeRecord* elementType)
			{
				if(!elementType->pointerType)
				{
					elementType->pointerType=new BasicPointerTypeRecord(this, elementType);
					allocatedTypes.Add(elementType->pointerType);
				}
				return elementType->pointerType;
			}

			BasicTypeRecord* BasicTypeManager::GetArrayType(BasicTypeRecord* elementType, int elementCount)
			{
				int index=elementType->arrayTypes.Keys().IndexOf(elementCount);
				if(index==-1)
				{
					BasicArrayTypeRecord* type=new BasicArrayTypeRecord(this, elementType, elementCount);
					allocatedTypes.Add(type);
					elementType->arrayTypes.Add(elementCount, type);
					return type;
				}
				else
				{
					return elementType->arrayTypes.Values()[elementCount];
				}
			}

			BasicTypeRecord* BasicTypeManager::GetFunctionType(BasicTypeRecord* returnType, const collections::IReadonlyList<BasicTypeRecord*>& parameterTypes)
			{
				for(int i=0;i<functionTypes.Count();i++)
				{
					BasicTypeRecord* type=functionTypes[i];
					if(type->ReturnType()==returnType && type->ParameterCount()==parameterTypes.Count())
					{
						bool found=true;
						for(int j=0;j<type->ParameterCount();j++)
						{
							if(type->ParameterType(j)!=parameterTypes[j])
							{
								found=false;
								break;
							}
						}
						if(found)
						{
							return type;
						}
					}
				}
				BasicFunctionTypeRecord* type=new BasicFunctionTypeRecord(this, returnType, parameterTypes);
				allocatedTypes.Add(type);
				functionTypes.Add(type);
				return type;
			}

			BasicTypeRecord* BasicTypeManager::CreateStructureType(const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types)
			{
				BasicStructureTypeRecord* type=new BasicStructureTypeRecord(this, names, types);
				allocatedTypes.Add(type);
				return type;
			}
		}
	}
}
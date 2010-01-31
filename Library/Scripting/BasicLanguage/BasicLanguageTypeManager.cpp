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
			{
			}

			BasicTypeRecord::TypeRecordType BasicTypeRecord::GetType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::GetType()#不支持此操作。");
			}

			BasicPrimitiveTypeEnum BasicTypeRecord::PrimitiveType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::PrimitiveType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ElementType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::ElementType()#不支持此操作。");
			}

			int BasicTypeRecord::ElementCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::ElementCount()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ReturnType()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::ReturnType()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::ParameterType(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::ParameterType()#不支持此操作。");
			}

			int BasicTypeRecord::ParameterCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::ParameterCount()#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::MemberType(int)#不支持此操作。");
			}

			BasicTypeRecord* BasicTypeRecord::MemberType(const WString& name)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::MemberType(const WString&)#不支持此操作。");
			}

			const WString& BasicTypeRecord::MemberName(int index)
			{
				CHECK_ERROR(false, L"BasicTypeRecord::MemberName(int)#不支持此操作。");
			}

			int BasicTypeRecord::MemberCount()
			{
				CHECK_ERROR(false, L"BasicTypeRecord::MemberCount()#不支持此操作。");
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

			BasicArrayTypeRecord::BasicArrayTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType, int elementCount)
				:CommonParameterizedTypeRecord<BasicTypeRecord, int>(elementType, elementCount)
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

			int BasicArrayTypeRecord::ElementCount()
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

			BasicStructureTypeRecord::BasicStructureTypeRecord()
			{
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

			CommonParameterizedTypeRecord<BasicTypeRecord, int>* BasicArrayTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType, int elementCount)
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
				for(int j=0;j<type->ParameterCount();j++)
				{
					if(type->ParameterType(j)!=argument.parameterTypes[j])
					{
						return false;
					}
				}
				return true;
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

			BasicTypeRecord* BasicTypeManager::GetArrayType(BasicTypeRecord* elementType, int elementCount)
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

			BasicTypeRecord* BasicTypeManager::CreateStructureType()
			{
				BasicStructureTypeRecord* type=new BasicStructureTypeRecord;
				CommonTypeManager<BasicTypeRecord>::RegisterTypeRecord(type);
				return type;
			}

			void BasicTypeManager::UpdateStructureType(BasicTypeRecord* structureType, const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types)
			{
				BasicStructureTypeRecord* type=dynamic_cast<BasicStructureTypeRecord*>(structureType);
				if(type)
				{
					CopyFrom(type->names.Wrap(), names);
					CopyFrom(type->types.Wrap(), types);
				}
			}
		}
	}
}
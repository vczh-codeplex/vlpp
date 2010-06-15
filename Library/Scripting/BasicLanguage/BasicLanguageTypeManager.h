/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
	BasicTypeRecord								：类型对象
	BasicTypeManager							：类型管理器
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGETYPEMANAGER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGETYPEMANAGER

#include "..\..\String.h"
#include "..\..\Pointer.h"
#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "..\Common\CommonTypeManager.h"
#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicTypeManager;

/***********************************************************************
BasicTypeManager
***********************************************************************/

			class BasicTypeRecord : public Object, private NotCopyable
			{
				friend class BasicTypeManager;
				typedef collections::Dictionary<int, CommonTypeRecord<BasicTypeRecord>*>		_ArrayTypeTable;
			protected:
				_ArrayTypeTable							arrayTypes;
			public:
				enum TypeRecordType
				{
					Primitive,
					Pointer,
					Array,
					Function,
					Structure,
					GenericArgument,
				};

				BasicTypeRecord();

				virtual TypeRecordType					GetType()=0;
				virtual BasicPrimitiveTypeEnum			PrimitiveType();						//primitive
				virtual BasicTypeRecord*				ElementType();							//pointer array
				virtual int								ElementCount();							//array
				virtual BasicTypeRecord*				ReturnType();							//function
				virtual BasicTypeRecord*				ParameterType(int index);				//function
				virtual int								ParameterCount();						//function
				virtual BasicTypeRecord*				MemberType(int index);					//structure
				virtual BasicTypeRecord*				MemberType(const WString& name);		//structure
				virtual const WString&					MemberName(int index);					//structure
				virtual int								MemberNameIndex(const WString& name);	//structure
				virtual int								MemberCount();							//structure
				virtual bool							Defined();								//structure
				virtual WString							ArgumentName();							//generic-argument
			};

/***********************************************************************
BasicTypeManager
***********************************************************************/

			class BasicPrimitiveTypeRecord : public CommonFlagTypeRecord<BasicTypeRecord, BasicPrimitiveTypeEnum>
			{
				friend CommonFlagTypeRecord<BasicTypeRecord, BasicPrimitiveTypeEnum>* BasicPrimitiveTypeRecordAllocator(BasicPrimitiveTypeEnum type);
			protected:

				BasicPrimitiveTypeRecord(BasicPrimitiveTypeEnum primitiveType);
			public:

				TypeRecordType							GetType();
				BasicPrimitiveTypeEnum					PrimitiveType();
			};

			class BasicPointerTypeRecord : public CommonDecoratorTypeRecord<BasicTypeRecord, 0>
			{
				friend CommonDecoratorTypeRecord<BasicTypeRecord, 0>* BasicPointerTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType);
			protected:

				BasicPointerTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
			};

			class BasicArrayTypeRecord : public CommonParameterizedTypeRecord<BasicTypeRecord, int>
			{
				friend CommonParameterizedTypeRecord<BasicTypeRecord, int>* BasicArrayTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType, int elementCount);
			protected:

				BasicArrayTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType, int elementCount);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
				int										ElementCount();
			};

			class BasicFunctionTypeRecord : public CommonTypeRecord<BasicTypeRecord>
			{
				friend class BasicTypeManager;

				typedef collections::IReadonlyList<BasicTypeRecord*>	ITypeList;

				struct P
				{
					BasicTypeRecord*					returnType;
					const ITypeList&					parameterTypes;
				};

				friend CommonTypeRecord<BasicTypeRecord>* BasicFunctionTypeRecordAllocator(P& argument);
				friend bool BasicFunctionTypeRecordComparer(P& argument, CommonTypeRecord<BasicTypeRecord>* type);
			protected:
				BasicTypeRecord*						returnType;
				collections::List<BasicTypeRecord*>		parameterTypes;

				BasicFunctionTypeRecord(P& argument);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ReturnType();
				BasicTypeRecord*						ParameterType(int index);
				int										ParameterCount();
			};

			class BasicStructureTypeRecord : public CommonTypeRecord<BasicTypeRecord>
			{
				friend class BasicTypeManager;
			protected:
				collections::List<WString>				names;
				collections::List<BasicTypeRecord*>		types;
				bool									defined;

				BasicStructureTypeRecord();
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						MemberType(int index);
				BasicTypeRecord*						MemberType(const WString& name);
				const WString&							MemberName(int index);
				int										MemberNameIndex(const WString& name);
				int										MemberCount();
				bool									Defined();
			};

			class BasicGenericArgumentTypeRecord : public CommonFlagTypeRecord<BasicTypeRecord, WString>
			{
				friend CommonFlagTypeRecord<BasicTypeRecord, WString>* BasicGenericArgumentRecordAllocator(WString name);
			protected:

				BasicGenericArgumentTypeRecord(const WString& name);
			public:

				TypeRecordType							GetType();
				WString									ArgumentName();
			};

/***********************************************************************
BasicTypeManager
***********************************************************************/

			class BasicTypeManager : public CommonTypeManager<BasicTypeRecord>
			{
				typedef collections::List<CommonTypeRecord<BasicTypeRecord>*>								_FunctionTypeTable;
				typedef collections::Dictionary<BasicPrimitiveTypeEnum, CommonTypeRecord<BasicTypeRecord>*>	_PrimitiveTypeTable;
				typedef collections::Dictionary<WString, CommonTypeRecord<BasicTypeRecord>*>				_GenericArgumentTypeTable;
			protected:
				_FunctionTypeTable						functionTypes;
				_PrimitiveTypeTable						primitiveTypes;
				_GenericArgumentTypeTable				genericArgumentTypes;
			public:
				BasicTypeManager();
				~BasicTypeManager();

				BasicTypeRecord*						GetPrimitiveType(BasicPrimitiveTypeEnum primitiveType);
				BasicTypeRecord*						GetPointerType(BasicTypeRecord* elementType);
				BasicTypeRecord*						GetArrayType(BasicTypeRecord* elementType, int elementCount);
				BasicTypeRecord*						GetFunctionType(BasicTypeRecord* returnType, const collections::IReadonlyList<BasicTypeRecord*>& parameterTypes);
				BasicTypeRecord*						CreateStructureType();
				void									UpdateStructureType(BasicTypeRecord* structureType, const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types);

				BasicTypeRecord*						GetGenericArgumentType(const WString& name);
			};
		}
	}
}

#endif
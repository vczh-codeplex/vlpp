/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGETYPEMANAGER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGETYPEMANAGER

#include "..\..\String.h"
#include "..\..\Pointer.h"
#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicTypeManager;

			class BasicTypeRecord : public Object, private NotCopyable
			{
				friend class BasicTypeManager;
				typedef collections::Dictionary<int, BasicTypeRecord*>		_ArrayTypeTable;
			protected:
				BasicTypeManager*						manager;
				BasicTypeRecord*						pointerType;
				_ArrayTypeTable							arrayTypes;
			public:
				enum TypeRecordType
				{
					Primitive,
					Pointer,
					Array,
					Function,
					Structure,
				};

				BasicTypeRecord();

				BasicTypeManager*						GetTypeManager();
				virtual TypeRecordType					GetType()=0;
				virtual BasicPrimitiveTypeEnum			PrimitiveType();
				virtual BasicTypeRecord*				ElementType();
				virtual int								ElementCount();
				virtual BasicTypeRecord*				ReturnType();
				virtual BasicTypeRecord*				ParameterType(int index);
				virtual int								ParameterCount();
				virtual BasicTypeRecord*				MemberType(int index);
				virtual BasicTypeRecord*				MemberType(const WString& name);
				virtual const WString&					MemberName(int index);
				virtual int								MemberCount();
			};

			class BasicPrimitiveTypeRecord : public BasicTypeRecord
			{
				friend class BasicTypeManager;
			protected:
				BasicPrimitiveTypeEnum					primitiveType;

				BasicPrimitiveTypeRecord(BasicTypeManager* _manager, BasicPrimitiveTypeEnum _primitiveType);
			public:

				TypeRecordType							GetType();
				BasicPrimitiveTypeEnum					PrimitiveType();
			};

			class BasicPointerTypeRecord : public BasicTypeRecord
			{
				friend class BasicTypeManager;
			protected:
				BasicTypeRecord*						elementType;

				BasicPointerTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _elementType);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
			};

			class BasicArrayTypeRecord : public BasicTypeRecord
			{
				friend class BasicTypeManager;
			protected:
				BasicTypeRecord*						elementType;
				int										elementCount;

				BasicArrayTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _elementType, int _elementCount);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
				int										ElementCount();
			};

			class BasicFunctionTypeRecord : public BasicTypeRecord
			{
				friend class BasicTypeManager;
			protected:
				BasicTypeRecord*						returnType;
				collections::List<BasicTypeRecord*>		parameterTypes;

				BasicFunctionTypeRecord(BasicTypeManager* _manager, BasicTypeRecord* _returnType, const collections::IReadonlyList<BasicTypeRecord*>& _parameterTypes);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ReturnType();
				BasicTypeRecord*						ParameterType(int index);
				int										ParameterCount();
			};

			class BasicStructureTypeRecord : public BasicTypeRecord
			{
				friend class BasicTypeManager;
			protected:
				collections::List<WString>				names;
				collections::List<BasicTypeRecord*>		types;

				BasicStructureTypeRecord(BasicTypeManager* _manager);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						MemberType(int index);
				BasicTypeRecord*						MemberType(const WString& name);
				const WString&							MemberName(int index);
				int										MemberCount();
			};

			class BasicTypeManager : public Object, private NotCopyable
			{
				typedef collections::List<Ptr<BasicTypeRecord>>								_TypeList;
				typedef collections::List<BasicTypeRecord*>									_FunctionTypeTable;
				typedef collections::Dictionary<BasicPrimitiveTypeEnum, BasicTypeRecord*>	_PrimitiveTypeTable;
			protected:
				_TypeList								allocatedTypes;
				_FunctionTypeTable						functionTypes;
				_PrimitiveTypeTable						primitiveTypes;
			public:
				BasicTypeManager();
				~BasicTypeManager();

				BasicTypeRecord*						GetPrimitiveType(BasicPrimitiveTypeEnum primitiveType);
				BasicTypeRecord*						GetPointerType(BasicTypeRecord* elementType);
				BasicTypeRecord*						GetArrayType(BasicTypeRecord* elementType, int elementCount);
				BasicTypeRecord*						GetFunctionType(BasicTypeRecord* returnType, const collections::IReadonlyList<BasicTypeRecord*>& parameterTypes);
				BasicTypeRecord*						CreateStructureType();
				void									UpdateStructureType(BasicTypeRecord* structureType, const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types);
			};
		}
	}
}

#endif
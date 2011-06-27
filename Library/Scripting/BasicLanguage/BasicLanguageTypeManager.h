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
				typedef collections::Dictionary<vint, CommonTypeRecord<BasicTypeRecord>*>		_ArrayTypeTable;
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
					Generic,
				};

				BasicTypeRecord();

				virtual TypeRecordType					GetType()=0;
				virtual BasicPrimitiveTypeEnum			PrimitiveType();						//primitive
				virtual BasicTypeRecord*				ElementType();							//pointer array generic structure(back reference to generic)
				virtual vint							ElementCount();							//array
				virtual BasicTypeRecord*				ReturnType();							//function
				virtual BasicTypeRecord*				ParameterType(vint index);				//function generic
				virtual vint							ParameterCount();						//function generic
				virtual BasicTypeRecord*				MemberType(vint index);					//structure
				virtual BasicTypeRecord*				MemberType(const WString& name);		//structure
				virtual const WString&					MemberName(vint index);					//structure
				virtual vint							MemberNameIndex(const WString& name);	//structure
				virtual vint							MemberCount();							//structure
				virtual bool							Defined();								//structure
				virtual BasicDeclaration*				Declaration();							//structure
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

			class BasicArrayTypeRecord : public CommonParameterizedTypeRecord<BasicTypeRecord, vint>
			{
				friend CommonParameterizedTypeRecord<BasicTypeRecord, vint>* BasicArrayTypeRecordAllocator(CommonTypeRecord<BasicTypeRecord>* elementType, vint elementCount);
			protected:

				BasicArrayTypeRecord(CommonTypeRecord<BasicTypeRecord>* elementType, vint elementCount);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
				vint									ElementCount();
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
				BasicTypeRecord*						ParameterType(vint index);
				vint									ParameterCount();
			};

			class BasicStructureTypeRecord : public CommonTypeRecord<BasicTypeRecord>
			{
				typedef collections::Dictionary<BasicTypeRecord*, BasicTypeRecord*>							_GenericInstanciatingTypeTable;

				struct P
				{
					Ptr<_GenericInstanciatingTypeTable>	typeTable;

					vint								Compare(const P& p)const;
					bool								operator==(const P& p)const;
					bool								operator!=(const P& p)const;
					bool								operator<(const P& p)const;
					bool								operator<=(const P& p)const;
					bool								operator>(const P& p)const;
					bool								operator>=(const P& p)const;
				};
				
				friend class BasicTypeManager;
				friend class collections::ReadonlyListEnumerator<P>;
				typedef collections::Dictionary<P, BasicTypeRecord*>										_ProxyTable;

			protected:
				collections::List<WString>				names;
				collections::List<BasicTypeRecord*>		types;
				bool									defined;
				_ProxyTable								proxyTable;
				BasicDeclaration*						declaration;
				BasicTypeRecord*						referencedGenericType;

				BasicStructureTypeRecord(BasicDeclaration* _declaration);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
				BasicTypeRecord*						MemberType(vint index);
				BasicTypeRecord*						MemberType(const WString& name);
				const WString&							MemberName(vint index);
				vint									MemberNameIndex(const WString& name);
				vint									MemberCount();
				bool									Defined();
				BasicDeclaration*						Declaration();
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

			class BasicGenericStructureProxyTypeRecord : public CommonTypeRecord<BasicTypeRecord>
			{
				friend class BasicTypeManager;

				typedef collections::Dictionary<BasicTypeRecord*, BasicTypeRecord*>					_GenericInstanciatingTypeTable;
				typedef collections::IDictionary<BasicTypeRecord*, BasicTypeRecord*>				_IGenericInstanciatingTypeTable;
			protected:
				BasicTypeRecord*						structureType;
				BasicTypeManager*						manager;
				Ptr<_GenericInstanciatingTypeTable>		typeTable;

				BasicGenericStructureProxyTypeRecord(BasicTypeRecord* _structureType, BasicTypeManager* _manager, Ptr<_GenericInstanciatingTypeTable> _typeTable);
			public:

				TypeRecordType							GetType();
				BasicTypeRecord*						MemberType(vint index);
				BasicTypeRecord*						MemberType(const WString& name);
				const WString&							MemberName(vint index);
				vint									MemberNameIndex(const WString& name);
				vint									MemberCount();
				bool									Defined();
				BasicDeclaration*						Declaration();

				BasicTypeRecord*						UninstantiatedStructureType();
				_IGenericInstanciatingTypeTable&		GenericArgumentMap();
			};

			class BasicGenericTypeRecord : public CommonTypeRecord<BasicTypeRecord>
			{
				friend class BasicTypeManager;

			protected:
				BasicTypeRecord*						elementType;
				collections::List<BasicTypeRecord*>		parameterTypes;
				BasicDeclaration*						declaration;

				BasicGenericTypeRecord(BasicDeclaration* _declaration);
			public:
				TypeRecordType							GetType();
				BasicTypeRecord*						ElementType();
				BasicTypeRecord*						ParameterType(vint index);
				vint									ParameterCount();
				BasicDeclaration*						Declaration();
			};

/***********************************************************************
BasicTypeManager
***********************************************************************/

			class BasicTypeManager : public CommonTypeManager<BasicTypeRecord>
			{
				friend class BasicGenericStructureProxyTypeRecord;
				typedef collections::List<CommonTypeRecord<BasicTypeRecord>*>								_FunctionTypeTable;
				typedef collections::Dictionary<BasicPrimitiveTypeEnum, CommonTypeRecord<BasicTypeRecord>*>	_PrimitiveTypeTable;
				typedef collections::Dictionary<WString, CommonTypeRecord<BasicTypeRecord>*>				_GenericArgumentTypeTable;
				typedef collections::IReadonlyDictionary<BasicTypeRecord*, BasicTypeRecord*>				_IGenericInstanciatingTypeTable;
				typedef collections::Dictionary<BasicTypeRecord*, BasicTypeRecord*>							_GenericInstanciatingTypeTable;
			protected:
				_FunctionTypeTable						functionTypes;
				_PrimitiveTypeTable						primitiveTypes;
				_GenericArgumentTypeTable				genericArgumentTypes;

				BasicTypeRecord*						Instanciate(BasicTypeRecord* genericType, Ptr<_GenericInstanciatingTypeTable> parameters);
			public:
				BasicTypeManager();
				~BasicTypeManager();

				BasicTypeRecord*						GetPrimitiveType(BasicPrimitiveTypeEnum primitiveType);
				BasicTypeRecord*						GetPointerType(BasicTypeRecord* elementType);
				BasicTypeRecord*						GetArrayType(BasicTypeRecord* elementType, vint elementCount);
				BasicTypeRecord*						GetFunctionType(BasicTypeRecord* returnType, const collections::IReadonlyList<BasicTypeRecord*>& parameterTypes);
				BasicTypeRecord*						CreateStructureType(BasicStructureDeclaration* declaration);
				void									UpdateStructureType(BasicStructureDeclaration* declaration, BasicTypeRecord* structureType, const collections::IReadonlyList<WString>& names, const collections::IReadonlyList<BasicTypeRecord*>& types);

				BasicTypeRecord*						GetGenericArgumentType(const WString& name);
				BasicTypeRecord*						CreateGenericType(BasicStructureDeclaration* declaration);
				void									UpdateGenericType(BasicTypeRecord* genericType, BasicTypeRecord* elementType, const collections::IReadonlyList<BasicTypeRecord*>& parameters);
				BasicTypeRecord*						Instanciate(BasicTypeRecord* genericType, const _IGenericInstanciatingTypeTable& parameters);
			};
		}
	}
}

#endif
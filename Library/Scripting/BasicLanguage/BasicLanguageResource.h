/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicLanguage

Classes:
  BasicTypeRes					������
  BasicTypeLinkRes				��������Ԫ������

  BasicParameterRes				����������
  BasicDeclarationRes			������
  BasicDeclarationLinkRes		����������

  BasicEntryRes					�����
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGERESOURCE
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGERESOURCE

#include "..\BasicIL\BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{
		struct BasicTypeRes;
		struct BasicSubTypeRes;

/***********************************************************************
����
***********************************************************************/

		struct BasicTypeRes
		{
			enum TypeEnum
			{
				Primitive,
				Pointer,
				Array,
				Function,
				Structure,
				GenericArgument,
				Concept,
				Instanciated,
			};

			enum PrimitiveTypeEnum
			{
				s8,s16,s32,s64,
				u8,u16,u32,u64,
				f32,f64,
				bool_type,
				void_type,
				char_type,
				wchar_type,
#ifdef VCZH_64
				int_type=s64,
				uint_type=u64,
#else
				int_type=s32,
				uint_type=u32,
#endif
			};

			TypeEnum									type;
			PrimitiveTypeEnum							primitiveType;			//Primitive
			ResourceHandle<BasicTypeRes>				elementType;			//Array, Pointer, Function(return type), Generic(element)
			vint										elementCount;
			ResourceArrayHandle<BasicSubTypeRes>		subTypes;				//Function(parameter), Structure(member), Concept(member), Generic(arguments)
			vint										size;
			ResourceString								genericArgumentName;	//GenericArgument
		};

		struct BasicSubTypeRes
		{
			ResourceHandle<BasicTypeRes>				type;					//Structure(member), Concept(member), Generic(arguments)
			ResourceString								name;					//Structure(member), Concept(member)
			vint										offset;					//Structure(member)
		};

/***********************************************************************
����
***********************************************************************/

		struct BasicParameterRes
		{
			ResourceString								name;
		};

		struct BasicConstraintRes
		{
			ResourceString								argumentName;
			ResourceString								conceptName;
		};

		struct BasicDeclarationRes
		{
			enum TypeEnum
			{
				Function,
				Variable,
				Structure,
				Concept,
				Instance,
			};

			TypeEnum									type;
			ResourceHandle<BasicTypeRes>				declarationType;				//Function, Variable, Structure, Concept, Instance
			ResourceString								name;
			ResourceArrayHandle<BasicParameterRes>		parameterNames;					//Function(parameter)
			vint										address;						//Function(ins index), Variable(data offset)
			ResourceString								linkingAssemblyName;			//Function, Variable, Structure
			ResourceString								linkingSymbolName;				//Function, Variable, Structure
			ResourceArrayHandle<BasicParameterRes>		genericArgumentNames;			//Function, Variable, Structure
			ResourceArrayHandle<BasicConstraintRes>		genericArgumentConstraints;
			ResourceString								instanceConceptAssemblyName;	//Instance
			ResourceString								instanceConceptSymbolName;		//Instance
		};

/***********************************************************************
���
***********************************************************************/

		struct BasicEntryRes
		{
			ResourceArrayHandle<BasicDeclarationRes>	declarations;
		};
	}
}

#endif
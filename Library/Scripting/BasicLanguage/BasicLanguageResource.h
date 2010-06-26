/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
  BasicTypeRes					：类型
  BasicTypeLinkRes				：类型子元素链表

  BasicParameterRes				：参数链表
  BasicDeclarationRes			：声明
  BasicDeclarationLinkRes		：声明链表

  BasicEntryRes					：入口
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
类型
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
			ResourceHandle<BasicTypeRes>				elementType;			//Array, Pointer, Function(return type)
			vint										elementCount;
			ResourceArrayHandle<BasicSubTypeRes>		subTypes;				//Function(parameter), Structure(member)
			vint										size;
			vint										alignment;
			ResourceString								genericArgumentName;	//GenericArgument
		};

		struct BasicSubTypeRes
		{
			ResourceHandle<BasicTypeRes>				type;
			ResourceString								name;					//Structure(member)
			vint										offset;					//Structure(member)
		};

/***********************************************************************
声明
***********************************************************************/

		struct BasicParameterRes
		{
			ResourceString								name;
		};

		struct BasicDeclarationRes
		{
			enum TypeEnum
			{
				Function,
				Variable,
				Structure
			};

			TypeEnum									type;
			ResourceHandle<BasicTypeRes>				declarationType;		//Function, Variable, Structure
			ResourceString								name;
			ResourceArrayHandle<BasicParameterRes>		parameterNames;			//Function(parameter)
			vint										address;				//Function(ins index), Variable(data offset)
			ResourceString								linkingAssemblyName;	//Function, Variable, Structure
			ResourceString								linkingSymbolName;		//Function, Variable, Structure
			ResourceArrayHandle<BasicParameterRes>		genericArgumentNames;	//Function, Variable, Structure
		};

/***********************************************************************
入口
***********************************************************************/

		struct BasicEntryRes
		{
			ResourceArrayHandle<BasicDeclarationRes>	declarations;
		};
	}
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILExportRes				：对外可见符号链表
  BasicILLinkingRes				：待链接符号链表
  BasicILEntryRes				：入口
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE
#define VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE

#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
可见符号
***********************************************************************/

		//可见符号
		struct BasicILExportRes
		{
			vint													address;				//地址
			ResourceString											name;					//符号
		};

/***********************************************************************
待链接符号
***********************************************************************/

		//链接目标
		struct BasicILLinkingRes
		{
			ResourceString											symbolName;				//符号
			ResourceString											assemblyName;			//Asembly名
		};

/***********************************************************************
模板符号
***********************************************************************/

		//字符串来源
		struct BasicILGenericNameRes
		{
			bool													isConstant;				//是否常数
			ResourceString											constantString;			//常数字符串
			vint													stringArgumentIndex;	//字符串参数（来源于特化函数传入的参数）
		};

		//线性对象参数
		struct BasicILGenericFactorItemRes
		{
			vint													factor;					//参数
		};

		//线性对象（值 = sum(fi*ti) + constant）
		struct BasicILGenericLinearRes
		{
			ResourceArrayHandle<BasicILGenericFactorItemRes>		factors;				//参数表
			vint													constant;				//常数项
		};

		//模板参数
		struct BasicILGenericArgumentRes
		{
			ResourceArrayHandle<BasicILGenericNameRes>				nameArgument;			//字符串参数
			ResourceHandle<BasicILGenericLinearRes>					sizeArgument;			//尺寸参数
		};

		//--------------------------------------------------------------------------------------------------------

		//模板函数入口
		struct BasicILGenericFunctionEntryRes
		{
			ResourceString											name;					//模板函数名称
			vint													genericArgumentCount;	//模板参数数量
			vint													startInstruction;		//起始指令位置
			vint													instructionCount;		//指令数量
			ResourceArrayHandle<BasicILGenericNameRes>				uniqueNameTemplate;		//唯一标识字符串模板（最终组成一个长字符串来唯一表示自己）
		};

		//模板函数特化
		struct BasicILGenericFunctionTargetRes
		{
			ResourceString											symbolName;				//符号
			ResourceString											assemblyName;			//Assembly名
			ResourceArrayHandle<BasicILGenericArgumentRes>			arguments;				//模板参数
		};

		//--------------------------------------------------------------------------------------------------------

		//模板函数入口
		struct BasicILGenericVariableEntryRes
		{
			ResourceString											name;					//模板变量名称
			vint													genericArgumentCount;	//模板参数数量
			ResourceHandle<BasicILGenericLinearRes>					size;					//模板变量尺寸
			ResourceArrayHandle<BasicILGenericNameRes>				uniqueNameTemplate;		//唯一标识字符串模板（最终组成一个长字符串来唯一表示自己）
		};

		//模板函数特化
		struct BasicILGenericVariableTargetRes
		{
			ResourceString											symbolName;				//符号
			ResourceString											assemblyName;			//Assembly名
			ResourceArrayHandle<BasicILGenericArgumentRes>			arguments;				//模板参数
		};

		//--------------------------------------------------------------------------------------------------------

		//模板符号表
		struct BasicILGenericRes
		{
			ResourceArrayHandle<BasicILGenericFunctionEntryRes>		functionEntries;		//函数入口
			ResourceArrayHandle<BasicILGenericFunctionTargetRes>	functionTargets;		//函数特化表
			ResourceArrayHandle<BasicILGenericVariableEntryRes>		variableEntries;		//变量函数入口
			ResourceArrayHandle<BasicILGenericVariableTargetRes>	variableTargets;		//变量特化表
			ResourceArrayHandle<BasicILGenericLinearRes>			linears;				//线性参数表
		};

/***********************************************************************
入口
***********************************************************************/

		 //总符号入口
		struct BasicILEntryRes
		{
			ResourceString											assemblyName;			//Assembly名
			ResourceArrayHandle<BasicILExportRes>					exports;				//可见符号
			ResourceArrayHandle<BasicILLinkingRes>					linkings;				//待链接符号
			ResourceHandle<BasicILGenericRes>						genericSymbols;			//模板符号
		};
	}
}

#endif
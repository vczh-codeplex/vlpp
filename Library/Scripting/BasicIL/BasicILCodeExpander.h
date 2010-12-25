/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILCodeExpander				：虚拟机指令展开器
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILCODEEXPANDER
#define VCZH_SCRIPTING_BASICIL_BASICILCODEEXPANDER

#include "BasicILRuntimeSymbol.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
模板符号特化表
***********************************************************************/

			struct BasicILGenericArgument
			{
				vint											size;
				WString											name;
				collections::Array<Ptr<BasicILGenericArgument>>	subArguments;

				bool											operator==(const BasicILGenericArgument& value)const;
				bool											operator!=(const BasicILGenericArgument& value)const;
			};

			struct BasicILGenericArgumentEnvironment
			{
				collections::Array<Ptr<BasicILGenericArgument>>	arguments;
			};

			struct BasicILGenericTarget : public BasicILGenericArgumentEnvironment
			{
				typedef collections::List<Ptr<BasicILGenericTarget>> ListType;

				WString											symbolName;
				WString											assemblyName;
			};

/***********************************************************************
虚拟机指令展开器
***********************************************************************/

			class BasicILCodeExpander : public Object
			{
				typedef collections::Dictionary<WString, vint>								_InstanciatedGenericFunctionMap;

			public:
				struct VariablePackage
				{
					collections::Array<char>					buffer;
					vint										remainBytes;

					VariablePackage(vint size);
					char*										Allocate(vint size);
				};

				struct VariableManager
				{
					collections::List<Ptr<VariablePackage>>	packages;
					collections::Dictionary<WString, char*>	variables;

					char*										Allocate(const WString& name, vint size);
				};
			protected:
				BasicILGenericTarget::ListType					genericTargets;
				_InstanciatedGenericFunctionMap					instanciatedGenericFunctions;
				VariableManager									instanciatedGenericVariables;
				BasicILRuntimeSymbol*							symbols;
				
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, ResourceHandle<BasicILGenericTargetRes> targetRecordHandle);
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex);
				BasicILGenericTarget*							GetTarget(vint index);

				vint											RegisterInstanceFunction(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex, bool& isGenericFunction);
				void											RewriteInstanceFunctionInstruction(BasicILGenericArgumentEnvironment* environment, BasicIns& ins, BasicIL* originIL, BasicIns::OpCode genericOp, BasicIns::OpCode normalOp);
				vint											InstanciateGenericFunction(BasicILGenericTarget* target);
				char*											InstanciateGenericVariable(BasicILGenericTarget* target);
			public:
				BasicILCodeExpander(BasicILRuntimeSymbol* _symbols);
				~BasicILCodeExpander();

				void											RewriteExecutingGenericInstruction(BasicIns& ins, BasicIL* il, vint insIndex);
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif
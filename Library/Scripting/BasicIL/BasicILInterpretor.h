/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  IBasicILForeignFunction			����Ӻ���
  BasicILInterpretor				�����������ʱ����
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILEnv.h"
#include "BasicILException.h"
#include "BasicILStack.h"
#include "BasicILRuntimeSymbol.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
ģ������ػ���
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
�����
***********************************************************************/

			class BasicILInterpretor : public Object
			{
				friend class BasicILStack;

				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
				typedef collections::Dictionary<WString, vint>								_InstanciatedGenericFunctionMap;
				typedef collections::List<Ptr<IBasicILForeignFunction>>						_ForeignFunctionList;
				typedef collections::List<BasicILLightFunctionInfo>							_LightFunctionList;

				static const vint								GenericFunctionSitingAssemblyKey=0;
				static const vint								ForeignFunctionSitingAssemblyKey=-2;
				static const vint								LightFunctionSitingAssemblyKey=-3;

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
				vint											stackSize;
				BasicILRuntimeSymbol							runtimeSymbol;

				BasicILGenericTarget::ListType					genericTargets;
				_InstanciatedGenericFunctionMap					instanciatedGenericFunctions;
				VariableManager									instanciatedGenericVariables;
				
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, ResourceHandle<BasicILGenericTargetRes> targetRecordHandle);
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex);
				vint											RegisterInstanceFunction(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex, bool& isGenericFunction);
				void											RewriteInstanceFunctionInstruction(BasicILGenericArgumentEnvironment* environment, BasicIns& ins, BasicIL* originIL, BasicIns::OpCode genericOp, BasicIns::OpCode normalOp);
				vint											InstanciateGenericFunction(BasicILGenericTarget* target);
				char*											InstanciateGenericVariable(BasicILGenericTarget* target);
			public:
				BasicILInterpretor(vint _stackSize);
				~BasicILInterpretor();

				vint											LoadIL(BasicIL* il);
				void											LogInternalState(stream::TextWriter& writer);
				BasicILRuntimeSymbol*							Symbols();
			};
		}
	}
}

#endif
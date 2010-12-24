/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  IBasicILForeignFunction			：外接函数
  BasicILInterpretor				：虚拟机运行时环境
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILDefinition.h"
#include "BasicILSymbolResource.h"
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
外接函数
***********************************************************************/

			class BasicILInterpretor;
			class BasicILStack;

			class IBasicILForeignFunction : public Interface
			{
			public:
				virtual void									Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)=0;
			};

			typedef void(*BasicILLightFunction)(void* result, void* arguments);

			struct BasicILLightFunctionInfo
			{
				BasicILLightFunction							function;
				vint											argumentSize;

				bool											operator==(const BasicILLightFunctionInfo& info)const;
				bool											operator!=(const BasicILLightFunctionInfo& info)const;
			};

/***********************************************************************
虚拟机
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
				collections::List<BasicIL*>						ils;
				_BasicILMap										ilMap;
				collections::List<BasicILLabel>					labels;
				_SymbolMap										symbolMap;

				_SymbolMap										foreignFunctionLabelMap;
				_ForeignFunctionList							foreignFunctionList;
				_LightFunctionList								lightFunctionList;

				BasicILGenericFunctionEntry::MapType			genericFunctionEntries;
				BasicILGenericVariableEntry::MapType			genericVariableEntries;
				BasicILGenericTarget::ListType					genericTargets;
				_SymbolList										genericConcepts;
				BasicILGenericInstanceEntry::MapType			genericInstances;

				_InstanciatedGenericFunctionMap					instanciatedGenericFunctions;
				VariableManager									instanciatedGenericVariables;
				Ptr<BasicIL>									genericFunctionSitingIL;
				
				void											LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
				void											LinkILSymbol(BasicIL* il, vint index, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
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
				void											UnloadIL(BasicIL* il);
				bool											RegisterForeignFunction(const WString& category, const WString& name, Ptr<IBasicILForeignFunction> function);
				bool											RegisterLightFunction(const WString& category, const WString& name, BasicILLightFunction function, vint argumentSize);
				collections::IList<BasicILLabel>&				GetLabels();
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif
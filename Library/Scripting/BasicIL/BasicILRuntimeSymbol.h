/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILRuntimeSymbol					：符号管理器
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILRUNTIMESYMBOL
#define VCZH_SCRIPTING_BASICIL_BASICILRUNTIMESYMBOL

#include "..\..\Entity\Linear.h"
#include "BasicILDefinition.h"
#include "BasicILSymbolResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
普通符号表
***********************************************************************/

			struct BasicILLabel
			{
				vint											instruction;
				vint											key;

				bool											operator==(const BasicILLabel& label)const;
				bool											operator!=(const BasicILLabel& label)const;
			};

/***********************************************************************
模板符号表
***********************************************************************/

			struct BasicILGenericFunctionEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericFunctionEntry>>	MapType;

				vint											instruction;
				vint											key;
				vint											count;
				vint											argumentCount;
				WString											uniqueEntryID;
			};

			struct BasicILGenericVariableEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericVariableEntry>>	MapType;

				vint											argumentCount;
				Linear<vint, vint>								size;
				WString											uniqueEntryID;
			};

			struct BasicILGenericInstanceEntry
			{
				struct Key
				{
					WString										symbolName;
					WString										assemblyName;
					WString										typeUniqueName;

					int Compare(const Key& k)const
					{
						if(symbolName<k.symbolName)return -1;
						if(symbolName>k.symbolName)return 1;
						if(assemblyName<k.assemblyName)return -1;
						if(assemblyName>k.assemblyName)return 1;
						if(typeUniqueName<k.typeUniqueName)return -1;
						if(typeUniqueName>k.typeUniqueName)return 1;
						return 0;
					}

					bool operator<(const Key& k)const
					{
						return Compare(k)<0;
					}

					bool operator>(const Key& k)const
					{
						return Compare(k)>0;
					}

					bool operator==(const Key& k)const
					{
						return Compare(k)==0;
					}
				};

				typedef collections::Dictionary<Key, Ptr<BasicILGenericInstanceEntry>>									MapType;
				typedef collections::Dictionary<WString, vint>															FunctionMap;

				vint											argumentCount;
				vint											instanceIndex;
				WString											assemblyName;
				FunctionMap										functions;
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
符号管理器
***********************************************************************/

			class BasicILRuntimeSymbol : public Object
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
				typedef collections::List<Ptr<IBasicILForeignFunction>>						_ForeignFunctionList;
				typedef collections::List<BasicILLightFunctionInfo>							_LightFunctionList;

				static const vint								GenericFunctionSitingAssemblyKey=0;
				static const vint								ForeignFunctionSitingAssemblyKey=-2;
				static const vint								LightFunctionSitingAssemblyKey=-3;
			private:
				collections::List<BasicIL*>						ils;
				_BasicILMap										ilMap;
				collections::List<BasicILLabel>					labels;
				_SymbolMap										symbolMap;
				Ptr<BasicIL>									genericFunctionSitingIL;

				BasicILGenericFunctionEntry::MapType			genericFunctionEntries;
				BasicILGenericVariableEntry::MapType			genericVariableEntries;
				_SymbolList										genericConcepts;
				BasicILGenericInstanceEntry::MapType			genericInstances;

				_SymbolMap										foreignFunctionLabelMap;
				_ForeignFunctionList							foreignFunctionList;
				_LightFunctionList								lightFunctionList;

			public:

				BasicILRuntimeSymbol();
				~BasicILRuntimeSymbol();
				
				BasicIL*										GetGenericFunctionSitingIL();
				BasicIL*										GetIL(vint index);
				BasicIL*										GetIL(const WString& name);
				vint											GetILIndex(BasicIL* il);
				vint											GetILIndex(const WString& name);
				bool											IsValidILIndex(vint index);
				vint											GetSymbol(const collections::Pair<WString, WString>& key);

				const BasicILLabel&								GetLabel(vint index);
				vint											GetLabelIndex(const BasicILLabel& label);
				vint											AddLabel(const BasicILLabel& label);
				bool											IsValidLabelIndex(vint index);

				Ptr<BasicILGenericFunctionEntry>				GetGenericFunctionEntry(const collections::Pair<WString, WString>& key);
				Ptr<BasicILGenericVariableEntry>				GetGenericVariableEntry(const collections::Pair<WString, WString>& key);
				const collections::Pair<WString, WString>&		GetGenericConcept(vint index);
				Ptr<BasicILGenericInstanceEntry>				GetGenericInstance(const BasicILGenericInstanceEntry::Key& key);
				
				vint											LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
				void											LinkILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
				bool											RegisterForeignFunction(const WString& category, const WString& name, Ptr<IBasicILForeignFunction> function);
				bool											RegisterLightFunction(const WString& category, const WString& name, BasicILLightFunction function, vint argumentSize);
				IBasicILForeignFunction*						GetForeignFunction(vint index);
				const BasicILLightFunctionInfo&					GetLightFunction(vint index);
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  IBasicILForeignFunction				：外接函数
  BasicILLightFunctionInfo				：轻量级外接函数
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

			struct BasicILLightFunction
			{
				typedef vint(*Handler)(void* result, void* arguments, BasicILInterpretor* interpretor, BasicILStack* stack, void* userData);

				Handler											handler;
				void*											userData;

				BasicILLightFunction()
					:handler(0)
					,userData(0)
				{
				}

				BasicILLightFunction(Handler _handler, void* _userData)
					:handler(_handler)
					,userData(_userData)
				{
				}

				bool operator==(const BasicILLightFunction&){return false;}
				bool operator!=(const BasicILLightFunction&){return true;}
			};

/***********************************************************************
符号管理器
***********************************************************************/

			class BasicILRuntimeSymbol : public Object
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
				typedef collections::List<BasicILLightFunction>								_LightFunctionList;

			public:
				static const vint								GenericFunctionSitingAssemblyKey=0;
				static const vint								LightFunctionSitingAssemblyKey=-2;

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
				_LightFunctionList								lightFunctionList;

			public:

				BasicILRuntimeSymbol();
				~BasicILRuntimeSymbol();
				
				BasicIL*										GetGenericFunctionSitingIL();
				BasicIL*										GetIL(vint index);
				BasicIL*										GetIL(const WString& name);
				vint											GetILIndex(BasicIL* il);
				vint											GetILIndex(const WString& name);
				vint											GetILCount();
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
				void											LinkILFixInstructionKeyOnly(BasicIL* il);
				bool											RegisterLightFunction(const WString& category, const WString& name, BasicILLightFunction function);
				BasicILLightFunction							GetLightFunction(vint index);
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILInterpretor				�����������ʱ����
  BasicILLinker						��������
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILEnv.h"
#include "BasicILException.h"
#include "BasicILStack.h"
#include "BasicILRuntimeSymbol.h"
#include "BasicILCodeExpander.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
�����
***********************************************************************/

			class BasicILInterpretor : public Object
			{
			protected:
				vint											stackSize;
				BasicILRuntimeSymbol							symbols;
				BasicILCodeExpander								expander;
			public:
				BasicILInterpretor(vint _stackSize);
				~BasicILInterpretor();

				vint											GetStackSize();
				vint											LoadIL(BasicIL* il);
				BasicILRuntimeSymbol*							Symbols();
				BasicILCodeExpander*							Expander();
				void											LogInternalState(stream::TextWriter& writer);
			};

/***********************************************************************
������
***********************************************************************/

			class BasicILLinker : public Object
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::Dictionary<collections::Pair<vint, vint>, vint>		_LabelMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
			public:
				struct LinkedFunctionInfo
				{
					WString										assemblyName;
					WString										symbolName;
					BasicIL*									originalIL;
					vint										originalOffset;
					vint										offset;
					vint										count;

					bool										operator==(const LinkedFunctionInfo& info);
					bool										operator!=(const LinkedFunctionInfo& info);
				};

				struct LinkedVariableInfo
				{
					WString										assemblyName;
					WString										symbolName;
					BasicIL*									originalIL;
					vint										originalOffset;
					vint										offset;

					bool										operator==(const LinkedVariableInfo& info);
					bool										operator!=(const LinkedVariableInfo& info);
				};

			protected:
				BasicILRuntimeSymbol							symbols;
				BasicILCodeExpander								expander;
				Ptr<BasicIL>									linkedIL;

				collections::List<LinkedFunctionInfo>			linkedFunctions;
				collections::List<LinkedVariableInfo>			linkedVariables;

				collections::Dictionary<BasicIL*, vint>			assemblyGlobalData;
				_SymbolMap										assemblyFunctionMap;
				_SymbolMap										assemblyVariableMap;
				_LabelMap										assemblyLabelMap;

				_SymbolList										foreignFunctions;
				_SymbolMap										foreignMap;

				void											ExpandIns(BasicIL* il, vint index);
				void											ExpandLoadedIL(BasicIL* il);
				void											ExpandSitingIL(BasicIL* il);
				void											ExpandAll();

				void											CopyInstructions(BasicIL* dst, BasicIL* src, vint start, vint count);
				void											CopyAssemblyInitializers();
				void											CopyInstanciatedGenericFunctions();
				void											CopyAssemblyExportedFunctions();
				
				void											CopyInstanciatedGenericVariables();
				void											CopyAssemblyExportedGlobalVariables();

				void											LinkInstructions();
				void											BuildExportedResource();
			public:
				BasicILLinker();
				~BasicILLinker();

				void											LoadIL(BasicIL* il);
				Ptr<BasicIL>									Link();
				BasicILRuntimeSymbol*							Symbols();
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif
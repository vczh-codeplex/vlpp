/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILRUNTIMESYMBOL
#define VCZH_SCRIPTING_BASICIL_BASICILRUNTIMESYMBOL

#include "..\..\Entity\Linear.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
ÆÕÍ¨·ûºÅ±í
***********************************************************************/

			struct BasicILLabel
			{
				vint											instruction;
				vint											key;

				bool											operator==(const BasicILLabel& label)const;
				bool											operator!=(const BasicILLabel& label)const;
			};

/***********************************************************************
Ä£°å·ûºÅ±í
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
		}
	}
}

#endif
#include "BasicILInterpretor.h"
#include "BasicILSymbolResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
BasicILInterpretor
***********************************************************************/

			BasicILInterpretor::BasicILInterpretor(vint _stackSize)
				:stackSize(_stackSize)
				,expander(&symbols, true)
			{
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
			}

			vint BasicILInterpretor::GetStackSize()
			{
				return stackSize;
			}

			vint BasicILInterpretor::LoadIL(BasicIL* il)
			{
				List<Pair<WString, WString>> linkingSymbols, foreignFunctions;
				vint index=symbols.LoadILSymbol(il, linkingSymbols, foreignFunctions);
				symbols.LinkILSymbol(il, linkingSymbols, foreignFunctions);
				return index;
			}

			BasicILRuntimeSymbol* BasicILInterpretor::Symbols()
			{
				return &symbols;
			}

			BasicILCodeExpander* BasicILInterpretor::Expander()
			{
				return &expander;
			}

/***********************************************************************
BasicILLinker
***********************************************************************/

			void BasicILLinker::ExpandIns(BasicIL* il, vint index)
			{
				while(true)
				{
					switch(il->instructions[index].opcode)
					{
					case BasicIns::generic_pushdata:
					case BasicIns::generic_callfunc:
					case BasicIns::generic_pushfunc:
					case BasicIns::generic_instance_callfunc:
					case BasicIns::generic_instance_pushfunc:
					case BasicIns::generic_callfunc_vm:
					case BasicIns::generic_pushfunc_vm:
						expander.RewriteExecutingGenericInstruction(il->instructions[index], il, index);
						break;
					default:
						return;
					}
				}
			}

			void BasicILLinker::ExpandLoadedIL(BasicIL* il)
			{
				Ptr<ResourceStream> resource=il->resources[BasicILResourceNames::ExportedSymbols];
				ResourceRecord<BasicILEntryRes> entry=resource->ReadRootRecord<BasicILEntryRes>();
				ResourceArrayRecord<BasicILExportRes> functions=resource->ReadArrayRecord(entry->exports);
				for(vint i=0;i<functions.Count();i++)
				{
					ResourceRecord<BasicILExportRes> function=functions.Get(i);
					if(function->type==BasicILExportRes::Function)
					{
						vint start=function->address;
						vint end=start+function->instructionCount;
						for(vint j=start;j<end;j++)
						{
							ExpandIns(il, j);
						}
					}
				}
			}

			void BasicILLinker::ExpandSitingIL(BasicIL* il)
			{
				for(vint i=0;i<il->instructions.Count();i++)
				{
					ExpandIns(il, i);
				}
			}

			void BasicILLinker::ExpandAll()
			{
				for(vint i=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey+1;i<symbols.GetILCount();i++)
				{
					BasicIL* il=symbols.GetIL(i);
					ExpandLoadedIL(il);
				}
				ExpandSitingIL(symbols.GetGenericFunctionSitingIL());
			}

			BasicILLinker::BasicILLinker()
				:expander(&symbols, false)
			{
			}

			BasicILLinker::~BasicILLinker()
			{
			}

			void BasicILLinker::LoadIL(BasicIL* il)
			{
				List<Pair<WString, WString>> linkingSymbols, foreignFunctions;
				symbols.LoadILSymbol(il, linkingSymbols, foreignFunctions);
				symbols.LinkILFixInstructionKeyOnly(il);
			}

			Ptr<BasicIL> BasicILLinker::Link()
			{
				ExpandAll();
				return 0;
			}

			BasicILRuntimeSymbol* BasicILLinker::Symbols()
			{
				return &symbols;
			}
		}
	}
}
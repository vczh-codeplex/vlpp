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
BasicILLinker::LinkedFunctionInfo
***********************************************************************/

			bool BasicILLinker::LinkedFunctionInfo::operator==(const LinkedFunctionInfo& info)
			{
				return
					assemblyName==info.assemblyName &&
					symbolName==info.symbolName &&
					originalIL==info.originalIL &&
					originalOffset==info.originalOffset &&
					offset==info.offset &&
					count==info.count;
			}

			bool BasicILLinker::LinkedFunctionInfo::operator!=(const LinkedFunctionInfo& info)
			{
				return
					assemblyName!=info.assemblyName ||
					symbolName!=info.symbolName ||
					originalIL!=info.originalIL ||
					originalOffset!=info.originalOffset ||
					offset!=info.offset ||
					count!=info.count;
			}

/***********************************************************************
BasicILLinker::LinkedVariableInfo
***********************************************************************/

			bool BasicILLinker::LinkedVariableInfo::operator==(const LinkedVariableInfo& info)
			{
				return
					assemblyName==info.assemblyName &&
					symbolName==info.symbolName &&
					originalIL==info.originalIL &&
					originalOffset==info.originalOffset &&
					offset==info.offset;
			}

			bool BasicILLinker::LinkedVariableInfo::operator!=(const LinkedVariableInfo& info)
			{
				return
					assemblyName!=info.assemblyName ||
					symbolName!=info.symbolName ||
					originalIL!=info.originalIL ||
					originalOffset!=info.originalOffset ||
					offset!=info.offset;
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

			void BasicILLinker::CopyInstructions(BasicIL* dst, BasicIL* src, vint start, vint count)
			{
				for(vint i=0;i<count;i++)
				{
					dst->instructions.Add(src->instructions[start+i]);
				}
			}

			void BasicILLinker::CopyAssemblyInitializers()
			{
				vint initCount=symbols.GetILCount()-1-BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey;
				vint initOffset=initCount*2+1;
				for(vint i=0;i<initCount;i++)
				{
					BasicIL* il=symbols.GetIL(BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey+1+i);
					Ptr<ResourceStream> resource=il->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> entry=resource->ReadRootRecord<BasicILEntryRes>();
					WString assemblyName=resource->ReadString(entry->assemblyName);

					LinkedFunctionInfo info;
					info.assemblyName=L"[INITIALIZER]";
					info.symbolName=assemblyName;
					info.originalIL=il;
					info.originalOffset=0;
					info.offset=initOffset;
					info.count=0;

					for(vint j=0;j<il->instructions.Count();j++)
					{
						if(il->instructions[j].opcode==BasicIns::ret)
						{
							info.count=j+1;
							initOffset+=info.count;
							break;
						}
					}

					linkedIL->Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer(0));
					linkedIL->Ins(BasicIns::call, BasicIns::MakeInt(info.offset));
					linkedFunctions.Add(info);
				}
				linkedIL->Ins(BasicIns::ret, BasicIns::MakeInt(0));

				for(vint i=0;i<linkedFunctions.Count();i++)
				{
					LinkedFunctionInfo& info=linkedFunctions[i];
					CopyInstructions(linkedIL.Obj(), info.originalIL, info.originalOffset, info.count);
				}
			}

			void BasicILLinker::CopyInstanciatedGenericFunctions()
			{
				BasicIL* sitingIL=symbols.GetGenericFunctionSitingIL();
				const Dictionary<WString, vint>& functions=expander.GetInstanciatedGenericFunctions();
				List<vint> offsets;
				List<vint> counts;
				for(vint i=0;i<functions.Count();i++)
				{
					offsets.Add(symbols.GetLabel(functions.Values()[i]).instruction);
				}
				{
					SortedList<vint> sortedOffsets;
					CopyFrom(sortedOffsets.Wrap(), offsets.Wrap());
					for(vint i=0;i<offsets.Count();i++)
					{
						vint offset=offsets[i];
						vint index=sortedOffsets.IndexOf(offset);
						if(index==sortedOffsets.Count()-1)
						{
							counts.Add(sitingIL->instructions.Count()-offset);
						}
						else
						{
							counts.Add(sortedOffsets[index+1]-offset);
						}
					}
				}

				for(vint i=0;i<functions.Count();i++)
				{
					LinkedFunctionInfo info;
					info.assemblyName=L"[SITING]";
					info.symbolName=functions.Keys()[i];
					info.originalIL=sitingIL;
					info.originalOffset=offsets[i];
					info.offset=linkedIL->instructions.Count();
					info.count=counts[i];
					linkedFunctions.Add(info);
					CopyInstructions(linkedIL.Obj(), info.originalIL, info.originalOffset, info.count);
				}
			}

			void BasicILLinker::CopyAssemblyExportedFunctions()
			{
				for(vint i=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey+1;i<symbols.GetILCount();i++)
				{
					BasicIL* il=symbols.GetIL(i);
					Ptr<ResourceStream> resource=il->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> entry=resource->ReadRootRecord<BasicILEntryRes>();
					WString assemblyName=resource->ReadString(entry->assemblyName);

					ResourceArrayRecord<BasicILExportRes> functions=resource->ReadArrayRecord(entry->exports);
					for(vint i=0;i<functions.Count();i++)
					{
						ResourceRecord<BasicILExportRes> function=functions.Get(i);
						if(function->type==BasicILExportRes::Function)
						{
							LinkedFunctionInfo info;
							info.assemblyName=L"[ASSEMBLY]"+assemblyName;
							info.symbolName=resource->ReadString(function->name);
							info.originalIL=il;
							info.originalOffset=function->address;
							info.offset=linkedIL->instructions.Count();
							info.count=function->instructionCount;
							linkedFunctions.Add(info);
							CopyInstructions(linkedIL.Obj(), info.originalIL, info.originalOffset, info.count);
						}
					}
				}
			}

			void BasicILLinker::CopyInstanciatedGenericVariables()
			{
				BasicIL* sitingIL=symbols.GetGenericFunctionSitingIL();
				const Dictionary<WString, Pair<char*, vint>>& variables=expander.GetInstanciatedGenericVariables();

				for(vint i=0;i<variables.Count();i++)
				{
					LinkedVariableInfo info;
					info.assemblyName=L"[SITING]";
					info.symbolName=variables.Keys()[i];
					info.originalIL=sitingIL;
					info.originalOffset=variables.Values()[i].value;
					info.offset=info.originalOffset;
					linkedVariables.Add(info);
				}
			}

			void BasicILLinker::CopyAssemblyExportedGlobalVariables()
			{
				vint totalSize=expander.GetInstanciatedGenericVariableSize();
				for(vint i=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey+1;i<symbols.GetILCount();i++)
				{
					totalSize+=symbols.GetIL(i)->globalData.Count();
				}
				linkedIL->globalData.Resize(totalSize);
				if(totalSize>0)
				{
					memset(&linkedIL->globalData[0], 0, (vint)(linkedIL->globalData.Count()*sizeof(linkedIL->globalData[0])));
					vint offset=expander.GetInstanciatedGenericVariableSize();
					
					for(vint i=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey+1;i<symbols.GetILCount();i++)
					{
						BasicIL* il=symbols.GetIL(i);
						if(il->globalData.Count()>0)
						{
							Ptr<ResourceStream> resource=il->resources[BasicILResourceNames::ExportedSymbols];
							ResourceRecord<BasicILEntryRes> entry=resource->ReadRootRecord<BasicILEntryRes>();
							WString assemblyName=resource->ReadString(entry->assemblyName);

							ResourceArrayRecord<BasicILExportRes> variables=resource->ReadArrayRecord(entry->exports);
							for(vint i=0;i<variables.Count();i++)
							{
								ResourceRecord<BasicILExportRes> variable=variables.Get(i);
								if(variable->type==BasicILExportRes::Variable)
								{
									LinkedVariableInfo info;
									info.assemblyName=L"[ASSEMBLY]"+assemblyName;
									info.symbolName=resource->ReadString(variable->name);
									info.originalIL=il;
									info.originalOffset=variable->address;
									info.offset=offset+info.originalOffset;
									linkedVariables.Add(info);
								}
							}
							memcpy(&linkedIL->globalData[offset], &il->globalData[0], (vint)(il->globalData.Count()*sizeof(il->globalData[0])));
							offset+=il->globalData.Count();
						}
					}
				}
			}

			void BasicILLinker::LinkInstructions()
			{
				//link_
				//codegen_
				//foreign function
			}

			void BasicILLinker::BuildExportedResource()
			{
				Ptr<ResourceStream> resource=new ResourceStream;
				linkedIL->resources.Add(BasicILResourceNames::ExportedSymbols, resource);

				ResourceRecord<BasicILEntryRes> entry=resource->CreateRecord<BasicILEntryRes>();
				entry->assemblyName=resource->CreateString(L"VCZH_LINKED_ASSEMBLY");
				entry->exports=ResourceArrayHandle<BasicILExportRes>::Null();
				entry->foreigns=ResourceArrayHandle<BasicILLinkingRes>::Null();
				entry->genericSymbols=ResourceHandle<BasicILGenericRes>::Null();
				entry->linkings=ResourceArrayHandle<BasicILLinkingRes>::Null();

				ResourceArrayRecord<BasicILExportRes> exports=resource->CreateArrayRecord<BasicILExportRes>(linkedFunctions.Count()+linkedVariables.Count());
				entry->exports=exports;
				for(vint i=0;i<linkedFunctions.Count();i++)
				{
					LinkedFunctionInfo& info=linkedFunctions[i];
					ResourceRecord<BasicILExportRes> exportRes=resource->CreateRecord<BasicILExportRes>();
					exports.Set(i, exportRes);

					exportRes->type=BasicILExportRes::Function;
					exportRes->name=resource->CreateString(info.assemblyName+L"::"+info.symbolName);
					exportRes->address=info.offset;
					exportRes->instructionCount=info.count;
				}
				for(vint i=0;i<linkedVariables.Count();i++)
				{
					LinkedVariableInfo& info=linkedVariables[i];
					ResourceRecord<BasicILExportRes> exportRes=resource->CreateRecord<BasicILExportRes>();
					exports.Set(linkedFunctions.Count()+i, exportRes);

					exportRes->type=BasicILExportRes::Variable;
					exportRes->name=resource->CreateString(info.assemblyName+L"::"+info.symbolName);
					exportRes->address=info.offset;
					exportRes->instructionCount=-1;
				}
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
				if(!linkedIL)
				{
					List<Pair<WString, WString>> linkingSymbols, foreignFunctions;
					symbols.LoadILSymbol(il, linkingSymbols, foreignFunctions);
					symbols.LinkILFixInstructionKeyOnly(il);
				}
			}

			Ptr<BasicIL> BasicILLinker::Link()
			{
				if(!linkedIL)
				{
					ExpandAll();
					linkedIL=new BasicIL;
					CopyAssemblyInitializers();
					CopyInstanciatedGenericFunctions();
					CopyAssemblyExportedFunctions();
					CopyInstanciatedGenericVariables();
					CopyAssemblyExportedGlobalVariables();
					LinkInstructions();
					BuildExportedResource();
				}
				return linkedIL;
			}

			BasicILRuntimeSymbol* BasicILLinker::Symbols()
			{
				return &symbols;
			}
		}
	}
}
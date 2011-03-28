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

			BasicILLock& BasicILInterpretor::GetLock()
			{
				return lock;
			}

			vint BasicILInterpretor::GetStackSize()
			{
				return stackSize;
			}

			vint BasicILInterpretor::LoadIL(BasicIL* il)
			{
				BasicILLock::Scope scope(lock);
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
					{
						BasicILLocalLabel label;
						label.instructionIndex=info.offset;
						label.instructionCount=info.count;
						vint labelIndex=linkedIL->labels.Add(label);
						assemblyLabelMap.Add(Pair<vint, vint>(symbols.GetILIndex(info.originalIL), info.originalOffset), labelIndex);
					}
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
							vint symbolIndex=linkedFunctions.Add(info);
							{
								BasicILLocalLabel label;
								label.instructionIndex=info.offset;
								label.instructionCount=info.count;
								vint labelIndex=linkedIL->labels.Add(label);
								assemblyFunctionMap.Add(Pair<WString, WString>(assemblyName, info.symbolName), symbolIndex);
								assemblyLabelMap.Add(Pair<vint, vint>(symbols.GetILIndex(info.originalIL), info.originalOffset), labelIndex);
							}
							CopyInstructions(linkedIL.Obj(), info.originalIL, info.originalOffset, info.count);
						}
					}
				}
			}

			void BasicILLinker::CopyInstanciatedGenericVariables()
			{
				BasicIL* sitingIL=symbols.GetGenericFunctionSitingIL();
				assemblyGlobalData.Add(sitingIL, 0);
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
						assemblyGlobalData.Add(il, offset);
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
									vint symbolIndex=linkedVariables.Add(info);
									assemblyVariableMap.Add(Pair<WString, WString>(assemblyName, info.symbolName), symbolIndex);
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
				BasicIL* currentIL=0;
				vint currentILIndex=-1;
				Ptr<ResourceStream> resource;
				ResourceRecord<BasicILEntryRes> entryRes;
				ResourceArrayRecord<BasicILLinkingRes> linkingsRes;
				ResourceArrayRecord<BasicILLinkingRes> foreignsRes;

				for(vint i=0;i<linkedFunctions.Count();i++)
				{
					LinkedFunctionInfo& info=linkedFunctions[i];
					if(currentIL!=info.originalIL)
					{
						currentIL=info.originalIL;
						currentILIndex=symbols.GetILIndex(currentIL);
						vint resourceIndex=info.originalIL->resources.Keys().IndexOf(BasicILResourceNames::ExportedSymbols);
						if(resourceIndex!=-1)
						{
							resource=info.originalIL->resources.Values()[resourceIndex];
							entryRes=resource->ReadRootRecord<BasicILEntryRes>();
							linkingsRes=resource->ReadArrayRecord(entryRes->linkings);
							foreignsRes=resource->ReadArrayRecord(entryRes->foreigns);
						}
					}
					for(vint j=0;j<info.count;j++)
					{
						BasicIns& srcIns=info.originalIL->instructions[j+info.originalOffset];
						BasicIns& dstIns=linkedIL->instructions[j+info.offset];
						dstIns.insKey=-1;
						dstIns.userData=0;

						switch(srcIns.opcode)
						{
						case BasicIns::pushins:
						case BasicIns::jump:
						case BasicIns::jumptrue:
						case BasicIns::jumpfalse:
						case BasicIns::call:
							{
								vint insIndex=srcIns.argument.int_value;
								vint labelMapIndex=assemblyLabelMap.Keys().IndexOf(Pair<vint, vint>(srcIns.insKey, insIndex));
								if(labelMapIndex==-1)
								{
									CHECK_ERROR(srcIns.insKey==currentILIndex, L"BasicILLinker::LinkInstructions()#内部错误。");
									CHECK_ERROR(info.originalOffset<=insIndex && insIndex<info.originalOffset+info.count, L"BasicILLinker::LinkInstructions()#内部错误。");
									dstIns.argument.int_value=insIndex-info.originalOffset+info.offset;
								}
								else
								{
									dstIns.argument.int_value=linkedIL->labels[assemblyLabelMap.Values()[labelMapIndex]].instructionIndex;
								}
							}
							break;
						case BasicIns::exception_handler_push:
							{
								vint insIndex=srcIns.argument.int_value;
								CHECK_ERROR(srcIns.insKey==currentILIndex, L"BasicILLinker::LinkInstructions()#内部错误。");
								CHECK_ERROR(info.originalOffset<=insIndex && insIndex<info.originalOffset+info.count, L"BasicILLinker::LinkInstructions()#内部错误。");
								dstIns.argument.int_value=insIndex-info.originalOffset+info.offset;
							}
							break;
						case BasicIns::pushlabel:
							{
								const BasicILLabel& label=symbols.GetLabel(srcIns.argument.int_value);
								vint labelIndex=assemblyLabelMap[Pair<vint, vint>(label.key, label.instruction)];
								dstIns.opcode=BasicIns::link_pushfunc;
								dstIns.argument.int_value=labelIndex;
							}
							break;
						case BasicIns::codegen_pushdata_siting:
							{
								dstIns.opcode=BasicIns::link_pushdata;
								dstIns.argument.int_value=assemblyGlobalData[symbols.GetGenericFunctionSitingIL()]+srcIns.argument.int_value;
							}
							break;
						case BasicIns::link_pushdata:
							{
								dstIns.argument.int_value=assemblyGlobalData[currentIL]+srcIns.argument.int_value;
							}
							break;
						case BasicIns::link_pushfunc:
							{
								BasicILLocalLabel& label=currentIL->labels[srcIns.argument.int_value];
								dstIns.argument.int_value=assemblyLabelMap[Pair<vint, vint>(currentILIndex, label.instructionIndex)];
							}
							break;
						case BasicIns::link_pushfardata:
							{
								ResourceRecord<BasicILLinkingRes> linkingRes=linkingsRes.Get(srcIns.argument.int_value);
								WString assemblyName=resource->ReadString(linkingRes->assemblyName);
								WString symbolName=resource->ReadString(linkingRes->symbolName);
								vint variableIndex=assemblyVariableMap[Pair<WString, WString>(assemblyName, symbolName)];
								LinkedVariableInfo& variable=linkedVariables[variableIndex];

								dstIns.opcode=BasicIns::link_pushdata;
								dstIns.argument.int_value=variable.offset;
							}
							break;
						case BasicIns::link_pushfarfunc:
							{
								ResourceRecord<BasicILLinkingRes> linkingRes=linkingsRes.Get(srcIns.argument.int_value);
								WString assemblyName=resource->ReadString(linkingRes->assemblyName);
								WString symbolName=resource->ReadString(linkingRes->symbolName);
								vint functionIndex=assemblyFunctionMap[Pair<WString, WString>(assemblyName, symbolName)];
								LinkedFunctionInfo& function=linkedFunctions[functionIndex];

								vint ilIndex=symbols.GetILIndex(function.originalIL);
								vint insIndex=function.originalOffset;
								dstIns.opcode=BasicIns::link_pushfunc;
								dstIns.argument.int_value=assemblyLabelMap[Pair<vint, vint>(ilIndex, insIndex)];
							}
							break;
						case BasicIns::link_callfarfunc:
							{
								ResourceRecord<BasicILLinkingRes> linkingRes=linkingsRes.Get(srcIns.argument.int_value);
								WString assemblyName=resource->ReadString(linkingRes->assemblyName);
								WString symbolName=resource->ReadString(linkingRes->symbolName);
								vint functionIndex=assemblyFunctionMap[Pair<WString, WString>(assemblyName, symbolName)];
								LinkedFunctionInfo& function=linkedFunctions[functionIndex];

								dstIns.opcode=BasicIns::call;
								dstIns.argument.int_value=function.offset;
							}
							break;
						case BasicIns::link_pushforeignfunc:
						case BasicIns::link_callforeignfunc:
							{
								ResourceRecord<BasicILLinkingRes> foreignRes=foreignsRes.Get(srcIns.argument.int_value);
								WString assemblyName=resource->ReadString(foreignRes->assemblyName);
								WString symbolName=resource->ReadString(foreignRes->symbolName);
								Pair<WString, WString> key(assemblyName, symbolName);
								vint foreignIndex=foreignMap.Keys().IndexOf(key);
								if(foreignIndex==-1)
								{
									vint index=foreignFunctions.Add(key);
									foreignMap.Add(key, index);
									dstIns.argument.int_value=index;
								}
								else
								{
									dstIns.argument.int_value=foreignMap.Values()[foreignIndex];
								}
							}
							break;
						}
					}
				}
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

				ResourceArrayRecord<BasicILLinkingRes> foreigns=resource->CreateArrayRecord<BasicILLinkingRes>(foreignFunctions.Count());
				entry->foreigns=foreigns;
				for(vint i=0;i<foreignFunctions.Count();i++)
				{
					ResourceRecord<BasicILLinkingRes> foreign=resource->CreateRecord<BasicILLinkingRes>();
					foreigns.Set(i, foreign);
					foreign->assemblyName=resource->CreateString(foreignFunctions[i].key);
					foreign->symbolName=resource->CreateString(foreignFunctions[i].value);
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
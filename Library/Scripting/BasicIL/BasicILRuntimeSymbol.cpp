#include "..\..\Collections\Pair.h"
#include "BasicILRuntimeSymbol.h"
#include "BasicILException.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
BasicILLabel
***********************************************************************/

			bool BasicILLabel::operator==(const BasicILLabel& label)const
			{
				return key==label.key && instruction==label.instruction;
			}

			bool BasicILLabel::operator!=(const BasicILLabel& label)const
			{
				return key!=label.key || instruction!=label.instruction;
			}

/***********************************************************************
BasicILLightFunctionInfo
***********************************************************************/

			bool BasicILLightFunctionInfo::operator==(const BasicILLightFunctionInfo& info)const
			{
				return function==info.function && argumentSize==info.argumentSize;
			}

			bool BasicILLightFunctionInfo::operator!=(const BasicILLightFunctionInfo& info)const
			{
				return function!=info.function || argumentSize!=info.argumentSize;
			}

/***********************************************************************
Helper Functions
***********************************************************************/

			Linear<vint, vint> TranslateLinear(Ptr<ResourceStream> exportedSymbols, ResourceRecord<BasicILGenericLinearRes> linearRecord)
			{
				Linear<vint, vint> value;
				value(linearRecord->constant);
				if(linearRecord->factors)
				{
					ResourceArrayRecord<BasicILGenericFactorItemRes> factors=exportedSymbols->ReadArrayRecord<BasicILGenericFactorItemRes>(linearRecord->factors);
					for(vint j=0;j<factors.Count();j++)
					{
						vint factor=factors.Get(j)->factor;
						value(j, factor);
					}
				}
				return value;
			}

/***********************************************************************
BasicILRuntimeSymbol
***********************************************************************/

			BasicILRuntimeSymbol::BasicILRuntimeSymbol()
			{
				BasicILLabel label;
				label.key=-1;
				label.instruction=-1;
				labels.Add(label);

				genericFunctionSitingIL=new BasicIL;
				ils.Add(genericFunctionSitingIL.Obj());
			}

			BasicILRuntimeSymbol::~BasicILRuntimeSymbol()
			{
			}

			BasicIL* BasicILRuntimeSymbol::GetGenericFunctionSitingIL()
			{
				return genericFunctionSitingIL.Obj();
			}
			
			BasicIL* BasicILRuntimeSymbol::GetIL(vint index)
			{
				return ils[index];
			}

			BasicIL* BasicILRuntimeSymbol::GetIL(const WString& name)
			{
				return ilMap[name];
			}

			vint BasicILRuntimeSymbol::GetILIndex(BasicIL* il)
			{
				return ils.IndexOf(il);
			}

			vint BasicILRuntimeSymbol::GetILIndex(const WString& name)
			{
				return ils.IndexOf(ilMap[name]);
			}

			vint BasicILRuntimeSymbol::GetILCount()
			{
				return ils.Count();
			}

			bool BasicILRuntimeSymbol::IsValidILIndex(vint index)
			{
				return index>=0 && index<ils.Count() && ils[index]!=0;
			}

			vint BasicILRuntimeSymbol::GetSymbol(const collections::Pair<WString, WString>& key)
			{
				return symbolMap[key];
			}

			const BasicILLabel& BasicILRuntimeSymbol::GetLabel(vint index)
			{
				return labels[index];
			}

			vint BasicILRuntimeSymbol::GetLabelIndex(const BasicILLabel& label)
			{
				return labels.IndexOf(label);
			}

			vint BasicILRuntimeSymbol::AddLabel(const BasicILLabel& label)
			{
				vint index=labels.Count();
				labels.Add(label);
				return index;
			}

			bool BasicILRuntimeSymbol::IsValidLabelIndex(vint index)
			{
				return index>=0 && index<labels.Count();
			}
			
			Ptr<BasicILGenericFunctionEntry> BasicILRuntimeSymbol::GetGenericFunctionEntry(const collections::Pair<WString, WString>& key)
			{
				vint index=genericFunctionEntries.Keys().IndexOf(key);
				return index==-1?0:genericFunctionEntries.Values()[index];
			}

			Ptr<BasicILGenericVariableEntry> BasicILRuntimeSymbol::GetGenericVariableEntry(const collections::Pair<WString, WString>& key)
			{
				vint index=genericVariableEntries.Keys().IndexOf(key);
				return index==-1?0:genericVariableEntries.Values()[index];
			}

			const collections::Pair<WString, WString>& BasicILRuntimeSymbol::GetGenericConcept(vint index)
			{
				return genericConcepts[index];
			}

			Ptr<BasicILGenericInstanceEntry> BasicILRuntimeSymbol::GetGenericInstance(const BasicILGenericInstanceEntry::Key& key)
			{
				vint index=genericInstances.Keys().IndexOf(key);
				return index==-1?0:genericInstances.Values()[index];
			}

/***********************************************************************
BasicILRuntimeSymbol::SymbolManagement
***********************************************************************/

			vint BasicILRuntimeSymbol::LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions)
			{
				vint exportedSymbolsIndex=il->resources.Keys().IndexOf(BasicILResourceNames::ExportedSymbols);
				if(exportedSymbolsIndex!=-1)
				{
					Ptr<ResourceStream> exportedSymbols=il->resources.Values()[exportedSymbolsIndex];
					ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
					WString assemblyName=exportedSymbols->ReadString(entry->assemblyName);
					if(ilMap.Keys().Contains(assemblyName))
					{
						throw ILLinkerException(ILLinkerException::DuplicatedAssemblyName, assemblyName, L"");
					}

					_SymbolMap currentSymbolMap;
					if(ResourceArrayHandle<BasicILExportRes> exportArrayHandle=entry->exports)
					{
						ResourceArrayRecord<BasicILExportRes> exportArray=exportedSymbols->ReadArrayRecord<BasicILExportRes>(exportArrayHandle);
						vint count=exportArray.Count();
						for(vint i=0;i<count;i++)
						{
							ResourceRecord<BasicILExportRes> currentExport=exportArray[i];
							Pair<WString, WString> symbol;
							symbol.key=assemblyName;
							symbol.value=exportedSymbols->ReadString(currentExport->name);

							if(currentSymbolMap.Keys().Contains(symbol))
							{
								throw ILLinkerException(ILLinkerException::DuplicatedSymbolName, assemblyName, symbol.value);
							}
							currentSymbolMap.Add(symbol, currentExport->address);
						}
					}

					if(ResourceArrayHandle<BasicILLinkingRes> linkingArrayHandle=entry->linkings)
					{
						ResourceArrayRecord<BasicILLinkingRes> linkingArray=exportedSymbols->ReadArrayRecord<BasicILLinkingRes>(linkingArrayHandle);
						vint count=linkingArray.Count();
						for(vint i=0;i<count;i++)
						{
							ResourceRecord<BasicILLinkingRes> currentLinking=linkingArray[i];
							Pair<WString, WString> symbol;
							symbol.key=exportedSymbols->ReadString(currentLinking->assemblyName);
							symbol.value=exportedSymbols->ReadString(currentLinking->symbolName);

							if(!ilMap.Keys().Contains(symbol.key))
							{
								throw ILLinkerException(ILLinkerException::AssemblyNotExists, symbol.key, symbol.value);
							}
							if(!symbolMap.Keys().Contains(symbol))
							{
								throw ILLinkerException(ILLinkerException::SymbolNotExists, symbol.key, symbol.value);
							}
							linkingSymbols.Add(symbol);
						}
					}

					if(ResourceArrayHandle<BasicILLinkingRes> foreignArrayHandle=entry->foreigns)
					{
						ResourceArrayRecord<BasicILLinkingRes> foreignArray=exportedSymbols->ReadArrayRecord<BasicILLinkingRes>(foreignArrayHandle);
						vint count=foreignArray.Count();
						for(vint i=0;i<count;i++)
						{
							ResourceRecord<BasicILLinkingRes> currentForeign=foreignArray[i];
							Pair<WString, WString> symbol;
							symbol.key=exportedSymbols->ReadString(currentForeign->assemblyName);
							symbol.value=exportedSymbols->ReadString(currentForeign->symbolName);

							if(!foreignFunctionLabelMap.Keys().Contains(symbol))
							{
								throw ILLinkerException(ILLinkerException::ForeignFunctionNotExists, symbol.key, symbol.value);
							}
							foreignFunctions.Add(symbol);
						}
					}

					BasicILGenericFunctionEntry::MapType	currentFunctionEntries;
					BasicILGenericVariableEntry::MapType	currentVariableEntries;
					_SymbolList								currentConcepts;
					BasicILGenericInstanceEntry::MapType	currentInstances;

					if(ResourceHandle<BasicILGenericRes> genericResHandle=entry->genericSymbols)
					{
						ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(genericResHandle);

						if(ResourceArrayRecord<BasicILGenericFunctionEntryRes> functionEntries=exportedSymbols->ReadArrayRecord<BasicILGenericFunctionEntryRes>(genericRes->functionEntries))
						{
							for(vint i=0;i<functionEntries.Count();i++)
							{
								ResourceRecord<BasicILGenericFunctionEntryRes> functionEntry=functionEntries.Get(i);

								Pair<WString, WString> symbol;
								symbol.key=assemblyName;
								symbol.value=exportedSymbols->ReadString(functionEntry->name);
								if(currentSymbolMap.Keys().Contains(symbol)
									|| currentFunctionEntries.Keys().Contains(symbol)
									|| currentVariableEntries.Keys().Contains(symbol)
									|| currentConcepts.Contains(symbol))
								{
									throw ILLinkerException(ILLinkerException::DuplicatedSymbolName, assemblyName, symbol.value);
								}

								Ptr<BasicILGenericFunctionEntry> genericFunctionEntry=new BasicILGenericFunctionEntry;
								genericFunctionEntry->key=-1;
								genericFunctionEntry->instruction=functionEntry->startInstruction;
								genericFunctionEntry->count=functionEntry->instructionCount;
								genericFunctionEntry->argumentCount=functionEntry->genericArgumentCount;
								genericFunctionEntry->uniqueEntryID=exportedSymbols->ReadString(functionEntry->uniqueEntryID);
								currentFunctionEntries.Add(symbol, genericFunctionEntry);
							}
						}

						if(ResourceArrayRecord<BasicILGenericVariableEntryRes> variableEntries=exportedSymbols->ReadArrayRecord<BasicILGenericVariableEntryRes>(genericRes->variableEntries))
						{
							for(vint i=0;i<variableEntries.Count();i++)
							{
								ResourceRecord<BasicILGenericVariableEntryRes> variableEntry=variableEntries.Get(i);

								Pair<WString, WString> symbol;
								symbol.key=assemblyName;
								symbol.value=exportedSymbols->ReadString(variableEntry->name);
								if(currentSymbolMap.Keys().Contains(symbol)
									|| currentFunctionEntries.Keys().Contains(symbol)
									|| currentVariableEntries.Keys().Contains(symbol)
									|| currentConcepts.Contains(symbol))
								{
									throw ILLinkerException(ILLinkerException::DuplicatedSymbolName, assemblyName, symbol.value);
								}

								Ptr<BasicILGenericVariableEntry> genericVariableEntry=new BasicILGenericVariableEntry;
								genericVariableEntry->argumentCount=variableEntry->genericArgumentCount;
								genericVariableEntry->size=TranslateLinear(exportedSymbols, exportedSymbols->ReadRecord<BasicILGenericLinearRes>(variableEntry->size));
								genericVariableEntry->uniqueEntryID=exportedSymbols->ReadString(variableEntry->uniqueEntryID);
								currentVariableEntries.Add(symbol, genericVariableEntry);
							}
						}

						if(ResourceArrayRecord<BasicILGenericTargetRes> targets=exportedSymbols->ReadArrayRecord<BasicILGenericTargetRes>(genericRes->targets))
						{
							for(vint i=0;i<targets.Count();i++)
							{
								ResourceRecord<BasicILGenericTargetRes> target=targets.Get(i);
								Pair<WString, WString> symbol;
								symbol.key=exportedSymbols->ReadString(target->assemblyName);
								symbol.value=exportedSymbols->ReadString(target->symbolName);

								if(assemblyName!=symbol.key && !ilMap.Keys().Contains(symbol.key))
								{
									throw ILLinkerException(ILLinkerException::AssemblyNotExists, symbol.key, symbol.value);
								}
								if(!genericFunctionEntries.Keys().Contains(symbol)
									&& !currentFunctionEntries.Keys().Contains(symbol)
									&& !genericVariableEntries.Keys().Contains(symbol)
									&& !currentVariableEntries.Keys().Contains(symbol))
								{
									throw ILLinkerException(ILLinkerException::SymbolNotExists, symbol.key, symbol.value);
								}
							}
						}

						if(ResourceArrayRecord<BasicILGenericConceptRes> concepts=exportedSymbols->ReadArrayRecord<BasicILGenericConceptRes>(genericRes->concepts))
						{
							for(vint i=0;i<concepts.Count();i++)
							{
								ResourceRecord<BasicILGenericConceptRes> concept=concepts.Get(i);

								Pair<WString, WString> symbol;
								symbol.key=assemblyName;
								symbol.value=exportedSymbols->ReadString(concept->name);
								if(currentSymbolMap.Keys().Contains(symbol)
									|| currentFunctionEntries.Keys().Contains(symbol)
									|| currentVariableEntries.Keys().Contains(symbol)
									|| currentConcepts.Contains(symbol))
								{
									throw ILLinkerException(ILLinkerException::DuplicatedSymbolName, assemblyName, symbol.value);
								}

								currentConcepts.Add(symbol);
							}
						}

						if(ResourceArrayRecord<BasicILGenericInstanceRes> instances=exportedSymbols->ReadArrayRecord<BasicILGenericInstanceRes>(genericRes->instances))
						{
							for(vint i=0;i<instances.Count();i++)
							{
								ResourceRecord<BasicILGenericInstanceRes> instance=instances.Get(i);
								BasicILGenericInstanceEntry::Key symbol;
								symbol.assemblyName=exportedSymbols->ReadString(instance->conceptAssemblyName);
								symbol.symbolName=exportedSymbols->ReadString(instance->conceptSymbolName);
								symbol.typeUniqueName=exportedSymbols->ReadString(instance->typeUniqueName);

								if(assemblyName!=symbol.assemblyName && !ilMap.Keys().Contains(symbol.assemblyName))
								{
									throw ILLinkerException(ILLinkerException::AssemblyNotExists, symbol.assemblyName, symbol.symbolName);
								}
								if(!currentConcepts.Contains(Pair<WString, WString>(symbol.assemblyName, symbol.symbolName))
									&& !genericConcepts.Contains(Pair<WString, WString>(symbol.assemblyName, symbol.symbolName)))
								{
									throw ILLinkerException(ILLinkerException::SymbolNotExists, symbol.assemblyName, symbol.symbolName);
								}
								if(currentInstances.Keys().Contains(symbol)
									|| genericInstances.Keys().Contains(symbol))
								{
									throw ILLinkerException(ILLinkerException::DuplicatedInstance, symbol.assemblyName+L"."+symbol.symbolName+L"<"+symbol.typeUniqueName+L">", L"");
								}

								Ptr<BasicILGenericInstanceEntry> genericInstance=new BasicILGenericInstanceEntry;
								genericInstance->argumentCount=instance->genericArgumentCount;
								genericInstance->instanceIndex=i;
								genericInstance->assemblyName=assemblyName;
								ResourceArrayRecord<BasicILGenericInstanceFunctionRes> functions=exportedSymbols->ReadArrayRecord<BasicILGenericInstanceFunctionRes>(instance->functions);
								for(vint j=0;j<functions.Count();j++)
								{
									ResourceRecord<BasicILGenericInstanceFunctionRes> function=functions.Get(j);
									genericInstance->functions.Add(exportedSymbols->ReadString(function->functionName), j);
								}
								currentInstances.Add(symbol, genericInstance);
							}
						}
					}

					ilMap.Add(assemblyName, il);
					CopyFrom(symbolMap.Wrap(), currentSymbolMap.Wrap(), true);

					vint index=ils.Count();
					for(vint i=0;i<currentFunctionEntries.Count();i++)
					{
						currentFunctionEntries.Values()[i]->key=index;
					}
					CopyFrom(genericFunctionEntries.Wrap(), currentFunctionEntries.Wrap(), true);
					CopyFrom(genericVariableEntries.Wrap(), currentVariableEntries.Wrap(), true);
					CopyFrom(genericConcepts.Wrap(), currentConcepts.Wrap(), true);
					CopyFrom(genericInstances.Wrap(), currentInstances.Wrap(), true);
				}
				return ils.Add(il);
			}

			void BasicILRuntimeSymbol::LinkILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions)
			{
				vint functionPointerOffset=labels.Count();
				vint index=ils.IndexOf(il);
				for(vint i=0;i<il->labels.Count();i++)
				{
					BasicILLabel label;
					label.key=index;
					label.instruction=il->labels[i].instructionIndex;
					labels.Add(label);
				}
				for(vint i=0;i<il->instructions.Count();i++)
				{
					BasicIns& ins=il->instructions[i];
					switch(ins.opcode)
					{
					case BasicIns::link_pushdata:
						{
							ins.opcode=BasicIns::push;
							ins.type1=BasicIns::pointer_type;
							ins.argument.pointer_value=&(il->globalData[0])+ins.argument.int_value;
						}
						break;
					case BasicIns::link_pushfunc:
						{
							ins.opcode=BasicIns::pushlabel;
							ins.argument.int_value+=functionPointerOffset;
						}
						break;
					case BasicIns::link_pushfardata:
						{
							Pair<WString, WString> symbol=linkingSymbols[ins.argument.int_value];
							vint ilIndex=ils.IndexOf(ilMap[symbol.key]);
							vint address=symbolMap[symbol];

							ins.opcode=BasicIns::push;
							ins.type1=BasicIns::pointer_type;
							ins.argument.pointer_value=&(ils[ilIndex]->globalData[0])+address;
						}
						break;
					case BasicIns::link_pushfarfunc:
						{
							Pair<WString, WString> symbol=linkingSymbols[ins.argument.int_value];
							BasicILLabel label;
							label.key=ils.IndexOf(ilMap[symbol.key]);
							label.instruction=symbolMap[symbol];

							vint labelIndex=labels.IndexOf(label);
							if(labelIndex!=-1)
							{
								ins.opcode=BasicIns::pushlabel;
								ins.argument.int_value=labelIndex;
							}
							else
							{
								throw ILLinkerException(ILLinkerException::SymbolNotALabel, symbol.key, symbol.value);
							}
						}
						break;
					case BasicIns::link_callfarfunc:
						{
							Pair<WString, WString> symbol=linkingSymbols[ins.argument.int_value];
							vint ilIndex=ils.IndexOf(ilMap[symbol.key]);
							vint address=symbolMap[symbol];

							ins.opcode=BasicIns::call;
							ins.insKey=ilIndex;
							ins.argument.int_value=address;
						}
						break;
					case BasicIns::link_pushforeignfunc:
						{
							Pair<WString, WString> symbol=foreignFunctions[ins.argument.int_value];
							vint labelIndex=foreignFunctionLabelMap[symbol];
							ins.opcode=BasicIns::pushlabel;
							ins.argument.int_value=labelIndex;
						}
						break;
					case BasicIns::link_callforeignfunc:
						{
							Pair<WString, WString> symbol=foreignFunctions[ins.argument.int_value];
							vint labelIndex=foreignFunctionLabelMap[symbol];
							BasicILLabel label=labels[labelIndex];

							ins.opcode=BasicIns::call;
							ins.insKey=label.key;
							ins.argument.int_value=label.instruction;
						}
						break;
					}
					if(ins.insKey==-1)
					{
						ins.insKey=index;
					}
				}
			}

			void BasicILRuntimeSymbol::LinkILFixInstructionKeyOnly(BasicIL* il)
			{
				vint functionPointerOffset=labels.Count();
				vint index=ils.IndexOf(il);
				for(vint i=0;i<il->labels.Count();i++)
				{
					BasicILLabel label;
					label.key=index;
					label.instruction=il->labels[i].instructionIndex;
					labels.Add(label);
				}
				for(vint i=0;i<il->instructions.Count();i++)
				{
					BasicIns& ins=il->instructions[i];
					switch(ins.opcode)
					{
					case BasicIns::link_pushdata:
					case BasicIns::link_pushfunc:
					case BasicIns::link_pushfardata:
					case BasicIns::link_pushfarfunc:
					case BasicIns::link_callfarfunc:
					case BasicIns::link_pushforeignfunc:
					case BasicIns::link_callforeignfunc:
						break;
					default:
						ins.insKey=index;
					}
				}
			}
			
			bool BasicILRuntimeSymbol::RegisterForeignFunction(const WString& category, const WString& name, Ptr<IBasicILForeignFunction> function)
			{
				Pair<WString, WString> symbol(category, name);
				if(foreignFunctionLabelMap.Keys().Contains(symbol))
				{
					return false;
				}
				else
				{
					BasicILLabel label;
					label.key=ForeignFunctionSitingAssemblyKey;
					label.instruction=foreignFunctionList.Count();
					foreignFunctionLabelMap.Add(symbol, labels.Count());

					foreignFunctionList.Add(function);
					labels.Add(label);
					return true;
				}
			}

			bool BasicILRuntimeSymbol::RegisterLightFunction(const WString& category, const WString& name, BasicILLightFunction function, vint argumentSize)
			{
				Pair<WString, WString> symbol(category, name);
				if(foreignFunctionLabelMap.Keys().Contains(symbol))
				{
					return false;
				}
				else
				{
					BasicILLabel label;
					label.key=LightFunctionSitingAssemblyKey;
					label.instruction=lightFunctionList.Count();
					foreignFunctionLabelMap.Add(symbol, labels.Count());

					BasicILLightFunctionInfo info;
					info.function=function;
					info.argumentSize=argumentSize;
					lightFunctionList.Add(info);
					labels.Add(label);
					return true;
				}
			}
		}
	}
}
#include "BasicILInterpretor.h"
#include "BasicILSymbolResource.h"
#include "..\Languages\BasicErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
BasicILEnv
***********************************************************************/

			BasicILEnv::BasicILEnv(vint _stackSize)
				:stackBase(_stackSize)
				,stackTop(_stackSize)
				,stackSize(_stackSize)
			{
				stack=new unsigned char[_stackSize];
			}

			BasicILEnv::~BasicILEnv()
			{
				delete[] stack;
			}

			vint BasicILEnv::StackBase()const
			{
				return stackBase;
			}

			vint BasicILEnv::StackSize()const
			{
				return stackSize;
			}

			vint BasicILEnv::StackTop()const
			{
				return stackTop;
			}

			void* BasicILEnv::DereferenceStack(vint stackPosition)const
			{
				if(stackPosition<0 || stackPosition>=stackSize)
				{
					throw ILException(BasicILStack::StackOverflow);
				}
				else
				{
					return &stack[stackPosition];
				}
			}

			void* BasicILEnv::Reserve(vint size)
			{
				stackTop-=size;
				if(stackTop<0 || stackTop>stackSize)
				{
					throw ILException(BasicILStack::StackOverflow);
				}
				else
				{
					return &stack[stackTop];
				}
			}

			void BasicILEnv::Reset()
			{
				stackBase=stackSize;
				stackTop=stackSize;
			}

			void BasicILEnv::SetBase(vint stackPosition)
			{
				stackBase=stackPosition;
			}

/***********************************************************************
BasicILGenericArgument
***********************************************************************/

			bool BasicILGenericArgument::operator==(const BasicILGenericArgument& value)const
			{
				return size==value.size && name==value.name;
			}

			bool BasicILGenericArgument::operator!=(const BasicILGenericArgument& value)const
			{
				return size!=value.size || name!=value.name;
			}

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
BasicILInterpretor
***********************************************************************/

			const vint BasicILInterpretor::GenericFunctionSitingAssemblyKey=0;

			BasicILInterpretor::VariablePackage::VariablePackage(vint size)
				:buffer(size)
				,remainBytes(size)
			{
			}

			char* BasicILInterpretor::VariablePackage::Allocate(vint size)
			{
				if(remainBytes>=size)
				{
					char* result=&buffer[0]+(buffer.Count()-remainBytes);
					remainBytes-=size;
					return result;
				}
				else
				{
					return 0;
				}
			}

			char* BasicILInterpretor::VariableManager::Allocate(const WString& name, vint size)
			{
				vint index=variables.Keys().IndexOf(name);
				if(index!=-1)
				{
					return variables.Values()[index];
				}
				else
				{
					char* result=0;
					for(vint i=0;i<packages.Count();i++)
					{
						if(result=packages[i]->Allocate(size))
						{
							break;
						}
					}
					if(!result)
					{
						vint minSize=size>65536?size:65536;
						VariablePackage* package=new VariablePackage(minSize);
						packages.Add(package);
						result=package->Allocate(size);
					}
					variables.Add(name, result);
					return result;
				}
			}

			vint CalculateLinear(Ptr<ResourceStream> exportedSymbols, ResourceRecord<BasicILGenericLinearRes> linearRecord, BasicILGenericArgumentEnvironment* environment)
			{
				vint value=linearRecord->constant;
				if(linearRecord->factors)
				{
					ResourceArrayRecord<BasicILGenericFactorItemRes> factors=exportedSymbols->ReadArrayRecord<BasicILGenericFactorItemRes>(linearRecord->factors);
					for(vint j=0;j<factors.Count();j++)
					{
						vint factor=factors.Get(j)->factor;
						value+=factor*environment->arguments[j]->size;
					}
				}
				return value;
			}

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

			void BasicILInterpretor::LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols)
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
				ils.Add(il);
			}

			void BasicILInterpretor::LinkILSymbol(BasicIL* il, vint index, _SymbolList& linkingSymbols)
			{
				vint functionPointerOffset=labels.Count();
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
					case BasicIns::link_pushforeigndata:
						{
							Pair<WString, WString> symbol=linkingSymbols[ins.argument.int_value];
							vint ilIndex=ils.IndexOf(ilMap[symbol.key]);
							vint address=symbolMap[symbol];

							ins.opcode=BasicIns::push;
							ins.type1=BasicIns::pointer_type;
							ins.argument.pointer_value=&(ils[ilIndex]->globalData[0])+address;
						}
						break;
					case BasicIns::link_pushforeignfunc:
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
					case BasicIns::link_callforeignfunc:
						{
							Pair<WString, WString> symbol=linkingSymbols[ins.argument.int_value];
							vint ilIndex=ils.IndexOf(ilMap[symbol.key]);
							vint address=symbolMap[symbol];

							ins.opcode=BasicIns::call;
							ins.insKey=ilIndex;
							ins.argument.int_value=address;
						}
						break;
					case BasicIns::generic_pushdata:
						{
							ins.insKey=index;
						}
						break;
					}
					if(ins.insKey==-1)
					{
						ins.insKey=index;
					}
				}
			}

			void TranslateTargetArguments(BasicILGenericArgumentEnvironment* environment, Ptr<ResourceStream> exportedSymbols, ResourceArrayRecord<BasicILGenericArgumentRes> arguments, Array<Ptr<BasicILGenericArgument>>& result)
			{
				result.Resize(arguments.Count());
				for(vint j=0;j<arguments.Count();j++)
				{
					ResourceRecord<BasicILGenericArgumentRes> argumentRecord=arguments.Get(j);
					Ptr<BasicILGenericArgument> newArgument=new BasicILGenericArgument;
					result[j]=newArgument;
					newArgument->size=CalculateLinear(exportedSymbols, exportedSymbols->ReadRecord(argumentRecord->sizeArgument), environment);

					ResourceRecord<BasicILGenericNameRes> nameRecord=exportedSymbols->ReadRecord(argumentRecord->nameArgument);
					if(nameRecord->isConstant)
					{
						newArgument->name=exportedSymbols->ReadString(nameRecord->constantString);
					}
					else
					{
						newArgument->name=environment->arguments[nameRecord->stringArgumentIndex]->name;
					}
					if(argumentRecord->subArgument)
					{
						TranslateTargetArguments(environment, exportedSymbols, exportedSymbols->ReadArrayRecord(argumentRecord->subArgument), newArgument->subArguments);
					}
				}
			}

			vint BasicILInterpretor::RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, ResourceHandle<BasicILGenericTargetRes> targetRecordHandle)
			{
				Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
				ResourceRecord<BasicILGenericTargetRes> targetRecord=exportedSymbols->ReadRecord(targetRecordHandle);
				Ptr<BasicILGenericTarget> newTarget=new BasicILGenericTarget;
				newTarget->assemblyName=exportedSymbols->ReadString(targetRecord->assemblyName);
				newTarget->symbolName=exportedSymbols->ReadString(targetRecord->symbolName);
				genericTargets.Add(newTarget);

				ResourceArrayRecord<BasicILGenericArgumentRes> arguments=exportedSymbols->ReadArrayRecord<BasicILGenericArgumentRes>(targetRecord->arguments);
				TranslateTargetArguments(environment, exportedSymbols, arguments, newTarget->arguments);

				return genericTargets.Count()-1;
			}

			vint BasicILInterpretor::RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex)
			{
				Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
				ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
				ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(entry->genericSymbols);
				ResourceArrayRecord<BasicILGenericTargetRes> targets=exportedSymbols->ReadArrayRecord<BasicILGenericTargetRes>(genericRes->targets);
				ResourceArrayRecord<BasicILGenericLinearRes> linears=exportedSymbols->ReadArrayRecord<BasicILGenericLinearRes>(genericRes->linears);
				
				return RegisterTarget(environment, il, targets.Get(targetIndex));
			}

			vint BasicILInterpretor::RegisterInstanceFunction(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex)
			{
				CHECK_ERROR(false, L"�ݲ�֧�֡�");
			}

			WString CalculateArgumentsName(Array<Ptr<BasicILGenericArgument>>& arguments)
			{
				if(arguments.Count())
				{
					WString uniqueName=L"<";
					for(vint i=0;i<arguments.Count();i++)
					{
						uniqueName+=arguments[i]->name+CalculateArgumentsName(arguments[i]->subArguments);
					}
					uniqueName+=L">";
					return uniqueName;
				}
				else
				{
					return L"";
				}
			}

			WString CalculateUniqueName(const WString uniqueEntryID, BasicILGenericTarget* target)
			{
				return uniqueEntryID+CalculateArgumentsName(target->arguments);
			}

			vint BasicILInterpretor::InstanciateGenericFunction(BasicILGenericTarget* target)
			{
				Pair<WString, WString> symbol;
				symbol.key=target->assemblyName;
				symbol.value=target->symbolName;
				BasicILGenericFunctionEntry* functionEntry=genericFunctionEntries[symbol].Obj();
				WString uniqueName=CalculateUniqueName(functionEntry->uniqueEntryID, target);

				vint instanciationIndex=instanciatedGenericFunctions.Keys().IndexOf(uniqueName);
				if(instanciationIndex!=-1)
				{
					return instanciatedGenericFunctions.Values()[instanciationIndex];
				}
				else
				{
					BasicILLabel label;
					label.key=BasicILInterpretor::GenericFunctionSitingAssemblyKey;
					label.instruction=genericFunctionSitingIL->instructions.Count();
					labels.Add(label);

					BasicIL* il=ils[functionEntry->key];
					Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
					ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(entry->genericSymbols);
					ResourceArrayRecord<BasicILGenericLinearRes> linears=exportedSymbols->ReadArrayRecord<BasicILGenericLinearRes>(genericRes->linears);

					List<BasicIns>& instructions=genericFunctionSitingIL->instructions;
					for(vint i=0;i<functionEntry->count;i++)
					{
						instructions.Add(il->instructions[functionEntry->instruction+i]);
						BasicIns& ins=instructions[instructions.Count()-1];
						switch(ins.opcode)
						{
						case BasicIns::generic_pushdata:
							{
								ins.opcode=BasicIns::push;
								ins.type1=BasicIns::pointer_type;

								vint index=RegisterTarget(target, il, ins.argument.int_value);
								BasicILGenericTarget* target=genericTargets[index].Obj();
								ins.argument.pointer_value=InstanciateGenericVariable(target);
							}
							break;
						case BasicIns::generic_callfunc:
							{
								ins.opcode=BasicIns::generic_callfunc_vm;
								ins.argument.int_value=RegisterTarget(target, il, ins.argument.int_value);
							}
							break;
						case BasicIns::generic_pushfunc:
							{
								ins.opcode=BasicIns::generic_pushfunc_vm;
								ins.argument.int_value=RegisterTarget(target, il, ins.argument.int_value);
							}
							break;
						case BasicIns::generic_instance_callfunc:
							{
								ins.opcode=BasicIns::generic_callfunc_vm;
								ins.argument.int_value=RegisterInstanceFunction(target, il, ins.argument.int_value);
							}
							break;
						case BasicIns::generic_instance_pushfunc:
							{
								ins.opcode=BasicIns::generic_pushfunc_vm;
								ins.argument.int_value=RegisterInstanceFunction(target, il, ins.argument.int_value);
							}
							break;
						case BasicIns::pushins:
						case BasicIns::jump:
						case BasicIns::jumpfalse:
						case BasicIns::jumptrue:
						case BasicIns::call:
							{
								if(ins.insKey==functionEntry->key
									&& ins.argument.int_value>=functionEntry->instruction
									&& ins.argument.int_value<functionEntry->instruction+functionEntry->count)
								{
									ins.insKey=BasicILInterpretor::GenericFunctionSitingAssemblyKey;
									ins.argument.int_value=label.instruction+(ins.argument.int_value-functionEntry->instruction);
								}
							}
							break;
						default:
							{
								if(ins.argumentType==BasicIns::linearArgument)
								{
									ResourceRecord<BasicILGenericLinearRes> linearRecord=linears.Get(ins.argument.int_value);
									ins.argumentType=BasicIns::constantArgument;
									ins.argument.int_value=CalculateLinear(exportedSymbols, linearRecord, target);
								}
							}
						}
					}

					vint result=labels.Count()-1;
					instanciatedGenericFunctions.Add(uniqueName, result);
					return result;
				}
			}

			char* BasicILInterpretor::InstanciateGenericVariable(BasicILGenericTarget* target)
			{
				Pair<WString, WString> symbol;
				symbol.key=target->assemblyName;
				symbol.value=target->symbolName;
				BasicILGenericVariableEntry* variableEntry=genericVariableEntries[symbol].Obj();
				WString uniqueName=CalculateUniqueName(variableEntry->uniqueEntryID, target);

				vint size=variableEntry->size.Constant();
				for(vint i=0;i<target->arguments.Count();i++)
				{
					size+=variableEntry->size.Factor(i)*target->arguments[i]->size;
				}

				return instanciatedGenericVariables.Allocate(uniqueName, size);
			}

			BasicILInterpretor::BasicILInterpretor(vint _stackSize)
				:stackSize(_stackSize)
			{
				BasicILLabel label;
				label.key=-1;
				label.instruction=-1;
				labels.Add(label);

				genericFunctionSitingIL=new BasicIL;
				ils.Add(genericFunctionSitingIL.Obj());
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
			}

			vint BasicILInterpretor::LoadIL(BasicIL* il)
			{
				_SymbolList linkingSymbols;
				LoadILSymbol(il, linkingSymbols);
				LinkILSymbol(il, ils.Count()-1, linkingSymbols);
				return ils.Count()-1;
			}

			void BasicILInterpretor::UnloadIL(BasicIL* il)
			{
				for(vint i=0;i<ils.Count();i++)
				{
					if(ils[i]==il)
					{
						ils[i]=0;
					}
				}
			}

			collections::IList<BasicILLabel>& BasicILInterpretor::GetLabels()
			{
				return labels.Wrap();
			}

/***********************************************************************
ILException
***********************************************************************/
			
			WString ILException::GetExceptionMessage(BasicILStack::RunningResult result)
			{
				switch(result)
				{
				case BasicILStack::StackOverflow:
					return basiclanguage::BasicErrorMessage::ILExceptionStackOverflow();
				case BasicILStack::DividByZero:
					return basiclanguage::BasicErrorMessage::ILExceptionDividByZero();
				case BasicILStack::AccessViolation:
					return basiclanguage::BasicErrorMessage::ILExceptionAccessViolation();
				case BasicILStack::InstructionIndexOutOfRange:
					return basiclanguage::BasicErrorMessage::ILExceptionInstructionIndexOutOfRange();
				case BasicILStack::UnknownInstruction:
					return basiclanguage::BasicErrorMessage::ILExceptionUnknownInstruction();
				case BasicILStack::BadInstructionArgument:
					return basiclanguage::BasicErrorMessage::ILExceptionBadInstructionArgument();
				default:
					return L"";
				}
			}

/***********************************************************************
ILLinkerException
***********************************************************************/
			
			WString ILLinkerException::GetExceptionMessage(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName)
			{
				switch(_errorType)
				{
				case DuplicatedAssemblyName:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedAssemblyName(_assemblyName);
				case AssemblyNotExists:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionAssemblyNotExists(_assemblyName);
				case DuplicatedSymbolName:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedSymbolName(_assemblyName, _symbolName);
				case SymbolNotExists:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionSymbolNotExists(_assemblyName, _symbolName);
				case SymbolNotALabel:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionSymbolNotALabel(_assemblyName, _symbolName);
				case DuplicatedInstance:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedInstance(_assemblyName);
				default:
					return L"";
				}
			}
		}
	}
}
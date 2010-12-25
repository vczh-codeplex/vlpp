#include "BasicILCodeExpander.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

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
Helper Functions
***********************************************************************/

			vint CalculateLinear(Ptr<ResourceStream> exportedSymbols, ResourceRecord<BasicILGenericLinearRes> linearRecord, BasicILGenericArgumentEnvironment* environment)
			{
				vint value=linearRecord->constant;
				if(linearRecord->factors)
				{
					ResourceArrayRecord<BasicILGenericFactorItemRes> factors=exportedSymbols->ReadArrayRecord<BasicILGenericFactorItemRes>(linearRecord->factors);
					for(vint j=0;j<factors.Count();j++)
					{
						vint factor=factors.Get(j)->factor;

						Array<Ptr<BasicILGenericArgument>>& arguments=environment->arguments;
						Ptr<BasicILGenericArgument> argument=arguments[j];
						vint size=argument->size;
						value+=factor*size;
					}
				}
				return value;
			}

			void TranslateTargetArguments(BasicILGenericArgumentEnvironment* environment, Ptr<ResourceStream> exportedSymbols, ResourceArrayRecord<BasicILGenericArgumentRes> arguments, Array<Ptr<BasicILGenericArgument>>& result);

			Ptr<BasicILGenericArgument> TranslateTargetArgument(BasicILGenericArgumentEnvironment* environment, Ptr<ResourceStream> exportedSymbols, ResourceHandle<BasicILGenericArgumentRes> argument)
			{
				ResourceRecord<BasicILGenericArgumentRes> argumentRecord=exportedSymbols->ReadRecord(argument);
				Ptr<BasicILGenericArgument> newArgument=new BasicILGenericArgument;
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

				return newArgument;
			}

			void TranslateTargetArguments(BasicILGenericArgumentEnvironment* environment, Ptr<ResourceStream> exportedSymbols, ResourceArrayRecord<BasicILGenericArgumentRes> arguments, Array<Ptr<BasicILGenericArgument>>& result)
			{
				result.Resize(arguments.Count());
				for(vint j=0;j<arguments.Count();j++)
				{
					ResourceRecord<BasicILGenericArgumentRes> argumentRecord=arguments.Get(j);
					Ptr<BasicILGenericArgument> newArgument=TranslateTargetArgument(environment, exportedSymbols, argumentRecord);
					result[j]=newArgument;
				}
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

/***********************************************************************
BasicILCodeExpander::VariablePackage
***********************************************************************/

			BasicILCodeExpander::VariablePackage::VariablePackage(vint size)
				:buffer(size)
				,remainBytes(size)
			{
			}

			char* BasicILCodeExpander::VariablePackage::Allocate(vint size)
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

			BasicILCodeExpander::VariableManager::VariableManager(bool _autoLink)
				:autoLink(_autoLink)
				,instanciatedGenericVariableSize(0)
			{
			}

			Pair<char*, vint> BasicILCodeExpander::VariableManager::Allocate(const WString& name, vint size)
			{
				vint index=variables.Keys().IndexOf(name);
				if(index!=-1)
				{
					return variables.Values()[index];
				}
				else
				{
					char* pointer=0;
					vint offset=instanciatedGenericVariableSize;
					instanciatedGenericVariableSize+=size;
					if(autoLink)
					{
						for(vint i=0;i<packages.Count();i++)
						{
							if(pointer=packages[i]->Allocate(size))
							{
								break;
							}
						}
						if(!pointer)
						{
							vint minSize=size>65536?size:65536;
							VariablePackage* package=new VariablePackage(minSize);
							packages.Add(package);
							pointer=package->Allocate(size);
						}
					}
					Pair<char*, vint> result(pointer, offset);
					variables.Add(name,result);
					return result;
				}
			}

/***********************************************************************
BasicILCodeExpander
***********************************************************************/

			vint BasicILCodeExpander::RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, ResourceHandle<BasicILGenericTargetRes> targetRecordHandle)
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

			vint BasicILCodeExpander::RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex)
			{
				Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
				ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
				ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(entry->genericSymbols);
				ResourceArrayRecord<BasicILGenericTargetRes> targets=exportedSymbols->ReadArrayRecord<BasicILGenericTargetRes>(genericRes->targets);
				ResourceArrayRecord<BasicILGenericLinearRes> linears=exportedSymbols->ReadArrayRecord<BasicILGenericLinearRes>(genericRes->linears);
				return RegisterTarget(environment, il, targets.Get(targetIndex));
			}

			BasicILGenericTarget* BasicILCodeExpander::GetTarget(vint index)
			{
				return genericTargets[index].Obj();
			}

			vint BasicILCodeExpander::RegisterInstanceFunction(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex, bool& isGenericFunction)
			{
				vint functionIndex=-1;
				vint instanceIndex=-1;
				BasicIL* instanceIL=0;
				Ptr<BasicILGenericArgument> type;
				{
					Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
					ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(entry->genericSymbols);
					ResourceArrayRecord<BasicILGenericInstanceTargetRes> instanceTargets=exportedSymbols->ReadArrayRecord<BasicILGenericInstanceTargetRes>(genericRes->instanceTargets);
					ResourceRecord<BasicILGenericInstanceTargetRes> instanceTarget=instanceTargets.Get(targetIndex);

					BasicILGenericInstanceEntry::Key key;
					type=TranslateTargetArgument(environment, exportedSymbols, instanceTarget->type);
					key.assemblyName=exportedSymbols->ReadString(instanceTarget->assemblyName);
					key.symbolName=exportedSymbols->ReadString(instanceTarget->symbolName);
					key.typeUniqueName=type->name;
					Ptr<BasicILGenericInstanceEntry> instance=symbols->GetGenericInstance(key);

					functionIndex=instance->functions[exportedSymbols->ReadString(instanceTarget->functionName)];
					instanceIndex=instance->instanceIndex;
					instanceIL=symbols->GetIL(instance->assemblyName);
				}
				{
					Ptr<ResourceStream> instanceSymbols=instanceIL->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> instanceEntryRes=instanceSymbols->ReadRootRecord<BasicILEntryRes>();
					ResourceRecord<BasicILGenericRes> instanceGenericRes=instanceSymbols->ReadRecord<BasicILGenericRes>(instanceEntryRes->genericSymbols);
					ResourceRecord<BasicILGenericInstanceRes> instanceRes=instanceSymbols->ReadArrayRecord<BasicILGenericInstanceRes>(instanceGenericRes->instances).Get(instanceIndex);
					ResourceRecord<BasicILGenericInstanceFunctionRes> instanceFunctionRes=instanceSymbols->ReadArrayRecord<BasicILGenericInstanceFunctionRes>(instanceRes->functions).Get(functionIndex);
					BasicILGenericArgumentEnvironment instanceEnvironment;
					CopyFrom(instanceEnvironment.arguments.Wrap(), type->subArguments.Wrap());

					if(instanceEnvironment.arguments.Count()==0)
					{
						isGenericFunction=false;
						ResourceRecord<BasicILGenericTargetRes> realTarget=instanceSymbols->ReadRecord<BasicILGenericTargetRes>(instanceFunctionRes->functionTarget);
						Pair<WString, WString> key;
						key.key=instanceSymbols->ReadString(realTarget->assemblyName);
						key.value=instanceSymbols->ReadString(realTarget->symbolName);

						BasicILLabel label;
						label.key=symbols->GetILIndex(key.key);
						label.instruction=symbols->GetSymbol(key);
						vint functionIndex=symbols->GetLabelIndex(label);
						return functionIndex;
					}
					else
					{
						isGenericFunction=true;
						return RegisterTarget(&instanceEnvironment, instanceIL, instanceFunctionRes->functionTarget);
					}
				}
			}

			void BasicILCodeExpander::RewriteInstanceFunctionInstruction(BasicILGenericArgumentEnvironment* environment, BasicIns& ins, BasicIL* originIL, BasicIns::OpCode genericOp, BasicIns::OpCode normalOp)
			{
				bool isGenericFunction=false;
				vint index=RegisterInstanceFunction(environment, originIL, ins.argument.int_value, isGenericFunction);
				if(isGenericFunction)
				{
					ins.opcode=genericOp;
					ins.argument.int_value=index;
				}
				else
				{
					ins.opcode=normalOp;
					const BasicILLabel& label=symbols->GetLabel(index);
					ins.insKey=label.key;
					ins.argument.int_value=label.instruction;
				}
			}

			vint BasicILCodeExpander::InstanciateGenericFunction(BasicILGenericTarget* target)
			{
				Pair<WString, WString> symbol;
				symbol.key=target->assemblyName;
				symbol.value=target->symbolName;
				BasicILGenericFunctionEntry* functionEntry=symbols->GetGenericFunctionEntry(symbol).Obj();
				WString uniqueName=CalculateUniqueName(functionEntry->uniqueEntryID, target);

				vint instanciationIndex=instanciatedGenericFunctions.Keys().IndexOf(uniqueName);
				if(instanciationIndex!=-1)
				{
					return instanciatedGenericFunctions.Values()[instanciationIndex];
				}
				else
				{
					BasicILLabel label;
					label.key=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey;
					label.instruction=symbols->GetGenericFunctionSitingIL()->instructions.Count();
					vint genericFunctionLabelIndex=symbols->AddLabel(label);

					BasicIL* il=symbols->GetIL(functionEntry->key);
					Ptr<ResourceStream> exportedSymbols=il->resources[BasicILResourceNames::ExportedSymbols];
					ResourceRecord<BasicILEntryRes> entry=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
					ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(entry->genericSymbols);
					ResourceArrayRecord<BasicILGenericLinearRes> linears=exportedSymbols->ReadArrayRecord<BasicILGenericLinearRes>(genericRes->linears);

					List<BasicIns>& instructions=symbols->GetGenericFunctionSitingIL()->instructions;
					for(vint i=0;i<functionEntry->count;i++)
					{
						instructions.Add(il->instructions[functionEntry->instruction+i]);
						BasicIns& ins=instructions[instructions.Count()-1];
						switch(ins.opcode)
						{
						case BasicIns::generic_pushdata:
							{
								if(instanciatedGenericVariables.autoLink)
								{
									ins.opcode=BasicIns::push;
									ins.type1=BasicIns::pointer_type;

									vint index=RegisterTarget(target, il, ins.argument.int_value);
									BasicILGenericTarget* target=genericTargets[index].Obj();
									ins.argument.pointer_value=InstanciateGenericVariable(target).key;
								}
								else
								{
									ins.opcode=BasicIns::codegen_pushdata_siting;
									ins.type1=BasicIns::int_type;
									
									vint index=RegisterTarget(target, il, ins.argument.int_value);
									BasicILGenericTarget* target=genericTargets[index].Obj();
									ins.argument.int_value=InstanciateGenericVariable(target).value;
								}
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
								RewriteInstanceFunctionInstruction(target, ins, il, BasicIns::generic_callfunc_vm, BasicIns::call);
							}
							break;
						case BasicIns::generic_instance_pushfunc:
							{
								RewriteInstanceFunctionInstruction(target, ins, il, BasicIns::generic_pushfunc_vm, BasicIns::pushins);
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
									ins.insKey=BasicILRuntimeSymbol::GenericFunctionSitingAssemblyKey;
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

					instanciatedGenericFunctions.Add(uniqueName, genericFunctionLabelIndex);
					return genericFunctionLabelIndex;
				}
			}

			Pair<char*, vint> BasicILCodeExpander::InstanciateGenericVariable(BasicILGenericTarget* target)
			{
				Pair<WString, WString> symbol;
				symbol.key=target->assemblyName;
				symbol.value=target->symbolName;
				BasicILGenericVariableEntry* variableEntry=symbols->GetGenericVariableEntry(symbol).Obj();
				WString uniqueName=CalculateUniqueName(variableEntry->uniqueEntryID, target);

				vint size=variableEntry->size.Constant();
				for(vint i=0;i<target->arguments.Count();i++)
				{
					size+=variableEntry->size.Factor(i)*target->arguments[i]->size;
				}

				return instanciatedGenericVariables.Allocate(uniqueName, size);
			}

			BasicILCodeExpander::BasicILCodeExpander(BasicILRuntimeSymbol* _symbols, bool _autoLink)
				:symbols(_symbols)
				,instanciatedGenericVariables(_autoLink)
			{
			}

			BasicILCodeExpander::~BasicILCodeExpander()
			{
			}

			void BasicILCodeExpander::RewriteExecutingGenericInstruction(BasicIns& ins, BasicIL* il, vint insIndex)
			{
				switch(ins.opcode)
				{
				case BasicIns::generic_pushdata:
					{
						if(instanciatedGenericVariables.autoLink)
						{
							ins.opcode=BasicIns::push;
							ins.type1=BasicIns::pointer_type;

							vint index=RegisterTarget(0, symbols->GetIL(ins.insKey), ins.argument.int_value);
							BasicILGenericTarget* target=GetTarget(index);
							ins.argument.pointer_value=InstanciateGenericVariable(target).key;
						}
						else
						{
							ins.opcode=BasicIns::codegen_pushdata_siting;
							ins.type1=BasicIns::int_type;

							vint index=RegisterTarget(0, symbols->GetIL(ins.insKey), ins.argument.int_value);
							BasicILGenericTarget* target=GetTarget(index);
							ins.argument.int_value=InstanciateGenericVariable(target).value;
						}
					}
					break;
				case BasicIns::generic_callfunc:
					{
						ins.opcode=BasicIns::generic_callfunc_vm;
						ins.argument.int_value=RegisterTarget(0, il, ins.argument.int_value);
					}
					break;
				case BasicIns::generic_instance_callfunc:
					{
						RewriteInstanceFunctionInstruction(0, ins, il, BasicIns::generic_callfunc_vm, BasicIns::call);
					}
					break;
				case BasicIns::generic_callfunc_vm:
					{
						BasicILGenericTarget* target=GetTarget(ins.argument.int_value);
						vint labelIndex=InstanciateGenericFunction(target);

						BasicIns& theIns=il->instructions[insIndex];
						const BasicILLabel& label=symbols->GetLabel(labelIndex);
						theIns.opcode=BasicIns::call;
						theIns.insKey=label.key;
						theIns.argument.int_value=label.instruction;
					}
					break;
				case BasicIns::generic_pushfunc:
					{
						ins.opcode=BasicIns::generic_pushfunc_vm;
						ins.argument.int_value=RegisterTarget(0, il, ins.argument.int_value);
					}
					break;
				case BasicIns::generic_instance_pushfunc:
					{
						RewriteInstanceFunctionInstruction(0, ins, il, BasicIns::generic_pushfunc_vm, BasicIns::pushins);
					}
					break;
				case BasicIns::generic_pushfunc_vm:
					{
						BasicILGenericTarget* target=GetTarget(ins.argument.int_value);
						vint labelIndex=InstanciateGenericFunction(target);

						BasicIns& theIns=il->instructions[insIndex];
						theIns.opcode=BasicIns::pushlabel;
						theIns.argument.int_value=labelIndex;
					}
					break;
				}
			}
		}
	}
}
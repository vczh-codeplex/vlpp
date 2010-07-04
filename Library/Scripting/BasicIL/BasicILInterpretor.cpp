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
BasicGenericName
***********************************************************************/

			BasicILGenericName::BasicILGenericName()
				:argumentIndex(-1)
			{
			}

			BasicILGenericName::BasicILGenericName(vint _argumentIndex)
				:argumentIndex(_argumentIndex)
			{
			}

			BasicILGenericName::BasicILGenericName(const WString& _name)
				:argumentIndex(-1)
				,name(_name)
			{
			}

			bool BasicILGenericName::operator==(const BasicILGenericName& value)const
			{
				return argumentIndex==value.argumentIndex && name==value.name;
			}

			bool BasicILGenericName::operator!=(const BasicILGenericName& value)const
			{
				return argumentIndex!=value.argumentIndex || name!=value.name;
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

					BasicILGenericFunctionEntry::MapType currentFunctionEntries;
					if(ResourceHandle<BasicILGenericRes> genericResHandle=entry->genericSymbols)
					{
						ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord<BasicILGenericRes>(genericResHandle);
						ResourceArrayRecord<BasicILGenericFunctionEntryRes> functionEntries=exportedSymbols->ReadArrayRecord<BasicILGenericFunctionEntryRes>(genericRes->functionEntries);

						for(vint i=0;i<functionEntries.Count();i++)
						{
							ResourceRecord<BasicILGenericFunctionEntryRes> functionEntry=functionEntries.Get(i);

							Pair<WString, WString> symbol;
							symbol.key=assemblyName;
							symbol.value=exportedSymbols->ReadString(functionEntry->name);
							if(currentSymbolMap.Keys().Contains(symbol) || currentFunctionEntries.Keys().Contains(symbol))
							{
								throw ILLinkerException(ILLinkerException::DuplicatedSymbolName, assemblyName, symbol.value);
							}

							Ptr<BasicILGenericFunctionEntry> genericFunctionEntry=new BasicILGenericFunctionEntry;
							genericFunctionEntry->key=-1;
							genericFunctionEntry->instruction=functionEntry->startInstruction;
							genericFunctionEntry->count=functionEntry->instructionCount;
							genericFunctionEntry->argumentCount=functionEntry->genericArgumentCount;
							ResourceArrayRecord<BasicILGenericNameRes> names=exportedSymbols->ReadArrayRecord<BasicILGenericNameRes>(functionEntry->uniqueNameTemplate);
							genericFunctionEntry->nameTemplate.Resize(names.Count());
							for(vint j=0;j<names.Count();j++)
							{
								ResourceRecord<BasicILGenericNameRes> name=names.Get(j);
								if(name->isConstant)
								{
									genericFunctionEntry->nameTemplate[j]=BasicILGenericName(exportedSymbols->ReadString(name->constantString));
								}
								else
								{
									genericFunctionEntry->nameTemplate[j]=BasicILGenericName(name->stringArgumentIndex);
								}
							}
							currentFunctionEntries.Add(symbol, genericFunctionEntry);
						}
					}

					ilMap.Add(assemblyName, il);
					CopyFrom(symbolMap.Wrap(), currentSymbolMap.Wrap(), true);

					vint index=ils.Count();
					for(vint i=0;i<currentFunctionEntries.Count();i++)
					{
						currentFunctionEntries.Values()[i]->key=index;
					}
					CopyFrom(genericFunctionEntries.Wrap(), currentFunctionEntries.Wrap());
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
					}
					if(ins.insKey==-1)
					{
						ins.insKey=index;
					}
				}
			}

			BasicILInterpretor::BasicILInterpretor(vint _stackSize)
				:stackSize(_stackSize)
			{
				BasicILLabel label;
				label.key=-1;
				label.instruction=-1;
				labels.Add(label);
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
				default:
					return L"";
				}
			}
		}
	}
}
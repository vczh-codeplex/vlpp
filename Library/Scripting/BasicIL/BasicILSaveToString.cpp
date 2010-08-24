#include <wchar.h>
#include "BasicILDefinition.h"
#include "BasicILInterpretor.h"
#include "BasicILSymbolResource.h"
#include "..\..\Stream\StreamSerialization.h"

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

			WString LinearToString(Linear<vint, vint> linear, vint count)
			{
				WString result;
				for(vint i=0;i<count;i++)
				{
					result+=itow(linear.Factor(i))+L"*T"+itow(i)+L" + ";
				}
				result+=itow(linear.Constant());
				return result;
			}

			void WriteGenericArgumentArray(stream::TextWriter& writer, const WString& propertyName, const WString& prefix, Array<Ptr<BasicILGenericArgument>>& arguments)
			{
				if(arguments.Count()>0)
				{
					writer.WriteLine(prefix+propertyName+L" = {");
					for(vint i=0;i<arguments.Count();i++)
					{
						writer.WriteLine(prefix+L"  Argument["+itow(i)+L"] = {");
						writer.WriteLine(prefix+L"    Name = "+arguments[i]->name);
						writer.WriteLine(prefix+L"    Size = "+itow(arguments[i]->size));
						WriteGenericArgumentArray(writer, L"Sub Arguments", prefix+L"    ", arguments[i]->subArguments);
						writer.WriteLine(prefix+L"  }");
					}
					writer.WriteLine(prefix+L"}");
				}
			}

			void BasicILInterpretor::LogInternalState(stream::TextWriter& writer)
			{
				for(vint i=0;i<ils.Count();i++)
				{
					writer.WriteLine(L"-----------------------------------------------");
					writer.WriteLine(L"Loaded Assemblies["+itow(i)+L"]");
					writer.WriteLine(L"-----------------------------------------------");
					ils[i]->SaveAsString(writer, 0);
					writer.WriteLine(L"");
				}

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Assembly Name Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<ilMap.Count();i++)
				{
					writer.WriteString(ilMap.Keys()[i]);
					writer.WriteString(L" = ");
					writer.WriteLine(itow(ils.IndexOf(ilMap.Values()[i])));
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Function Pointer Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<labels.Count();i++)
				{
					BasicILLabel& label=labels[i];
					writer.WriteLine(itow(i)+L" = Assemblies["+itow(label.key)+L"].Instructions["+itow(label.instruction)+L"]");
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Loaded Symbol Names");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<symbolMap.Count();i++)
				{
					writer.WriteLine(symbolMap.Keys()[i].key+L"."+symbolMap.Keys()[i].value);
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Generic Function Entry Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<genericFunctionEntries.Count();i++)
				{
					Pair<WString, WString> key=genericFunctionEntries.Keys()[i];
					BasicILGenericFunctionEntry* value=genericFunctionEntries.Values()[i].Obj();
					writer.WriteLine(key.key+L"."+key.value);
					writer.WriteLine(L"  Instruction = "+itow(value->instruction));
					writer.WriteLine(L"  Count = "+itow(value->count));
					writer.WriteLine(L"  Assembly = "+itow(value->key));
					writer.WriteLine(L"  Generic Argument Count = "+itow(value->argumentCount));
					writer.WriteLine(L"  Unique Entry ID = "+value->uniqueEntryID);
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Generic Variable Entry Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<genericVariableEntries.Count();i++)
				{
					Pair<WString, WString> key=genericVariableEntries.Keys()[i];
					BasicILGenericVariableEntry* value=genericVariableEntries.Values()[i].Obj();
					writer.WriteLine(key.key+L"."+key.value);
					writer.WriteLine(L"  Generic Argument Count = "+itow(value->argumentCount));
					writer.WriteLine(L"  Size = "+LinearToString(value->size, value->argumentCount));
					writer.WriteLine(L"  Unique Entry ID = "+value->uniqueEntryID);
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Instanciated Generic Function Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<instanciatedGenericFunctions.Count();i++)
				{
					WString key=instanciatedGenericFunctions.Keys()[i];
					vint value=instanciatedGenericFunctions.Values()[i];
					writer.WriteLine(key+L" = "+itow(value));
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Instanciated Generic Variable Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<instanciatedGenericVariables.variables.Count();i++)
				{
					WString key=instanciatedGenericVariables.variables.Keys()[i];
					char* value=instanciatedGenericVariables.variables.Values()[i];
					writer.WriteLine(key+L" = "+itow((vint)value));
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Cached Generic Target List");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<genericTargets.Count();i++)
				{
					BasicILGenericTarget* target=genericTargets[i].Obj();
					writer.WriteLine(L"Assembly Name = "+target->assemblyName);
					writer.WriteLine(L"Symbol Name = "+target->symbolName);
					WriteGenericArgumentArray(writer, L"Arguments", L"", target->arguments);
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Generic Concept Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<genericConcepts.Count();i++)
				{
					Pair<WString, WString> key=genericConcepts[i];
					BasicILGenericInstanceEntry* instance=genericInstances.Values()[i].Obj();
					writer.WriteLine(key.key+L"."+key.value);
				}
				writer.WriteLine(L"");

				writer.WriteLine(L"-----------------------------------------------");
				writer.WriteLine(L"Generic Instance Map");
				writer.WriteLine(L"-----------------------------------------------");
				for(vint i=0;i<genericInstances.Count();i++)
				{
					BasicILGenericInstanceEntry::Key key=genericInstances.Keys()[i];
					BasicILGenericInstanceEntry* instance=genericInstances.Values()[i].Obj();
					writer.WriteLine(key.assemblyName+L"."+key.symbolName+L"<"+key.typeUniqueName+L">");
					writer.WriteLine(L"Generic Argument Count = "+itow(instance->argumentCount));

					for(vint j=0;j<instance->functions.Count();j++)
					{
						WString key=instance->functions.Keys()[j];
						writer.WriteLine(L"Function "+key+L" =");
						BasicILGenericTarget* target=instance->functions.Values()[j].Obj();
						writer.WriteLine(L"  Assembly Name = "+target->assemblyName);
						writer.WriteLine(L"  Symbol Name = "+target->symbolName);
						WriteGenericArgumentArray(writer, L"Arguments", L"  ", target->arguments);
					}
				}
				writer.WriteLine(L"");
			}

/***********************************************************************
BasicIL
***********************************************************************/

			WString OpCodeToString(BasicIns::OpCode opCode)
			{
#define CASE(NAME)if(opCode==BasicIns::NAME)return L#NAME
				CASE(push);
				CASE(pushins);
				CASE(pushlabel);
				CASE(label);
				CASE(add);
				CASE(sub);
				CASE(mul);
				CASE(div);
				CASE(mod);
				CASE(shl);
				CASE(shr);
				CASE(neg);
				CASE(and);
				CASE(or);
				CASE(xor);
				CASE(not);
				CASE(eq);
				CASE(ne);
				CASE(lt);
				CASE(le);
				CASE(gt);
				CASE(ge);
				CASE(read);
				CASE(write);
				CASE(readmem);
				CASE(writemem);
				CASE(copymem);
				CASE(jump);
				CASE(jumptrue);
				CASE(jumpfalse);
				CASE(call);
				CASE(call_indirect);
				CASE(call_foreign);
				CASE(call_raw);
				CASE(convert);
				CASE(stack_offset);
				CASE(stack_top);
				CASE(stack_reserve);
				CASE(resptr);
				CASE(ret);
				CASE(link_pushdata);
				CASE(link_pushfunc);
				CASE(link_pushforeigndata);
				CASE(link_pushforeignfunc);
				CASE(link_callforeignfunc);
				CASE(generic_pushdata);
				CASE(generic_callfunc);
				CASE(generic_pushfunc);
				CASE(generic_callfunc_vm);
				CASE(generic_pushfunc_vm);
				CASE(generic_instance_pushfunc);
				CASE(generic_instance_callfunc);
				CASE(codegen_callfunc);
				return L"<UNKNOWN-OPCODE>";
#undef CASE
			}

			WString ValueTypeToString(BasicIns::ValueType type)
			{
#define CASE(NAME)if(type==BasicIns::NAME)return L#NAME
				CASE(s8);
				CASE(s16);
				CASE(s32);
				CASE(s64);
				CASE(u8);
				CASE(u16);
				CASE(u32);
				CASE(u64);
				CASE(f32);
				CASE(f64);
				return L"<UNKNOWN-TYPE>";
#undef CASE
			}

			WString ValueToString(BasicIns::ValueType type, BasicIns::ArgumentType argumentType, BasicIns::Argument argument)
			{
				if(argumentType==BasicIns::linearArgument)
				{
					return itow(argument.int_value)+L"[Linear]";
				}
				else
				{
					switch(type)
					{
					case BasicIns::s8:	return itow(argument.s8);
					case BasicIns::s16:	return itow(argument.s16);
					case BasicIns::s32:	return itow(argument.s32);
					case BasicIns::s64:	return i64tow(argument.s64);
					case BasicIns::u8:	return utow(argument.u8);
					case BasicIns::u16:	return utow(argument.u16);
					case BasicIns::u32:	return utow(argument.u32);
					case BasicIns::u64:	return u64tow(argument.u64);
					case BasicIns::f32:	return ftow(argument.f32);
					case BasicIns::f64:	return ftow(argument.f64);
					default:
						return L"<UNKNOWN-VALUE>";
					}
				}
			}

			WString BasicInsToStringSingle(BasicIns ins)
			{
				return OpCodeToString(ins.opcode);
			}

			WString BasicInsToStringType(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueTypeToString(ins.type1);
			}

			WString BasicInsToStringType2(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueTypeToString(ins.type1)+L" "+ValueTypeToString(ins.type2);
			}

			WString BasicInsToStringTypeArgument(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueTypeToString(ins.type1)+L" "+ValueToString(ins.type1, ins.argumentType, ins.argument);
			}

			WString BasicInsToStringConstant(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueToString(BasicIns::int_type, ins.argumentType, ins.argument);
			}

			WString BasicInsToStringJump(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueToString(BasicIns::int_type, ins.argumentType, ins.argument)+L" "+itow(ins.insKey);
			}

			WString BasicInsToString(BasicIns ins)
			{
#define CASE(NAME,FUNCTION)if(ins.opcode==BasicIns::NAME)return BasicInsToString##FUNCTION(ins)
				CASE(push,							TypeArgument);
				CASE(pushins,						Jump);
				CASE(pushlabel,						Constant);
				CASE(label,							Single);
				CASE(add,							Type);
				CASE(sub,							Type);
				CASE(mul,							Type);
				CASE(div,							Type);
				CASE(mod,							Type);
				CASE(shl,							Type);
				CASE(shr,							Type);
				CASE(neg,							Type);
				CASE(and,							Type);
				CASE(or,							Type);
				CASE(xor,							Type);
				CASE(not,							Type);
				CASE(eq,							Type);
				CASE(ne,							Type);
				CASE(lt,							Type);
				CASE(le,							Type);
				CASE(gt,							Type);
				CASE(ge,							Type);
				CASE(read,							Type);
				CASE(write,							Type);
				CASE(readmem,						Constant);
				CASE(writemem,						Constant);
				CASE(copymem,						Constant);
				CASE(jump,							Jump);
				CASE(jumptrue,						Jump);
				CASE(jumpfalse,						Jump);
				CASE(call,							Jump);
				CASE(call_indirect,					Single);
				CASE(call_foreign,					Constant);
				CASE(call_raw,						Constant);
				CASE(convert,						Type2);
				CASE(stack_offset,					Constant);
				CASE(stack_top,						Constant);
				CASE(stack_reserve,					Constant);
				CASE(resptr,						Single);
				CASE(ret,							Constant);
				CASE(link_pushdata,					Constant);
				CASE(link_pushfunc,					Constant);
				CASE(link_pushforeigndata,			Constant);
				CASE(link_pushforeignfunc,			Constant);
				CASE(link_callforeignfunc,			Constant);
				CASE(generic_pushdata,				Constant);
				CASE(generic_callfunc,				Constant);
				CASE(generic_pushfunc,				Constant);
				CASE(generic_callfunc_vm,			Constant);
				CASE(generic_pushfunc_vm,			Constant);
				CASE(generic_instance_pushfunc,	Constant);
				CASE(generic_instance_callfunc,	Constant);
				CASE(codegen_callfunc,				Constant);
				return L"<UNKNOWN-OPCODE>";
#undef CASE
			}

			void WriteInteger(vint integer, stream::TextWriter& writer)
			{
				WString intstr=itow(integer);
				vint spaces=6-intstr.Length();
				while(spaces--)
				{
					writer.WriteString(L" ");
				}
				writer.WriteString(intstr);
			}

			void WriteData(Array<char>& data, stream::TextWriter& writer)
			{
				const vint lineBytes=16;
				const wchar_t* hex=L"0123456789ABCDEF";

				vint lines=data.Count()/lineBytes;
				if(data.Count()%lineBytes)
				{
					lines+=1;
				}

				for(vint i=0;i<lines;i++)
				{
					vint start=i*lineBytes;
					{
						wchar_t buffer[100];
						ITOW_S(start, buffer, sizeof(buffer)/sizeof*(buffer), 16);
						size_t length=wcslen(buffer);
						vint count=8-length;
						writer.WriteString(L"0x");
						while(count--)
						{
							writer.WriteString(L"0");
						}
						writer.WriteString(buffer);
						writer.WriteString(L": ");
					}
					for(vint j=start;j<data.Count()&&j-start<lineBytes;j++)
					{
						unsigned char c=(unsigned char)data[j];
						writer.WriteChar(hex[c/16]);
						writer.WriteChar(hex[c%16]);
						writer.WriteString(L" ");
					}
					writer.WriteLine(L"");
				}
			}

			WString LinearToString(Ptr<ResourceStream> exportedSymbols, ResourceRecord<BasicILGenericLinearRes> linearRes)
			{
				WString result;
				if(ResourceArrayRecord<BasicILGenericFactorItemRes> factorsRes=exportedSymbols->ReadArrayRecord(linearRes->factors))
				{
					for(vint i=0;i<factorsRes.Count();i++)
					{
						result+=itow(factorsRes.Get(i)->factor)+L"*T"+itow(i)+L" + ";
					}
				}
				result+=itow(linearRes->constant);
				return result;
			}

			void WriteNameToString(Ptr<ResourceStream> exportedSymbols, ResourceRecord<BasicILGenericArgumentRes> argumentRes, const WString& prefix, stream::TextWriter& writer)
			{
				ResourceRecord<BasicILGenericNameRes> nameRes=exportedSymbols->ReadRecord(argumentRes->nameArgument);
				if(nameRes->isConstant)
				{
					writer.WriteString(exportedSymbols->ReadString(nameRes->constantString));
				}
				else
				{
					writer.WriteString(L"{"+itow(nameRes->stringArgumentIndex)+L"}");
				}
				writer.WriteString(L" : ");
				writer.WriteString(LinearToString(exportedSymbols, exportedSymbols->ReadRecord(argumentRes->sizeArgument)));

				ResourceArrayRecord<BasicILGenericArgumentRes> arguments=exportedSymbols->ReadArrayRecord(argumentRes->subArgument);
				if(arguments && arguments.Count()>0)
				{
					writer.WriteLine(L" {");
					for(vint i=0;i<arguments.Count();i++)
					{
						writer.WriteString(prefix+L"  ");
						WriteNameToString(exportedSymbols, arguments.Get(i), prefix+L"  ", writer);
					}
					writer.WriteLine(prefix+L"}");
				}
				else
				{
					writer.WriteLine(L"");
				}
			}

			void WriteExportSymbol(Ptr<ResourceStream> exportedSymbols, stream::TextWriter& writer)
			{
				ResourceRecord<BasicILEntryRes> entryRes=exportedSymbols->ReadRootRecord<BasicILEntryRes>();
				writer.WriteLine(L"Assembly Name: "+exportedSymbols->ReadString(entryRes->assemblyName));

				if(ResourceArrayRecord<BasicILExportRes> exportsRes=exportedSymbols->ReadArrayRecord(entryRes->exports))
				{
					for(vint i=0;i<exportsRes.Count();i++)
					{
						ResourceRecord<BasicILExportRes> exportRes=exportsRes.Get(i);
						writer.WriteLine(L"Exports["+itow(i)+L"] = ("+itow(exportRes->address)+L", "+exportedSymbols->ReadString(exportRes->name)+L")");
					}
				}

				if(ResourceArrayRecord<BasicILLinkingRes> linkingsRes=exportedSymbols->ReadArrayRecord(entryRes->linkings))
				{
					for(vint i=0;i<linkingsRes.Count();i++)
					{
						ResourceRecord<BasicILLinkingRes> linkingRes=linkingsRes.Get(i);
						writer.WriteLine(L"Linkings["+itow(i)+L"] = "+exportedSymbols->ReadString(linkingRes->assemblyName)+L"::"+exportedSymbols->ReadString(linkingRes->symbolName));
					}
				}

				if(ResourceRecord<BasicILGenericRes> genericRes=exportedSymbols->ReadRecord(entryRes->genericSymbols))
				{
					if(ResourceArrayRecord<BasicILGenericFunctionEntryRes> entriesRes=exportedSymbols->ReadArrayRecord(genericRes->functionEntries))
					{
						for(vint i=0;i<entriesRes.Count();i++)
						{
							ResourceRecord<BasicILGenericFunctionEntryRes> entryRes=entriesRes.Get(i);
							writer.WriteLine(L"Function Entries["+itow(i)+L"] = {");
							writer.WriteLine(L"  Name = "+exportedSymbols->ReadString(entryRes->name));
							writer.WriteLine(L"  Arguments = "+itow(entryRes->genericArgumentCount));
							writer.WriteLine(L"  Instruction = "+itow(entryRes->startInstruction));
							writer.WriteLine(L"  Length = "+itow(entryRes->instructionCount));
							writer.WriteLine(L"  UniqueName = "+exportedSymbols->ReadString(entryRes->uniqueEntryID));
							writer.WriteLine(L"}");
						}
					}

					if(ResourceArrayRecord<BasicILGenericVariableEntryRes> entriesRes=exportedSymbols->ReadArrayRecord(genericRes->variableEntries))
					{
						for(vint i=0;i<entriesRes.Count();i++)
						{
							ResourceRecord<BasicILGenericVariableEntryRes> entryRes=entriesRes.Get(i);
							writer.WriteLine(L"Variable Entries["+itow(i)+L"] = {");
							writer.WriteLine(L"  Name = "+exportedSymbols->ReadString(entryRes->name));
							writer.WriteLine(L"  Arguments = "+itow(entryRes->genericArgumentCount));
							writer.WriteLine(L"  Size = "+LinearToString(exportedSymbols, exportedSymbols->ReadRecord(entryRes->size)));
							writer.WriteLine(L"  UniqueName = "+exportedSymbols->ReadString(entryRes->uniqueEntryID));
							writer.WriteLine(L"}");
						}
					}
					
					if(ResourceArrayRecord<BasicILGenericTargetRes> targetsRes=exportedSymbols->ReadArrayRecord(genericRes->targets))
					{
						for(vint i=0;i<targetsRes.Count();i++)
						{
							ResourceRecord<BasicILGenericTargetRes> targetRes=targetsRes.Get(i);
							writer.WriteLine(L"Targets["+itow(i)+L"] = {");
							writer.WriteLine(L"  AssemblyName = "+exportedSymbols->ReadString(targetRes->assemblyName));
							writer.WriteLine(L"  SymbolName = "+exportedSymbols->ReadString(targetRes->symbolName));
							ResourceArrayRecord<BasicILGenericArgumentRes> argumentsRes=exportedSymbols->ReadArrayRecord(targetRes->arguments);
							for(vint j=0;j<argumentsRes.Count();j++)
							{
								ResourceRecord<BasicILGenericArgumentRes> argumentRes=argumentsRes.Get(j);
								writer.WriteString(L"  Argument["+itow(j)+L"] = ");
								WriteNameToString(exportedSymbols, argumentRes, L"  ", writer);
							}
							writer.WriteLine(L"}");
						}
					}
					
					if(ResourceArrayRecord<BasicILGenericLinearRes> linearsRes=exportedSymbols->ReadArrayRecord(genericRes->linears))
					{
						for(vint i=0;i<linearsRes.Count();i++)
						{
							ResourceRecord<BasicILGenericLinearRes> linearRes=linearsRes.Get(i);
							writer.WriteLine(L"Linears["+itow(i)+L"] = "+LinearToString(exportedSymbols, linearRes));
						}
					}
					
					if(ResourceArrayRecord<BasicILGenericConceptRes> conceptsRes=exportedSymbols->ReadArrayRecord(genericRes->concepts))
					{
						for(vint i=0;i<conceptsRes.Count();i++)
						{
							ResourceRecord<BasicILGenericConceptRes> conceptRes=conceptsRes.Get(i);
							writer.WriteLine(L"Concepts["+itow(i)+L"] = {");
							writer.WriteLine(L"  Name = "+exportedSymbols->ReadString(conceptRes->name));
							if(ResourceArrayRecord<BasicILGenericConceptFunctionRes> functionsRes=exportedSymbols->ReadArrayRecord(conceptRes->functions))
							{
								for(vint j=0;j<functionsRes.Count();j++)
								{
									writer.WriteLine(L"  Functions["+itow(j)+L"] = "+exportedSymbols->ReadString(functionsRes.Get(j)->name));
								}
							}
							writer.WriteLine(L"}");
						}
					}
					
					if(ResourceArrayRecord<BasicILGenericInstanceRes> instancesRes=exportedSymbols->ReadArrayRecord(genericRes->instances))
					{
						for(vint i=0;i<instancesRes.Count();i++)
						{
							ResourceRecord<BasicILGenericInstanceRes> instanceRes=instancesRes.Get(i);
							writer.WriteLine(L"Instances["+itow(i)+L"] = {");
							writer.WriteLine(L"  ConcpetAssemblyName = "+exportedSymbols->ReadString(instanceRes->conceptAssemblyName));
							writer.WriteLine(L"  ConceptSymbolName = "+exportedSymbols->ReadString(instanceRes->conceptSymbolName));
							writer.WriteLine(L"  TypeUniqueName = "+exportedSymbols->ReadString(instanceRes->typeUniqueName));
							writer.WriteLine(L"  Arguments = "+itow(instanceRes->genericArgumentCount));
							if(ResourceArrayRecord<BasicILGenericInstanceFunctionRes> functionsRes=exportedSymbols->ReadArrayRecord(instanceRes->functions))
							{
								for(vint j=0;j<functionsRes.Count();j++)
								{
									ResourceRecord<BasicILGenericInstanceFunctionRes> functionRes=functionsRes.Get(j);
									ResourceRecord<BasicILGenericTargetRes> targetRes=exportedSymbols->ReadRecord(functionRes->functionTarget);
									writer.WriteLine(L"  Functions["+itow(j)+L"] = {");
									writer.WriteLine(L"    FunctionName = "+exportedSymbols->ReadString(functionRes->functionName));
									writer.WriteLine(L"    AssemblyName = "+exportedSymbols->ReadString(targetRes->assemblyName));
									writer.WriteLine(L"    SymbolName = "+exportedSymbols->ReadString(targetRes->symbolName));
									ResourceArrayRecord<BasicILGenericArgumentRes> argumentsRes=exportedSymbols->ReadArrayRecord(targetRes->arguments);
									for(vint j=0;j<argumentsRes.Count();j++)
									{
										ResourceRecord<BasicILGenericArgumentRes> argumentRes=argumentsRes.Get(j);
										writer.WriteString(L"    Argument["+itow(j)+L"] = ");
										WriteNameToString(exportedSymbols, argumentRes, L"    ", writer);
									}
									writer.WriteLine(L"  }");
								}
							}
							writer.WriteLine(L"}");
						}
					}
					
					if(ResourceArrayRecord<BasicILGenericInstanceTargetRes> instanceTargetsRes=exportedSymbols->ReadArrayRecord(genericRes->instanceTargets))
					{
						for(vint i=0;i<instanceTargetsRes.Count();i++)
						{
							ResourceRecord<BasicILGenericInstanceTargetRes> instanceTargetRes=instanceTargetsRes.Get(i);
							writer.WriteLine(L"Instance Targets["+itow(i)+L"] = {");
							writer.WriteLine(L"  AssemblyName = "+exportedSymbols->ReadString(instanceTargetRes->assemblyName));
							writer.WriteLine(L"  SymbolName = "+exportedSymbols->ReadString(instanceTargetRes->symbolName));
							writer.WriteLine(L"  FunctionName = "+exportedSymbols->ReadString(instanceTargetRes->functionName));
							ResourceRecord<BasicILGenericArgumentRes> argumentRes=exportedSymbols->ReadRecord(instanceTargetRes->type);
							writer.WriteString(L"  Argument = ");
							WriteNameToString(exportedSymbols, argumentRes, L"  ", writer);
							writer.WriteLine(L"}");
						}
					}
				}
			}

			void BasicIL::SaveAsString(stream::TextWriter& writer, ICommentProvider* commentProvider)
			{
				writer.WriteLine(L".data");
				WriteData(globalData, writer);

				writer.WriteLine(L".label");
				for(vint i=0;i<labels.Count();i++)
				{
					WriteInteger(i, writer);
					writer.WriteString(L": ");
					writer.WriteLine(L"instruction "+itow(labels[i].instructionIndex));
				}

				writer.WriteLine(L".code");
				if(commentProvider)
				{
					commentProvider->StartProvideComment();
				}
				for(vint i=0;i<instructions.Count();i++)
				{
					if(commentProvider)
					{
						commentProvider->AppendComment(writer, instructions[i].userData);
					}
					WriteInteger(i, writer);
					writer.WriteString(L": ");
					writer.WriteLine(BasicInsToString(instructions[i]));
				}

				vint exportedSymbolsIndex=resources.Keys().IndexOf(BasicILResourceNames::ExportedSymbols);
				if(exportedSymbolsIndex!=-1)
				{
					writer.WriteLine(L".exports");
					Ptr<ResourceStream> exportedSymbols=resources.Values()[exportedSymbolsIndex];
					if(exportedSymbols)
					{
						WriteExportSymbol(exportedSymbols, writer);
					}
				}
			}
		}
	}
}
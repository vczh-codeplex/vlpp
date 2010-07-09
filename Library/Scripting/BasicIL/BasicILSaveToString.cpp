#include <wchar.h>
#include "BasicILDefinition.h"
#include "..\..\Stream\StreamSerialization.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
BasicIns
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

			WString ValueToString(BasicIns::ValueType type, BasicIns::Argument argument)
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
				return OpCodeToString(ins.opcode)+L" "+ValueTypeToString(ins.type1)+L" "+ValueToString(ins.type1, ins.argument);
			}

			WString BasicInsToStringConstant(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueToString(BasicIns::int_type, ins.argument);
			}

			WString BasicInsToStringJump(BasicIns ins)
			{
				return OpCodeToString(ins.opcode)+L" "+ValueToString(BasicIns::int_type, ins.argument)+L" "+itow(ins.insKey);
			}

			WString BasicInsToString(BasicIns ins)
			{
#define CASE(NAME,FUNCTION)if(ins.opcode==BasicIns::NAME)return BasicInsToString##FUNCTION(ins)
				CASE(push,					TypeArgument);
				CASE(pushins,				Jump);
				CASE(pushlabel,				Constant);
				CASE(label,					Single);
				CASE(add,					Type);
				CASE(sub,					Type);
				CASE(mul,					Type);
				CASE(div,					Type);
				CASE(mod,					Type);
				CASE(shl,					Type);
				CASE(shr,					Type);
				CASE(neg,					Type);
				CASE(and,					Type);
				CASE(or,					Type);
				CASE(xor,					Type);
				CASE(not,					Type);
				CASE(eq,					Type);
				CASE(ne,					Type);
				CASE(lt,					Type);
				CASE(le,					Type);
				CASE(gt,					Type);
				CASE(ge,					Type);
				CASE(read,					Type);
				CASE(write,					Type);
				CASE(readmem,				Constant);
				CASE(writemem,				Constant);
				CASE(copymem,				Constant);
				CASE(jump,					Jump);
				CASE(jumptrue,				Jump);
				CASE(jumpfalse,				Jump);
				CASE(call,					Jump);
				CASE(call_indirect,			Single);
				CASE(call_foreign,			Constant);
				CASE(call_raw,				Constant);
				CASE(convert,				Type2);
				CASE(stack_offset,			Constant);
				CASE(stack_top,				Constant);
				CASE(stack_reserve,			Constant);
				CASE(resptr,				Single);
				CASE(ret,					Constant);
				CASE(link_pushdata,			Constant);
				CASE(link_pushfunc,			Constant);
				CASE(link_pushforeigndata,	Constant);
				CASE(link_pushforeignfunc,	Constant);
				CASE(link_callforeignfunc,	Constant);
				CASE(generic_callfunc,		Constant);
				CASE(generic_pushfunc,		Constant);
				CASE(generic_callfunc_vm,	Constant);
				CASE(generic_pushfunc_vm,	Constant);
				CASE(codegen_callfunc,		Constant);
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
			}
		}
	}
}
#include <wchar.h>
#include "BasicILDefinition.h"

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

			BasicIns::BasicIns()
			{
				opcode=(OpCode)0;
				type1=(ValueType)0;
				type2=(ValueType)0;
				argument.s64=0;
				insKey=-1;
				userData=0;
			}

			bool BasicIns::operator==(const BasicIns& ins)const
			{
				return opcode==ins.opcode && type1==ins.type1 && type2==ins.type2 && argument.s64==ins.argument.s64 && insKey==ins.insKey;
			}

			bool BasicIns::operator!=(const BasicIns& ins)const
			{
				return opcode!=ins.opcode || type1!=ins.type1 || type2!=ins.type2 || argument.s64!=ins.argument.s64 || insKey!=ins.insKey;
			}

			BasicIns::Argument BasicIns::Makes8(signed __int8 s8)
			{
				Argument argument;
				argument.s64=0;
				argument.s8=s8;
				return argument;
			}

			BasicIns::Argument BasicIns::Makes16(signed __int16 s16)
			{
				Argument argument;
				argument.s64=0;
				argument.s16=s16;
				return argument;
			}

			BasicIns::Argument BasicIns::Makes32(signed __int32 s32)
			{
				Argument argument;
				argument.s64=0;
				argument.s32=s32;
				return argument;
			}

			BasicIns::Argument BasicIns::Makes64(signed __int64 s64)
			{
				Argument argument;
				argument.s64=0;
				argument.s64=s64;
				return argument;
			}

			BasicIns::Argument BasicIns::Makeu8(unsigned __int8 u8)
			{
				Argument argument;
				argument.s64=0;
				argument.u8=u8;
				return argument;
			}

			BasicIns::Argument BasicIns::Makeu16(unsigned __int16 u16)
			{
				Argument argument;
				argument.s64=0;
				argument.u16=u16;
				return argument;
			}

			BasicIns::Argument BasicIns::Makeu32(unsigned __int32 u32)
			{
				Argument argument;
				argument.s64=0;
				argument.u32=u32;
				return argument;
			}

			BasicIns::Argument BasicIns::Makeu64(unsigned __int64 u64)
			{
				Argument argument;
				argument.s64=0;
				argument.u64=u64;
				return argument;
			}

			BasicIns::Argument BasicIns::Makef32(float f32)
			{
				Argument argument;
				argument.s64=0;
				argument.f32=f32;
				return argument;
			}

			BasicIns::Argument BasicIns::Makef64(double f64)
			{
				Argument argument;
				argument.s64=0;
				argument.f64=f64;
				return argument;
			}
			
			BasicIns::Argument BasicIns::MakeInt(int value)
			{
#ifdef _WIN64
				return Makes64(value);
#else
				return Makes32(value);
#endif
			}

			BasicIns::Argument BasicIns::MakePointer(void* value)
			{
#ifdef _WIN64
				return Makeu64((unsigned __int64)value);
#else
				return Makeu32((unsigned __int32)value);
#endif
			}

/***********************************************************************
BasicILResourceNames
***********************************************************************/

			const wchar_t* const BasicILResourceNames::BasicLanguageInterfaces	= L"[BASIC-LANGUAGE-INTERFACES]";
			const wchar_t* const BasicILResourceNames::ExportedSymbols			= L"[EXPORTED-SYMBOLS]";

/***********************************************************************
BasicIL::Label
***********************************************************************/

			bool BasicIL::Label::operator==(const Label& label)
			{
				return instructionIndex==label.instructionIndex;
			}

/***********************************************************************
BasicIL
***********************************************************************/

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode)
			{
				BasicIns ins;
				ins.opcode=opcode;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::Argument argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argument=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::Argument argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argument=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.type2=type2;
				instructions.Add(ins);
				return *this;
			}
			
			BasicIL& BasicIL::InsUD(BasicIns::OpCode opcode, void* userData)
			{
				Ins(opcode);
				instructions[instructions.Count()-1].userData=userData;
				return *this;
			}

			BasicIL& BasicIL::InsUD(BasicIns::OpCode opcode, BasicIns::Argument argument, void* userData)
			{
				Ins(opcode, argument);
				instructions[instructions.Count()-1].userData=userData;
				return *this;
			}

			BasicIL& BasicIL::InsUD(BasicIns::OpCode opcode, BasicIns::ValueType type1, void* userData)
			{
				Ins(opcode, type1);
				instructions[instructions.Count()-1].userData=userData;
				return *this;
			}

			BasicIL& BasicIL::InsUD(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::Argument argument, void* userData)
			{
				Ins(opcode, type1, argument);
				instructions[instructions.Count()-1].userData=userData;
				return *this;
			}

			BasicIL& BasicIL::InsUD(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2, void* userData)
			{
				Ins(opcode, type1, type2);
				instructions[instructions.Count()-1].userData=userData;
				return *this;
			}


			// Deserialization Begin

			int ReadInt(stream::IStream& stream)
			{
				int result=0;
				stream.Read(&result, sizeof(result));
				return result;
			}

			WString ReadString(stream::IStream& stream)
			{
				Array<wchar_t> buffer(ReadInt(stream)+1);
				stream.Read(&buffer[0], sizeof(wchar_t)*(buffer.Count()-1));
				buffer[buffer.Count()-1]=0;
				return &buffer[0];
			}

			template<typename T>
			void ReadList(stream::IStream& stream, List<T>& collection)
			{
				int count=ReadInt(stream);
				collection.Clear();
				T buffer;
				for(int i=0;i<count;i++)
				{
					stream.Read(&buffer, sizeof(buffer));
					collection.Add(buffer);
				}
			}

			template<typename T>
			void ReadArray(stream::IStream& stream, Array<T>& collection)
			{
				collection.Resize(ReadInt(stream));
				if(collection.Count()>0)
				{
					stream.Read(&collection[0], sizeof(T)*collection.Count());
				}
			}

			void BasicIL::LoadFromStream(stream::IStream& stream)
			{
				ReadList(stream, instructions);
				ReadList(stream, labels);
				ReadArray(stream, globalData);

				resources.Clear();
				int count=ReadInt(stream);
				for(int i=0;i<count;i++)
				{
					WString name=ReadString(stream);
					Ptr<ResourceStream> resource=new ResourceStream;
					resource->LoadFromStream(stream);
					resources.Add(name, resource);
				}
			}

			// Deserialization End

			// Serialization Begin

			void WriteInt(stream::IStream& stream, int i)
			{
				stream.Write(&i, sizeof(i));
			}

			void WriteString(stream::IStream& stream, const WString& string)
			{
				WriteInt(stream, string.Length());
				if(string.Length()>0)
				{
					stream.Write((void*)string.Buffer(), sizeof(wchar_t)*string.Length());
				}
			}

			template<typename T>
			void WriteCollection(stream::IStream& stream, T& collection)
			{
				WriteInt(stream, collection.Count());
				if(collection.Count()>0)
				{
					stream.Write(&collection[0], sizeof(collection[0])*collection.Count());
				}
			}

			void WriteResource(stream::IStream& stream, Ptr<ResourceStream> resource)
			{
				resource->SaveToStream(stream);
			}

			void BasicIL::SaveToStream(stream::IStream& stream)
			{
				WriteCollection(stream, instructions);
				WriteCollection(stream, labels);
				WriteCollection(stream, globalData);

				WriteInt(stream, resources.Count());
				for(int i=0;i<resources.Count();i++)
				{
					WriteString(stream, resources.Keys()[i]);
					WriteResource(stream, resources.Values()[i]);
				}
			}

			// Serialization End

			// SaveAsString End

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
				CASE(push,				TypeArgument);
				CASE(pushins,			Jump);
				CASE(pushlabel,			Constant);
				CASE(label,				Single);
				CASE(add,				Type);
				CASE(sub,				Type);
				CASE(mul,				Type);
				CASE(div,				Type);
				CASE(mod,				Type);
				CASE(shl,				Type);
				CASE(shr,				Type);
				CASE(neg,				Type);
				CASE(and,				Type);
				CASE(or,				Type);
				CASE(xor,				Type);
				CASE(not,				Type);
				CASE(eq,				Type);
				CASE(ne,				Type);
				CASE(lt,				Type);
				CASE(le,				Type);
				CASE(gt,				Type);
				CASE(ge,				Type);
				CASE(read,				Type);
				CASE(write,				Type);
				CASE(readmem,			Constant);
				CASE(writemem,			Constant);
				CASE(copymem,			Constant);
				CASE(jump,				Jump);
				CASE(jumptrue,			Jump);
				CASE(jumpfalse,			Jump);
				CASE(call,				Jump);
				CASE(call_indirect,		Single);
				CASE(call_foreign,		Constant);
				CASE(call_raw,			Constant);
				CASE(convert,			Type2);
				CASE(stack_offset,		Constant);
				CASE(stack_top,			Constant);
				CASE(stack_reserve,		Constant);
				CASE(resptr,			Single);
				CASE(ret,				Constant);
				CASE(link_pushdata,		Constant);
				CASE(link_pushfunc,		Constant);
				CASE(codegen_callfunc,	Constant);
				return L"<UNKNOWN-OPCODE>";
#undef CASE
			}

			void WriteInteger(int integer, stream::TextWriter& writer)
			{
				WString intstr=itow(integer);
				int spaces=6-intstr.Length();
				while(spaces--)
				{
					writer.WriteString(L" ");
				}
				writer.WriteString(intstr);
			}

			void WriteData(Array<char>& data, stream::TextWriter& writer)
			{
				const int lineBytes=16;
				const wchar_t* hex=L"0123456789ABCDEF";

				int lines=data.Count()/lineBytes;
				if(data.Count()%lineBytes)
				{
					lines+=1;
				}

				for(int i=0;i<lines;i++)
				{
					int start=i*lineBytes;
					{
						wchar_t buffer[100];
						_itow_s(start, buffer, sizeof(buffer)/sizeof*(buffer), 16);
						size_t length=wcslen(buffer);
						int count=8-length;
						writer.WriteString(L"0x");
						while(count--)
						{
							writer.WriteString(L"0");
						}
						writer.WriteString(buffer);
						writer.WriteString(L": ");
					}
					for(int j=start;j<data.Count()&&j-start<lineBytes;j++)
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
				for(int i=0;i<labels.Count();i++)
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
				for(int i=0;i<instructions.Count();i++)
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

			// SaveAsString End
		}
	}
}
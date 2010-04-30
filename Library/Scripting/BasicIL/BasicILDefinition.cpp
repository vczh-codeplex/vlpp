#include "BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

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

			const wchar_t* const BasicILResourceNames::BasicLanguageInterfaces = L"[BASIC-LANGUAGE-INTERFACES]";

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

			// Deserialization Begin

			void BasicIL::LoadFromStream(stream::IStream& stream)
			{
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
					stream.Write((void*)string.Buffer(), string.Length());
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
				WriteInt(stream, resource->GetInternalSize());
				if(resource->GetInternalSize()>0)
				{
					stream.Write(resource->GetInternalData(), resource->GetInternalSize());
				}
			}

			void BasicIL::SaveToStream(stream::IStream& stream)
			{
				WriteCollection(stream, instructions);
				WriteCollection(stream, labels);
				WriteCollection(stream, globalData);
				WriteInt(stream, resources.Count());
				for(int i=0;i<resources.Count();i++)
				{
					WriteString(stream, resources.Keys()[0]);
					WriteResource(stream, resources.Values()[0]);
				}
			}

			// Serialization End
		}
	}
}
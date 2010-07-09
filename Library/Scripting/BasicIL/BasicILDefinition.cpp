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

			BasicIns::BasicIns()
			{
				opcode=(OpCode)0;
				type1=(ValueType)0;
				type2=(ValueType)0;
				argument.s64=0;
				insKey=-1;
				userData=0;
				argumentType=constantArgument;
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
			
			BasicIns::Argument BasicIns::MakeInt(vint value)
			{
#ifdef VCZH_64
				return Makes64(value);
#else
				return Makes32(value);
#endif
			}

			BasicIns::Argument BasicIns::MakePointer(void* value)
			{
#ifdef VCZH_64
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
BasicILLabel
***********************************************************************/

			bool BasicILLocalLabel::operator==(const BasicILLocalLabel& label)
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

			//---------------------------------------------------------------------------------------

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

			//---------------------------------------------------------------------------------------
			
			BasicIL& BasicIL::InsG(BasicIns::OpCode opcode)
			{
				Ins(opcode);
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsG(BasicIns::OpCode opcode, BasicIns::Argument argument)
			{
				Ins(opcode, argument);
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsG(BasicIns::OpCode opcode, BasicIns::ValueType type1)
			{
				Ins(opcode, type1);
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsG(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::Argument argument)
			{
				Ins(opcode, type1, argument);
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsG(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2)
			{
				Ins(opcode, type1, type2);
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			//---------------------------------------------------------------------------------------
			
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

			//---------------------------------------------------------------------------------------
			
			BasicIL& BasicIL::InsUDG(BasicIns::OpCode opcode, void* userData)
			{
				Ins(opcode);
				instructions[instructions.Count()-1].userData=userData;
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsUDG(BasicIns::OpCode opcode, BasicIns::Argument argument, void* userData)
			{
				Ins(opcode, argument);
				instructions[instructions.Count()-1].userData=userData;
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsUDG(BasicIns::OpCode opcode, BasicIns::ValueType type1, void* userData)
			{
				Ins(opcode, type1);
				instructions[instructions.Count()-1].userData=userData;
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsUDG(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::Argument argument, void* userData)
			{
				Ins(opcode, type1, argument);
				instructions[instructions.Count()-1].userData=userData;
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			BasicIL& BasicIL::InsUDG(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2, void* userData)
			{
				Ins(opcode, type1, type2);
				instructions[instructions.Count()-1].userData=userData;
				instructions[instructions.Count()-1].argumentType=BasicIns::linearArgument;
				return *this;
			}

			//---------------------------------------------------------------------------------------

			void BasicIL::LoadFromStream(stream::IStream& stream)
			{
				ReadStream(stream, instructions);
				ReadStream(stream, labels);
				ReadStream(stream, globalData);
				ReadStream(stream, resources);
			}

			void BasicIL::SaveToStream(stream::IStream& stream)
			{
				WriteStream(stream, instructions);
				WriteStream(stream, labels);
				WriteStream(stream, globalData);
				WriteStream(stream, resources);
			}
		}
	}
}
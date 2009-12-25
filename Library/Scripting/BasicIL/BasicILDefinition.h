/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:

OpCode:
  <binary opcode>	:*stack_top* left_operand right_operand

  push				TYPE			CONSTANT	:*stack_top*						-> TYPE
  add|sub|mul|div	TYPE						:*stack_top* TYPE TYPE				-> TYPE
  eq|ne|lt|le|gt|ge	TYPE						:*stack_top* TYPE TYPE				-> bool
  mod				INTEGER_TYPE				:*stack_top* TYPE TYPE				-> TYPE
  shl|shr			INTEGER_TYPE	CONSTANT	:*stack_top* TYPE					-> TYPE
  read				TYPE						:*stack_top* TYPE*					->
  write				TYPE						:*stack_top* TYPE* TYPE				->
  jump				INSTRUCTION_INDEX(int)
  jumptrue			INSTRUCTION_INDEX(int)		:*stack_top* bool					->
  jumpfalse			INSTRUCTION_INDEX(int)		:*stack_top* bool					->
  call				INSTRUCTION_INDEX(int)		:*stack_top* RETPTR					-> *stack_offset_zero* RETINS RETPTR
  call_foreign		FOREIGN_FUNCTION_INDEX(int)	:*stack_top* RETPTR					->
  convert			DEST_TYPE		SOURCE_TYPE	:*stack_top* SOURCE_TYPE			-> DEST_TYPE
  stack_offset		BYTES(int)					:*stack_top*						-> pointer
  stack_reserve		BYTES(int)
  resptr										:*stack_top*						-> pointer
  ret				STACK_RESERVE_BYTES(int)	:*stack_top* RETINS RETPTR			->

  memcpy										:*stack_top* DSTPTR SRCPTR BYTES	->
  memzero										:*stack_top* DSTPTR BYTES			->
  memcmp										:*stack_top* DSTPTR SRCPTR BYTES	-> int
  strcpy										:*stack_top* DSTPTR SRCPTR			->
  wcscpy										:*stack_top* DSTPTR SRCPTR			->
  strcmp										:*stack_top* DSTPTR SRCPTR			-> int
  wcscmp										:*stack_top* DSTPTR SRCPTR			-> int
  strncmp										:*stack_top* DSTPTR SRCPTR BYTES	-> int
  wcsncmp										:*stack_top* DSTPTR SRCPTR BYTES	-> int
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILDEFINITION
#define VCZH_SCRIPTING_BASICIL_BASICILDEFINITION

#include "..\..\String.h"
#include "..\..\Collections\List.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			class BasicIns
			{
			public:
				enum ValueType
				{
					s8,s16,s32,s64,
					u8,u16,u32,u64,
					f32,f64,

					bool_type=u8,
					char_type=s8,
					wchar_type=u16,
#ifdef _WIN64
					int_type=s64,
					pointer_type=u64,
#else
					int_type=s32,
					pointer_type=u32,
#endif
				};

				enum OpCode
				{
					push,
					add,sub,mul,div,mod,shl,shr,
					eq,ne,lt,le,gt,ge,
					read,write,
					jump,jumptrue,jumpfalse,call,call_foreign,
					convert,
					stack_offset,
					stack_reserve,
					resptr,
					ret,

					memcpy,memzero,memcmp,strcpy,wcscpy,strcmp,wcscmp,strncmp,wcsncmp,

					instruction_count
				};

				OpCode							opcode;
				ValueType						type1;
				ValueType						type2;
				union Argument
				{
					signed __int8				s8;
					signed __int16				s16;
					signed __int32				s32;
					signed __int64				s64;
					unsigned __int8				u8;
					unsigned __int16			u16;
					unsigned __int32			u32;
					unsigned __int64			u64;
					float						f32;
					double						f64;

					unsigned __int8				bool_value;
					char						char_value;
					wchar_t						wchar_value;
#ifdef _WIN64
					unsigned __int64			int_value;
					signed __int64				pointer_value;
#else
					unsigned __int32			int_value;
					signed __int32				pointer_value;
#endif
				}								argument;

				BasicIns();

				bool							operator==(const BasicIns& ins)const;
				bool							operator!=(const BasicIns& ins)const;

				static Argument					Makes8(signed __int8 s8);
				static Argument					Makes16(signed __int16 s16);
				static Argument					Makes32(signed __int32 s32);
				static Argument					Makes64(signed __int64 s64);
				static Argument					Makeu8(unsigned __int8 u8);
				static Argument					Makeu16(unsigned __int16 u16);
				static Argument					Makeu32(unsigned __int32 u32);
				static Argument					Makeu64(unsigned __int64 u64);
				static Argument					Makef32(float f32);
				static Argument					Makef64(double f64);
			};
		}
	}

	template<>
	struct POD<vl::scripting::basicil::BasicIns>
	{
		static const bool Result=true;
	};

	namespace scripting
	{
		namespace basicil
		{
			class BasicIL : public Object
			{
			public:
				collections::List<BasicIns>		instructions;

				BasicIL&						Ins(BasicIns::OpCode opcode);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::Argument argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::Argument argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2);
			};
		}
	}
}

#endif
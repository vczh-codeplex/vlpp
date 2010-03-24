/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:
  BasicIns									£ºÖ¸Áî
  BasicIL									£ºÖ¸Áî±í

OpCode:
  stack				:low(0) push<--data-->pop high(size)
  <binary opcode>	:*stack_top* left_operand right_operand
  ------------------instructions---------------------
  push					TYPE			CONSTANT		:*stack_top*									-> TYPE
  pushins				CONSTANT		INSKEY			:*stack_top*									-> instruction_pointer instruction_key
  pushlabel				CONSTANT						:*stack_top*									-> instruction_label_index
  label													:*stack_top* FUNCTION_INDEX						-> instruction_pointer instruction key
  add|sub|mul|div		TYPE							:*stack_top* TYPE TYPE							-> TYPE
  eq|ne|lt|le|gt|ge		TYPE							:*stack_top* TYPE TYPE							-> bool
  mod|shl|shr			INTEGER_TYPE					:*stack_top* TYPE TYPE							-> TYPE
  and|or|xor			INTEGER_TYPE					:*stack_typ* TYPE TYPE							-> TYPE
  not					INTEGER_TYPE					:*stack_top* TYPE								-> TYPE
  neg					INTEGER_TYPE					:*stack_top* TYPE								-> TYPE
  read					TYPE							:*stack_top* TYPE*								-> TYPE
  write					TYPE							:*stack_top* TYPE* TYPE							->
  readmem				BYTES(int)						:*stack_top* TYPE*								-> data[BYTES]
  writemem				BYTES(int)						:*stack_top* TYPE* data[BYTES]					->
  jump					INSTRUCTION_INDEX(int)	INSKEY
  jumptrue				INSTRUCTION_INDEX(int)	INSKEY	:*stack_top* bool								->
  jumpfalse				INSTRUCTION_INDEX(int)	INSKEY	:*stack_top* bool								->
  call					INSTRUCTION_INDEX(int)	INSKEY	:*stack_top* RETPTR								-> *stack_offset_zero* RETSTACK RETINS RETINSKEY RETPTR
  call_indirect											:*stack_top* PUSHINS RETPTR						-> *stack_offset_zero* RETSTACK RETINS RETINSKEY RETPTR
  call_foreign			FOREIGN_FUNCTION_INDEX(int)		:*stack_top* RETPTR								-> RETPTR
  call_raw				RAW_FUNCTION_POINTER			:*stack_top* ARGUMENTS							->
  convert				DEST_TYPE		SOURCE_TYPE		:*stack_top* SOURCE_TYPE						-> DEST_TYPE
  stack_offset			BYTES(int)						:*stack_top*									-> pointer
  stack_top				BYTES(int)						:*stack_top*									-> pointer(old stack top pointer)
  stack_reserve			BYTES(int)(+=push, -=pop)
  resptr												:*stack_top*									-> pointer
  ret					STACK_RESERVE_BYTES(int)		:*stack_top* RETSTACK RETINS RETINSKEY RETPTR	->
------------------link time only---------------------
  link_pushdata			OFFSET(int)						:*stack_top*									-> pointer
  link_pushfunc			INDEX(int)						:*stack_top*									-> instruction_label_index
------------------compile time only------------------
  codegen_pushfunc		INDEX(int)						:*stack top*									-> instruction_label_index
  codegen_callfunc		INDEX(int)						:*stack top* RETPTR								-> *stack_offset_zero* RETSTACK RETINS RETINSKEY RETPTR
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILDEFINITION
#define VCZH_SCRIPTING_BASICIL_BASICILDEFINITION

#include "..\..\String.h"
#include "..\..\Collections\List.h"
#include "BasicILResourceStream.h"

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
					uint_type=u64,
					pointer_type=u64,
#else
					int_type=s32,
					uint_type=u32,
					pointer_type=u32,
#endif
				};

				enum OpCode
				{
					push,pushins,pushlabel,label,
					add,sub,mul,div,mod,shl,shr,neg,
					and,or,xor,not,
					eq,ne,lt,le,gt,ge,
					read,write,readmem,writemem,
					jump,jumptrue,jumpfalse,call,call_indirect,call_foreign,call_raw,
					convert,
					stack_offset,stack_top,stack_reserve,
					resptr,
					ret,

					link_pushdata,
					link_pushfunc,

					codegen_pushfunc,
					codegen_callfunc,
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

					bool						bool_value;
					char						char_value;
					wchar_t						wchar_value;
					int							(*raw_function)(void* arguments);
#ifdef _WIN64
					signed __int64				int_value;
					void*						pointer_value;
#else
					signed __int32				int_value;
					void*						pointer_value;
#endif
				}								argument;
				int								insKey;

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
				static Argument					MakeInt(int value);
				static Argument					MakePointer(void* value);
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
				struct Label
				{
					int							instructionIndex;

					bool						operator==(const Label& label);
				};

				collections::List<BasicIns>		instructions;
				collections::List<Label>		labels;
				collections::Array<char>		globalData;

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
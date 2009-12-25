/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:

OpCode:
  push				TYPE			CONSTANT	:*stack_top*				-> TYPE
  pop				BYTES						:*stack_top* bytes			->
  add|sub|mul|div	TYPE						:*stack_top* TYPE TYPE		-> TYPE
  eq|ne|lt|le|gt|ge	TYPE						:*stack_top* TYPE TYPE		-> bool
  mod				INTEGER_TYPE				:*stack_top* TYPE TYPE		-> TYPE
  shl|shr			INTEGER_TYPE	CONSTANT	:*stack_top* TYPE			-> TYPE
  read				TYPE						:*stack_top* TYPE*			->
  write				TYPE						:*stack_top* TYPE* TYPE		->
  jump				INSTRUCTION_INDEX
  jumptrue			INSTRUCTION_INDEX			:*stack_top* bool			->
  jumpfalse			INSTRUCTION_INDEX			:*stack_top* bool			->
  call				INSTRUCTION_INDEX			:*stack_top* RETPTR			-> *stack_offset_zero* RETINS RETPTR
  call_foreign		FOREIGN_FUNCTION_INDEX		:*stack_top* RETPTR			->
  convert			DEST_TYPE		SOURCE_TYPE	:*stack_top* SOURCE_TYPE	-> DEST_TYPE
  stack_offset		BYTES						:*stack_top*				-> pointer
  stack_reserve		BYTES
  resptr										:*stack_top*				-> pointer
  ret				STACK_RESERVE_BYTES			:*stack_top* RETINS RETPTR	->
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
					int_type=(sizeof(int)==4?s32:s64),
					pointer_type=(sizeof(int)==4?u32:u64),
				};

				enum OpCode
				{
					push,pop,
					add,sub,mul,div,mod,shl,shr,
					eq,ne,lt,le,gt,ge,
					read,write,
					jump,jumptrue,jumpfalse,call,call_foreign,
					convert,
					stack_offset,
					stack_reserve,
					resptr,
					ret,
				};

				OpCode							opcode;
				ValueType						type1;
				ValueType						type2;
				unsigned __int64				argint;
				float							argf32;
				double							argf64;

				BasicIns();

				bool							operator==(const BasicIns& ins)const;
				bool							operator!=(const BasicIns& ins)const;
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
				BasicIL&						Ins(BasicIns::OpCode opcode, unsigned __int64 argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, int argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, bool argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, float argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, double argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, unsigned __int64 argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, int argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, bool argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, float argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, double argument);
				BasicIL&						Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, BasicIns::ValueType type2);
			};
		}
	}
}

#endif
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
				argint=0;
				argf32=0;
				argf64=0;
			}

			bool BasicIns::operator==(const BasicIns& ins)const
			{
				return opcode==ins.opcode && type1==ins.type1 && type2==ins.type2 && argint==ins.argint && argf32==ins.argf32 && argf64==ins.argf64;
			}

			bool BasicIns::operator!=(const BasicIns& ins)const
			{
				return opcode!=ins.opcode || type1!=ins.type1 || type2!=ins.type2 || argint!=ins.argint || argf32!=ins.argf32 || argf64!=ins.argf64;
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

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, unsigned __int64 argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argint=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, int argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argint=(unsigned __int64)argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, bool argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argint=(unsigned __int64)argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, float argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argf32=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, double argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.argf64=argument;
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

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, unsigned __int64 argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argint=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, int argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argint=(unsigned __int64)argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, bool argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argint=(unsigned __int64)argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, float argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argf32=argument;
				instructions.Add(ins);
				return *this;
			}

			BasicIL& BasicIL::Ins(BasicIns::OpCode opcode, BasicIns::ValueType type1, double argument)
			{
				BasicIns ins;
				ins.opcode=opcode;
				ins.type1=type1;
				ins.argf64=argument;
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
		}
	}
}
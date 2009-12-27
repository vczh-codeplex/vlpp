#define _CRT_SECURE_NO_WARNINGS
#include "BasicILInterpretor.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			class ILException
			{
			public:
				BasicILInterpretor::RunningResult result;

				ILException(BasicILInterpretor::RunningResult _result)
					:result(_result)
				{
				}
			};

/***********************************************************************
BasicILEnv
***********************************************************************/

			BasicILEnv::BasicILEnv(int _stackSize)
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

			int BasicILEnv::StackBase()const
			{
				return stackBase;
			}

			int BasicILEnv::StackSize()const
			{
				return stackSize;
			}

			int BasicILEnv::StackTop()const
			{
				return stackTop;
			}

			void* BasicILEnv::DereferenceStack(int stackPosition)const
			{
				if(stackPosition<0 || stackPosition>=stackSize)
				{
					throw ILException(BasicILInterpretor::StackOverflow);
				}
				else
				{
					return &stack[stackPosition];
				}
			}

			void* BasicILEnv::Reserve(int size)
			{
				stackTop-=size;
				if(stackTop<0 || stackTop>stackSize)
				{
					throw ILException(BasicILInterpretor::StackOverflow);
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

			void BasicILEnv::SetBase(int stackPosition)
			{
				stackBase=stackPosition;
			}

/***********************************************************************
Instructions
***********************************************************************/

			template<typename T>
			void Add_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left+right);
			}

			template<typename T>
			void Sub_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left-right);
			}

			template<typename T>
			void Mul_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left*right);
			}

			template<typename T>
			void Div_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				if(right==0)
				{
					throw ILException(BasicILInterpretor::DividByZero);
				}
				else
				{
					env->Push<T>(left/right);
				}
			}

			template<typename T>
			void Mod_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left%right);
			}

			template<typename T>
			void Shl_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left<<right);
			}

			template<typename T>
			void Shr_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left>>right);
			}

			template<typename T>
			void Eq_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left==right);
			}

			template<typename T>
			void Ne_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left!=right);
			}

			template<typename T>
			void Lt_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left<right);
			}

			template<typename T>
			void Le_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left<=right);
			}

			template<typename T>
			void Gt_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left>right);
			}

			template<typename T>
			void Ge_(BasicIns& ins, BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left>=right);
			}

			template<typename T>
			void Read_(BasicIns& ins, BasicILEnv* env)
			{
				T* pointer=env->Pop<T*>();
				env->Push<T>(*pointer);
			}

			template<typename T>
			void Write_(BasicIns& ins, BasicILEnv* env)
			{
				T* pointer=env->Pop<T*>();
				T value=env->Pop<T>();
				*pointer=value;
			}

			template<typename D, typename S>
			void Convert_(BasicIns& ins, BasicILEnv* env)
			{
				S value=env->Pop<S>();
				env->Push<D>((D)value);
			}

/***********************************************************************
BasicILInterpretor
***********************************************************************/

			BasicILInterpretor::BasicILInterpretor(int _stackSize, BasicIL* _instructions)
			{
				env=new BasicILEnv(_stackSize);
				instructions=_instructions;
				instruction=-1;
				foreignFunctionIndex=-1;
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
				delete env;
			}

			void BasicILInterpretor::Reset(int entryInstruction, int returnSize)
			{
				// reserve returnSize
				// push returnPointer
				// push returnInstruction
				// push returnStackBase
				void* returnPointer=env->Reserve(returnSize);
				env->Push<void*>(returnPointer);
				env->Push<int>(-1);
				env->Push<int>(env->StackSize());
				env->SetBase(env->StackTop());
				instruction=entryInstruction;
			}

			int BasicILInterpretor::GetForeignFunctionIndex()
			{
				return foreignFunctionIndex;
			}

			BasicILEnv* BasicILInterpretor::GetEnv()
			{
				return env;
			}

			int BasicILInterpretor::GetInstruction()
			{
				return instruction;
			}

#define NUMERIC_INSTRUCTION(METHOD)\
			switch(ins.type1)\
			{\
			case BasicIns::s8:\
				METHOD<signed __int8>(ins, env);\
				break;\
			case BasicIns::s16:\
				METHOD<signed __int16>(ins, env);\
				break;\
			case BasicIns::s32:\
				METHOD<signed __int32>(ins, env);\
				break;\
			case BasicIns::s64:\
				METHOD<signed __int64>(ins, env);\
				break;\
			case BasicIns::u8:\
				METHOD<unsigned __int8>(ins, env);\
				break;\
			case BasicIns::u16:\
				METHOD<unsigned __int16>(ins, env);\
				break;\
			case BasicIns::u32:\
				METHOD<unsigned __int32>(ins, env);\
				break;\
			case BasicIns::u64:\
				METHOD<unsigned __int64>(ins, env);\
				break;\
			case BasicIns::f32:\
				METHOD<float>(ins, env);\
				break;\
			case BasicIns::f64:\
				METHOD<double>(ins, env);\
				break;\
			default:\
				return BasicILInterpretor::BadInstructionArgument;\
			}

#define INTEGER_INSTRUCTION(METHOD)\
			switch(ins.type1)\
			{\
			case BasicIns::s8:\
				METHOD<signed __int8>(ins, env);\
				break;\
			case BasicIns::s16:\
				METHOD<signed __int16>(ins, env);\
				break;\
			case BasicIns::s32:\
				METHOD<signed __int32>(ins, env);\
				break;\
			case BasicIns::s64:\
				METHOD<signed __int64>(ins, env);\
				break;\
			case BasicIns::u8:\
				METHOD<unsigned __int8>(ins, env);\
				break;\
			case BasicIns::u16:\
				METHOD<unsigned __int16>(ins, env);\
				break;\
			case BasicIns::u32:\
				METHOD<unsigned __int32>(ins, env);\
				break;\
			case BasicIns::u64:\
				METHOD<unsigned __int64>(ins, env);\
				break;\
			default:\
				return BasicILInterpretor::BadInstructionArgument;\
			}

#define CONVERT_2_INSTRUCTION(DSTTYPE)\
			switch(ins.type2)\
			{\
			case BasicIns::s8:\
				Convert_<DSTTYPE, signed __int8>(ins, env);\
				break;\
			case BasicIns::s16:\
				Convert_<DSTTYPE, signed __int16>(ins, env);\
				break;\
			case BasicIns::s32:\
				Convert_<DSTTYPE, signed __int32>(ins, env);\
				break;\
			case BasicIns::s64:\
				Convert_<DSTTYPE, signed __int64>(ins, env);\
				break;\
			case BasicIns::u8:\
				Convert_<DSTTYPE, unsigned __int8>(ins, env);\
				break;\
			case BasicIns::u16:\
				Convert_<DSTTYPE, unsigned __int16>(ins, env);\
				break;\
			case BasicIns::u32:\
				Convert_<DSTTYPE, unsigned __int32>(ins, env);\
				break;\
			case BasicIns::u64:\
				Convert_<DSTTYPE, unsigned __int64>(ins, env);\
				break;\
			case BasicIns::f32:\
				Convert_<DSTTYPE, float>(ins, env);\
				break;\
			case BasicIns::f64:\
				Convert_<DSTTYPE, double>(ins, env);\
				break;\
			default:\
				return BasicILInterpretor::BadInstructionArgument;\
			}

#define CONVERT_INSTRUCTION\
			switch(ins.type1)\
			{\
			case BasicIns::s8:\
				CONVERT_2_INSTRUCTION(signed __int8)\
				break;\
			case BasicIns::s16:\
				CONVERT_2_INSTRUCTION(signed __int16)\
				break;\
			case BasicIns::s32:\
				CONVERT_2_INSTRUCTION(signed __int32)\
				break;\
			case BasicIns::s64:\
				CONVERT_2_INSTRUCTION(signed __int64)\
				break;\
			case BasicIns::u8:\
				CONVERT_2_INSTRUCTION(unsigned __int8)\
				break;\
			case BasicIns::u16:\
				CONVERT_2_INSTRUCTION(unsigned __int16)\
				break;\
			case BasicIns::u32:\
				CONVERT_2_INSTRUCTION(unsigned __int32)\
				break;\
			case BasicIns::u64:\
				CONVERT_2_INSTRUCTION(unsigned __int64)\
				break;\
			case BasicIns::f32:\
				CONVERT_2_INSTRUCTION(float)\
				break;\
			case BasicIns::f64:\
				CONVERT_2_INSTRUCTION(double)\
				break;\
			default:\
				return BasicILInterpretor::BadInstructionArgument;\
			}

			BasicILInterpretor::RunningResult BasicILInterpretor::Run()
			{
				try
				{
					while(instruction!=-1)
					{
						if(instruction<0||instruction>=instructions->instructions.Count())
						{
							return BasicILInterpretor::InstructionIndexOutOfRange;
						}
						int nextInstruction=instruction+1;
						BasicIns& ins=instructions->instructions[instruction];
						switch(ins.opcode)
						{
						case BasicIns::push:
							switch(ins.type1)
							{
							case BasicIns::s8:
								env->Push<signed __int8>(ins.argument.s8);
								break;
							case BasicIns::s16:
								env->Push<signed __int16>(ins.argument.s16);
								break;
							case BasicIns::s32:
								env->Push<signed __int32>(ins.argument.s32);
								break;
							case BasicIns::s64:
								env->Push<signed __int64>(ins.argument.s64);
								break;
							case BasicIns::u8:
								env->Push<unsigned __int8>(ins.argument.u8);
								break;
							case BasicIns::u16:
								env->Push<unsigned __int16>(ins.argument.u16);
								break;
							case BasicIns::u32:
								env->Push<unsigned __int32>(ins.argument.u32);
								break;
							case BasicIns::u64:
								env->Push<unsigned __int64>(ins.argument.u64);
								break;
							case BasicIns::f32:
								env->Push<float>(ins.argument.f32);
								break;
							case BasicIns::f64:
								env->Push<double>(ins.argument.f64);
								break;
							default:
								return BasicILInterpretor::BadInstructionArgument;
							}
							break;
						case BasicIns::pushins:
							env->Push<int>(ins.argument.int_value);
							break;
						case BasicIns::add:
							NUMERIC_INSTRUCTION(Add_)
							break;
						case BasicIns::sub:
							NUMERIC_INSTRUCTION(Sub_)
							break;
						case BasicIns::mul:
							NUMERIC_INSTRUCTION(Mul_)
							break;
						case BasicIns::div:
							NUMERIC_INSTRUCTION(Div_)
							break;
						case BasicIns::mod:
							INTEGER_INSTRUCTION(Mod_)
							break;
						case BasicIns::shl:
							INTEGER_INSTRUCTION(Shl_)
							break;
						case BasicIns::shr:
							INTEGER_INSTRUCTION(Shr_)
							break;
						case BasicIns::eq:
							NUMERIC_INSTRUCTION(Eq_)
							break;
						case BasicIns::ne:
							NUMERIC_INSTRUCTION(Ne_)
							break;
						case BasicIns::lt:
							NUMERIC_INSTRUCTION(Lt_)
							break;
						case BasicIns::le:
							NUMERIC_INSTRUCTION(Le_)
							break;
						case BasicIns::gt:
							NUMERIC_INSTRUCTION(Gt_)
							break;
						case BasicIns::ge:
							NUMERIC_INSTRUCTION(Ge_)
							break;
						case BasicIns::read:
							NUMERIC_INSTRUCTION(Read_)
							break;
						case BasicIns::write:
							NUMERIC_INSTRUCTION(Write_)
							break;
						case BasicIns::jump:
							nextInstruction=ins.argument.int_value;
							break;
						case BasicIns::jumptrue:
							if(env->Pop<bool>())
							{
								nextInstruction=ins.argument.int_value;
							}
							break;
						case BasicIns::jumpfalse:
							if(!env->Pop<bool>())
							{
								nextInstruction=ins.argument.int_value;
							}
							break;
						case BasicIns::call:
							env->Push<int>(nextInstruction);
							env->Push<int>(env->StackBase());
							env->SetBase(env->StackTop());
							nextInstruction=ins.argument.int_value;
							break;
						case BasicIns::call_indirect:
							{
								int pushins=env->Pop<int>();
								env->Push<int>(nextInstruction);
								env->Push<int>(env->StackBase());
								env->SetBase(env->StackTop());
								nextInstruction=pushins;
							}
							break;
						case BasicIns::call_foreign:
							foreignFunctionIndex=ins.argument.int_value;
							return BasicILInterpretor::ForeignFunctionCall;
						case BasicIns::convert:
							CONVERT_INSTRUCTION
							break;
						case BasicIns::stack_offset:
							{
								int stackBase=env->StackBase();
								env->Push<void*>(env->DereferenceStack(stackBase+ins.argument.int_value));
							}
							break;
						case BasicIns::stack_reserve:
							env->Reserve(ins.argument.int_value);
							break;
						case BasicIns::resptr:
							{
								void* pointer=*(void**)(env->DereferenceStack(env->StackBase()+2*sizeof(void*)));
								env->Push<void*>(pointer);
							}
							break;
						case BasicIns::ret:
							{
								int stackBase=env->Pop<int>();
								int returnInstruction=env->Pop<int>();
								env->Pop<void*>();
								env->SetBase(stackBase);
								env->Reserve(-ins.argument.int_value);
								nextInstruction=returnInstruction;
							}
							break;
						case BasicIns::memcpy:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								int size=env->Pop<int>();
								memcpy(dstptr, srcptr, size);
							}
							break;
						case BasicIns::memzero:
							{
								void* dstptr=env->Pop<void*>();
								int size=env->Pop<int>();
								memset(dstptr, 0, size);
							}
							break;
						case BasicIns::memcmp:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								int size=env->Pop<int>();
								env->Push<int>(memcmp(dstptr, srcptr, size));
							}
							break;
						case BasicIns::strcpy:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								strcpy((char*)dstptr, (const char*)srcptr);
							}
							break;
						case BasicIns::wcscpy:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								wcscpy((wchar_t*)dstptr, (const wchar_t*)srcptr);
							}
							break;
						case BasicIns::strcmp:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								env->Push<int>(strcmp((char*)dstptr, (const char*)srcptr));
							}
							break;
						case BasicIns::wcscmp:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								env->Push<int>(wcscmp((wchar_t*)dstptr, (const wchar_t*)srcptr));
							}
							break;
						case BasicIns::strncmp:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								int size=env->Pop<int>();
								env->Push<int>(strncmp((char*)dstptr, (const char*)srcptr, size));
							}
							break;
						case BasicIns::wcsncmp:
							{
								void* dstptr=env->Pop<void*>();
								void* srcptr=env->Pop<void*>();
								int size=env->Pop<int>();
								env->Push<int>(wcsncmp((wchar_t*)dstptr, (const wchar_t*)srcptr, size));
							}
							break;
						default:
							return BasicILInterpretor::UnknownInstruction;
						}
						instruction=nextInstruction;
					}
					return BasicILInterpretor::Finished;
				}
				catch(const ILException& e)
				{
					return e.result;
				}
			}
		}
	}
}
#include "BasicILInterpretor.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
Instructions
***********************************************************************/

			template<typename T>
			void Add_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left+right);
			}

			template<typename T>
			void Sub_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left-right);
			}

			template<typename T>
			void Mul_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left*right);
			}

			template<typename T>
			void Div_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				if(right==0)
				{
					throw ILException(BasicILStack::DividByZero);
				}
				else
				{
					env->Push<T>(left/right);
				}
			}

			template<typename T>
			void Mod_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left%right);
			}

			template<typename T>
			void Shl_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left<<right);
			}

			template<typename T>
			void Shr_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left>>right);
			}

			template<typename T>
			void Neg_(BasicILEnv* env)
			{
				T value=env->Pop<T>();
				env->Push<T>(-value);
			}

			template<typename T>
			void And_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left&right);
			}

			template<typename T>
			void Or_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left|right);
			}

			template<typename T>
			void Xor_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<T>(left^right);
			}

			template<typename T>
			void Not_(BasicILEnv* env)
			{
				T value=env->Pop<T>();
				env->Push<T>(~value);
			}

			template<typename T>
			void Eq_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left==right);
			}

			template<typename T>
			void Ne_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left!=right);
			}

			template<typename T>
			void Lt_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left<right);
			}

			template<typename T>
			void Le_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left<=right);
			}

			template<typename T>
			void Gt_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left>right);
			}

			template<typename T>
			void Ge_(BasicILEnv* env)
			{
				T left=env->Pop<T>();
				T right=env->Pop<T>();
				env->Push<bool>(left>=right);
			}

			template<typename T>
			void Read_(BasicILEnv* env)
			{
				T* pointer=env->Pop<T*>();
				env->Push<T>(*pointer);
			}

			template<typename T>
			void Write_(BasicILEnv* env)
			{
				T* pointer=env->Pop<T*>();
				T value=env->Pop<T>();
				*pointer=value;
			}

			template<typename D, typename S>
			void Convert_(BasicILEnv* env)
			{
				S value=env->Pop<S>();
				env->Push<D>((D)value);
			}

/***********************************************************************
BasicILStack Helper Macros
***********************************************************************/

#define NUMERIC_INSTRUCTION(METHOD)\
			switch(ins.type1)\
			{\
			case BasicIns::s8:\
				METHOD<signed __int8>(env);\
				break;\
			case BasicIns::s16:\
				METHOD<signed __int16>(env);\
				break;\
			case BasicIns::s32:\
				METHOD<signed __int32>(env);\
				break;\
			case BasicIns::s64:\
				METHOD<signed __int64>(env);\
				break;\
			case BasicIns::u8:\
				METHOD<unsigned __int8>(env);\
				break;\
			case BasicIns::u16:\
				METHOD<unsigned __int16>(env);\
				break;\
			case BasicIns::u32:\
				METHOD<unsigned __int32>(env);\
				break;\
			case BasicIns::u64:\
				METHOD<unsigned __int64>(env);\
				break;\
			case BasicIns::f32:\
				METHOD<float>(env);\
				break;\
			case BasicIns::f64:\
				METHOD<double>(env);\
				break;\
			default:\
				return BasicILStack::BadInstructionArgument;\
			}

#define SIGNED_NUMERIC_INSTRUCTION(METHOD)\
			switch(ins.type1)\
			{\
			case BasicIns::s8:case BasicIns::u8:\
				METHOD<signed __int8>(env);\
				break;\
			case BasicIns::s16:case BasicIns::u16:\
				METHOD<signed __int16>(env);\
				break;\
			case BasicIns::s32:case BasicIns::u32:\
				METHOD<signed __int32>(env);\
				break;\
			case BasicIns::s64:case BasicIns::u64:\
				METHOD<signed __int64>(env);\
				break;\
			case BasicIns::f32:\
				METHOD<float>(env);\
				break;\
			case BasicIns::f64:\
				METHOD<double>(env);\
				break;\
			default:\
				return BasicILStack::BadInstructionArgument;\
			}

#define INTEGER_INSTRUCTION(METHOD)\
			switch(ins.type1)\
			{\
			case BasicIns::s8:\
				METHOD<signed __int8>(env);\
				break;\
			case BasicIns::s16:\
				METHOD<signed __int16>(env);\
				break;\
			case BasicIns::s32:\
				METHOD<signed __int32>(env);\
				break;\
			case BasicIns::s64:\
				METHOD<signed __int64>(env);\
				break;\
			case BasicIns::u8:\
				METHOD<unsigned __int8>(env);\
				break;\
			case BasicIns::u16:\
				METHOD<unsigned __int16>(env);\
				break;\
			case BasicIns::u32:\
				METHOD<unsigned __int32>(env);\
				break;\
			case BasicIns::u64:\
				METHOD<unsigned __int64>(env);\
				break;\
			default:\
				return BasicILStack::BadInstructionArgument;\
			}

#define CONVERT_2_INSTRUCTION(DSTTYPE)\
			switch(ins.type2)\
			{\
			case BasicIns::s8:\
				Convert_<DSTTYPE, signed __int8>(env);\
				break;\
			case BasicIns::s16:\
				Convert_<DSTTYPE, signed __int16>(env);\
				break;\
			case BasicIns::s32:\
				Convert_<DSTTYPE, signed __int32>(env);\
				break;\
			case BasicIns::s64:\
				Convert_<DSTTYPE, signed __int64>(env);\
				break;\
			case BasicIns::u8:\
				Convert_<DSTTYPE, unsigned __int8>(env);\
				break;\
			case BasicIns::u16:\
				Convert_<DSTTYPE, unsigned __int16>(env);\
				break;\
			case BasicIns::u32:\
				Convert_<DSTTYPE, unsigned __int32>(env);\
				break;\
			case BasicIns::u64:\
				Convert_<DSTTYPE, unsigned __int64>(env);\
				break;\
			case BasicIns::f32:\
				Convert_<DSTTYPE, float>(env);\
				break;\
			case BasicIns::f64:\
				Convert_<DSTTYPE, double>(env);\
				break;\
			default:\
				return BasicILStack::BadInstructionArgument;\
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
				return BasicILStack::BadInstructionArgument;\
			}

/***********************************************************************
BasicILStack
***********************************************************************/
			
			BasicILStack::BasicILStack(BasicILInterpretor* _interpretor)
				:interpretor(_interpretor)
			{
				env=new BasicILEnv(interpretor->stackSize);
				instruction=-1;
				insKey=-1;
				foreignFunctionIndex=-1;
				foreignFunctionResult=0;
			}

			BasicILStack::~BasicILStack()
			{
				delete env;
			}

			BasicILEnv* BasicILStack::GetEnv()
			{
				return env;
			}

			void BasicILStack::Reset(vint entryInstruction, vint entryInsKey, vint returnSize)
			{
				// reserve returnSize
				// push returnPointer
				// push returnInsKey
				// push returnInstruction
				// push returnStackBase
				void* returnPointer=env->Reserve(returnSize);
				ResetBuffer(entryInstruction, entryInsKey, returnPointer);
			}

			void BasicILStack::ResetBuffer(vint entryInstruction, vint entryInsKey, void* returnPointer)
			{
				env->Push<void*>(returnPointer);
				env->Push<vint>(-2);
				env->Push<vint>(-1);
				env->Push<vint>(env->StackSize());
				env->SetBase(env->StackTop());
				instruction=entryInstruction;
				insKey=entryInsKey;
				foreignFunctionIndex=-1;
				foreignFunctionResult=0;
			}

			vint BasicILStack::GetInstruction()
			{
				return instruction;
			}

			vint BasicILStack::GetForeignFunctionIndex()
			{
				return foreignFunctionIndex;
			}

			void* BasicILStack::GetForeignFunctionResult()
			{
				return foreignFunctionResult;
			}

#define DO_NOT_MOVE_TO_NEXT_INSTRUCTION		\
			nextInstruction=instruction;	\
			nextInsKey=insKey;

			BasicILStack::RunningResult BasicILStack::Run()
			{
				try
				{
					while(instruction!=-1)
					{
						if(insKey<0 || insKey>=interpretor->ils.Count() || interpretor->ils[insKey]==0)
						{
							return BasicILStack::InstructionIndexOutOfRange;
						}
						if(instruction<0||instruction>=interpretor->ils[insKey]->instructions.Count())
						{
							return BasicILStack::InstructionIndexOutOfRange;
						}
						vint nextInstruction=instruction+1;
						vint nextInsKey=insKey;
						BasicIns& ins=interpretor->ils[insKey]->instructions[instruction];
						switch(ins.opcode)
						{
						case BasicIns::generic_pushdata:
							{
								ins.opcode=BasicIns::push;
								ins.type1=BasicIns::pointer_type;

								vint index=interpretor->RegisterTarget(0, interpretor->ils[ins.insKey], ins.argument.int_value);
								BasicILGenericTarget* target=interpretor->genericTargets[index].Obj();
								ins.argument.pointer_value=interpretor->InstanciateGenericVariable(target);

								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_callfunc:
							{
								ins.opcode=BasicIns::generic_callfunc_vm;
								ins.argument.int_value=interpretor->RegisterTarget(0, interpretor->ils[insKey], ins.argument.int_value);

								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_instance_callfunc:
							{
								bool isGenericFunction=false;
								vint index=interpretor->RegisterInstanceFunction(0, interpretor->ils[insKey], ins.argument.int_value, isGenericFunction);
								if(isGenericFunction)
								{
									ins.opcode=BasicIns::generic_callfunc_vm;
									ins.argument.int_value=index;
								}
								else
								{
									ins.opcode=BasicIns::call;
									BasicILLabel& label=interpretor->labels[index];
									ins.insKey=label.key;
									ins.argument.int_value=label.instruction;
								}

								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_callfunc_vm:
							{
								BasicILGenericTarget* target=interpretor->genericTargets[ins.argument.int_value].Obj();
								vint labelIndex=interpretor->InstanciateGenericFunction(target);

								BasicIns& theIns=interpretor->ils[insKey]->instructions[instruction];
								BasicILLabel& label=interpretor->labels[labelIndex];
								theIns.opcode=BasicIns::call;
								theIns.insKey=label.key;
								theIns.argument.int_value=label.instruction;
								
								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_pushfunc:
							{
								ins.opcode=BasicIns::generic_pushfunc_vm;
								ins.argument.int_value=interpretor->RegisterTarget(0, interpretor->ils[insKey], ins.argument.int_value);
								
								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_instance_pushfunc:
							{
								bool isGenericFunction=false;
								vint index=interpretor->RegisterInstanceFunction(0, interpretor->ils[insKey], ins.argument.int_value, isGenericFunction);
								if(isGenericFunction)
								{
									ins.opcode=BasicIns::generic_pushfunc_vm;
									ins.argument.int_value=index;
								}
								else
								{
									ins.opcode=BasicIns::call;
									BasicILLabel& label=interpretor->labels[index];
									ins.insKey=label.key;
									ins.argument.int_value=label.instruction;
								}

								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
						case BasicIns::generic_pushfunc_vm:
							{
								BasicILGenericTarget* target=interpretor->genericTargets[ins.argument.int_value].Obj();
								vint labelIndex=interpretor->InstanciateGenericFunction(target);

								BasicIns& theIns=interpretor->ils[insKey]->instructions[instruction];
								theIns.opcode=BasicIns::pushlabel;
								theIns.argument.int_value=labelIndex;
								
								DO_NOT_MOVE_TO_NEXT_INSTRUCTION
							}
							break;
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
								return BasicILStack::BadInstructionArgument;
							}
							break;
						case BasicIns::pushins:
							env->Push<vint>(ins.insKey);
							env->Push<vint>(ins.argument.int_value);
							break;
						case BasicIns::pushlabel:
							env->Push<vint>(ins.argument.int_value);
							break;
						case BasicIns::label:
							{
								vint index=env->Pop<vint>();
								if(index>=0 && index<interpretor->labels.Count())
								{
									BasicILLabel label=interpretor->labels[index];
									env->Push<vint>(label.key);
									env->Push<vint>(label.instruction);
								}
								else
								{
									return BasicILStack::BadInstructionArgument;
								}
							}
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
						case BasicIns::neg:
							SIGNED_NUMERIC_INSTRUCTION(Neg_)
							break;
						case BasicIns::and:
							INTEGER_INSTRUCTION(And_)
							break;
						case BasicIns::or:
							INTEGER_INSTRUCTION(Or_)
							break;
						case BasicIns::xor:
							INTEGER_INSTRUCTION(Xor_)
							break;
						case BasicIns::not:
							INTEGER_INSTRUCTION(Not_)
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
						case BasicIns::readmem:
							{
								vint size=ins.argument.int_value;
								void* src=env->Pop<void*>();
								env->Reserve(size);
								void* dst=env->DereferenceStack(env->StackTop());
								memcpy(dst, src, size);
							}
							break;
						case BasicIns::writemem:
							{
								vint size=ins.argument.int_value;
								void* dst=env->Pop<void*>();
								void* src=env->DereferenceStack(env->StackTop());
								memcpy(dst, src, size);
								env->Reserve(-size);
							}
							break;
						case BasicIns::copymem:
							{
								vint size=ins.argument.int_value;
								void* dst=env->Pop<void*>();
								void* src=env->Pop<void*>();
								memcpy(dst, src, size);
							}
							break;
						case BasicIns::jump:
							nextInstruction=ins.argument.int_value;
							nextInsKey=ins.insKey;
							break;
						case BasicIns::jumptrue:
							if(env->Pop<bool>())
							{
								nextInstruction=ins.argument.int_value;
								nextInsKey=ins.insKey;
							}
							break;
						case BasicIns::jumpfalse:
							if(!env->Pop<bool>())
							{
								nextInstruction=ins.argument.int_value;
								nextInsKey=ins.insKey;
							}
							break;
						case BasicIns::call:
							env->Push<vint>(insKey);
							env->Push<vint>(nextInstruction);
							env->Push<vint>(env->StackBase());
							env->SetBase(env->StackTop());
							nextInstruction=ins.argument.int_value;
							nextInsKey=ins.insKey;
							break;
						case BasicIns::call_indirect:
							{
								vint pushins=env->Pop<vint>();
								vint pushkey=env->Pop<vint>();
								env->Push<vint>(insKey);
								env->Push<vint>(nextInstruction);
								env->Push<vint>(env->StackBase());
								env->SetBase(env->StackTop());
								nextInstruction=pushins;
								nextInsKey=pushkey;
							}
							break;
						case BasicIns::call_foreign:
							foreignFunctionIndex=ins.argument.int_value;
							foreignFunctionResult=env->Pop<void*>();
							return BasicILStack::ForeignFunctionCall;
						case BasicIns::call_raw:
							{
								void* arguments=env->DereferenceStack(env->StackTop());
								vint size=ins.argument.raw_function(arguments);
								env->Reserve(-size);
							}
							break;
						case BasicIns::convert:
							CONVERT_INSTRUCTION
							break;
						case BasicIns::stack_offset:
							{
								vint stackBase=env->StackBase();
								env->Push<void*>(env->DereferenceStack(stackBase+ins.argument.int_value));
							}
							break;
						case BasicIns::stack_top:
							{
								vint stackTop=env->StackTop();
								env->Push<void*>(env->DereferenceStack(stackTop+ins.argument.int_value));
							}
							break;
						case BasicIns::stack_reserve:
							env->Reserve(ins.argument.int_value);
							break;
						case BasicIns::resptr:
							{
								void* pointer=*(void**)(env->DereferenceStack(env->StackBase()+3*sizeof(void*)));
								env->Push<void*>(pointer);
							}
							break;
						case BasicIns::ret:
							{
								vint stackBase=env->Pop<vint>();
								vint returnInstruction=env->Pop<vint>();
								vint returnInsKey=env->Pop<vint>();
								env->Pop<void*>();
								env->SetBase(stackBase);
								env->Reserve(-ins.argument.int_value);
								nextInstruction=returnInstruction;
								nextInsKey=returnInsKey;
							}
							break;
						default:
							return BasicILStack::UnknownInstruction;
						}
						instruction=nextInstruction;
						insKey=nextInsKey;
					}
					return BasicILStack::Finished;
				}
				catch(const ILException& e)
				{
					return e.result;
				}
			}
		}
	}
}
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
					throw ILException(ILException::DividByZero);
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
				return ILException::BadInstructionArgument;\
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
				return ILException::BadInstructionArgument;\
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
				return ILException::BadInstructionArgument;\
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
				return ILException::BadInstructionArgument;\
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
				return ILException::BadInstructionArgument;\
			}

/***********************************************************************
BasicILStack
***********************************************************************/
			
			BasicILStack::BasicILStack(BasicILInterpretor* _interpretor)
				:interpretor(_interpretor)
				,needToPause(false)
			{
				env=new BasicILEnv(interpretor->GetStackSize());
				env->ReserveTop(BasicILStack::ExceptionObjectOffset);
				instruction=-1;
				insKey=-1;
				userData=0;
			}

			BasicILStack::~BasicILStack()
			{
				delete env;
			}

			BasicILEnv* BasicILStack::GetEnv()
			{
				return env;
			}

			BasicILInterpretor* BasicILStack::GetInterpretor()
			{
				return interpretor;
			}

			vint BasicILStack::GetInstruction()
			{
				return instruction;
			}

			vint BasicILStack::GetInsKey()
			{
				return insKey;
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
				SetExceptionHandler(0);

				instruction=entryInstruction;
				insKey=entryInsKey;
			}

			void BasicILStack::Pause()
			{
				needToPause=true;
			}

#define DO_NOT_MOVE_TO_NEXT_INSTRUCTION		\
			nextInstruction=instruction;	\
			nextInsKey=insKey;

			ILException::RunningResult BasicILStack::Run()
			{
				needToPause=false;
				try
				{
					while(instruction!=-1)
					{
						if(needToPause)
						{
							return ILException::Paused;
						}
						else if(!interpretor->Symbols()->IsValidILIndex(insKey))
						{
							return ILException::InstructionIndexOutOfRange;
						}
						BasicIL* il=interpretor->Symbols()->GetIL(insKey);
						if(instruction<0||instruction>=il->instructions.Count())
						{
							return ILException::InstructionIndexOutOfRange;
						}
						vint nextInstruction=instruction+1;
						vint nextInsKey=insKey;
						BasicIns& ins=il->instructions[instruction];
						switch(ins.opcode)
						{
						case BasicIns::generic_pushdata:
						case BasicIns::generic_callfunc:
						case BasicIns::generic_pushfunc:
						case BasicIns::generic_instance_callfunc:
						case BasicIns::generic_instance_pushfunc:
						case BasicIns::generic_callfunc_vm:
						case BasicIns::generic_pushfunc_vm:
							{
								BasicILLock::Scope scope(interpretor->GetLock());
								interpretor->Expander()->RewriteExecutingGenericInstruction(ins, il, instruction);
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
								return ILException::BadInstructionArgument;
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
								BasicILLock::Scope scope(interpretor->GetLock());
								vint index=env->Pop<vint>();
								if(interpretor->Symbols()->IsValidLabelIndex(index))
								{
									BasicILLabel label=interpretor->Symbols()->GetLabel(index);
									env->Push<vint>(label.key);
									env->Push<vint>(label.instruction);
								}
								else
								{
									return ILException::BadInstructionArgument;
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
							{
								switch(ins.insKey)
								{
								case BasicILRuntimeSymbol::LightFunctionSitingAssemblyKey:
									InvokeLightFunction(ins.argument.int_value);
									break;
								default:
									env->Push<vint>(insKey);
									env->Push<vint>(nextInstruction);
									env->Push<vint>(env->StackBase());
									env->SetBase(env->StackTop());
									nextInstruction=ins.argument.int_value;
									nextInsKey=ins.insKey;
								}
							}
							break;
						case BasicIns::call_indirect:
							{
								vint pushins=env->Pop<vint>();
								vint pushkey=env->Pop<vint>();
								switch(pushkey)
								{
								case BasicILRuntimeSymbol::LightFunctionSitingAssemblyKey:
									InvokeLightFunction(pushins);
									break;
								default:
									env->Push<vint>(insKey);
									env->Push<vint>(nextInstruction);
									env->Push<vint>(env->StackBase());
									env->SetBase(env->StackTop());
									nextInstruction=pushins;
									nextInsKey=pushkey;
								}
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
						case BasicIns::stack_data:
							env->Push<void*>(env->DereferenceStack(0));
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
						case BasicIns::exception_handler_push:
							{
								BasicILExceptionHandler* handler=(BasicILExceptionHandler*)env->Reserve(sizeof(BasicILExceptionHandler));
								handler->key=insKey;
								handler->instruction=ins.argument.int_value;
								handler->stackBase=env->StackBase();
								handler->stackTop=env->StackTop();
								handler->previous=GetExceptionHandler();
								SetExceptionHandler(handler);
							}
							break;
						case BasicIns::exception_handler_pop:
							{
								BasicILExceptionHandler* stackHandler=(BasicILExceptionHandler*)env->DereferenceStack(env->StackTop());
								BasicILExceptionHandler* registeredHandler=GetExceptionHandler();
								if(stackHandler!=registeredHandler)
								{
									return ILException::BadInstructionArgument;
								}
								SetExceptionHandler(registeredHandler->previous);
								env->Reserve(-(vint)sizeof(BasicILExceptionHandler));
							}
							break;
						case BasicIns::exception_object_reserve:
							{
								env->ReserveTop(BasicILStack::ExceptionObjectOffset+ins.argument.int_value);
							}
							break;
						case BasicIns::exception_object_address:
							{
								void* buffer=env->DereferenceStack(BasicILStack::ExceptionObjectOffset);
								env->Push<void*>(buffer);
							}
							break;
						case BasicIns::exception_raise:
							{
								BasicILExceptionHandler* handler=GetExceptionHandler();
								if(handler)
								{
									env->SetBase(handler->stackBase);
									env->SetTop(handler->stackTop);
									nextInstruction=handler->instruction;
									nextInsKey=handler->key;
								}
								else
								{
									return ILException::UnhandledException;
								}
							}
							break;
						default:
							return ILException::UnknownInstruction;
						}
						instruction=nextInstruction;
						insKey=nextInsKey;
					}
					return ILException::Finished;
				}
				catch(const ILException& e)
				{
					return e.result;
				}
			}

			BasicILExceptionHandler* BasicILStack::GetExceptionHandler()
			{
				return *(BasicILExceptionHandler**)env->DereferenceStack(BasicILStack::ExceptionHandlerOffset);
			}

			void BasicILStack::SetExceptionHandler(BasicILExceptionHandler* handler)
			{
				*((BasicILExceptionHandler**)env->DereferenceStack(BasicILStack::ExceptionHandlerOffset))=handler;
			}

			void BasicILStack::InvokeLightFunction(vint index)
			{
				void* stackTop=env->DereferenceStack(env->StackTop());
				void* result=((void**)stackTop)[0];
				void* arguments=&((void**)stackTop)[1];
				BasicILLightFunction function=interpretor->Symbols()->GetLightFunction(index);
				vint argumentSize=function.handler(result, arguments, interpretor, this, function.userData);
				env->Reserve(-(vint)(sizeof(void*)+argumentSize));
			}

			void* BasicILStack::GetUserData()
			{
				return userData;
			}

			void BasicILStack::SetUserData(void* data)
			{
				userData=data;
			}
		}
	}
}
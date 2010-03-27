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
				BasicILStack::RunningResult result;

				ILException(BasicILStack::RunningResult _result)
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
					throw ILException(BasicILStack::StackOverflow);
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
					throw ILException(BasicILStack::StackOverflow);
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
BasicILLabel
***********************************************************************/

			bool BasicILLabel::operator==(const BasicILLabel& label)const
			{
				return key==label.key && instruction==label.instruction;
			}

			bool BasicILLabel::operator!=(const BasicILLabel& label)const
			{
				return key!=label.key || instruction!=label.instruction;
			}

/***********************************************************************
BasicILInterpretor
***********************************************************************/

			BasicILInterpretor::BasicILInterpretor(int _stackSize)
				:stackSize(_stackSize)
			{
				ils=0;
				ilCount=0;

				BasicILLabel label;
				label.key=-1;
				label.instruction=-1;
				labels.Add(label);
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
				if(ils)
				{
					delete[] ils;
				}
			}

			int BasicILInterpretor::LoadIL(BasicIL* il)
			{
				BasicIL** newils=new BasicIL*[ilCount+1];
				if(ils)
				{
					memcpy(newils, ils, ilCount*sizeof(BasicIL*));
					delete[] ils;
				}
				ils=newils;
				ils[ilCount]=il;

				int functionPointerOffset=labels.Count();
				for(int i=0;i<il->labels.Count();i++)
				{
					BasicILLabel label;
					label.key=ilCount;
					label.instruction=il->labels[i].instructionIndex;
					labels.Add(label);
				}
				for(int i=0;i<il->instructions.Count();i++)
				{
					BasicIns& ins=il->instructions[i];
					switch(ins.opcode)
					{
					case BasicIns::link_pushdata:
						ins.opcode=BasicIns::push;
						ins.type1=BasicIns::pointer_type;
						ins.argument.pointer_value=&(il->globalData[0])+ins.argument.int_value;
						break;
					case BasicIns::link_pushfunc:
						ins.opcode=BasicIns::pushlabel;
						ins.argument.int_value+=functionPointerOffset;
						break;
					}
				}

				return ilCount++;
			}

			void BasicILInterpretor::UnloadIL(BasicIL* il)
			{
				for(int i=0;i<ilCount;i++)
				{
					if(ils[i]==il)
					{
						ils[i]=0;
					}
				}
			}

			collections::IList<BasicILLabel>& BasicILInterpretor::GetLabels()
			{
				return labels.Wrap();
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

			void BasicILStack::Reset(int entryInstruction, int entryInsKey, int returnSize)
			{
				// reserve returnSize
				// push returnPointer
				// push returnInsKey
				// push returnInstruction
				// push returnStackBase
				void* returnPointer=env->Reserve(returnSize);
				env->Push<void*>(returnPointer);
				env->Push<int>(-2);
				env->Push<int>(-1);
				env->Push<int>(env->StackSize());
				env->SetBase(env->StackTop());
				instruction=entryInstruction;
				insKey=entryInsKey;
				foreignFunctionIndex=-1;
				foreignFunctionResult=0;
			}

			int BasicILStack::GetInstruction()
			{
				return instruction;
			}

			int BasicILStack::GetForeignFunctionIndex()
			{
				return foreignFunctionIndex;
			}

			void* BasicILStack::GetForeignFunctionResult()
			{
				return foreignFunctionResult;
			}

			BasicILStack::RunningResult BasicILStack::Run()
			{
				try
				{
					while(instruction!=-1)
					{
						if(insKey<0 || insKey>=interpretor->ilCount || interpretor->ils[insKey]==0)
						{
							return BasicILStack::InstructionIndexOutOfRange;
						}
						if(instruction<0||instruction>=interpretor->ils[insKey]->instructions.Count())
						{
							return BasicILStack::InstructionIndexOutOfRange;
						}
						int nextInstruction=instruction+1;
						int nextInsKey=insKey;
						BasicIns& ins=interpretor->ils[insKey]->instructions[instruction];
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
								return BasicILStack::BadInstructionArgument;
							}
							break;
						case BasicIns::pushins:
							env->Push<int>(ins.insKey);
							env->Push<int>(ins.argument.int_value);
							break;
						case BasicIns::pushlabel:
							env->Push<int>(ins.argument.int_value);
							break;
						case BasicIns::label:
							{
								int index=env->Pop<int>();
								if(index>=0 && index<interpretor->labels.Count())
								{
									BasicILLabel label=interpretor->labels[index];
									env->Push<int>(label.key);
									env->Push<int>(label.instruction);
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
								int size=ins.argument.int_value;
								void* src=env->Pop<void*>();
								env->Reserve(size);
								void* dst=env->DereferenceStack(env->StackTop());
								memcpy(dst, src, size);
							}
							break;
						case BasicIns::writemem:
							{
								int size=ins.argument.int_value;
								void* dst=env->Pop<void*>();
								void* src=env->DereferenceStack(env->StackTop());
								memcpy(dst, src, size);
								env->Reserve(-size);
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
							env->Push<int>(insKey);
							env->Push<int>(nextInstruction);
							env->Push<int>(env->StackBase());
							env->SetBase(env->StackTop());
							nextInstruction=ins.argument.int_value;
							nextInsKey=ins.insKey;
							break;
						case BasicIns::call_indirect:
							{
								int pushins=env->Pop<int>();
								int pushkey=env->Pop<int>();
								env->Push<int>(insKey);
								env->Push<int>(nextInstruction);
								env->Push<int>(env->StackBase());
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
								int size=ins.argument.raw_function(arguments);
								env->Reserve(-size);
							}
							break;
						case BasicIns::convert:
							CONVERT_INSTRUCTION
							break;
						case BasicIns::stack_offset:
							{
								int stackBase=env->StackBase();
								env->Push<void*>(env->DereferenceStack(stackBase+ins.argument.int_value));
							}
							break;
						case BasicIns::stack_top:
							{
								int stackTop=env->StackTop();
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
								int stackBase=env->Pop<int>();
								int returnInstruction=env->Pop<int>();
								int returnInsKey=env->Pop<int>();
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
						if(nextInsKey!=-1)
						{
							insKey=nextInsKey;
						}
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
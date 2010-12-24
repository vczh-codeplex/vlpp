/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
  BasicFunctionExecotor						£ºBasic Script Languageº¯Êý¶ÔÏó
***********************************************************************/
#ifndef VCZH_SCRIPTING_LANGUAGES_BASICFUNCTIONEXECUTOR
#define VCZH_SCRIPTING_LANGUAGES_BASICFUNCTIONEXECUTOR
#include "LanguageRuntime.h"
namespace vl
{
	namespace scripting
	{
		template<typename T>
		class BasicFunctionExecutor
		{
		};
 
/***********************************************************************
vl::Func<R()>
***********************************************************************/
		template<typename R >
		class BasicFunctionExecutor<R()> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType();
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R()>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R()>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()()const
			{
				R resultValue;

				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void()>
***********************************************************************/
		template< >
		class BasicFunctionExecutor<void()> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType();
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void()>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void()>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()()const
			{
				 

				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0)>
***********************************************************************/
		template<typename R,typename T0>
		class BasicFunctionExecutor<R(T0)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0)const
			{
				R resultValue;
				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0)>
***********************************************************************/
		template< typename T0>
		class BasicFunctionExecutor<void(T0)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0)const
			{
				 
				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1)>
***********************************************************************/
		template<typename R,typename T0,typename T1>
		class BasicFunctionExecutor<R(T0,T1)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1)const
			{
				R resultValue;
				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1)>
***********************************************************************/
		template< typename T0,typename T1>
		class BasicFunctionExecutor<void(T0,T1)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1)const
			{
				 
				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2>
		class BasicFunctionExecutor<R(T0,T1,T2)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2)const
			{
				R resultValue;
				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2)>
***********************************************************************/
		template< typename T0,typename T1,typename T2>
		class BasicFunctionExecutor<void(T0,T1,T2)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2)const
			{
				 
				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3>
		class BasicFunctionExecutor<R(T0,T1,T2,T3)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
			{
				R resultValue;
				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3>
		class BasicFunctionExecutor<void(T0,T1,T2,T3)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
			{
				 
				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
			{
				R resultValue;
				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
			{
				 
				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4,T5);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
			{
				R resultValue;
				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4,T5);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
			{
				 
				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
			{
				R resultValue;
				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
			{
				 
				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
			{
				R resultValue;
				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
			{
				 
				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
			{
				R resultValue;
				state->Push<T8>(p8);
 				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
			{
				 
				state->Push<T8>(p8);
 				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef R ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
			{
				R resultValue;
				state->Push<T9>(p9);
 				state->Push<T8>(p8);
 				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, (void*)&resultValue);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				return resultValue;
			}
			operator bool()const
			{
				return state;
			}
		};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object
		{
		protected:
			BasicDeclarationInfo				declaration;
			Ptr<LanguageState>					state;
		public:
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef void ResultType;
			BasicFunctionExecutor(const BasicDeclarationInfo& _declaration, Ptr<LanguageState> _state)
				:declaration(_declaration)
				,state(_state)
			{
			}
			bool operator==(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
			{
				return declaration.IsSameRecord(function.declaration) && state==function.state;
			}
			bool operator!=(const BasicFunctionExecutor<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
			{
				return !declaration.IsSameRecord(function.declaration) || state!=function.state;
			}
			void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
			{
				 
				state->Push<T9>(p9);
 				state->Push<T8>(p8);
 				state->Push<T7>(p7);
 				state->Push<T6>(p6);
 				state->Push<T5>(p5);
 				state->Push<T4>(p4);
 				state->Push<T3>(p3);
 				state->Push<T2>(p2);
 				state->Push<T1>(p1);
 				state->Push<T0>(p0);
 
				state->PrepareToRun(declaration, 0);
				basicil::ILException::RunningResult result=state->Run();
				if(result!=basicil::ILException::Finished)
				{
					throw basicil::ILException(result);
				}
				 
			}
			operator bool()const
			{
				return state;
			}
		};
 
	}
}
#endif
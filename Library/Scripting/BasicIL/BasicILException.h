/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:
  ILException						£ºÔËÐÐÊ±´íÎó
  ILLinkerException					£ºÁ´½Ó´íÎó
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILEXCEPTION
#define VCZH_SCRIPTING_BASICIL_BASICILEXCEPTION

#include "..\..\Exception.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
Òì³£
***********************************************************************/

			class ILException : public Exception
			{
			public:
				enum RunningResult
				{
					Finished,
					Paused,
					StackOverflow,
					DividByZero,
					AccessViolation,
					InstructionIndexOutOfRange,
					UnknownInstruction,
					BadInstructionArgument,
					UnhandledException,
				};
			private:
				static WString				GetExceptionMessage(RunningResult result);
			public:
				RunningResult result;

				ILException(RunningResult _result)
					:Exception(GetExceptionMessage(_result))
					,result(_result)
				{
				}
			};

			class ILLinkerException : public Exception
			{
			public:
				enum ErrorType
				{
					DuplicatedAssemblyName,
					AssemblyNotExists,
					DuplicatedSymbolName,
					SymbolNotExists,
					SymbolNotALabel,
					DuplicatedInstance,
					ForeignFunctionNotExists,
				};
			private:
				static WString				GetExceptionMessage(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName);
			public:
				ErrorType					errorType;
				WString						assemblyName;
				WString						symbolName;

				ILLinkerException(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName)
					:Exception(GetExceptionMessage(_errorType, _assemblyName, _symbolName))
					,errorType(_errorType)
					,assemblyName(_assemblyName)
					,symbolName(_symbolName)
				{
				}
			};
		}
	}
}

#endif
#include "BasicILException.h"
#include "..\Languages\BasicErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
ILException
***********************************************************************/
			
			WString ILException::GetExceptionMessage(RunningResult result)
			{
				switch(result)
				{
				case StackOverflow:
					return basiclanguage::BasicErrorMessage::ILExceptionStackOverflow();
				case DividByZero:
					return basiclanguage::BasicErrorMessage::ILExceptionDividByZero();
				case AccessViolation:
					return basiclanguage::BasicErrorMessage::ILExceptionAccessViolation();
				case InstructionIndexOutOfRange:
					return basiclanguage::BasicErrorMessage::ILExceptionInstructionIndexOutOfRange();
				case UnknownInstruction:
					return basiclanguage::BasicErrorMessage::ILExceptionUnknownInstruction();
				case BadInstructionArgument:
					return basiclanguage::BasicErrorMessage::ILExceptionBadInstructionArgument();
				case UnhandledException:
					return basiclanguage::BasicErrorMessage::ILExceptionUnhandledException();
				default:
					return L"";
				}
			}

/***********************************************************************
ILLinkerException
***********************************************************************/
			
			WString ILLinkerException::GetExceptionMessage(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName)
			{
				switch(_errorType)
				{
				case DuplicatedAssemblyName:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedAssemblyName(_assemblyName);
				case AssemblyNotExists:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionAssemblyNotExists(_assemblyName);
				case DuplicatedSymbolName:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedSymbolName(_assemblyName, _symbolName);
				case SymbolNotExists:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionSymbolNotExists(_assemblyName, _symbolName);
				case SymbolNotALabel:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionSymbolNotALabel(_assemblyName, _symbolName);
				case DuplicatedInstance:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionDuplicatedInstance(_assemblyName);
				case ForeignFunctionNotExists:
					return basiclanguage::BasicErrorMessage::ILLinkerExceptionForeignFunctionNotExists(_assemblyName, _symbolName);
				default:
					return L"";
				}
			}
		}
	}
}
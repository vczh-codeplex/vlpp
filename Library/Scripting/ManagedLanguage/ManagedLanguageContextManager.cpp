#include "ManagedLanguageContextManager.h"
#include "ManagedLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedContextManager
***********************************************************************/

			ManagedContextManager::ManagedContextManager()
			{
				memset(&predefinedTypes, 0, sizeof(predefinedTypes));
			}

			ManagedContextManager::~ManagedContextManager()
			{
			}

/***********************************************************************
InitializeContextManager
***********************************************************************/

			void InitializeContextManager(ManagedProgram* program, const MAP& argument)
			{
				ManagedContextManager::PredefinedTypes& pt=argument.contextManager->predefinedTypes;
				pt.sint8			=GetSystemType(program, L"SInt8", argument);
				pt.sint16			=GetSystemType(program, L"SInt16", argument);
				pt.sint32			=GetSystemType(program, L"SInt32", argument);
				pt.sint64			=GetSystemType(program, L"SInt64", argument);
				pt.uint8			=GetSystemType(program, L"UInt8", argument);
				pt.uint16			=GetSystemType(program, L"UInt16", argument);
				pt.uint32			=GetSystemType(program, L"UInt32", argument);
				pt.uint64			=GetSystemType(program, L"UInt64", argument);
				pt.singleType		=GetSystemType(program, L"Single", argument);
				pt.doubleType		=GetSystemType(program, L"Double", argument);
				pt.charType			=GetSystemType(program, L"Char", argument);
				pt.boolType			=GetSystemType(program, L"Bool", argument);
				pt.stringType		=GetSystemType(program, L"String", argument);
				pt.typeType			=GetSystemType(program, L"Reflection", L"Type", argument);
				pt.attributeType	=GetSystemType(program, L"Attribute", argument);
			}
		}
	}
}
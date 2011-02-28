#define _CRT_SECURE_NO_WARNINGS
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\BasicFunctionExecutor.h"
#include "..\..\Library\Scripting\Utility\ScriptingUtilityMake.h"

#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\MemoryStream.h"
#include "..\..\Library\Stream\Accessor.h"
#include "..\..\Library\Stream\CharFormat.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::collections;
using namespace vl::scripting::basicil;
using namespace vl::scripting::utility;
using namespace vl::stream;

extern WString GetPath();

namespace TestScriptingUtilityHelper
{
	void CompileAssembly(LanguageMaker& maker, LanguageMakeFile& makeFile, const WString& basePath, const WString& fileName)
	{
		WString path=basePath+fileName;
		FileStream fileStream(path, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader reader(decoderStream);
		makeFile.Load(basePath, reader);
		maker.Execute(makeFile);
	}

	Ptr<LanguageAssembly> LoadAssembly(LanguageHost& host, const WString& fileName)
	{
		FileStream stream(fileName, FileStream::ReadOnly);
		Ptr<LanguageAssembly> assembly=new LanguageAssembly(stream);
		TEST_ASSERT(host.LoadAssembly(assembly));
		return assembly;
	}

	void __stdcall UnitTestPluginPrinter(bool condition, wchar_t* description)
	{
		if(condition)
		{
			vl::unittest::UnitTest::PrintInfo(L"        PASS: "+WString(description));
		}
		else
		{
			vl::unittest::UnitTest::PrintError(L"        FAIL: "+WString(description));
			throw 0;
		}
	}

	void __stdcall ConsolePluginReader(wchar_t* text)
	{
		wcscpy(text, L"TextFromConsole");
		vl::unittest::UnitTest::PrintInfo(L"        CONSOLE.READ: "+WString(text));
	}

	void __stdcall ConsolePluginWriter(wchar_t* text)
	{
		vl::unittest::UnitTest::PrintInfo(L"        CONSOLE.WRITE: "+WString(text));
	}
};
using namespace TestScriptingUtilityHelper;

TEST_CASE(TestScriptingUtility_System_CoreNative)
{
	WString basePath=GetPath()+L"..\\ScriptCoreLibrary\\";
	LanguageMaker maker;

	LanguageMakeFile syscrnat;
	CompileAssembly(maker, syscrnat, basePath, L"System.CoreNative.Make.txt");

	LanguageMakeFile sysutnat;
	CompileAssembly(maker, sysutnat, basePath, L"System.UnitTestNative.Make.txt");

	LanguageMakeFile syscsnat;
	CompileAssembly(maker, syscsnat, basePath, L"System.ConsoleNative.Make.txt");

	LanguageMakeFile testCoreNative;
	CompileAssembly(maker, testCoreNative, basePath, L"Test.CoreNative.Make.txt");

	{
		LanguageHost host(65536);
		host.RegisterPlugin(CreateMemoryManagerPlugin());
		host.RegisterPlugin(CreateUnitTestPlugin(UnitTestPluginPrinter));
		host.RegisterPlugin(CreateConsolePlugin(ConsolePluginReader, ConsolePluginWriter));
		host.RegisterPlugin(CreateThreadingPlugin());
		host.RegisterPlugin(CreateStdlibPlugin());
		host.RegisterPlugin(CreateGcSingleThreadPlugin());
		Ptr<LanguageAssembly> syscrnatAssembly=LoadAssembly(host, basePath+syscrnat.assembly.value);
		Ptr<LanguageAssembly> sysutnatAssembly=LoadAssembly(host, basePath+sysutnat.assembly.value);
		Ptr<LanguageAssembly> syscsnatAssembly=LoadAssembly(host, basePath+syscsnat.assembly.value);
		Ptr<LanguageAssembly> unitTestAssembly=LoadAssembly(host, basePath+testCoreNative.assembly.value);

		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(syscrnatAssembly)==ILException::Finished);
		TEST_ASSERT(state->RunInitialization(sysutnatAssembly)==ILException::Finished);
		TEST_ASSERT(state->RunInitialization(syscsnatAssembly)==ILException::Finished);
		TEST_ASSERT(state->RunInitialization(unitTestAssembly)==ILException::Finished);

		BasicLanguageMetadata* metadata=unitTestAssembly->GetBasicLanguageMetadata();
		for(vint i=0;i<metadata->GetDeclarationCount();i++)
		{
			BasicDeclarationInfo declaration=metadata->GetDeclaration(i);
			if(declaration.IsFunction() && declaration.GetName().Length()>9 && declaration.GetName().Left(9)==L"TestCase_")
			{
				vl::unittest::UnitTest::PrintInfo(L"    "+declaration.GetName());
				BasicFunctionExecutor<void()> function(declaration, state);
				function();
			}
		}
	}
}
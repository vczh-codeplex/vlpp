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
}
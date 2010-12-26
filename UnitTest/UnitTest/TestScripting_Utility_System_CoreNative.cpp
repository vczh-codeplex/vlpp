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

TEST_CASE(TestScriptingUtility_System_CoreNative)
{
	WString basePath=GetPath()+L"..\\ScriptCoreLibrary\\";
	LanguageMakeFile makeFile;
	{
		WString path=basePath+L"System.CoreNative.Make.txt";
		FileStream fileStream(path, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader reader(decoderStream);
		makeFile.Load(basePath, reader);
	}
	LanguageMaker maker;
	maker.Execute(makeFile);
}
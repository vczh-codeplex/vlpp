#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "..\..\..\..\..\Library\Basic.h"

using namespace vl;

int wmain(vint argc , wchar_t* args[])
{
	_CrtDumpMemoryLeaks();
	return 0;
}

//------------you can organize your cases in different cpp files------------------//

#include "..\..\..\..\..\Library\UnitTest\UnitTest.h"

using namespace vl::unittest;

TEST_CASE(MyTestCase)
{
	TEST_ASSERT(true);
}

//--------tutorial-----------//

#include "..\..\..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\..\..\Library\Stream\Accessor.h"
#include "..\..\..\..\..\Library\Stream\CharFormat.h"

using namespace vl::stream;

void ReadFile()
{
	FileStream fileStream(L"input.txt", FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);

	reader.ReadLine();
	reader.ReadString(10);
	reader.ReadToEnd();
}

void WriteFile()
{
	FileStream fileStream(L"output.txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);

	writer.WriteChar(L'a');
	writer.WriteString(L"a");
	writer.WriteLine(L"a");
}
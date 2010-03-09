#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"

using namespace vl;
using namespace vl::scripting;

TEST_CASE(TestCreateNativeXProvider)
{
	Ptr<ILanguageProvider> provider=CreateNativeXLanguageProvider();
}
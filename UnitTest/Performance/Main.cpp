#include <windows.h>
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"

using namespace vl;
using namespace vl::scripting;

void LaunchNativeXLanguageProvider()
{
	CreateNativeXLanguageProvider();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LaunchNativeXLanguageProvider();
	return 0;
}
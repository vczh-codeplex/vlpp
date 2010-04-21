#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "..\Presentation\Core\GuiTypes.h"

using namespace vl;
using namespace vl::presentation;

#define RUN_MAIN(NAME)do{extern void NAME(); NAME();}while(0)

Rect GuiGetWorkArea()
{
	RECT result;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &result, 0);
	return Rect(result.left, result.top, result.right, result.bottom);
}

void NativeMain()
{
	RUN_MAIN(LayoutVisualizerMain);
	_CrtDumpMemoryLeaks();
}
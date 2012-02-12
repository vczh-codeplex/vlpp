#ifdef GACUI_EXPORTS
#define GACUI_API __declspec(dllexport)
#else
#define GACUI_API __declspec(dllimport)
#endif

extern "C"
{
	GACUI_API void SetGuiMain(void(__stdcall*guimain)());
	GACUI_API void StartGDI();
	GACUI_API void StartDirect2D();
}
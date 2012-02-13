#ifdef GACUI_EXPORTS
#define GACUI_API __declspec(dllexport)
#else
#define GACUI_API __declspec(dllimport)
#endif

namespace gacui_cpp
{
	class GacString
	{
		friend struct GacStringInternalData;
	protected:
		GacStringInternalData*			internalData;

		GacString(GacStringInternalData* _internalData);
	public:
		GacString(const wchar_t* data);
		GacString(const GacString& data);
		~GacString();

		int								Length();
		const wchar_t*					Buffer();

		int								CalculateLengthForAsciiString();
		void							FillAsciiStringBuffer(char* buffer);
	};

	template<typename T>
	class rptr{};

	template<typename T>
	class sptr{};
}

extern "C"
{
	GACUI_API void SetGuiMain(void(__stdcall*guimain)());
	GACUI_API void StartGDI();
	GACUI_API void StartDirect2D();
}
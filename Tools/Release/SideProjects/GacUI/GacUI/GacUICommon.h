#ifndef GACUI_COMMON
#define GACUI_COMMON

#ifdef GACUI_EXPORTS
#define GACUI_API __declspec(dllexport)
#else
#define GACUI_API __declspec(dllimport)
#endif

namespace gacui_cpp_1_0
{
	class GACUI_API GacString
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

	class GACUI_API GacPointerHolder
	{
	protected:
		void*							__internal_pointer_holder;

		GacPointerHolder(void* __internal_pointer_holder_input);
		~GacPointerHolder();
	public:
	};

	template<typename T>
	class rptr : private T
	{
		template<typename T>
		friend class _RptrBuilder;
	private:
		rptr(void* __internal_object_reference_input)
			:T(__internal_object_reference_input)
		{
		}
	public:
		rptr(const rptr<T>& ptr)
			:T(ptr.__internal_object_reference)
		{
		}

		T* operator->()
		{
			return this;
		}
	};

	template<typename T>
	class sptr : private T, private GacPointerHolder
	{
		template<typename T>
		friend class _SptrBuilder;
	private:
		sptr(void* __internal_object_reference_input, void* __internal_pointer_holder_input)
			:T(__internal_object_reference_input)
			,GacPointerHolder(__internal_pointer_holder_input)
		{
		}
	public:
		sptr(const rptr<T>& ptr)
			:T(ptr.__internal_object_reference)
			,GacPointerHolder(__internal_pointer_holder)
		{
		}

		T* operator->()
		{
			return this;
		}
	};
}

extern "C"
{
	GACUI_API void SetGuiMain(void(__stdcall*guimain)());
	GACUI_API void StartGDI();
	GACUI_API void StartDirect2D();
}

#endif
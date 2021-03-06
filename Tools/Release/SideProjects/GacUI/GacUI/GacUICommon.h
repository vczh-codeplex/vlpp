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

		int								Length()const;
		const wchar_t*					Buffer()const;

		int								CalculateLengthForAsciiString()const;
		void							FillAsciiStringBuffer(char* buffer)const;
	};

	class GacPointerHolderData;

	class GACUI_API GacPointerHolder
	{
	protected:
		GacPointerHolderData*			__internal_pointer_holder;

		GacPointerHolder(GacPointerHolderData* __internal_pointer_holder_input);
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
		rptr()
			:T(0)
		{
		}

		rptr(const rptr<T>& ptr)
			:T(ptr.__internal_object_reference)
		{
		}

		~rptr()
		{
			ClearInternalObjectReference();
		}

		operator bool()const
		{
			return __internal_object_reference!=0;
		}

		T* operator->()const
		{
			return const_cast<T*>(static_cast<const T*>(this));
		}
	};

	template<typename T>
	class sptr : private T, private GacPointerHolder
	{
		template<typename T>
		friend class _SptrBuilder;
	private:
		sptr(void* __internal_object_reference_input, GacPointerHolderData* __internal_pointer_holder_input)
			:T(__internal_object_reference_input)
			,GacPointerHolder(__internal_pointer_holder_input)
		{
		}
	public:
		sptr()
			:T(0)
			,GacPointerHolder(0)
		{
		}

		sptr(const rptr<T>& ptr)
			:T(ptr.__internal_object_reference)
			,GacPointerHolder(__internal_pointer_holder)
		{
		}

		operator bool()const
		{
			return __internal_object_reference!=0;
		}

		T* operator->()const
		{
			return const_cast<T*>(static_cast<const T*>(this));
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
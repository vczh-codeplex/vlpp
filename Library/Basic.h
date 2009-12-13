/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Basic

Classes:
	NotCopyable									：不可复制对象
	Error										：内部错误，检查到了不可出现的变量、参数或状态错误
	Object										：对象基类

Macros:
	CHECK_ERROR(CONDITION,DESCRIPTION)			：检查内部错误
***********************************************************************/

#ifndef VCZH_BASIC
#define VCZH_BASIC

namespace vl
{

/***********************************************************************
基础
***********************************************************************/

	class NotCopyable
	{
	private:
		NotCopyable(const NotCopyable&);
		NotCopyable& operator=(const NotCopyable&);
	public:
		NotCopyable();
	};

	class Error
	{
	private:
		wchar_t*			description;
	public:
		Error(wchar_t* _description);

		wchar_t*			Description();
	};
#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)

	class Object
	{
	public:
		virtual ~Object();
	};

	template<typename T>
	class ObjectBox : public Object
	{
	private:
		T					object;
	public:
		ObjectBox(const T& _object)
		{
			object=_object;
		}

		const T& GetObject()
		{
			return object;
		}
	};

	template<typename T, size_t minSize>
	union BinaryRetriver
	{
		T t;
		char binary[sizeof(T)>minSize?sizeof(T):minSize];
	};

/***********************************************************************
配置
***********************************************************************/

	template<typename T>
	struct KeyType
	{
	public:
		typedef T Type;
	};

	template<typename T>
	struct POD
	{
		static const bool Result=false;
	};

	template<>struct POD<bool>{static const bool Result=true;};
	template<>struct POD<unsigned __int8>{static const bool Result=true;};
	template<>struct POD<signed __int8>{static const bool Result=true;};
	template<>struct POD<unsigned __int16>{static const bool Result=true;};
	template<>struct POD<signed __int16>{static const bool Result=true;};
	template<>struct POD<unsigned __int32>{static const bool Result=true;};
	template<>struct POD<signed __int32>{static const bool Result=true;};
	template<>struct POD<unsigned __int64>{static const bool Result=true;};
	template<>struct POD<signed __int64>{static const bool Result=true;};
	template<>struct POD<char>{static const bool Result=true;};
	template<>struct POD<wchar_t>{static const bool Result=true;};
	template<typename T>struct POD<T*>{static const bool Result=true;};
	template<typename T>struct POD<T&>{static const bool Result=true;};
	template<typename T, typename C>struct POD<T C::*>{static const bool Result=true;};
	template<typename T, int _Size>struct POD<T[_Size]>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<volatile T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const volatile T>{static const bool Result=POD<T>::Result;};

	typedef signed __int64	pos_t;

/***********************************************************************
接口
***********************************************************************/

	class Interface : private NotCopyable
	{
	public:
		virtual ~Interface();
	};
}

#endif
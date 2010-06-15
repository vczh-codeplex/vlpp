#include <stdlib.h>
#include <windows.h>
#include "String.h"

namespace vl
{
	vint atoi(const AString& string)
	{
		char* endptr=0;
		return strtol(string.Buffer(), &endptr, 10);
	}

	vint wtoi(const WString& string)
	{
		wchar_t* endptr=0;
		return wcstol(string.Buffer(), &endptr, 10);
	}

	__int64 atoi64(const AString& string)
	{
		char* endptr=0;
		return _strtoi64(string.Buffer(), &endptr, 10);
	}

	__int64 wtoi64(const WString& string)
	{
		wchar_t* endptr=0;
		return _wcstoi64(string.Buffer(), &endptr, 10);
	}

	vuint atou(const AString& string)
	{
		char* endptr=0;
		return strtoul(string.Buffer(), &endptr, 10);
	}

	vuint wtou(const WString& string)
	{
		wchar_t* endptr=0;
		return wcstoul(string.Buffer(), &endptr, 10);
	}

	unsigned __int64 atou64(const AString& string)
	{
		char* endptr=0;
		return _strtoui64(string.Buffer(), &endptr, 10);
	}

	unsigned __int64 wtou64(const WString& string)
	{
		wchar_t* endptr=0;
		return _wcstoui64(string.Buffer(), &endptr, 10);
	}

	double atof(const AString& string)
	{
		char* endptr=0;
		return strtod(string.Buffer(), &endptr);
	}

	double wtof(const WString& string)
	{
		wchar_t* endptr=0;
		return wcstod(string.Buffer(), &endptr);
	}

	AString itoa(vint number)
	{
		char buffer[100];
		ITOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString itow(vint number)
	{
		wchar_t buffer[100];
		ITOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString i64toa(__int64 number)
	{
		char buffer[100];
		I64TOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString i64tow(__int64 number)
	{
		wchar_t buffer[100];
		I64TOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString utoa(vuint number)
	{
		char buffer[100];
		UITOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString utow(vuint number)
	{
		wchar_t buffer[100];
		UITOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString u64toa(unsigned __int64 number)
	{
		char buffer[100];
		UI64TOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString u64tow(unsigned __int64 number)
	{
		wchar_t buffer[100];
		UI64TOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString ftoa(double number)
	{
		char buffer[100];
		_gcvt_s(buffer, 100, number, 30);
		vint len=(vint)strlen(buffer);
		if(buffer[len-1]=='.')
		{
			buffer[len-1]='\0';
		}
		return buffer;
	}

	WString ftow(double number)
	{
		return atow(ftoa(number));
	}

	AString wtoa(const WString& string)
	{
		vint Length=WideCharToMultiByte(CP_THREAD_ACP,0,string.Buffer(),(int)string.Length(),0,0,0,0);
		char* Temp=new char[Length+1];
		WideCharToMultiByte(CP_THREAD_ACP,0,string.Buffer(),(int)string.Length(),Temp,(int)Length,0,0);
		Temp[Length]='\0';
		AString s=Temp;
		delete[] Temp;
		return s;
	}

	WString atow(const AString& string)
	{
		vint Length=MultiByteToWideChar(CP_THREAD_ACP,0,string.Buffer(),(int)string.Length(),0,0);
		wchar_t* Temp=new wchar_t[Length+1];
		MultiByteToWideChar(CP_THREAD_ACP,0,string.Buffer(),(int)string.Length(),Temp,(int)Length);
		Temp[Length]=L'\0';
		WString s=Temp;
		delete[] Temp;
		return s;
	}

	AString alower(const AString& string)
	{
		AString result=string.Buffer();
		_strlwr_s((char*)result.Buffer(), result.Length()+1);
		return result;
	}

	WString wlower(const WString& string)
	{
		WString result=string.Buffer();
		_wcslwr_s((wchar_t*)result.Buffer(), result.Length()+1);
		return result;
	}

	AString aupper(const AString& string)
	{
		AString result=string.Buffer();
		_strupr_s((char*)result.Buffer(), result.Length()+1);
		return result;
	}

	WString wupper(const WString& string)
	{
		WString result=string.Buffer();
		_wcsupr_s((wchar_t*)result.Buffer(), result.Length()+1);
		return result;
	}
}
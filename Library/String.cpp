#include <stdlib.h>
#include <windows.h>
#include "String.h"

namespace vl
{
	int atoi(const AString& string)
	{
		char* endptr=0;
		return strtol(string.Buffer(), &endptr, 10);
	}

	int wtoi(const WString& string)
	{
		wchar_t* endptr=0;
		return wcstol(string.Buffer(), &endptr, 10);
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

	AString itoa(int number)
	{
		char buffer[100];
		_itoa_s(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString itow(int number)
	{
		wchar_t buffer[100];
		_itow_s(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString ftoa(double number)
	{
		char buffer[_CVTBUFSIZE];
		int dec;
		int sign;
		_ecvt_s(buffer, _CVTBUFSIZE, number, 100, &dec, &sign);
		return buffer;
	}

	WString ftow(double number)
	{
		return atow(ftoa(number));
	}

	AString wtoa(const WString& string)
	{
		int Length=WideCharToMultiByte(CP_THREAD_ACP,0,string.Buffer(),string.Length(),0,0,0,0);
		char* Temp=new char[Length+1];
		WideCharToMultiByte(CP_THREAD_ACP,0,string.Buffer(),string.Length(),Temp,Length,0,0);
		Temp[Length]='\0';
		AString s=Temp;
		delete[] Temp;
		return s;
	}

	WString atow(const AString& string)
	{
		int Length=MultiByteToWideChar(CP_THREAD_ACP,0,string.Buffer(),string.Length(),0,0);
		wchar_t* Temp=new wchar_t[Length+1];
		MultiByteToWideChar(CP_THREAD_ACP,0,string.Buffer(),string.Length(),Temp,Length);
		Temp[Length]=L'\0';
		WString s=Temp;
		delete[] Temp;
		return s;
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace collections;

			class SystemCoreStdlibPlugin : public LanguagePlugin
			{
			protected:

				inline static char* __strcpy(char* dst, const char* src){return strcpy(dst, src);}
				inline static char* __strncpy(char* dst, const char* src, vint size){return strncpy(dst, src, (size_t)size);}
				inline static char* __strcat(char* dst, const char* src){return strcat(dst, src);}
				inline static char* __strncat(char* dst, const char* src, vint size){return strncat(dst, src, (size_t)size);}
				inline static vint __strcmp(const char* a, const char* b){return strcmp(a, b);}
				inline static vint __strncmp(const char* a, const char* b, vint size){return strncmp(a, b, (size_t)size);}
				inline static vint __stricmp(const char* a, const char* b){return _stricmp(a, b);}
				inline static vint __strnicmp(const char* a, const char* b, vint size){return _strnicmp(a, b, (size_t)size);}
				inline static const char* __strstr(const char* str, const char* sub){return strstr(str, sub);}
				inline static vint __strlen(const char* str){return strlen(str);}

				inline static wchar_t* __wcscpy(wchar_t* dst, const wchar_t* src){return wcscpy(dst, src);}
				inline static wchar_t* __wcsncpy(wchar_t* dst, const wchar_t* src, vint size){return wcsncpy(dst, src, (size_t)size);}
				inline static wchar_t* __wcscat(wchar_t* dst, const wchar_t* src){return wcscat(dst, src);}
				inline static wchar_t* __wcsncat(wchar_t* dst, const wchar_t* src, vint size){return wcsncat(dst, src, (size_t)size);}
				inline static vint __wcscmp(const wchar_t* a, const wchar_t* b){return wcscmp(a, b);}
				inline static vint __wcsncmp(const wchar_t* a, const wchar_t* b, vint size){return wcsncmp(a, b, (size_t)size);}
				inline static vint __wcsicmp(const wchar_t* a, const wchar_t* b){return _wcsicmp(a, b);}
				inline static vint __wcsnicmp(const wchar_t* a, const wchar_t* b, vint size){return _wcsnicmp(a, b, (size_t)size);}
				inline static const wchar_t* __wcsstr(const wchar_t* str, const wchar_t* sub){return wcsstr(str, sub);}
				inline static vint __wcslen(const wchar_t* str){return wcslen(str);}

				inline static void* __memcpy(void* dst, void* src, vint size){return memcpy(dst, src, (size_t)size);}
				inline static vint __memcmp(void* dst, void* src, vint size){return memcmp(dst, src, (size_t)size);}

				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						REGISTER_LIGHT_FUNCTION(strcpy, char*(char*, const char*), __strcpy) &&
						REGISTER_LIGHT_FUNCTION(strncpy, char*(char*, const char*, vint), __strncpy) &&
						REGISTER_LIGHT_FUNCTION(strcat, char*(char*, const char*), __strcat) &&
						REGISTER_LIGHT_FUNCTION(strncat, char*(char*, const char*, vint), __strncat) &&
						REGISTER_LIGHT_FUNCTION(strcmp, vint(const char*, const char*), __strcmp) &&
						REGISTER_LIGHT_FUNCTION(strncmp, vint(const char*, const char*, vint), __strncmp) &&
						REGISTER_LIGHT_FUNCTION(stricmp, vint(const char*, const char*), __stricmp) &&
						REGISTER_LIGHT_FUNCTION(strnicmp, vint(const char*, const char*, vint), __strnicmp) &&
						REGISTER_LIGHT_FUNCTION(strstr, const char*(const char*, const char*), __strstr) &&
						REGISTER_LIGHT_FUNCTION(strlen, vint(const char*), __strlen) &&
						
						REGISTER_LIGHT_FUNCTION(wcscpy, wchar_t*(wchar_t*, const wchar_t*), __wcscpy) &&
						REGISTER_LIGHT_FUNCTION(wcsncpy, wchar_t*(wchar_t*, const wchar_t*, vint), __wcsncpy) &&
						REGISTER_LIGHT_FUNCTION(wcscat, wchar_t*(wchar_t*, const wchar_t*), __wcscat) &&
						REGISTER_LIGHT_FUNCTION(wcsncat, wchar_t*(wchar_t*, const wchar_t*, vint), __wcsncat) &&
						REGISTER_LIGHT_FUNCTION(wcscmp, vint(const wchar_t*, const wchar_t*), __wcscmp) &&
						REGISTER_LIGHT_FUNCTION(wcsncmp, vint(const wchar_t*, const wchar_t*, vint), __wcsncmp) &&
						REGISTER_LIGHT_FUNCTION(wcsicmp, vint(const wchar_t*, const wchar_t*), __wcsicmp) &&
						REGISTER_LIGHT_FUNCTION(wcsnicmp, vint(const wchar_t*, const wchar_t*, vint), __wcsnicmp) &&
						REGISTER_LIGHT_FUNCTION(wcsstr, const wchar_t*(const wchar_t*, const wchar_t*), __wcsstr) &&
						REGISTER_LIGHT_FUNCTION(wcslen, vint(const wchar_t*), __wcslen) &&

						REGISTER_LIGHT_FUNCTION(memcpy, void*(void*, void*, vint), __memcpy) &&
						REGISTER_LIGHT_FUNCTION(memcmp, vint(void*, void*, vint), __memcmp) &&

						REGISTER_LIGHT_FUNCTION(sin, double(double), sin) &&
						REGISTER_LIGHT_FUNCTION(cos, double(double), cos) &&
						REGISTER_LIGHT_FUNCTION(tan, double(double), tan) &&
						REGISTER_LIGHT_FUNCTION(asin, double(double), asin) &&
						REGISTER_LIGHT_FUNCTION(acos, double(double), acos) &&
						REGISTER_LIGHT_FUNCTION(atan, double(double), atan) &&
						REGISTER_LIGHT_FUNCTION(exp, double(double), exp) &&
						REGISTER_LIGHT_FUNCTION(log2, double(double), log) &&
						REGISTER_LIGHT_FUNCTION(log10, double(double), log10) &&
						REGISTER_LIGHT_FUNCTION(floor, double(double), floor) &&
						REGISTER_LIGHT_FUNCTION(ceil, double(double), ceil) &&

						REGISTER_LIGHT_FUNCTION(absf, float(float), abs) &&
						REGISTER_LIGHT_FUNCTION(sinf, float(float), sin) &&
						REGISTER_LIGHT_FUNCTION(cosf, float(float), cos) &&
						REGISTER_LIGHT_FUNCTION(tanf, float(float), tan) &&
						REGISTER_LIGHT_FUNCTION(asinf, float(float), asin) &&
						REGISTER_LIGHT_FUNCTION(acosf, float(float), acos) &&
						REGISTER_LIGHT_FUNCTION(atanf, float(float), atan) &&
						REGISTER_LIGHT_FUNCTION(expf, float(float), exp) &&
						REGISTER_LIGHT_FUNCTION(log2f, float(float), log) &&
						REGISTER_LIGHT_FUNCTION(log10f, float(float), log10) &&
						REGISTER_LIGHT_FUNCTION(floorf, float(float), floor) &&
						REGISTER_LIGHT_FUNCTION(ceilf, float(float), ceil) &&
						REGISTER_LIGHT_FUNCTION(absf, float(float), abs) &&
						true;
				}
			public:
				SystemCoreStdlibPlugin()
				{
				}
			};

			Ptr<LanguagePlugin> CreateStdlibPlugin()
			{
				return new SystemCoreStdlibPlugin();
			}
		}
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include <time.h>
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

				inline static vint __atoi(const char* input, char** endptr, vint radix){return strtol(input, endptr, (int)radix);}
				inline static vint __wtoi(const wchar_t* input, wchar_t** endptr, vint radix){return wcstol(input, endptr, (int)radix);}
				inline static __int64 __atoi64(const char* input, char** endptr, vint radix){return _strtoi64(input, endptr, (int)radix);}
				inline static __int64 __wtoi64(const wchar_t* input, wchar_t** endptr, vint radix){return _wcstoi64(input, endptr, (int)radix);}
				inline static vuint __atou(const char* input, char** endptr, vint radix){return strtoul(input, endptr, (int)radix);}
				inline static vuint __wtou(const wchar_t* input, wchar_t** endptr, vint radix){return wcstoul(input, endptr, (int)radix);}
				inline static unsigned __int64 __atou64(const char* input, char** endptr, vint radix){return _strtoui64(input, endptr, (int)radix);}
				inline static unsigned __int64 __wtou64(const wchar_t* input, wchar_t** endptr, vint radix){return _wcstoui64(input, endptr, (int)radix);}
				inline static double __atof(const char* input, char** endptr){return strtod(input, endptr);}
				inline static double __wtof(const wchar_t* input, wchar_t** endptr){return wcstod(input, endptr);}

				inline static char* __itoa(vint number, char* output, vint size, vint radix){ITOA_S(number, output, size, (int)radix);return output;}
				inline static wchar_t* __itow(vint number, wchar_t* output, vint size, vint radix){ITOW_S(number, output, size, (int)radix);return output;}
				inline static char* __i64toa(__int64 number, char* output, vint size, vint radix){I64TOA_S(number, output, size, (int)radix);return output;}
				inline static wchar_t* __i64tow(__int64 number, wchar_t* output, vint size, vint radix){I64TOW_S(number, output, size, (int)radix);return output;}
				inline static char* __utoa(vuint number, char* output, vint size, vint radix){UITOA_S(number, output, size, (int)radix);return output;}
				inline static wchar_t* __utow(vuint number, wchar_t* output, vint size, vint radix){UITOW_S(number, output, size, (int)radix);return output;}
				inline static char* __u64toa(unsigned __int64 number, char* output, vint size, vint radix){UI64TOA_S(number, output, size, (int)radix);return output;}
				inline static wchar_t* __u64tow(unsigned __int64 number, wchar_t* output, vint size, vint radix){UI64TOW_S(number, output, size, (int)radix);return output;}

				inline static char* __ftoa(double number, char* output, vint size)
				{
					AString s=ftoa(number);
					strncpy(output, s.Buffer(), size);
					return output;
				}

				inline static wchar_t* __ftow(double number, wchar_t* output, vint size)
				{
					WString s=ftow(number);
					wcsncpy(output, s.Buffer(), size);
					return output;
				}

				inline static vint __rand16(){return rand();}

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

						REGISTER_LIGHT_FUNCTION(atoi, vint(const char*, char**, vint), __atoi) &&
						REGISTER_LIGHT_FUNCTION(wtoi, vint(const wchar_t*, wchar_t**, vint), __wtoi) &&
						REGISTER_LIGHT_FUNCTION(atoi64, __int64(const char*, char**, vint), __atoi64) &&
						REGISTER_LIGHT_FUNCTION(wtoi64, __int64(const wchar_t*, wchar_t**, vint), __wtoi64) &&
						REGISTER_LIGHT_FUNCTION(atou, vuint(const char*, char**, vint), __atou) &&
						REGISTER_LIGHT_FUNCTION(wtou, vuint(const wchar_t*, wchar_t**, vint), __wtou) &&
						REGISTER_LIGHT_FUNCTION(atou64, unsigned __int64(const char*, char**, vint), __atou64) &&
						REGISTER_LIGHT_FUNCTION(wtou64, unsigned __int64(const wchar_t*, wchar_t**, vint), __wtou64) &&
						REGISTER_LIGHT_FUNCTION(atof, double(const char*, char**), __atof) &&
						REGISTER_LIGHT_FUNCTION(wtof, double(const wchar_t*, wchar_t**), __wtof) &&
						
						REGISTER_LIGHT_FUNCTION(itoa, char*(vint, char*, vint, vint), __itoa) &&
						REGISTER_LIGHT_FUNCTION(itow, wchar_t*(vint, wchar_t*, vint, vint), __itow) &&
						REGISTER_LIGHT_FUNCTION(i64toa, char*(__int64, char*, vint, vint), __i64toa) &&
						REGISTER_LIGHT_FUNCTION(i64tow, wchar_t*(__int64, wchar_t*, vint, vint), __i64tow) &&
						REGISTER_LIGHT_FUNCTION(utoa, char*(vuint, char*, vint, vint), __utoa) &&
						REGISTER_LIGHT_FUNCTION(utow, wchar_t*(vuint, wchar_t*, vint, vint), __utow) &&
						REGISTER_LIGHT_FUNCTION(u64toa, char*(unsigned __int64, char*, vint, vint), __u64toa) &&
						REGISTER_LIGHT_FUNCTION(u64tow, wchar_t*(unsigned __int64, wchar_t*, vint, vint), __u64tow) &&
						REGISTER_LIGHT_FUNCTION(ftoa, char*(double, char*, vint), __ftoa) &&
						REGISTER_LIGHT_FUNCTION(ftow, wchar_t*(double, wchar_t*, vint), __ftow) &&

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
						REGISTER_LIGHT_FUNCTION(abs, double(double), abs) &&

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

						REGISTER_LIGHT_FUNCTION(rand16, vint(), __rand16) &&
						true;
				}
			public:
				SystemCoreStdlibPlugin()
				{
					srand((unsigned)time(0));
				}
			};

			Ptr<LanguagePlugin> CreateStdlibPlugin()
			{
				return new SystemCoreStdlibPlugin();
			}
		}
	}
}
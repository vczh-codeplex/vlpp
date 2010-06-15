#include <windows.h>
#include "Console.h"

namespace vl
{
	namespace console
	{
		
/***********************************************************************
Console
***********************************************************************/

		void Console::Write(const WString& string)
		{
			DWORD count=0;
			WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),string.Buffer(),(int)string.Length(),&count,0);
		}

		void Console::WriteLine(const WString& string)
		{
			Write(string);
			Write(L"\r\n");
		}

		WString Console::Read()
		{
			WString result;
			DWORD count;
			for(;;)
			{
				wchar_t buffer;
				ReadConsole(GetStdHandle(STD_INPUT_HANDLE),&buffer,1,&count,0);
				if(buffer==L'\r')
				{
					ReadConsole(GetStdHandle(STD_INPUT_HANDLE),&buffer,1,&count,0);
					break;
				}
				else if(buffer==L'\n')
				{
					break;
				}
				else
				{
					result=result+buffer;
				}
			}
			return result;
		}

		void Console::SetColor(bool red, bool green, bool blue, bool light)
		{
			WORD attribute=0;
			if(red)attribute		|=FOREGROUND_RED;
			if(green)attribute		|=FOREGROUND_GREEN;
			if(blue)attribute		|=FOREGROUND_BLUE;
			if(light)attribute		|=FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),attribute);
			SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE),attribute);
		}

		void Console::SetTitle(const WString& string)
		{
			SetConsoleTitle(string.Buffer());
		}
	}
}
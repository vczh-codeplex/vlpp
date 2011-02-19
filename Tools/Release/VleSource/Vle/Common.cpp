#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"

wchar_t* ProcVersion()
{
	return L"v0.1";
}

void UnitTestPluginPrinter(bool condition, wchar_t* description)
{
	if(condition)
	{
		Console::SetColor(false, true, false, true);
		Console::WriteLine(description);
		Console::SetColor(true, true, true, false);
	}
	else
	{
		Console::SetColor(true, false, false, true);
		Console::WriteLine(description);
		Console::SetColor(true, true, true, false);
		throw Exception(L"Testing stopped.");
	}
}

void ConsolePluginReader(wchar_t* text)
{
	WString result=Console::Read();
	wcscpy(text, result.Buffer());
}

void ConsolePluginWriter(wchar_t* text)
{
	Console::Write(text);
}

void InitHost(LanguageHost& host)
{
	host.RegisterPlugin(CreateMemoryManagerPlugin());
	host.RegisterPlugin(CreateUnitTestPlugin(UnitTestPluginPrinter));
	host.RegisterPlugin(CreateConsolePlugin(ConsolePluginReader, ConsolePluginWriter));
	host.RegisterPlugin(CreateThreadingPlugin());
	host.RegisterPlugin(CreateStdlibPlugin());
	host.RegisterPlugin(CreateGcSingleThreadPlugin());
}

Ptr<LanguageState> LoadAssembly(LanguageHost& host, const List<WString>& fileNames, List<Ptr<LanguageAssembly>>& assemblies)
{
	for(vint i=0;i<fileNames.Count();i++)
	{
		FileStream stream(fileNames.Get(i), FileStream::ReadOnly);
		if(!stream.IsAvailable())
		{
			throw Exception(L"Cannot open file to read: \""+fileNames.Get(i)+L"\".");
		}
		Ptr<LanguageAssembly> assembly=new LanguageAssembly(stream);
		host.LoadAssembly(assembly);
		assemblies.Add(assembly);
	}
	Ptr<LanguageState> state=host.CreateState();
	for(vint i=0;i<assemblies.Count();i++)
	{
		ILException::RunningResult result=state->RunInitialization(assemblies[i]);
		if(result!=ILException::Finished)
		{
			throw ILException(result);
		}
	}
	return state;
}

void ReadAssemblyListFile(const WString& fileName, List<WString>& fileNames)
{
	FileStream stream(fileName, FileStream::ReadOnly);
	if(!stream.IsAvailable())
	{
		throw Exception(L"Cannot open file to read: \""+fileName+L"\".");
	}

	BomDecoder decoder;
	DecoderStream decoderStream(stream, decoder);
	StreamReader reader(decoderStream);

	fileNames.Clear();
	while(!reader.IsEnd())
	{
		WString line=reader.ReadLine();
		if(line!=L"")fileNames.Add(line);
	}
}
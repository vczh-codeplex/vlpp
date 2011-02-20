#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"

wchar_t* ProcVersion()
{
	return L"v0.1";
}

WString GetFolder(WString fileName)
{
	vint index=-1;
	for(vint i=0;i<fileName.Length();i++)
	{
		if(fileName[i]==L'\\')index=i;
	}
	WString dir=fileName.Left(index+1);
	if(dir==L"")dir=L".\\";
	return dir;
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

Ptr<LanguageState> LoadAssembly(LanguageHost& host, const WString& baseDir, const List<WString>& fileNames, List<Ptr<LanguageAssembly>>& assemblies)
{
	for(vint i=0;i<fileNames.Count();i++)
	{
		WString fileName=baseDir+fileNames.Get(i);
		FileStream stream(fileName, FileStream::ReadOnly);
		if(!stream.IsAvailable())
		{
			throw Exception(L"Cannot open file to read: \""+fileName+L"\".");
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

void InitLinker(LanguageLinker& linker)
{
	linker.RegisterPlugin(CreateMemoryManagerPlugin());
	linker.RegisterPlugin(CreateUnitTestPlugin(UnitTestPluginPrinter));
	linker.RegisterPlugin(CreateConsolePlugin(ConsolePluginReader, ConsolePluginWriter));
	linker.RegisterPlugin(CreateThreadingPlugin());
	linker.RegisterPlugin(CreateStdlibPlugin());
	linker.RegisterPlugin(CreateGcSingleThreadPlugin());
}

void LoadAssembly(LanguageLinker& linker, const WString& baseDir, const List<WString>& fileNames, List<Ptr<LanguageAssembly>>& assemblies)
{
	for(vint i=0;i<fileNames.Count();i++)
	{
		WString fileName=baseDir+fileNames.Get(i);
		FileStream stream(fileName, FileStream::ReadOnly);
		if(!stream.IsAvailable())
		{
			throw Exception(L"Cannot open file to read: \""+fileName+L"\".");
		}
		linker.LoadAssembly(stream);
	}
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
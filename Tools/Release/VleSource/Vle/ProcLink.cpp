#include "Common.h"

bool ProcLink(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Linker ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==3 || args.Count()==4)
	{
		List<WString> assemblyList;
		List<Ptr<LanguageAssembly>> assemblies;
		LanguageLinker linker;
		ReadAssemblyListFile(args[1], assemblyList);
		InitLinker(linker);
		LoadAssembly(linker, GetFolder(args[1]), assemblyList, assemblies);
		Ptr<LanguageAssembly> assembly=linker.Link();
		{
			FileStream fileStream(args[2], FileStream::WriteOnly);
			if(!fileStream.IsAvailable())
			{
				throw Exception(L"Cannot open file to write: \""+args[2]+L"\".");
			}
			assembly->SaveToStream(fileStream);
		}
		if(args.Count()==4)
		{
			FileStream fileStream(args[3], FileStream::WriteOnly);
			if(!fileStream.IsAvailable())
			{
				throw Exception(L"Cannot open file to write: \""+args[3]+L"\".");
			}
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			assembly->LogInternalState(writer);
		}
		return true;
	}
	return false;
}
#include "Common.h"

bool ProcMake(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Make ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==2)
	{
		FileStream fileStream(args[1], FileStream::ReadOnly);
		if(fileStream.IsAvailable())
		{
			LanguageMakeFile makefile;
			{
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader streamReader(decoderStream);
				makefile.Load(GetFolder(args[1]), streamReader);
			}
			LanguageMaker maker;
			maker.Execute(makefile);
			return true;
		}
		else
		{
			throw LanguageMakerException(L"Cannot open file to read: \""+args[1]+L"\".");
		}
	}
	return false;
}
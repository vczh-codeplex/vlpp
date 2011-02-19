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
				vint index=-1;
				for(vint i=0;i<args[1].Length();i++)
				{
					if(args[1][i]==L'\\')index=i;
				}
				WString dir=L".\\"+args[1].Left(index+1);

				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader streamReader(decoderStream);
				makefile.Load(dir, streamReader);
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
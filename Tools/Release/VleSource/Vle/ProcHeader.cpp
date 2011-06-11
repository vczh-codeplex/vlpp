#include "Common.h"
#include "..\..\..\..\Library\Scripting\Languages\LanguageProviderBasicExtension.h"
#include "..\..\..\..\Library\Scripting\Languages\NativeX\NativeX.h"

void ProcHeaderGenerateBasic(Ptr<LanguageAssembly> assembly, Ptr<IBasicLanguageProvider> provider, TextWriter& writer)
{
	provider->GenerateHeader(assembly, 0, writer);
}

bool ProcHeader(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Header Extracter ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==4)
	{
		Ptr<ILanguageProvider> provider;
		if(args[1]==L"NativeX")
		{
			provider=CreateNativeXLanguageProvider();
		}
		else
		{
			throw LanguageMakerException(L"Don't know language: \""+args[1]+L"\".");
		}

		Ptr<LanguageAssembly> assembly;
		{
			FileStream fileStream(args[2], FileStream::ReadOnly);
			if(!fileStream.IsAvailable())
			{
				throw LanguageMakerException(L"Cannot open file to read: \""+args[2]+L"\".");
			}
			assembly=new LanguageAssembly(fileStream);
		}

		FileStream fileStream(args[3], FileStream::WriteOnly);
		if(!fileStream.IsAvailable())
		{
			throw LanguageMakerException(L"Cannot open file to write: \""+args[3]+L"\".");
		}
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		switch(provider->LanguageType())
		{
		case ILanguageProvider::Native:
			ProcHeaderGenerateBasic(assembly, provider.Cast<IBasicLanguageProvider>(), writer);
			break;
		}
		return true;
	}
	return false;
}
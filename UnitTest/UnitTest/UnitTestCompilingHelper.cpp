#include "UnitTestCompilingHelper.h"

void LogInterpretor(BasicILInterpretor& interpretor, const WString& fileName)
{
	WString path=GetPath()+L"Codegen_"+fileName+L".txt";
	FileStream fileStream(path, FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter fileWriter(encoderStream);
	interpretor.LogInternalState(fileWriter);
}

Ptr<BasicIL> Compile(Ptr<BasicProgram> program, const WString& name, const WString& fileName)
{
	if(fileName!=L"")
	{
		ConvertToNativeXProgram(program);
	}
	BasicAlgorithmConfiguration configuration;
	SetConfiguration(configuration);
	BasicAnalyzer analyzer(program, 0, configuration);
	analyzer.Analyze();
	TEST_ASSERT(analyzer.GetErrors().Count()==0);
	BasicCodeGenerator codegen(&analyzer, 0, name);
	codegen.GenerateCode();

	if(fileName!=L"")
	{
		WString path=GetPath()+L"Codegen_"+fileName+L".txt";
		FileStream fileStream(path, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter fileWriter(encoderStream);

		fileWriter.WriteLine(L"/*NativeX Code*/");
		PrintNativeXProgram(program, fileWriter);
		fileWriter.WriteLine(L"");

		BasicLanguageCommentProvider commentProvider;
		TextWriter* commentProviderWriter=commentProvider.OpenWriter();
		PrintNativeXProgram(program, *commentProviderWriter);
		commentProvider.CloseWriter();

		fileWriter.WriteLine(L"/*Assembly*/");
		codegen.GetIL()->SaveAsString(fileWriter, &commentProvider);
	}
	return codegen.GetIL();
}
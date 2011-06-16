#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\ManagedX\ManagedXParser.h"
#include "..\..\Library\Scripting\Languages\ManagedX\ManagedXCodeGenerator.h"
#include "..\..\Library\Scripting\Languages\ManagedX\ManagedX.h"
#include "..\..\Library\Scripting\Languages\LanguageProviderManagedExtension.h"
#include "..\..\Library\Scripting\Utility\ScriptingUtilityMake.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\CharFormat.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::scripting;
using namespace vl::scripting::managedlanguage;
using namespace vl::scripting::utility;
using namespace vl::scripting::language_managedx;

extern WString GetPath();

namespace TestManagedXParserHelper
{
	void LoadCodePaths(List<WString>& codeFiles)
	{
		WString indexFilePath=GetPath()+L"Code\\ManagedX\\Parsing\\Index.txt";
		FileStream fileStream(indexFilePath, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader streamReader(decoderStream);
		while(!streamReader.IsEnd())
		{
			codeFiles.Add(streamReader.ReadLine());
		}
	}

	WString ReadFile(const WString& intputPath)
	{
		FileStream fileStream(intputPath, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader streamReader(decoderStream);
		return streamReader.ReadToEnd();
	}

	Ptr<ManagedXUnit> Parse(Ptr<ManagedXParser> parser, const WString& inputPath)
	{
		WString code=ReadFile(inputPath);
			
		List<Ptr<LanguageException>> errors;
		Ptr<ManagedXUnit> unit;

		unit=parser->Parse(code, 0, errors.Wrap());
		for(vint i=0;i<errors.Count();i++)
		{
			vl::unittest::UnitTest::PrintError(L"ERROR ("+itow(errors[i]->LineIndex())+L"): "+errors[i]->Message());
		}
		TEST_ASSERT(errors.Count()==0);
		if(!unit)
		{
			unit=new ManagedXUnit;
			unit->program=new ManagedProgram;
			unit->codeIndex=0;
		}
		TEST_ASSERT(unit);
		return unit;
	}

	void Generate(Ptr<ManagedXUnit> unit, const WString& outputPath)
	{
		FileStream fileStream(outputPath, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter streamWriter(encoderStream);
		ManagedX_GenerateCode_Program(unit->program, streamWriter);
	}

	void CheckParser(Ptr<ManagedXParser> parser, const WString& fileName)
	{
		WString intputPath=GetPath()+L"Code\\ManagedX\\Parsing\\"+fileName;
		WString outputPath1=GetPath()+L"[ManagedX.Parsing]["+fileName+L"][1].txt";
		WString outputPath2=GetPath()+L"[ManagedX.Parsing]["+fileName+L"][2].txt";

		Ptr<ManagedXUnit> unit;
		vl::unittest::UnitTest::PrintInfo(L"  pass 1");
		unit=Parse(parser, intputPath);
		Generate(unit, outputPath1);
		vl::unittest::UnitTest::PrintInfo(L"  pass 2");
		unit=Parse(parser, outputPath1);
		Generate(unit, outputPath2);

		WString generated1=ReadFile(outputPath1);
		WString generated2=ReadFile(outputPath2);
		TEST_ASSERT(generated1==generated2);
	}
}
using namespace TestManagedXParserHelper;

TEST_CASE(Test_ManagedX_Parser)
{
	vl::unittest::UnitTest::PrintInfo(L"Initializing ManagedX Parser...");
	Ptr<ManagedXParser> parser=ManagedXParser::Create();
	List<WString> codeFiles;
	LoadCodePaths(codeFiles);

	for(vint i=0;i<codeFiles.Count();i++)
	{
		vl::unittest::UnitTest::PrintInfo(L"Parsing ManagedX program: "+codeFiles[i]);
		CheckParser(parser, codeFiles[i]);
	}
}

TEST_CASE(Test_ManagedX_Parser_System_CoreManaged)
{
	vl::unittest::UnitTest::PrintInfo(L"Initializing ManagedX Provider...");
	Ptr<ILanguageProvider> provider=CreateManagedXLanguageProvider();
	Ptr<IManagedLanguageProvider> managedProvider=provider.Cast<IManagedLanguageProvider>();
	WString inputPath=GetPath()+L"..\\ScriptCoreLibrary\\System.CoreManaged\\";
	LanguageMakeFile makeFile;
	{
		FileStream fileStream(inputPath+L"Make.txt", FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader reader(decoderStream);
		makeFile.Load(inputPath, reader);
	}

	for(vint i=0;i<makeFile.compiles.Count();i++)
	{
		WString codePath=makeFile.baseLocation+makeFile.compiles[i];
		vl::unittest::UnitTest::PrintInfo(L"Parsing ManagedX program: "+makeFile.compiles[i]);

		WString code;
		{
			FileStream fileStream(codePath, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			code=reader.ReadToEnd();
		}
		Ptr<Object> extra;
		List<Ptr<LanguageException>> errors;
		Ptr<ManagedProgram> program=managedProvider->ParseProgram(code, extra, errors.Wrap());

		TEST_ASSERT(program);
		for(vint i=0;i<errors.Count();i++)
		{
			vl::unittest::UnitTest::PrintError(L"ERROR ("+itow(errors[i]->LineIndex())+L"): "+errors[i]->Message());
		}
		TEST_ASSERT(errors.Count()==0);
	}
}
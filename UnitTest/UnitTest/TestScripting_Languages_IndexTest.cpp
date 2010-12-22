#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"
#include "..\..\Library\Scripting\Languages\BasicFunctionExecutor.h"
#include "..\..\Library\Scripting\Languages\LanguageProviderExtension.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageResource.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCommentProvider.h"

#include "..\..\Library\GlobalStorage.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\MemoryStream.h"
#include "..\..\Library\Stream\Accessor.h"
#include "..\..\Library\Stream\CharFormat.h"
#include "..\..\Library\Regex\Regex.h"
#include "..\..\Library\Tuple.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::collections;
using namespace vl::scripting::basicil;
using namespace vl::scripting::basiclanguage;
using namespace vl::stream;
using namespace vl::regex;

extern WString GetPath();

class Test_BasicLanguage_ForeignFunction_Summer : public Object, public ILanguageForeignFunction
{
public:
	void Invoke(LanguageArguments& arguments)
	{
		vint* numbers=arguments.NextArgument<vint*>();
		vint count=arguments.NextArgument<vint>();
		vint result=0;
		for(vint i=0;i<count;i++)
		{
			result+=numbers[i];
		}
		arguments.Result<vint>()=result;
	}
};

void Test_BasicLanguage_ForeignFunction_LightSummer(void* result, void* arguments)
{
	vint* numbers=*(vint**)(arguments);
	vint count=*(vint*)((char*)arguments+sizeof(vint*));
	vint sum=0;
	for(vint i=0;i<count;i++)
	{
		sum+=numbers[i];
	}
	*((vint*)result)=sum;
}

void Test_BasicLanguage_PrintNativeXHeaderFile(Ptr<LanguageAssembly> assembly, Ptr<ILanguageProvider> provider, const WString& filePath)
{
	Ptr<IBasicLanguageProvider> printer=provider.Cast<IBasicLanguageProvider>();
	MemoryStream memoryStream;
	StreamWriter memoryWriter(memoryStream);
	if(printer->GenerateHeader(assembly, new NativeXHeaderExtra(), memoryWriter))
	{
		memoryStream.SeekFromBegin(0);
		StreamReader memoryReader(memoryStream);
		WString headerCode=memoryReader.ReadToEnd();
		{
			FileStream fileStream(filePath, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			writer.WriteString(headerCode);
		}
		Ptr<Object> outputExtra;
		List<Ptr<LanguageException>> errors;
		Ptr<BasicProgram> headerProgram=printer->ParseProgram(headerCode, outputExtra, errors.Wrap());
		TEST_ASSERT(errors.Count()==0);
		TEST_ASSERT(headerProgram);
	}
}

TEST_CASE(TestCodeInIndex)
{
	vl::unittest::UnitTest::PrintInfo(L"Initializing NativeX Language Provider...");
	Ptr<ILanguageProvider> nativexProvider=CreateNativeXLanguageProvider();
	vl::unittest::UnitTest::PrintInfo(L"Loading Cases Index File...");
	Regex regexCase(L"(<make>[^=:]+)=(<type>[^=:]+):(<value>[^=:]+)");
	Regex regexMake(L"(<key>[^=]+)=(<value>[^=]+)");

	List<WString> cases;
	{
		WString indexFilePath=GetPath()+L"Code\\Index.txt";
		FileStream fileStream(indexFilePath, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader streamReader(decoderStream);
		while(!streamReader.IsEnd())
		{
			cases.Add(streamReader.ReadLine());
		}
	}
	for(vint i=0;i<cases.Count();i++)
	{
		WString singleCase=cases[i];
		vl::unittest::UnitTest::PrintInfo(L"Running Code Case: "+singleCase);
		RegexMatch::Ref singleCaseMatch=regexCase.Match(singleCase);
		WString makeFileName=singleCaseMatch->Groups()[L"make"][0].Value();
		WString makeFilePath=GetPath()+L"Code\\"+makeFileName;
		WString resultType=singleCaseMatch->Groups()[L"type"][0].Value();
		WString resultValue=singleCaseMatch->Groups()[L"value"][0].Value();

		List<WString> makeKeys, makeValues;
		{
			FileStream fileStream(makeFilePath, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader streamReader(decoderStream);
			while(!streamReader.IsEnd())
			{
				WString pair=streamReader.ReadLine();
				RegexMatch::Ref pairMatch=regexMake.Match(pair);
				makeKeys.Add(pairMatch->Groups()[L"key"][0].Value());
				makeValues.Add(pairMatch->Groups()[L"value"][0].Value());
			}
		}
		List<vint> assemblyStarts;
		{
			for(vint i=0;i<makeKeys.Count();i++)
			{
				if(makeKeys[i]==L"Language")
				{
					assemblyStarts.Add(i);
				}
			}
			assemblyStarts.Add(makeKeys.Count());
		}

		List<Ptr<LanguageAssembly>> assemblies;
		List<WString> assemblyExecutedLogPaths;
		for(vint i=0;i<assemblyStarts.Count()-1;i++)
		{
			vint start=assemblyStarts[i];
			vint end=assemblyStarts[i+1];

			WString languageName, assemblyName;
			List<WString> codes;
			for(vint j=start;j<end;j++)
			{
				if(makeKeys[j]==L"Language")
				{
					languageName=makeValues[j];
				}
				else if(makeKeys[j]==L"Assembly")
				{
					assemblyName=makeValues[j];
				}
				else if(makeKeys[j]==L"Compile")
				{
					WString codePath=GetPath()+L"Code\\"+makeValues[j];
					FileStream fileStream(codePath, FileStream::ReadOnly);
					BomDecoder decoder;
					DecoderStream decoderStream(fileStream, decoder);
					StreamReader streamReader(decoderStream);
					codes.Add(streamReader.ReadToEnd());
				}
				else
				{
					TEST_ASSERT(false);
				}
			}

			Ptr<ILanguageProvider> provider;
			if(languageName==L"NativeX")
			{
				provider=nativexProvider;
			}
			List<Ptr<LanguageAssembly>> references;
			List<Ptr<LanguageException>> errors;
			Ptr<LanguageAssembly> assembly;
			{
				WString logOutputPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"]["+assemblyName+L"][Compiled].txt";
				FileStream fileStream(logOutputPath, FileStream::WriteOnly);
				BomEncoder encoder(BomEncoder::Utf16);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);

				BasicLanguageCommentProvider commentProvider;
				for(vint j=0;j<codes.Count();j++)
				{
					TextWriter* textWriter=commentProvider.OpenWriter();
					textWriter->WriteString(codes[j]);
					commentProvider.CloseWriter();
				}
				assembly=provider->Compile(assemblyName, references.Wrap(), codes.Wrap(), errors.Wrap(), &writer, &commentProvider);
				WString logExecutedPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"]["+assemblyName+L"][AfterExecuted].txt";
				assemblyExecutedLogPaths.Add(logExecutedPath);
			}
			if(assembly)
			{
				{
					WString logOutputPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"]["+assemblyName+L"][BeforeExecuted].txt";
					FileStream fileStream(logOutputPath, FileStream::WriteOnly);
					BomEncoder encoder(BomEncoder::Utf16);
					EncoderStream encoderStream(fileStream, encoder);
					StreamWriter writer(encoderStream);
					assembly->LogInternalState(writer);
				}
				{
					WString logOutputPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"]["+assemblyName+L"][Header][NativeX].txt";
					Test_BasicLanguage_PrintNativeXHeaderFile(assembly, nativexProvider, logOutputPath);
				}
			}
			for(vint j=0;j<errors.Count();j++)
			{
				vl::unittest::UnitTest::PrintError(L"Line["+itow(errors[j]->LineIndex())+L"]: "+errors[j]->Message());
			}
			TEST_ASSERT(errors.Count()==0);
			assemblies.Add(assembly);
			{
				WString binaryOutputPath=GetPath()+L"[Assembly][Binary]["+makeFileName+L"]["+assemblyName+L"].assembly";
				FileStream fileStream(binaryOutputPath, FileStream::WriteOnly);
				assembly->SaveToStream(fileStream);
			}
		}

		LanguageHost host(65536);
		host.RegisterForeignFunction(L"Foreign", L"Sum", new Test_BasicLanguage_ForeignFunction_Summer);
		host.RegisterForeignFunction(L"Foreign", L"SumLight", Test_BasicLanguage_ForeignFunction_LightSummer, (vint)(sizeof(vint*)+sizeof(vint)));
		Ptr<LanguageState> state=host.CreateState();
		for(vint i=0;i<assemblies.Count();i++)
		{
			host.LoadAssembly(assemblies[i]);
			TEST_ASSERT(state->RunInitialization(assemblies[i])==BasicILStack::Finished);
			TEST_ASSERT(state->GetStack()->StackTop()==state->GetStack()->StackSize());
		}

		BasicDeclarationInfo entryInfo;
		for(vint i=0;i<assemblies.Count() && !entryInfo;i++)
		{
			Ptr<LanguageAssembly> assembly=assemblies[i];
			for(vint j=0;j<assembly->GetBasicLanguageMetadata()->GetDeclarationCount() && !entryInfo;j++)
			{
				BasicDeclarationInfo info=assembly->GetBasicLanguageMetadata()->GetDeclaration(j);
				if(info.GetName()==L"main")
				{
					entryInfo=info;
				}
			}
		}
		TEST_ASSERT(entryInfo);

		if(resultType==L"int")
		{
			BasicFunctionExecutor<vint()> entry(entryInfo, state);
			vint result=entry();
			if(result!=wtoi(resultValue))
			{
				vl::unittest::UnitTest::PrintError(L"Expect: "+resultValue+L", Actual: "+itow(result));
			}
			TEST_ASSERT(result==wtoi(resultValue));
		}
		else if(resultType==L"bool")
		{
			BasicFunctionExecutor<bool()> entry(entryInfo, state);
			bool result=entry();
			if((result?L"true":L"false")!=resultValue)
			{
				vl::unittest::UnitTest::PrintError(L"Expect: "+resultValue+L", Actual: "+(result?L"true":L"false"));
			}
			TEST_ASSERT(result==(resultValue==L"true"));
		}
		else
		{
			TEST_ASSERT(false);
		}

		for(vint i=0;i<assemblies.Count();i++)
		{
			FileStream fileStream(assemblyExecutedLogPaths[i], FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			assemblies[i]->LogInternalState(writer);
		}
		{
			WString logOutputPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"][Host].txt";
			FileStream fileStream(logOutputPath, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			host.LogInternalState(writer);
		}
		TEST_ASSERT(state->GetStack()->StackTop()==state->GetStack()->StackSize());
	}
}
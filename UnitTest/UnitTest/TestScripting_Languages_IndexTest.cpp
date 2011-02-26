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

vint __stdcall Test_BasicLanguage_ForeignFunction_LightSummer(void* result, void* arguments)
{
	vint* numbers=*(vint**)(arguments);
	vint count=*(vint*)((char*)arguments+sizeof(vint*));
	vint sum=0;
	for(vint i=0;i<count;i++)
	{
		sum+=numbers[i];
	}
	*((vint*)result)=sum;
	return sizeof(vint*)+sizeof(vint);
}

void Test_BasicLanguage_PrintNativeXHeaderFile(Ptr<LanguageAssembly> assembly, Ptr<ILanguageProvider> provider, const WString& filePath, bool publicOnly)
{
	Ptr<IBasicLanguageProvider> printer=provider.Cast<IBasicLanguageProvider>();
	MemoryStream memoryStream;
	StreamWriter memoryWriter(memoryStream);

	Ptr<NativeXHeaderExtra> headerExtra=new NativeXHeaderExtra;
	headerExtra->publicOnly=publicOnly;
	if(printer->GenerateHeader(assembly, headerExtra, memoryWriter))
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

namespace TestCodeInIndexHelper
{
	struct CompiledAssembly
	{
		Ptr<LanguageAssembly>				assembly;
		WString								logPathPrefix;
		WString								binPath;
		bool								headerPublicOnly;
	};

	struct CompilerInfo
	{
		Regex								regexCase;
		Regex								regexMake;
		Ptr<ILanguageProvider>				nativexProvider;

		CompilerInfo()
			:regexCase(L"(<make>[^=:]+)=(<type>[^=:]+):(<value>[^=:]+)")
			,regexMake(L"(<key>[^=]+)=(<value>[^=]+)")
			,nativexProvider(CreateNativeXLanguageProvider())
		{
		}
	};

	void LoadCases(List<WString>& cases)
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

	void CompileCase(CompilerInfo& info, const WString& testCase, List<Ptr<CompiledAssembly>>& assemblies, WString& makeFileName, WString& resultType, WString& resultValue)
	{
		RegexMatch::Ref singleCaseMatch=info.regexCase.Match(testCase);
		makeFileName=singleCaseMatch->Groups()[L"make"][0].Value();
		WString makeFilePath=GetPath()+L"Code\\"+makeFileName;
		resultType=singleCaseMatch->Groups()[L"type"][0].Value();
		resultValue=singleCaseMatch->Groups()[L"value"][0].Value();

		List<WString> makeKeys, makeValues;
		{
			FileStream fileStream(makeFilePath, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader streamReader(decoderStream);
			while(!streamReader.IsEnd())
			{
				WString pair=streamReader.ReadLine();
				RegexMatch::Ref pairMatch=info.regexMake.Match(pair);
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

		for(vint i=0;i<assemblyStarts.Count()-1;i++)
		{
			vint start=assemblyStarts[i];
			vint end=assemblyStarts[i+1];

			WString languageName, assemblyName;
			bool headerPublicOnly=false;
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
				else if(makeKeys[j]==L"Header")
				{
					if(makeValues[j]==L"PublicOnly")
					{
						headerPublicOnly=true;
					}
					else
					{
						TEST_ASSERT(false);
					}
				}
				else
				{
					TEST_ASSERT(false);
				}
			}

			Ptr<ILanguageProvider> provider;
			if(languageName==L"NativeX")
			{
				provider=info.nativexProvider;
			}
			List<Ptr<LanguageAssembly>> references;
			List<Ptr<LanguageException>> errors;
			Ptr<CompiledAssembly> assembly=new CompiledAssembly;
			{
				assembly->logPathPrefix=GetPath()+L"[Assembly][Log]["+makeFileName+L"]["+assemblyName+L"]";
				assembly->binPath=GetPath()+L"[Assembly][Binary]["+makeFileName+L"]["+assemblyName+L"].assembly";
				assembly->headerPublicOnly=headerPublicOnly;

				WString logOutputPath=assembly->logPathPrefix+L"[Compiled].txt";
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
				assembly->assembly=provider->Compile(assemblyName, references.Wrap(), codes.Wrap(), errors.Wrap(), &writer, &commentProvider);
			}
			for(vint j=0;j<errors.Count();j++)
			{
				vl::unittest::UnitTest::PrintError(L"Line["+itow(errors[j]->LineIndex())+L"]: "+errors[j]->Message());
			}
			TEST_ASSERT(errors.Count()==0);
			assemblies.Add(assembly);
		}
	}

	void SaveAssembly(CompilerInfo& info, Ptr<CompiledAssembly> assembly)
	{
		WString binaryOutputPath=assembly->binPath;
		FileStream fileStream(binaryOutputPath, FileStream::WriteOnly);
		assembly->assembly->SaveToStream(fileStream);
	}

	void LogBeforeExecuting(CompilerInfo& info, Ptr<CompiledAssembly> assembly)
	{
		{
			WString logOutputPath=assembly->logPathPrefix+L"[BeforeExecuted].txt";
			FileStream fileStream(logOutputPath, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			assembly->assembly->LogInternalState(writer);
		}
		{
			WString logOutputPath=assembly->logPathPrefix+L"[Header][NativeX].txt";
			Test_BasicLanguage_PrintNativeXHeaderFile(assembly->assembly, info.nativexProvider, logOutputPath, assembly->headerPublicOnly);
		}
	}

	void LogAfterExecuting(CompilerInfo& info, Ptr<CompiledAssembly> assembly)
	{
		FileStream fileStream(assembly->logPathPrefix+L"[AfterExecuted].txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		assembly->assembly->LogInternalState(writer);
	}

	void LogHost(CompilerInfo& info, LanguageHost& host, const WString& makeFileName)
	{
		WString logOutputPath=GetPath()+L"[Assembly][Log]["+makeFileName+L"][Host].txt";
		FileStream fileStream(logOutputPath, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		host.LogInternalState(writer);
	}

	Ptr<LanguageState> InitializeHost(CompilerInfo& info, LanguageHost& host, List<Ptr<CompiledAssembly>>& assemblies)
	{
		host.RegisterForeignFunction(L"Foreign", L"Sum", Test_BasicLanguage_ForeignFunction_LightSummer);
		host.RegisterForeignFunction(L"Foreign", L"SumLight", Test_BasicLanguage_ForeignFunction_LightSummer);
		Ptr<LanguageState> state=host.CreateState();
		for(vint j=0;j<assemblies.Count();j++)
		{
			host.LoadAssembly(assemblies[j]->assembly);
			TEST_ASSERT(state->RunInitialization(assemblies[j]->assembly)==ILException::Finished);
			TEST_ASSERT(state->GetStack()->StackTop()==state->GetStack()->StackSize());
		}
		return state;
	}

	BasicDeclarationInfo FindMainFunction(CompilerInfo& info, List<Ptr<CompiledAssembly>>& assemblies)
	{
		BasicDeclarationInfo entryInfo;
		for(vint j=0;j<assemblies.Count() && !entryInfo;j++)
		{
			Ptr<LanguageAssembly> assembly=assemblies[j]->assembly;
			for(vint k=0;k<assembly->GetBasicLanguageMetadata()->GetDeclarationCount() && !entryInfo;k++)
			{
				BasicDeclarationInfo info=assembly->GetBasicLanguageMetadata()->GetDeclaration(k);
				if(info.GetName()==L"main")
				{
					entryInfo=info;
				}
			}
		}
		TEST_ASSERT(entryInfo);
		return entryInfo;
	}

	void RunMainFunction(CompilerInfo& info, Ptr<LanguageState> state, BasicDeclarationInfo entryInfo, const WString& resultType, const WString& resultValue)
	{
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
	}

	void LogAndRunCase(CompilerInfo& info, List<Ptr<CompiledAssembly>>& assemblies, const WString& makeFileName, const WString& resultType, const WString& resultValue)
	{
		for(vint j=0;j<assemblies.Count();j++)
		{
			Ptr<CompiledAssembly> assembly=assemblies[j];
			SaveAssembly(info, assembly);
			LogBeforeExecuting(info, assembly);
		}

		LanguageHost host(65536);
		Ptr<LanguageState> state=InitializeHost(info, host, assemblies);
		BasicDeclarationInfo entryInfo=FindMainFunction(info, assemblies);
		RunMainFunction(info, state, entryInfo, resultType, resultValue);

		for(vint j=0;j<assemblies.Count();j++)
		{
			LogAfterExecuting(info, assemblies[j]);
		}
		LogHost(info, host, makeFileName);
		TEST_ASSERT(state->GetStack()->StackTop()==state->GetStack()->StackSize());
	}

	void MergeAndRunCase(CompilerInfo& info, List<Ptr<CompiledAssembly>>& assemblies, const WString& makeFileName, const WString& resultType, const WString& resultValue)
	{
		LanguageLinker linker;
		linker.RegisterForeignFunction(L"Foreign", L"Sum", Test_BasicLanguage_ForeignFunction_LightSummer);
		linker.RegisterForeignFunction(L"Foreign", L"SumLight", Test_BasicLanguage_ForeignFunction_LightSummer);
		for(vint i=0;i<assemblies.Count();i++)
		{
			FileStream stream(assemblies[i]->binPath, FileStream::ReadOnly);
			linker.LoadAssembly(stream);
		}
		Ptr<LanguageAssembly> linkedAssembly=linker.Link();
		{
			WString logLinkerPath=GetPath()+L"[MergedAssembly][Log]["+makeFileName+L"][Linker].txt";
			FileStream fileStream(logLinkerPath, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			linker.LogInternalState(writer);
		}
		{
			WString logLinkerPath=GetPath()+L"[MergedAssembly][Log]["+makeFileName+L"][Assembly].txt";
			FileStream fileStream(logLinkerPath, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			linkedAssembly->LogInternalState(writer);
		}
		{
			WString logLinkerPath=GetPath()+L"[MergedAssembly][Binary]["+makeFileName+L"]["+linkedAssembly->GetAssemblyName()+L"].assembly";
			FileStream fileStream(logLinkerPath, FileStream::WriteOnly);
			linkedAssembly->SaveToStream(fileStream);
		}

		vint mainFunctionIndex=-1;
		{
			Ptr<ResourceStream> resource=linkedAssembly->GetResource(BasicILResourceNames::ExportedSymbols);
			ResourceArrayRecord<BasicILExportRes> exportsRes=resource->ReadArrayRecord(resource->ReadRootRecord<BasicILEntryRes>()->exports);
			for(vint i=0;i<exportsRes.Count();i++)
			{
				ResourceRecord<BasicILExportRes> exportRes=exportsRes.Get(i);
				WString symbolName=resource->ReadString(exportRes->name);
				if(symbolName.Left(10)==L"[ASSEMBLY]" && symbolName.Right(6)==L"::main")
				{
					mainFunctionIndex=exportRes->address;
				}
			}
		}
		TEST_ASSERT(mainFunctionIndex!=-1);
		LanguageHost host(65536);
		host.RegisterForeignFunction(L"Foreign", L"Sum", Test_BasicLanguage_ForeignFunction_LightSummer);
		host.RegisterForeignFunction(L"Foreign", L"SumLight", Test_BasicLanguage_ForeignFunction_LightSummer);
		host.LoadAssembly(linkedAssembly);
		Ptr<LanguageState> state=host.CreateState();
		state->RunInitialization(linkedAssembly);

		if(resultType==L"int")
		{
			vint result=0;
			state->PrepareToRun(linkedAssembly, mainFunctionIndex, &result);
			TEST_ASSERT(state->Run()==ILException::Finished);
			if(result!=wtoi(resultValue))
			{
				vl::unittest::UnitTest::PrintError(L"Expect: "+resultValue+L", Actual: "+itow(result));
			}
			TEST_ASSERT(result==wtoi(resultValue));
		}
		else if(resultType==L"bool")
		{
			bool result=false;
			state->PrepareToRun(linkedAssembly, mainFunctionIndex, &result);
			TEST_ASSERT(state->Run()==ILException::Finished);
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
	}
}
using namespace TestCodeInIndexHelper;

TEST_CASE(TestCodeInIndex)
{
	vl::unittest::UnitTest::PrintInfo(L"Initializing Case Maker...");
	CompilerInfo info;
	List<WString> cases;
	LoadCases(cases);

	for(vint i=0;i<cases.Count();i++)
	{
		vl::unittest::UnitTest::PrintInfo(L"Running Code Case: "+cases[i]);

		List<Ptr<CompiledAssembly>> assemblies;
		WString makeFileName, resultType, resultValue;
		CompileCase(info, cases[i], assemblies, makeFileName, resultType, resultValue);
		LogAndRunCase(info, assemblies, makeFileName, resultType, resultValue);
		MergeAndRunCase(info, assemblies, makeFileName, resultType, resultValue);
	}
}
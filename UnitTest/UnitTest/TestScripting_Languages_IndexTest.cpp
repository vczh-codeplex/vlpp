#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"
#include "..\..\Library\Scripting\Languages\BasicFunctionExecutor.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageResource.h"
#include "..\..\Library\Scripting\Languages\LanguageProviderExtension.h"

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

TEST_CASE(TestCodeInIndex)
{
	Ptr<ILanguageProvider> naitvexProvider=CreateNativeXLanguageProvider();
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
		vl::unittest::UnitTest::PrintMessage(L"Running Code Case: "+singleCase);
		RegexMatch::Ref singleCaseMatch=regexCase.Match(singleCase);
		WString makeFilePath=GetPath()+L"Code\\"+singleCaseMatch->Groups()[L"make"][0].Value();
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
				provider=naitvexProvider;
			}
			List<Ptr<LanguageAssembly>> references;
			List<Ptr<LanguageException>> errors;
			Ptr<LanguageAssembly> assembly=provider->Compile(assemblyName, references.Wrap(), codes.Wrap(), errors.Wrap());
			TEST_ASSERT(errors.Count()==0);
			assemblies.Add(assembly);
		}

		LanguageHost host(65536);
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
			TEST_ASSERT(result==wtoi(resultValue));
		}
		else
		{
			TEST_ASSERT(false);
		}
		TEST_ASSERT(state->GetStack()->StackTop()==state->GetStack()->StackSize());
	}
}
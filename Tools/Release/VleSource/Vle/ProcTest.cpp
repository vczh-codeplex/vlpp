#include "Common.h"

bool ProcTest(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Unit Test Launcher ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==3 && wupper(args[1])==L"LINKED")
	{
		LanguageHost host(65536);
		InitHost(host);
		Ptr<LanguageAssembly> assembly;
		{
			FileStream fileStream(args[2], FileStream::ReadOnly);
			if(!fileStream.IsAvailable())
			{
				throw Exception(L"Cannot open file to read: \""+args[2]+L"\".");
			}
			assembly=new LanguageAssembly(fileStream);
		}
		host.LoadAssembly(assembly);

		Ptr<LanguageState> state=host.CreateState();
		ILException::RunningResult result=state->RunInitialization(assembly);
		if(result!=ILException::Finished)
		{
			throw ILException(result);
		}

		List<BasicILExportInfo> testCases;
		BasicILMetadata* metadata=assembly->GetBasicILMetadata();
		if(metadata->IsAvailable())
		{
			for(vint i=0;i<metadata->GetExportCount();i++)
			{
				BasicILExportInfo info=metadata->GetExport(i);
				if(info.IsFunction())
				{
					WString name=info.GetName();
					if(name.Length()>=21 && name.Left(10)==L"[ASSEMBLY]" && wcsstr(name.Buffer(), L"::TestCase_")!=0)
					{
						testCases.Add(info);
					}
				}
			}
		}
		
		for(vint i=0;i<testCases.Count();i++)
		{
			Console::WriteLine(L"Running Test Case: "+testCases[i].GetName());
			state->PrepareToRun(assembly, testCases[i].GetInstructionIndex(), 0);
			ILException::RunningResult result=state->Run();
			if(result!=ILException::Finished)
			{
				throw ILException(result);
			}
		}

		return true;
	}
	if(args.Count()==2)
	{
		List<WString> assemblyList;
		List<Ptr<LanguageAssembly>> assemblies;
		LanguageHost host(65536);
		ReadAssemblyListFile(args[1], assemblyList);
		InitHost(host);
		Ptr<LanguageState> state=LoadAssembly(host, GetFolder(args[1]), assemblyList, assemblies);

		List<BasicDeclarationInfo> testCases;
		for(vint i=0;i<assemblies.Count();i++)
		{
			Ptr<LanguageAssembly> assembly=assemblies[i];
			BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
			if(metadata->IsAvailable())
			{
				for(vint j=0;j<metadata->GetDeclarationCount();j++)
				{
					BasicDeclarationInfo declaration=metadata->GetDeclaration(j);
					if(declaration.IsFunction() && declaration.GetParameterCount()==0)
					{
						BasicTypeInfo resultType=declaration.GetType().GetElementType();
						if(resultType.IsPrimitive() && resultType.GetPrimitive()==BasicTypeRes::void_type)
						{
							WString name=declaration.GetName();
							if(name.Length()>9 && name.Left(9)==L"TestCase_")
							{
								testCases.Add(declaration);
							}
						}
					}
				}
			}
		}

		for(vint i=0;i<testCases.Count();i++)
		{
			Console::WriteLine(L"Running Test Case: "+testCases[i].GetName());
			BasicFunctionExecutor<void()> testCase(testCases[i], state);
			testCase();
		}

		return true;
	}
	return false;
}
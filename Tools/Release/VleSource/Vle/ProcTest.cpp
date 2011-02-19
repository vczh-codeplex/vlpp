#include "Common.h"

bool ProcTest(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Unit Test Launcher ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==2)
	{
		List<WString> assemblyList;
		List<Ptr<LanguageAssembly>> assemblies;
		LanguageHost host(65536);
		ReadAssemblyListFile(args[1], assemblyList);
		InitHost(host);
		Ptr<LanguageState> state=LoadAssembly(host, assemblyList, assemblies);

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
			BasicFunctionExecutor<void()> testCase(testCases[0], state);
			testCase();
		}

		return true;
	}
	return false;
}
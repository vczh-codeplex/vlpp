#include "Common.h"

bool ProcSimulate(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Console Simulator ");
	Console::WriteLine(ProcVersion());
	if(args.Count()==2)
	{
		List<WString> assemblyList;
		List<Ptr<LanguageAssembly>> assemblies;
		LanguageHost host(65536);
		ReadAssemblyListFile(args[1], assemblyList);
		InitHost(host);
		Ptr<LanguageState> state=LoadAssembly(host, GetFolder(args[1]), assemblyList, assemblies);

		List<BasicDeclarationInfo> mainFunctions;
		for(vint i=0;i<assemblies.Count();i++)
		{
			Ptr<LanguageAssembly> assembly=assemblies[i];
			BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
			if(metadata->IsAvailable())
			{
				for(vint j=0;j<metadata->GetDeclarationCount();j++)
				{
					BasicDeclarationInfo declaration=metadata->GetDeclaration(j);
					if(declaration.IsFunction() && declaration.GetParameterCount()==0 && declaration.GetName()==L"main")
					{
						BasicTypeInfo resultType=declaration.GetType().GetElementType();
						if(resultType.IsPrimitive() && resultType.GetPrimitive()==BasicTypeRes::void_type)
						{
							mainFunctions.Add(declaration);
						}
					}
				}
			}
		}

		if(mainFunctions.Count()==0)
		{
			throw Exception(L"Cannot not find the main function.");
		}
		else if(mainFunctions.Count()>1)
		{
			throw Exception(L"Too many main functions");
		}
		else
		{
			BasicFunctionExecutor<void()> main(mainFunctions[0], state);
			main();
		}

		return true;
	}
	return false;
}
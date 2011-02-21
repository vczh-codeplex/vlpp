#include "Common.h"

bool ProcSimulate(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Console Simulator ");
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

		List<BasicILExportInfo> mainFunctions;
		BasicILMetadata* metadata=assembly->GetBasicILMetadata();
		if(metadata->IsAvailable())
		{
			for(vint i=0;i<metadata->GetExportCount();i++)
			{
				BasicILExportInfo info=metadata->GetExport(i);
				if(info.IsFunction())
				{
					WString name=info.GetName();
					if(name.Length()>=16 && name.Left(10)==L"[ASSEMBLY]" && name.Right(6)==L"::main")
					{
						mainFunctions.Add(info);
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
			state->PrepareToRun(assembly, mainFunctions[0].GetInstructionIndex(), 0);
			ILException::RunningResult result=state->Run();
			if(result!=ILException::Finished)
			{
				throw ILException(result);
			}
		}

		return true;
	}
	else if(args.Count()==2)
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
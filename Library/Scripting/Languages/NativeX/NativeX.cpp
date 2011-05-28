#include "NativeX.h"
#include "NativeXErrorMessage.h"
#include "NativeXParser.h"
#include "NativeXErrorMessageTranslator.h"
#include "NativeXCodeGenerator.h"

#include "..\LanguageProviderExtension.h"
#include "..\..\BasicLanguage\BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
Extra
***********************************************************************/

		NativeXHeaderExtra::NativeXHeaderExtra()
			:publicOnly(true)
			,currentAssemblyOnly(true)
			,importReferencedAssemblies(true)
		{
		}

		namespace language_nativex
		{
			using namespace basicil;

/***********************************************************************
½Ó¿Ú
***********************************************************************/

			class NativeXProvider : public Object, public ILanguageProvider, public IBasicLanguageProvider
			{
			protected:
				Ptr<NativeXParser>					parser;

				bool Parse(IReadonlyList<WString>& codes, IList<Ptr<LanguageException>>& errors, IDictionary<WString, Ptr<NativeXUnit>>& units)
				{
					for(vint i=0;i<codes.Count();i++)
					{
						Ptr<NativeXUnit> unit=parser->Parse(codes[i], i, errors);
						if(unit)
						{
							if(units.Keys().Contains(unit->name))
							{
								errors.Add(new LanguageException(NativeXErrorMessage::UnitAlreadyExists(unit->name), 0, 0, 0, unit->codeIndex));
							}
							else
							{
								units.Add(unit->name, unit);
							}
						}
					}
					for(vint i=0;i<units.Count();i++)
					{
						Ptr<NativeXUnit> unit=units.Values()[i];
						for(vint j=0;j<unit->imports.Count();j++)
						{
							if(!units.Keys().Contains(unit->imports[j]))
							{
								errors.Add(new LanguageException(NativeXErrorMessage::UnitNotExists(unit->imports[j]), 0, 0, 0, unit->codeIndex));
							}
						}
					}
					return errors.Count()>0;
				}

				Ptr<NativeXUnit> SearchCircularImportsInternal(
					IReadonlyDictionary<WString, Ptr<NativeXUnit>>& units,
					ICollection<Ptr<NativeXUnit>>& searchedUnits,
					ICollection<Ptr<NativeXUnit>>& safeUnits,
					Ptr<NativeXUnit> current
					)
				{
					if(safeUnits.Contains(current.Obj()))return 0;
					if(searchedUnits.Contains(current.Obj()))return current;
					searchedUnits.Add(current);
					for(vint i=0;i<current->imports.Count();i++)
					{
						Ptr<NativeXUnit> result=SearchCircularImportsInternal(units, searchedUnits, safeUnits, units[current->imports[i]]);
						if(result)
						{
							return result;
						}
					}
					safeUnits.Add(current);
					return 0;
				}

				Ptr<NativeXUnit> SearchCircularImports(IReadonlyDictionary<WString, Ptr<NativeXUnit>>& units)
				{
					SortedList<Ptr<NativeXUnit>> searchedUnits;
					SortedList<Ptr<NativeXUnit>> safeUnits;
					for(vint i=0;i<units.Count();i++)
					{
						searchedUnits.Clear();
						Ptr<NativeXUnit> result=SearchCircularImportsInternal(units, searchedUnits.Wrap(), safeUnits.Wrap(), units.Values()[i]);
						if(result)
						{
							return result;
						}
					}
					return 0;
				}

				void SortUnits(IDictionary<WString, Ptr<NativeXUnit>>& units, IList<Ptr<NativeXUnit>>& sortedUnits)
				{
					while(units.Count())
					{
						for(vint i=0;i<units.Count();i++)
						{
							WString name=units.Keys()[i];
							Ptr<NativeXUnit> unit=units.Values()[i];
							vint count=0;
							for(vint j=0;j<units.Count();j++)
							{
								if(unit->imports.Contains(units.Values()[j]->name))
								{
									count++;
								}
							}
							if(count==0)
							{
								sortedUnits.Add(units.Values()[i]);
								units.Remove(name);
								break;
							}
						}
					}
				}

				void SetConfiguration(BasicAlgorithmConfiguration& config)
				{
					config.treatCharacterAsInteger=true;
					config.enableImplicitBooleanToIntegerConversion=true;
					config.enableImplicitIntegerToBooleanConversion=true;
					config.enableImplicitIntegerToFloatConversion=true;
					config.enableImplicitUnsignedToSignedConversion=true;
					config.enableImplicitPointerToBooleanConversion=true;
					config.enablePointerArithmetic=true;
					config.enableSubscribeOnPointer=true;
				}
			public:
				NativeXProvider()
				{
					parser=NativeXParser::Create();
				}

				WString LanguageName()
				{
					return L"NativeX";
				}

				LanguageTypeEnum LanguageType()
				{
					return ILanguageProvider::Native;
				}

				Ptr<LanguageAssembly> Compile(
					const WString& assemblyName,
					IReadonlyList<Ptr<LanguageAssembly>>& references,
					IReadonlyList<WString>& codes, 
					IList<Ptr<LanguageException>>& errors,
					TextWriter* assemblyLogTextWriter,
					BasicIL::ICommentProvider* assemblyLogCommentProvider
					)
				{
					Dictionary<WString, Ptr<NativeXUnit>> units;
					Parse(codes, errors, units.Wrap());
					if(errors.Count()>0)
					{
						return 0;
					}
					if(Ptr<NativeXUnit> unit=SearchCircularImports(units.Wrap()))
					{
						errors.Add(new LanguageException(NativeXErrorMessage::UnitCircularReferenced(unit->name), 0, 0, 0, unit->codeIndex));
						return 0;
					}

					List<Ptr<NativeXUnit>> sortedUnits;
					SortUnits(units.Wrap(), sortedUnits.Wrap());
					Ptr<BasicProgram> program=new BasicProgram;
					for(vint i=0;i<sortedUnits.Count();i++)
					{
						CopyFrom(program->declarations.Wrap(), sortedUnits[i]->program->declarations.Wrap(), true);
					}

					BasicAlgorithmConfiguration configuration;
					SetConfiguration(configuration);
					BasicAnalyzer analyzer(program, 0, configuration);
					analyzer.Analyze();
					if(analyzer.GetErrors().Count()>0)
					{
						Ptr<BasicErrorMessageTranslator> translator=CreateNativeXErrorMessageTranslator(&analyzer);
						for(vint i=0;i<analyzer.GetErrors().Count();i++)
						{
							errors.Add(translator->Translate(analyzer.GetErrors()[i], analyzer.GetErrors()[i]->GetBasicLanguageElement()->position.codeIndex));
						}
						return 0;
					}

					BasicCodeGenerator codegen(&analyzer, 0, assemblyName);
					codegen.GenerateCode();
					Ptr<LanguageAssembly> assembly=new LanguageAssembly(codegen.GetIL());
					if(assemblyLogTextWriter)
					{
						assembly->LogInternalState(*assemblyLogTextWriter, assemblyLogCommentProvider);
					}
					return assembly;
				}
				
				Ptr<BasicProgram> ParseProgram(const WString& code, Ptr<Object>& outputExtra, IList<Ptr<LanguageException>>& errors)
				{
					Ptr<NativeXUnit> unit=parser->Parse(code, 0, errors);
					if(unit)
					{
						Ptr<NativeXLanguageExtra> extra=new NativeXLanguageExtra;
						extra->name=unit->name;
						CopyFrom(extra->imports.Wrap(), unit->imports.Wrap());
						outputExtra=extra;
						return unit->program;
					}
					else
					{
						outputExtra=0;
						return 0;
					}
				}
				
				void GenerateCode(Ptr<basiclanguage::BasicProgram> program, Ptr<Object> inputExtra, stream::TextWriter& writer)
				{
					Ptr<NativeXLanguageExtra> extra=inputExtra.Cast<NativeXLanguageExtra>();
					if(extra)
					{
						writer.WriteString(L"unit ");
						writer.WriteString(extra->name);
						writer.WriteLine(L";");
						if(extra->imports.Count()>0)
						{
							writer.WriteString(L"uses ");
							for(vint i=0;i<extra->imports.Count();i++)
							{
								if(i>0)writer.WriteString(L", ");
								writer.WriteString(extra->imports[0]);
							}
							writer.WriteLine(L";");
						}
						writer.WriteLine(L"");
					}
					else
					{
						writer.WriteLine(L"unit nativex_program_generated;");
						writer.WriteLine(L"");
					}
					NativeX_BasicProgram_GenerateCode(program, writer);
				}
				
				bool GenerateHeader(Ptr<LanguageAssembly> assembly, Ptr<Object> inputExtra, stream::TextWriter& writer)
				{
					Ptr<ResourceStream> resource=assembly->GetResource(BasicILResourceNames::BasicLanguageInterfaces);
					if(resource)
					{
						Ptr<NativeXHeaderExtra> headerExtra=inputExtra.Cast<NativeXHeaderExtra>();
						if(!headerExtra)
						{
							headerExtra=new NativeXHeaderExtra();
						}

						List<WString> referencedAssemblies;
						Ptr<BasicProgram> program=BasicLanguage_GenerateHeaderFile(
							assembly->GetAssemblyName(),
							resource,
							headerExtra->publicOnly,
							headerExtra->currentAssemblyOnly,
							headerExtra->declarationPrefix,
							referencedAssemblies.Wrap()
							);

						Ptr<NativeXLanguageExtra> generateExtra=new NativeXLanguageExtra;
						generateExtra->name=headerExtra->assemblyPrefix+assembly->GetAssemblyName();
						if(headerExtra->importReferencedAssemblies)
						{
							for(vint i=0;i<referencedAssemblies.Count();i++)
							{
								generateExtra->imports.Add(headerExtra->assemblyPrefix+referencedAssemblies[i]);
							}
						}
						GenerateCode(program, generateExtra, writer);
						return true;
					}
					else
					{
						return false;
					}
				}
			};
		}

		Ptr<ILanguageProvider> CreateNativeXLanguageProvider()
		{
			return new language_nativex::NativeXProvider;
		}
	}
}
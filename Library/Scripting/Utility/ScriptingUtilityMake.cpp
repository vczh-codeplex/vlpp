#include "ScriptingUtilityMake.h"
#include "..\..\Stream\FileStream.h"
#include "..\..\Stream\CharFormat.h"

#include "..\BasicLanguage\BasicLanguageCommentProvider.h"
#include "..\Languages\LanguageProviderExtension.h"
#include "..\Languages\NativeX\NativeX.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace collections;
			using namespace stream;
			using namespace scripting;

/***********************************************************************
LanguageMakerException
***********************************************************************/

			LanguageMakerException::LanguageMakerException(const WString& message)
				:Exception(message)
			{
			}

			LanguageMakerException::LanguageMakerException(const WString& message, const IReadonlyList<Ptr<LanguageException>>& _errors)
				:Exception(message)
			{
				errors=new List<Ptr<LanguageException>>;
				CopyFrom(errors->Wrap(), _errors);
			}

/***********************************************************************
LanguageMakeFile
***********************************************************************/

			LanguageMakeFile::LanguageMakeFile()
			{
			}

			void LanguageMakeFile::Load(const WString& _baseLocation, TextReader& reader)
			{
				baseLocation=_baseLocation;
				while(!reader.IsEnd())
				{
					WString line=reader.ReadLine();
					if(line!=L"")
					{
						if(line.Left(9)==L"Language=")
						{
							language
								=line.Right(line.Length()-9);
						}
						else if(line.Left(13)==L"AssemblyName=")
						{
							assemblyName
								=line.Right(line.Length()-13);
						}
						else if(line.Left(15)==L"AssemblyOutput=")
						{
							assemblyOutput
								=line.Right(line.Length()-15);
						}
						else if(line.Left(15)==L"HeaderLanguage=")
						{
							headerLanguage
								=line.Right(line.Length()-15);
						}
						else if(line.Left(13)==L"HeaderOutput=")
						{
							headerOutput
								=line.Right(line.Length()-13);
						}
						else if(line.Left(20)==L"DebugAssemblyOutput=")
						{
							debugAssemblyOutput
								=line.Right(line.Length()-20);
						}
						else if(line.Left(8)==L"Compile=")
						{
							compiles.Add(line.Right(line.Length()-8));
						}
						else
						{
							throw LanguageMakerException(L"Don't know how to do: \""+line+L"\".");
						}
					}
				}
			}

/***********************************************************************
LanguageMaker
***********************************************************************/

			Ptr<ILanguageProvider> LanguageMaker::GetProvider(const WString& name)
			{
				vint index=providers.Keys().IndexOf(name);
				if(index!=-1)
				{
					return providers.Values()[index];
				}
				else
				{
					index=providerFactories.Keys().IndexOf(name);
					if(index!=-1)
					{
						Ptr<ILanguageProvider> provider=providerFactories.Values()[index]();
						providers.Add(name, provider);
						return provider;
					}
					else
					{
						throw LanguageMakerException(L"Don't know language: \""+name+L"\".");
					}
				}
			}

			LanguageMaker::LanguageMaker()
			{
				providerFactories.Add(L"NativeX", &CreateNativeXLanguageProvider);
			}

			void LanguageMaker::Execute(const LanguageMakeFile& makeFile)
			{
				Ptr<ILanguageProvider> provider=GetProvider(makeFile.language);
				List<WString> codes;
				for(vint i=0;i<makeFile.compiles.Count();i++)
				{
					WString path=makeFile.baseLocation+makeFile.compiles.Get(i);
					FileStream fileStream(path, FileStream::ReadOnly);
					if(!fileStream.IsAvailable())
					{
						throw LanguageMakerException(L"Cannot open file to read: \""+path+L"\".");
					}
					BomDecoder decoder;
					DecoderStream decoderStream(fileStream, decoder);
					StreamReader reader(decoderStream);
					codes.Add(reader.ReadToEnd());
				}

				List<Ptr<LanguageAssembly>> references;
				List<Ptr<LanguageException>> errors;
				Ptr<LanguageAssembly> assembly;
				if(makeFile.debugAssemblyOutput==L"")
				{
					assembly=provider->Compile(makeFile.assemblyName, references.Wrap(), codes.Wrap(), errors.Wrap());
				}
				else
				{
					Ptr<basicil::BasicIL::ICommentProvider> commentProvider;
					switch(provider->LanguageType())
					{
					case ILanguageProvider::Native:
						{
							basiclanguage::BasicLanguageCommentProvider* basicProvider=new basiclanguage::BasicLanguageCommentProvider;
							commentProvider=basicProvider;
							for(vint i=0;i<codes.Count();i++)
							{
								TextWriter* writer=basicProvider->OpenWriter();
								writer->WriteString(codes[i]);
								basicProvider->CloseWriter();
							}
						}
						break;
					}
					WString path=makeFile.baseLocation+makeFile.debugAssemblyOutput;
					FileStream fileStream(path, FileStream::WriteOnly);
					if(!fileStream.IsAvailable())
					{
						throw LanguageMakerException(L"Cannot open file to write: \""+path+L"\".");
					}
					BomEncoder encoder(BomEncoder::Utf16);
					EncoderStream encoderStream(fileStream, encoder);
					StreamWriter writer(encoderStream);
					assembly=provider->Compile(makeFile.assemblyName, references.Wrap(), codes.Wrap(), errors.Wrap(), &writer, commentProvider.Obj());
				}

				if(errors.Count()>0)
				{
					throw LanguageMakerException(L"Errors.", errors.Wrap());
				}

				if(makeFile.assemblyOutput!=L"")
				{
					WString path=makeFile.baseLocation+makeFile.assemblyOutput;
					FileStream fileStream(path, FileStream::WriteOnly);
					assembly->SaveToStream(fileStream);
				}

				if(makeFile.headerLanguage!=L"" && makeFile.headerOutput!=L"")
				{
					WString path=makeFile.baseLocation+makeFile.headerOutput;
					FileStream fileStream(path, FileStream::WriteOnly);
					if(!fileStream.IsAvailable())
					{
						throw LanguageMakerException(L"Cannot open file to write: \""+path+L"\".");
					}
					BomEncoder encoder(BomEncoder::Utf16);
					EncoderStream encoderStream(fileStream, encoder);
					StreamWriter writer(encoderStream);

					Ptr<ILanguageProvider> headerProvider=GetProvider(makeFile.headerLanguage);
					switch(headerProvider->LanguageType())
					{
					case ILanguageProvider::Native:
						{
							Ptr<IBasicLanguageProvider> basicProvider=headerProvider.Cast<IBasicLanguageProvider>();
							basicProvider->GenerateHeader(assembly, 0, writer);
						}
						break;
					default:
						throw LanguageMakerException(L"Don't know how to generate header file in language: \""+makeFile.headerLanguage+L"\".");
					}
				}
			}
		}
	}
}
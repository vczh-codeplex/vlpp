#include "ScriptingUtilityMake.h"
#include "..\..\Stream\FileStream.h"
#include "..\..\Stream\CharFormat.h"

#include "..\BasicLanguage\BasicLanguageCommentProvider.h"
#include "..\Languages\LanguageProviderBasicExtension.h"
#include "..\Languages\LanguageProviderManagedExtension.h"
#include "..\Languages\NativeX\NativeX.h"
#include "..\Languages\ManagedX\ManagedX.h"

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
			
			bool LanguageMakeFile::Read(const WString& line, const WString& name, WString& result)
			{
				if(line.Length()>name.Length()+1 && line.Left(name.Length()+1)==name+L"=")
				{
					result=line.Right(line.Length()-name.Length()-1);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool LanguageMakeFile::Read(const WString& line, const WString& name, Pair& result)
			{
				WString temp;
				bool successful=false;
				if(successful=Read(line, name, temp))
				{
					vint index=temp.IndexOf(L';');
					if(index==-1)
					{
						result.name=L"";
						result.value=temp;
					}
					else
					{
						result.name=temp.Left(index);
						result.value=temp.Right(temp.Length()-index-1);
					}
				}
				return successful;
			}

			bool LanguageMakeFile::Read(const WString& line, const WString& name, collections::List<WString>& result)
			{
				WString temp;
				bool successful=false;
				if(successful=Read(line, name, temp))
				{
					result.Add(temp);
				}
				return successful;
			}

			bool LanguageMakeFile::Read(const WString& line, const WString& name, collections::List<Pair>& result)
			{
				Pair temp;
				bool successful=false;
				if(successful=Read(line, name, temp))
				{
					result.Add(temp);
				}
				return successful;
			}

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
						if(!(
							Read(line, L"Language", language) ||
							Read(line, L"DebugAssemblyOutput", debugAssemblyOutput) ||
							Read(line, L"ErrorLog", errorLog) ||
							Read(line, L"Assembly", assembly) ||
							Read(line, L"Header", headers) ||
							Read(line, L"Compile", compiles)))
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
						CHECK_ERROR(name==provider->LanguageName(), L"LanguageMaker::GetProvider(const WString&)#±àÒëÆ÷Ãû×Ö²»Æ¥Åä¡£");
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
				providerFactories.Add(L"ManagedX", &CreateManagedXLanguageProvider);
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
					assembly=provider->Compile(makeFile.assembly.name, references.Wrap(), codes.Wrap(), errors.Wrap());
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
					assembly=provider->Compile(makeFile.assembly.name, references.Wrap(), codes.Wrap(), errors.Wrap(), &writer, commentProvider.Obj());
				}

				if(errors.Count()>0)
				{
					if(makeFile.errorLog!=L"")
					{
						FileStream fileStream(makeFile.baseLocation+makeFile.errorLog, FileStream::WriteOnly);
						BomEncoder encoder(BomEncoder::Utf16);
						EncoderStream encoderStream(fileStream, encoder);
						StreamWriter writer(encoderStream);

						for(vint i=0;i<errors.Count();i++)
						{
							Ptr<LanguageException> error=errors[i];
							writer.WriteLine(makeFile.compiles.Get(error->CodeIndex())+L"("+itow(error->LineIndex()+1)+L"):");
							writer.WriteLine(L"  "+error->Message());
						}
					}
					throw LanguageMakerException(L"Errors.", errors.Wrap());
				}

				if(makeFile.assembly.value!=L"")
				{
					WString path=makeFile.baseLocation+makeFile.assembly.value;
					FileStream fileStream(path, FileStream::WriteOnly);
					assembly->SaveToStream(fileStream);
				}

				for(vint i=0;i<makeFile.headers.Count();i++)
				{
					const LanguageMakeFile::Pair& header=makeFile.headers.Get(i);
					if(header.name!=L"" && header.value!=L"")
					{
						WString path=makeFile.baseLocation+header.value;
						FileStream fileStream(path, FileStream::WriteOnly);
						if(!fileStream.IsAvailable())
						{
							throw LanguageMakerException(L"Cannot open file to write: \""+path+L"\".");
						}
						BomEncoder encoder(BomEncoder::Utf16);
						EncoderStream encoderStream(fileStream, encoder);
						StreamWriter writer(encoderStream);

						Ptr<ILanguageProvider> headerProvider=GetProvider(header.name);
						switch(headerProvider->LanguageType())
						{
						case ILanguageProvider::Native:
							{
								Ptr<IBasicLanguageProvider> basicProvider=headerProvider.Cast<IBasicLanguageProvider>();
								basicProvider->GenerateHeader(assembly, 0, writer);
							}
							break;
						default:
							throw LanguageMakerException(L"Don't know how to generate header file in language: \""+header.value+L"\".");
						}
					}
				}
			}
		}
	}
}
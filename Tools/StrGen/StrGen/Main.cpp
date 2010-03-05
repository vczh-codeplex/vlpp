#include "..\..\..\Library\Console.h"
#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\Library\Stream\Accessor.h"
#include "..\..\..\Library\Stream\CharFormat.h"
#include "..\..\..\Library\Collections\List.h"
#include "..\..\..\Library\Collections\OperationCopyFrom.h"
#include "..\..\..\Library\Regex\Regex.h"

#include <windows.h>

using namespace vl;
using namespace vl::console;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;

void PrintError(const WString& message)
{
	Console::SetColor(true, false, false, true);
	Console::WriteLine(message);
	Console::SetColor(false, true, false, true);
}

WString GenerateIncludePath(const WString& includedFileName, const WString& currentFileName)
{
	WString lowerCurrent=wlower(currentFileName);
	WString lowerIncluded=wlower(includedFileName);
	int index=0;
	if(lowerCurrent==lowerIncluded)
	{
		index=lowerCurrent.Length();
	}
	else while(lowerCurrent[index]==lowerIncluded[index])
	{
		index++;
	}

	int count=0;
	for(int i=index;i<currentFileName.Length();i++)
	{
		if(currentFileName[i]==L'\\')
		{
			count++;
		}
	}

	WString result;
	for(int i=0;i<count;i++)
	{
		result+=L"..\\";
	}
	result+=includedFileName.Sub(index, includedFileName.Length()-index);
	return result;
}

WString BuildHeaderComment(const WString& className)
{
	return
L"/***********************************************************************"	L"\r\n"
L"Vczh Library++ 3.0"														L"\r\n"
L"Developer: �����(vczh)"													L"\r\n"
L"StringResource::"+className+												L"\r\n"
L""																			L"\r\n"
L"Classes:"																	L"\r\n"
L"	"+className+L"									���ַ�����Դ��"			L"\r\n"
L"	"																		L"\r\n"
L"���ļ�ʹ��Vczh String Resource Code Generator�����Զ�����"				L"\r\n"
L"***********************************************************************/"	L"\r\n";
}

void GenerateCode(TextReader& resource, TextWriter& header, TextWriter& code, const WString& headerName, const WString& libraryPath)
{
	WString headerContent;
	WString codeContent;
	Regex funcdef(L"^(<NAME>/w+)(/((<PARAM>/w+)(,(<PARAM>/w+))*/))?=(<VALUE>/.*)$");
	Regex varref(L"/{(<NAME>/w+)/}");

	//���������ռ������
	WString resourceNameSpace=resource.ReadLine();
	if(resourceNameSpace.Length()<10 && resourceNameSpace.Left(9)!=L"resource:")
	{
		PrintError(L"The format of the first line is resource:namespace.");
		return;
	}
	List<WString> namespaceItems;
	resourceNameSpace=resourceNameSpace.Sub(9, resourceNameSpace.Length()-9)+L'.';
	int previousIndex=0;
	for(int i=0;i<resourceNameSpace.Length();i++)
	{
		if(resourceNameSpace[i]==L'.')
		{
			namespaceItems.Add(resourceNameSpace.Sub(previousIndex, i-previousIndex));
			previousIndex=i+1;
		}
	}
	WString className=namespaceItems[namespaceItems.Count()-1];
	namespaceItems.RemoveAt(namespaceItems.Count()-1);

	//����ͷ�ļ�ͷ��
	headerContent+=BuildHeaderComment(className);
	WString protectMacro=L"VCZH_STRING_RESOURCE_CODE_FILE_";
	for(int i=0;i<namespaceItems.Count();i++)
	{
		protectMacro+=wupper(namespaceItems[i])+L"_";
	}
	protectMacro+=wupper(className);
	headerContent+=
		L"\r\n"
		L"#ifndef "+protectMacro+L"\r\n"
		L"#define "+protectMacro+L"\r\n"
		L"\r\n";
	headerContent+=L"#include \""+libraryPath+L"String.h\"\r\n\r\n";
	headerContent+=L"using namespace vl;\r\n\r\n";

	//���ɴ����ļ�ͷ��
	codeContent+=L"#include \""+headerName+L"\"\r\n\r\n";

	//���������ռ�ͷ��
	WString prefix;
	for(int i=0;i<namespaceItems.Count();i++)
	{
		headerContent+=prefix+L"namespace "+namespaceItems[i]+L"\r\n"+prefix+L"{\r\n";
		codeContent+=prefix+L"namespace "+namespaceItems[i]+L"\r\n"+prefix+L"{\r\n";
		prefix+=L"\t";
	}
	headerContent+=prefix+L"class "+className+L"\r\n"+prefix+L"{\r\n"+prefix+L"public:\r\n";

	//����Ϊ�ļ�����
	while(!resource.IsEnd())
	{
		WString line=resource.ReadLine();
		if(line!=L"")
		{
			Ptr<RegexMatch> match=funcdef.MatchHead(line);
			if(match->Success())
			{
				WString functionName=match->Groups()[L"NAME"][0].Value();
				List<WString> parameters;
				if(match->Groups().Keys().Contains(L"PARAM"))
				{
					const IReadonlyList<RegexString>& strings=match->Groups()[L"PARAM"];
					for(int i=0;i<strings.Count();i++)
					{
						parameters.Add(strings[i].Value());
					}
				}
				//����ͷ�ļ�����
				headerContent+=prefix+L"\tstatic WString "+functionName+L"(";
				for(int i=0;i<parameters.Count();i++)
				{
					if(i)headerContent+=L", ";
					headerContent+=L"const WString& "+parameters[i];
				}
				headerContent+=L");\r\n";
				//���ɴ����ļ�����
				codeContent+=prefix+L"WString "+className+L"::"+functionName+L"(";
				for(int i=0;i<parameters.Count();i++)
				{
					if(i)codeContent+=L", ";
					codeContent+=L"const WString& "+parameters[i];
				}
				codeContent+=L")\r\n"+prefix+L"{\r\n"+prefix+L"\treturn ";
				List<Ptr<RegexMatch>> values;
				varref.Cut(match->Groups()[L"VALUE"][0].Value(), false, values);
				bool previousIsConst=true;
				for(int i=0;i<values.Count();i++)
				{
					if(values[i]->Success() && parameters.Contains(values[i]->Groups()[L"NAME"][0].Value()))
					{
						codeContent+=L"+"+values[i]->Groups()[L"NAME"][0].Value();
						previousIsConst=false;
					}
					else if(previousIsConst)
					{
						codeContent+=L"L\""+values[i]->Result().Value()+L"\"";
					}
					else
					{
						codeContent+=L"+L\""+values[i]->Result().Value()+L"\"";
						previousIsConst;
					}
				}
				codeContent+=L";\r\n"+prefix+L"}\r\n\r\n";
			}
			else
			{
				PrintError(L"Syntax error: "+line);
				PrintError(L"The format should be name=value or name(p1,p2,..)=value.");
				return;
			}
		}
	}

	//���������ռ�β��
	headerContent+=prefix+L"};\r\n";
	for(int i=0;i<namespaceItems.Count();i++)
	{
		prefix=prefix.Left(prefix.Length()-1);
		headerContent+=prefix+L"}\r\n";
		codeContent+=prefix+L"}\r\n";
	}

	//����ͷ�ļ�β��
	headerContent+=L"\r\n#endif";

	header.WriteString(headerContent);
	code.WriteString(codeContent);
}

int wmain(int argc, wchar_t* argv[])
{
	WString baseDirectory;
	{
		wchar_t currentDirectory[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH, currentDirectory);
		baseDirectory=currentDirectory;
		if(baseDirectory[baseDirectory.Length()-1]!=L'\\')
		{
			baseDirectory+=L'\\';
		}
	}

	Console::SetTitle(L"Vczh String Resource Code Generator");
	Console::SetColor(false, true, false, true);
	if(argc!=5)
	{
		PrintError(L"StrGen.exe only accepts 4 parameters for <Library> path, resource file, header file and code file.");
		return 0;
	}
	WString libraryPath=argv[1];
	WString resourceFileName=argv[2];
	WString headerFileName=argv[3];
	WString codeFileName=argv[4];
	if(libraryPath[libraryPath.Length()-1]!=L'\\')
	{
		libraryPath+=L'\\';
	}

	WString headerName;
	int headerDirectoryIndex=0;
	for(int i=headerFileName.Length()-1;i>=0;i--)
	{
		if(headerFileName[i]==L'\\')
		{
			headerDirectoryIndex=i+1;
			break;
		}
	}
	headerName=headerFileName.Right(headerFileName.Length()-headerDirectoryIndex);

	FileStream headerFileStream(headerFileName, FileStream::WriteOnly);
	if(!headerFileStream.IsAvailable())
	{
		PrintError(L"error> \""+headerFileName+L"\" can not be opened for writing.");
		return 0;
	}

	FileStream codeFileStream(codeFileName, FileStream::WriteOnly);
	if(!codeFileStream.IsAvailable())
	{
		PrintError(L"error> \""+codeFileName+L"\" can not be opened for writing.");
		return 0;
	}

	FileStream resourceFileStream(resourceFileName, FileStream::ReadOnly);
	if(!resourceFileStream.IsAvailable())
	{
		PrintError(L"error> \""+resourceFileName+L"\" can not be opened for reading.");
		return 0;
	}

	BomEncoder headerFileEncoder(BomEncoder::Mbcs);
	EncoderStream headerStream(headerFileStream, headerFileEncoder);
	StreamWriter headerWriter(headerStream);

	BomEncoder codeFileEncoder(BomEncoder::Mbcs);
	EncoderStream codeStream(codeFileStream, codeFileEncoder);
	StreamWriter codeWriter(codeStream);

	BomDecoder resourceFileDecoder;
	DecoderStream resourceStream(resourceFileStream, resourceFileDecoder);
	StreamReader resourceReader(resourceStream);

	GenerateCode(resourceReader, headerWriter, codeWriter, headerName, GenerateIncludePath(libraryPath, headerFileName));
	Console::WriteLine(L"Finished!");
	return 0;
}
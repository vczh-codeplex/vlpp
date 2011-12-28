#include "Dia2.h"
#include "..\..\GUI\GacVlppReferences.h"
#include <stdio.h>

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;

namespace dumppdb
{

	//--------------------------------------------------------------------

	void PrintString(TextWriter& file, const wchar_t* text, int len=-1)
	{
		if(len==-1) len=(int)wcslen(text);
		file.WriteString(text, len);
	}

	void PrintSpaces(TextWriter& file, int level)
	{
		for(int i=0;i<level;i++) PrintString(file, L"  ");
	}

	void PrintEscapedName(TextWriter& file, const wchar_t* name)
	{
		const wchar_t* head=name;
		const wchar_t* reading=head;
		while(*reading)
		{
			switch(*reading)
			{
			case L'<':
				PrintString(file, head, reading-head);
				PrintString(file, L"&lt;");
				head=reading+1;
				reading=head;
				break;
			case L'>':
				PrintString(file, head, reading-head);
				PrintString(file, L"&gt;");
				head=reading+1;
				reading=head;
				break;
			case L'&':
				PrintString(file, head, reading-head);
				PrintString(file, L"&amp;");
				head=reading+1;
				reading=head;
				break;
			case L'\"':
				PrintString(file, head, reading-head);
				PrintString(file, L"&quot;");
				head=reading+1;
				reading=head;
				break;
			default:
				reading++;
			}
		}
		PrintString(file, head, reading-head);
	}

	void PrintXMLOpen(
		TextWriter& file, int level, const wchar_t* tagName, const wchar_t* symbolName
		,const wchar_t* a1=0, const wchar_t* v1=0
		,const wchar_t* a2=0, const wchar_t* v2=0
		,const wchar_t* a3=0, const wchar_t* v3=0
		)
	{
		PrintSpaces(file, level);
		PrintString(file, L"<");
		PrintString(file, tagName);
		if(symbolName)
		{
			PrintString(file, L" name=\"");
			PrintEscapedName(file, symbolName);
			PrintString(file, L"\"");
		}
		if(a1)
		{
			PrintString(file, L" ");
			PrintString(file, a1);
			PrintString(file, L"=\"");
			PrintEscapedName(file, v1);
			PrintString(file, L"\"");
		}
		if(a2)
		{
			PrintString(file, L" ");
			PrintString(file, a2);
			PrintString(file, L"=\"");
			PrintEscapedName(file, v2);
			PrintString(file, L"\"");
		}
		if(a3)
		{
			PrintString(file, L" ");
			PrintString(file, a3);
			PrintString(file, L"=\"");
			PrintEscapedName(file, v3);
			PrintString(file, L"\"");
		}
		PrintString(file, L" >\r\n");
	}

	void PrintXMLClose(TextWriter& file, int level, const wchar_t* tagName)
	{
		PrintSpaces(file, level);
		PrintString(file, L"</");
		PrintString(file, tagName);
		PrintString(file, L">\r\n");
	}

	//--------------------------------------------------------------------

	Dictionary<WString, IDiaSymbol*> udtSymbols;

	void FindClasses(IDiaSymbol* exeSymbol)
	{
		// enumerate compilands
		IDiaEnumSymbols* compilandEnum=0;
		if(SUCCEEDED(exeSymbol->findChildren(SymTagCompiland, NULL, nsNone, &compilandEnum)))
		{
			DWORD compilandCelt=0;
			IDiaSymbol* compilandSymbol=0;
			while(SUCCEEDED(compilandEnum->Next(1, &compilandSymbol, &compilandCelt)) && compilandSymbol && compilandCelt)
			{
				// enumerate functions
				IDiaEnumSymbols* functionEnum=0;
				if(SUCCEEDED(compilandSymbol->findChildren(SymTagFunction, NULL, nsNone, &functionEnum)))
				{
					DWORD functionCelt=0;
					IDiaSymbol* functionSymbol=0;
					while(SUCCEEDED(functionEnum->Next(1, &functionSymbol, &functionCelt)) && functionSymbol && functionCelt)
					{
						// enumerate this argument
						IDiaEnumSymbols* thisEnum=0;
						if(SUCCEEDED(functionSymbol->findChildren(SymTagData, L"this", nsCaseSensitive, &thisEnum)))
						{
							DWORD thisCelt=0;
							IDiaSymbol* thisSymbol=0;
							while(SUCCEEDED(thisEnum->Next(1, &thisSymbol, &thisCelt)) && thisSymbol && thisCelt)
							{
								// check this argument
								enum DataKind dataKind;
								if(SUCCEEDED(thisSymbol->get_dataKind((DWORD*)&dataKind)))
								{
									if(dataKind==DataIsObjectPtr)
									{
										// get pointer type
										IDiaSymbol* pointerType=0;
										if(SUCCEEDED(thisSymbol->get_type(&pointerType)) && pointerType)
										{
											enum SymTagEnum pointerTypeTag;
											if(SUCCEEDED(pointerType->get_symTag((DWORD*)&pointerTypeTag)) && pointerTypeTag==SymTagPointerType)
											{
												// get this class type
												IDiaSymbol* udtType=0;
												if(SUCCEEDED(pointerType->get_type(&udtType)) && udtType)
												{
													enum SymTagEnum udtTypeTag;
													if(SUCCEEDED(udtType->get_symTag((DWORD*)&udtTypeTag)) && udtTypeTag==SymTagUDT)
													{
														// get class name
														BSTR classNameBSTR=0;
														if(SUCCEEDED(udtType->get_name(&classNameBSTR)) && classNameBSTR)
														{
															WString className=classNameBSTR;
															if(!udtSymbols.Keys().Contains(className))
															{
																// record class symbol
																udtSymbols.Add(className, udtType);
																udtType=0;
															}
														}
													}
												}
												if(udtType) udtType->Release();
											}
										}
										pointerType->Release();
									}
								}
								thisSymbol->Release();
							}
							thisEnum->Release();
						}
						functionSymbol->Release();
					}
					functionEnum->Release();
				}
				compilandSymbol->Release();
			}
			compilandEnum->Release();
		}
	}

	//--------------------------------------------------------------------

	const wchar_t* GetAccessName(enum CV_access_e access)
	{
		switch(access)
		{
		case CV_private: return L"private";
		case CV_protected: return L"protected";
		case CV_public: return L"public";
		default: return L"";
		}
	}

	const wchar_t* GetCallingConversionName(enum CV_call_e callconv)
	{
		switch(callconv)
		{
		case CV_CALL_NEAR_C: return L"cdecl";
		case CV_CALL_NEAR_FAST: return L"fastcall";
		case CV_CALL_NEAR_STD: return L"stdcall";
		case CV_CALL_NEAR_SYS: return L"syscall";
		case CV_CALL_THISCALL: return L"thiscall";
		case CV_CALL_CLRCALL: return L"clrcall";
		default: return L"";
		}
	}

	const wchar_t* GetBasicTypeName(enum BasicType type, int length)
	{
		switch(type)
		{
		case btVoid:		return L"void";
		case btChar:		return L"char";
		case btWChar:		return L"wchar_t";
		case btInt:
		case btLong:		return length==1?L"signed __int8":length==2?L"signed __int16":length==4?L"signed __int32":length==8?L"signed __int64":L"[UnknownSInt]";
		case btUInt:
		case btULong:		return length==1?L"unsigned __int8":length==2?L"unsigned __int16":length==4?L"unsigned __int32":length==8?L"unsigned __int64":L"[UnknownUInt]";
		case btFloat:		return length==4?L"float":length==8?L"double":L"[UnknownFloat]";
		case btBool:		return L"bool";

		case btBCD:			return L"[BCD]";
		case btCurrency:	return L"[Currency]";
		case btDate:		return L"[Date]";
		case btVariant:		return L"[Variant]";
		case btComplex:		return L"[Complex]";
		case btBit:			return L"[Bit]";
		case btBSTR:		return L"[BSTR]";
		case btHresult:		return L"[HRESULT]";
		default:			return L"[NoType]";
		}
	}

	//--------------------------------------------------------------------

	extern void DumpType(TextWriter& file, IDiaSymbol* typeSymbol, int level);

	void DumpTypeHelper(TextWriter& file, IDiaSymbol* typeSymbol, int level, const wchar_t* tagName, const wchar_t* symbolName)
	{
		BOOL constType=FALSE, volatileType=FALSE;
		typeSymbol->get_constType(&constType);
		typeSymbol->get_volatileType(&volatileType);
		PrintXMLOpen(file, level, tagName, symbolName, L"const", (constType?L"true":L"false"), L"volatile", (volatileType?L"true":L"false"));
	}

	void DumpFunctionType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		CV_call_e callconv;
		typeSymbol->get_callingConvention((DWORD*)&callconv);
		PrintXMLOpen(file, level, L"function", NULL, L"callconv", GetCallingConversionName(callconv));
		{
			PrintXMLOpen(file, level+1, L"arguments", NULL);
			{
				IDiaEnumSymbols* argumentEnum=0;
				if(SUCCEEDED(typeSymbol->findChildren(SymTagFunctionArgType, NULL, nsNone, &argumentEnum)) && argumentEnum)
				{
					DWORD argumentCelt=0;
					IDiaSymbol* argumentSymbol=0;
					while(SUCCEEDED(argumentEnum->Next(1, &argumentSymbol, &argumentCelt)) && argumentSymbol && argumentCelt)
					{
						IDiaSymbol* argumentType=0;
						if(SUCCEEDED(argumentSymbol->get_type(&argumentType)))
						{
							PrintXMLOpen(file, level+2, L"argument", NULL);
							DumpType(file, argumentType, level+3);
							PrintXMLClose(file, level+2, L"argument");
							argumentType->Release();
						}
						argumentSymbol->Release();
					}
					argumentEnum->Release();
				}
			}
			PrintXMLClose(file, level+1, L"arguments");
		}
		IDiaSymbol* returnTypeSymbol=0;
		if(SUCCEEDED(typeSymbol->get_type(&returnTypeSymbol)) && returnTypeSymbol)
		{
			PrintXMLOpen(file, level+1, L"return", NULL);
			DumpType(file, returnTypeSymbol, level+2);
			PrintXMLClose(file, level+1, L"return");
			returnTypeSymbol->Release();
		}
		PrintXMLClose(file, level, L"function");
	}

	void DumpPointerType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		IDiaSymbol* elementTypeSymbol=0;
		if(SUCCEEDED(typeSymbol->get_type(&elementTypeSymbol)) && elementTypeSymbol)
		{
			BOOL lref=FALSE;
			BOOL rref=FALSE;
			typeSymbol->get_reference(&lref);
			typeSymbol->get_RValueReference(&rref);
			if(lref)
			{
				PrintXMLOpen(file, level, L"reference", NULL);
				DumpType(file, elementTypeSymbol, level+1);
				PrintXMLClose(file, level, L"reference");
			}
			else if(rref)
			{
				PrintXMLOpen(file, level, L"rightValueReference", NULL);
				DumpType(file, elementTypeSymbol, level+1);
				PrintXMLClose(file, level, L"rightValueReference");
			}
			else
			{
				PrintXMLOpen(file, level, L"pointer", NULL);
				DumpType(file, elementTypeSymbol, level+1);
				PrintXMLClose(file, level, L"pointer");
			}
			elementTypeSymbol->Release();
		}
	}

	void DumpArrayType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		IDiaSymbol* indexTypeSymbol=0;
		IDiaSymbol* elementTypeSymbol=0;
		if(SUCCEEDED(typeSymbol->get_type(&elementTypeSymbol)) && elementTypeSymbol)
		{
			ULONGLONG arraySize=0, elementSize=0;
			typeSymbol->get_length(&arraySize);
			elementTypeSymbol->get_length(&elementSize);
			int elementCount=arraySize?(int)(arraySize/elementSize):0;
			wchar_t elementCountBuffer[20];
			_itow_s(elementCount, elementCountBuffer, 10);

			PrintXMLOpen(file, level, L"array", NULL, L"count", elementCountBuffer);
			if(SUCCEEDED(typeSymbol->get_arrayIndexType(&indexTypeSymbol)) && indexTypeSymbol)
			{
				PrintXMLOpen(file, level+1, L"index", NULL);
				DumpType(file, indexTypeSymbol, level+2);
				PrintXMLClose(file, level+1, L"index");
				indexTypeSymbol->Release();
			}
			PrintXMLOpen(file, level+1, L"element", NULL);
			DumpType(file, elementTypeSymbol, level+2);
			PrintXMLClose(file, level+1, L"element");
			PrintXMLClose(file, level, L"array");
			elementTypeSymbol->Release();
		}
	}

	void DumpBaseType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		enum BasicType basicType=btNoType;
		ULONGLONG length=0;
		if(SUCCEEDED(typeSymbol->get_baseType((DWORD*)&basicType)) && SUCCEEDED(typeSymbol->get_length(&length)))
		{
			DumpTypeHelper(file, typeSymbol, level, L"primitive", GetBasicTypeName(basicType, (int)length));
		}
	}

	void DumpUserType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		BSTR nameBSTR=0;
		if(SUCCEEDED(typeSymbol->get_name(&nameBSTR)) && nameBSTR)
		{
			DumpTypeHelper(file, typeSymbol, level, L"classType", nameBSTR);
		}
	}

	void DumpType(TextWriter& file, IDiaSymbol* typeSymbol, int level)
	{
		enum SymTagEnum symTag=SymTagNull;
		typeSymbol->get_symTag((DWORD*)&symTag);
		switch(symTag)
		{
		case SymTagFunctionType:
			return DumpFunctionType(file, typeSymbol, level);
		case SymTagPointerType:
			return DumpPointerType(file, typeSymbol, level);
		case SymTagArrayType:
			return DumpArrayType(file, typeSymbol, level);
		case SymTagBaseType:
			return DumpBaseType(file, typeSymbol, level);
		case SymTagUDT:
			return DumpUserType(file, typeSymbol, level);
		}
	}

	void DumpSymbolType(TextWriter& file, IDiaSymbol* symbolWithType, int symbolLevel)
	{
		IDiaSymbol* typeSymbol=0;
		if(SUCCEEDED(symbolWithType->get_type(&typeSymbol)) && typeSymbol)
		{
			PrintXMLOpen(file, symbolLevel+1, L"type", NULL);
			DumpType(file, typeSymbol, symbolLevel+2);
			PrintXMLClose(file, symbolLevel+1, L"type");
			typeSymbol->Release();
		}
	}

	//--------------------------------------------------------------------

	void DumpBaseClasses(TextWriter& file, IDiaSymbol* udtSymbol)
	{
		PrintXMLOpen(file, 2, L"baseClasses", NULL, false);
		IDiaEnumSymbols* baseClassEnum=0;
		if(SUCCEEDED(udtSymbol->findChildren(SymTagBaseClass, NULL, nsNone, &baseClassEnum)) && baseClassEnum)
		{
			DWORD baseClassCelt=0;
			IDiaSymbol* baseClassSymbol=0;
			while(SUCCEEDED(baseClassEnum->Next(1, &baseClassSymbol, &baseClassCelt)) && baseClassSymbol && baseClassCelt)
			{
				BSTR nameBSTR=0;
				if(SUCCEEDED(baseClassSymbol->get_name(&nameBSTR)) && nameBSTR)
				{
					PrintXMLOpen(file, 3, L"baseClass", nameBSTR);
					PrintXMLClose(file, 3, L"baseClass");
				}
				baseClassSymbol->Release();
			}
			baseClassEnum->Release();
		}
		PrintXMLClose(file, 2, L"baseClasses");
	}

	void DumpFields(TextWriter& file, IDiaSymbol* udtSymbol)
	{
		PrintXMLOpen(file, 2, L"fields", NULL);
		IDiaEnumSymbols* fieldEnum=0;
		if(SUCCEEDED(udtSymbol->findChildren(SymTagData, NULL, nsNone, &fieldEnum)) && fieldEnum)
		{
			DWORD fieldCelt=0;
			IDiaSymbol* fieldSymbol=0;
			while(SUCCEEDED(fieldEnum->Next(1, &fieldSymbol, &fieldCelt)) && fieldSymbol && fieldCelt)
			{
				enum DataKind dataKind;
				if(SUCCEEDED(fieldSymbol->get_dataKind((DWORD*)&dataKind)) && (dataKind==DataIsMember || dataKind==DataIsStaticMember))
				{
					enum CV_access_e access;
					fieldSymbol->get_access((DWORD*)&access);
					BSTR nameBSTR=0;
					if(SUCCEEDED(fieldSymbol->get_name(&nameBSTR)) && nameBSTR)
					{
						if(dataKind==DataIsMember)
						{
							PrintXMLOpen(file, 3, L"field", nameBSTR, L"access", GetAccessName(access));
							DumpSymbolType(file, fieldSymbol, 3);
							PrintXMLClose(file, 3, L"field");
						}
						else if(dataKind==DataIsStaticMember)
						{
							PrintXMLOpen(file, 3, L"staticField", nameBSTR, L"access", GetAccessName(access));
							DumpSymbolType(file, fieldSymbol, 3);
							PrintXMLClose(file, 3, L"staticField");
						}
					}
				}
				fieldSymbol->Release();
			}
			fieldEnum->Release();
		}
		PrintXMLClose(file, 2, L"fields");
	}

	void DumpMethodArguments(TextWriter& file, IDiaSymbol* methodSymbol)
	{
		PrintXMLOpen(file, 4, L"arguments", NULL);
		IDiaEnumSymbols* argumentEnum=0;
		if(SUCCEEDED(methodSymbol->findChildren(SymTagData, NULL, nsNone, &argumentEnum)) && argumentEnum)
		{
			DWORD argumentCelt=0;
			IDiaSymbol* argumentSymbol=0;
			while(SUCCEEDED(argumentEnum->Next(1, &argumentSymbol, &argumentCelt)) && argumentSymbol && argumentCelt)
			{
				enum DataKind dataKind;
				if(SUCCEEDED(argumentSymbol->get_dataKind((DWORD*)&dataKind)) && dataKind==DataIsParam)
				{
					BSTR nameBSTR=0;
					if(SUCCEEDED(argumentSymbol->get_name(&nameBSTR)) && nameBSTR)
					{
						PrintXMLOpen(file, 5, L"argument", nameBSTR);
						DumpSymbolType(file, argumentSymbol, 5);
						PrintXMLClose(file, 5, L"argument");
					}
				}
				argumentSymbol->Release();
			}
			argumentEnum->Release();
		}
		PrintXMLClose(file, 4, L"arguments");
	}

	void DumpMethods(TextWriter& file, IDiaSymbol* udtSymbol)
	{
		PrintXMLOpen(file, 2, L"methods", NULL);
		IDiaEnumSymbols* methodEnum=0;
		if(SUCCEEDED(udtSymbol->findChildren(SymTagFunction, NULL, nsNone, &methodEnum)) && methodEnum)
		{
			DWORD methodCelt=0;
			IDiaSymbol* methodSymbol=0;
			while(SUCCEEDED(methodEnum->Next(1, &methodSymbol, &methodCelt)) && methodSymbol && methodCelt)
			{
				enum CV_access_e access;
				methodSymbol->get_access((DWORD*)&access);
				BSTR nameBSTR=0;
				if(SUCCEEDED(methodSymbol->get_name(&nameBSTR)) && nameBSTR)
				{
					PrintXMLOpen(file, 3, L"method", nameBSTR, L"access", GetAccessName(access));
					DumpMethodArguments(file, methodSymbol);
					DumpSymbolType(file, methodSymbol, 3);
					PrintXMLClose(file, 3, L"method");
				}
				methodSymbol->Release();
			}
			methodEnum->Release();
		}
		PrintXMLClose(file, 2, L"methods");
	}

	void Dump(TextWriter& file, IDiaSymbol* exeSymbol)
	{
		FindClasses(exeSymbol);
		for(int i=0;i<udtSymbols.Count();i++)
		{
			WString className=udtSymbols.Keys()[i];
			IDiaSymbol* classSymbol=udtSymbols.Values()[i];

			PrintXMLOpen(file, 1, L"class", className.Buffer());
			DumpBaseClasses(file, classSymbol);
			DumpFields(file, classSymbol);
			DumpMethods(file, classSymbol);
			PrintXMLClose(file, 1, L"class");
		}
	}

	void DumpPdbToXml(IDiaSymbol* exeSymbol, const wchar_t* xml)
	{
		FileStream fileStream(xml, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter file(encoderStream);
		PrintString(file, L"<?xml version=\"1.0\" encoding=\"utf-16\" ?>\r\n");
		PrintXMLOpen(file, 0, L"pdb", NULL);
		Dump(file, exeSymbol);
		PrintXMLClose(file, 0, L"pdb");
	}
}
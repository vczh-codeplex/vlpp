#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Collections\Operation.h"
#include "..\..\Library\Entity\TreeQuery.h"
#include "..\..\Library\Entity\TreeXml.h"
#include "..\..\Library\Stream\MemoryStream.h"
#include "..\..\Library\Stream\CharFormat.h"

using namespace vl;
using namespace vl::entities;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::unittest;

/***********************************************************************
TreeNode
***********************************************************************/

namespace TreeNodeInterfaceImplemenatationHelper
{
	Ptr<TreeElement> CreateBook(const WString& name)
	{
		Ptr<TreeElement> book=new TreeElement;
		book->name=L"book";
		book->attributes.Add(L"name", new TreeText(name));
		book->attributes.Add(L"id", new TreeText(L"id_"+name));
		return book;
	}

	void AddBook(Ptr<TreeElement> books, const WString& name)
	{
		books->children.Add(CreateBook(name));
		books->children.Add(new TreeText(L"text_"+name));
		books->children.Add(new TreeComment(L"comment_"+name));
	}

	Ptr<TreeElement> CreateBooks()
	{
		Ptr<TreeElement> books=new TreeElement;
		books->name=L"books";

		AddBook(books, L"A");
		AddBook(books, L"B");
		AddBook(books, L"C");
		AddBook(books, L"D");

		return books;
	}

	bool IsText(Ptr<ITreeQuerable> node, const WString& value)
	{
		return node && node->IsTextNode() && node->GetText()==value;
	}

	bool IsBookWithName(const WString& name, Ptr<ITreeQuerable> book)
	{
		return IsText(book->GetAttribute(L"name"), name);
	}
}
using namespace TreeNodeInterfaceImplemenatationHelper;

TEST_CASE(TestEntity_TreeNodeInterfaceImplemenatation)
{
	Ptr<ITreeQuerable> books=CreateBooks();
	TEST_ASSERT(books->GetName()==L"books");
	{
		List<Ptr<ITreeQuerable>> elements;
		CopyFrom(elements.Wrap(), books->GetElements());
		TEST_ASSERT(elements.Count()==4);
		for(vint i=0;i<4;i++)
		{
			List<Ptr<ITreeQuerable>> book;
			CopyFrom(book.Wrap(), books->GetElement(L"book")>>Where(Curry(IsBookWithName)(WString((wchar_t)(L'A'+i)))));
			TEST_ASSERT(book.Count()==1);
			TEST_ASSERT(elements[i]==book[0]);

			TEST_ASSERT(elements[i]->GetName()==L"book");
			TEST_ASSERT(elements[i]->IsAttributeExists(L"name")==true);
			TEST_ASSERT(IsText(elements[i]->GetAttribute(L"name"), WString((wchar_t)(L'A'+i))));
			TEST_ASSERT(elements[i]->IsAttributeExists(L"id")==true);
			TEST_ASSERT(IsText(elements[i]->GetAttribute(L"id"), L"id_"+WString((wchar_t)(L'A'+i))));
			TEST_ASSERT(elements[i]->IsAttributeExists(L"unknown")==false);
		}
	}
	{
		List<Ptr<ITreeQuerable>> elements;
		CopyFrom(elements.Wrap(), books->GetChildren());
		TEST_ASSERT(elements.Count()==8);
		for(vint i=0;i<4;i++)
		{
			TEST_ASSERT(elements[i*2]->IsTextNode()==false);
			TEST_ASSERT(IsText(elements[i*2]->GetAttribute(L"name"), WString((wchar_t)(L'A'+i))));
			
			TEST_ASSERT(elements[i*2+1]->IsTextNode()==true);
			TEST_ASSERT(elements[i*2+1]->GetText()==L"text_"+WString((wchar_t)(L'A'+i)));
		}
	}
}

/***********************************************************************
XmlWriter
***********************************************************************/

#define CREATE_XML_WRITER \
	MemoryStream memoryStream; \
	StreamWriter streamWriter(memoryStream); \
	XmlWriter xml(streamWriter)

#define ASSERT_XML_CONTENT(CONTENT) \
	memoryStream.SeekFromBegin(0); \
	StreamReader streamReader(memoryStream); \
	WString xmlText=streamReader.ReadToEnd()+L"\r\n"; \
	if(xmlText!=CONTENT) \
	{ \
		vl::unittest::UnitTest::PrintError(L"Wrong XML Text!"); \
		vl::unittest::UnitTest::PrintError(xmlText); \
	} \
	TEST_ASSERT(xmlText==CONTENT)

#define _(LINE) L#LINE L"\r\n"
#define _____(LINE) L"    " L#LINE L"\r\n"
#define _________(LINE) L"        " L#LINE L"\r\n"

TEST_CASE(TestEntity_XmlWriter)
{
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"root");
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<root/>)
			);
	}
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"root");
		xml.WriteAttribute(L"name", L"value");
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<root name = "value"/>)
			);
	}
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"root");
		xml.WriteAttribute(L"condition", L"1<2 && 3>4");
		xml.WriteAttribute(L"message", L"\"abc\";\'def\'");
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<root condition = "1&lt;2 &amp;&amp; 3&gt;4" message = "&quot;abc&quot;;&apos;def&apos;"/>)
			);
	}
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"books");
		xml.WriteElement(L"book", L"C++ Primer");
		xml.WriteElement(L"book", L"C# Primer");
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<books>)
			_____(<book>C++ Primer</book>)
			_____(<book>C# Primer</book>)
			_(</books>)
			);
	}
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"books");
		xml.WriteElement(L"book", L"C++ Primer");
		xml.OpenElement(L"book");
		xml.WriteAttribute(L"name", L"C# Primer");
		xml.WriteElement(L"author", L"I don't know");
		xml.CloseElement();
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<books>)
			_____(<book>C++ Primer</book>)
			_____(<book name = "C# Primer">)
			_________(<author>I don&apos;t know</author>)
			_____(</book>)
			_(</books>)
			);
	}
	{
		CREATE_XML_WRITER;
		xml.OpenElement(L"books");
		xml.WriteComment(L"C++ Primer Content");
		xml.OpenElement(L"book");
		xml.WriteAttribute(L"name", L"C++ Primer");
		xml.WriteText(L"This is the content of \"C++ Primer\".");
		xml.CloseElement();
		xml.WriteComment(L"C# Primer Content");
		xml.OpenElement(L"book");
		xml.WriteAttribute(L"name", L"C# Primer");
		xml.WriteCData(L"This is the content of \"C# Primer\".");
		xml.CloseElement();
		xml.CloseElement();

		ASSERT_XML_CONTENT(
			_(<books>)
			_____(<!--C++ Primer Content-->)
			_____(<book name = "C++ Primer">)
			_________(This is the content of &quot;C++ Primer&quot;.)
			_____(</book>)
			_____(<!--C# Primer Content-->)
			_____(<book name = "C# Primer"><![CDATA[This is the content of "C# Primer".]]>)
			_____(</book>)
			_(</books>)
			);
	}
}

/***********************************************************************
XmlReader
***********************************************************************/

namespace TestEntityHelper
{
	void AssertXmlReader_BeginOfFile(XmlReader& xml)
	{
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::BeginOfFile);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==L"");
	}

	void AssertXmlReader_ElementHeadOpening(XmlReader& xml, const WString& name)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::ElementHeadOpening);
		TEST_ASSERT(xml.CurrentName()==name);
		TEST_ASSERT(xml.CurrentValue()==L"");
	}
	
	void AssertXmlReader_ElementHeadClosing(XmlReader& xml)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::ElementHeadClosing);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==L"");
	}
	
	void AssertXmlReader_ElementClosing(XmlReader& xml)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::ElementClosing);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==L"");
	}
	
	void AssertXmlReader_Attribute(XmlReader& xml, const WString& name, const WString& value)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::Attribute);
		TEST_ASSERT(xml.CurrentName()==name);
		TEST_ASSERT(xml.CurrentValue()==value);
	}
	
	void AssertXmlReader_Text(XmlReader& xml, const WString& value)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::Text);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==value);
	}
	
	void AssertXmlReader_CData(XmlReader& xml, const WString& value)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::CData);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==value);
	}
	
	void AssertXmlReader_Comment(XmlReader& xml, const WString& value)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::Comment);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==value);
	}
	
	void AssertXmlReader_EndOfFile(XmlReader& xml)
	{
		xml.Next();
		TEST_ASSERT(xml.CurrentComponentType()==XmlReader::EndOfFile);
		TEST_ASSERT(xml.CurrentName()==L"");
		TEST_ASSERT(xml.CurrentValue()==L"");
	}
};
using namespace TestEntityHelper;

#define CREATE_XML_READER(CONTENT)\
	MemoryStream memoryStream;\
	StreamWriter streamWriter(memoryStream);\
	streamWriter.WriteString(CONTENT);\
	memoryStream.SeekFromBegin(0);\
	StreamReader streamReader(memoryStream);\
	XmlReader xml(streamReader);\
	AssertXmlReader_BeginOfFile(xml)

TEST_CASE(TestEntity_XmlReader)
{
	{
		CREATE_XML_READER(
			_(<root/>)
			);
		AssertXmlReader_ElementHeadOpening(xml, L"root");
		AssertXmlReader_ElementHeadClosing(xml);
		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
	{
		CREATE_XML_READER(
			_(<root name = "value"/>)
			);
		AssertXmlReader_ElementHeadOpening(xml, L"root");
		AssertXmlReader_Attribute(xml, L"name", L"value");
		AssertXmlReader_ElementHeadClosing(xml);
		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
	{
		CREATE_XML_READER(
			_(<root condition = "1&lt;2 &amp;&amp; 3&gt;4" message = "&quot;abc&quot;;&apos;def&apos;"/>)
			);
		AssertXmlReader_ElementHeadOpening(xml, L"root");
		AssertXmlReader_Attribute(xml, L"condition", L"1<2 && 3>4");
		AssertXmlReader_Attribute(xml, L"message", L"\"abc\";\'def\'");
		AssertXmlReader_ElementHeadClosing(xml);
		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
	{
		CREATE_XML_READER(
			_(<books>)
			_____(<book>C++ Primer</book>)
			_____(<book>C# Primer</book>)
			_(</books>)
			);
		AssertXmlReader_ElementHeadOpening(xml, L"books");
		AssertXmlReader_ElementHeadClosing(xml);

			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_ElementHeadClosing(xml);
			AssertXmlReader_Text(xml, L"C++ Primer");
			AssertXmlReader_ElementClosing(xml);

			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_ElementHeadClosing(xml);
			AssertXmlReader_Text(xml, L"C# Primer");
			AssertXmlReader_ElementClosing(xml);

		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
	{
		CREATE_XML_READER(
			_(<books>)
			_____(<book>C++ Primer</book>)
			_____(<book name = "C# Primer">)
			_________(<author>I don&apos;t know</author>)
			_____(</book>)
			_(</books>)
			);
		AssertXmlReader_ElementHeadOpening(xml, L"books");
		AssertXmlReader_ElementHeadClosing(xml);

			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_ElementHeadClosing(xml);
			AssertXmlReader_Text(xml, L"C++ Primer");
			AssertXmlReader_ElementClosing(xml);

			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_Attribute(xml, L"name", L"C# Primer");
			AssertXmlReader_ElementHeadClosing(xml);
				AssertXmlReader_ElementHeadOpening(xml, L"author");
				AssertXmlReader_ElementHeadClosing(xml);
				AssertXmlReader_Text(xml, L"I don't know");
				AssertXmlReader_ElementClosing(xml);
			AssertXmlReader_ElementClosing(xml);

		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
	{
		CREATE_XML_READER(
			_(<books>)
			_____(<!--C++ Primer Content-->)
			_____(<book name = "C++ Primer">)
			_________(This is the content of &quot;C++ Primer&quot;.)
			_____(</book>)
			_____(<!--C# Primer Content-->)
			_____(<book name = "C# Primer"><![CDATA[This is the content of "C# Primer".]]>)
			_____(</book>)
			_(</books>)
			);

		AssertXmlReader_ElementHeadOpening(xml, L"books");
		AssertXmlReader_ElementHeadClosing(xml);

			AssertXmlReader_Comment(xml, L"C++ Primer Content");
			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_Attribute(xml, L"name", L"C++ Primer");
			AssertXmlReader_ElementHeadClosing(xml);
			AssertXmlReader_Text(xml, L"This is the content of \"C++ Primer\".");
			AssertXmlReader_ElementClosing(xml);

			AssertXmlReader_Comment(xml, L"C# Primer Content");
			AssertXmlReader_ElementHeadOpening(xml, L"book");
			AssertXmlReader_Attribute(xml, L"name", L"C# Primer");
			AssertXmlReader_ElementHeadClosing(xml);
			AssertXmlReader_CData(xml, L"This is the content of \"C# Primer\".");
			AssertXmlReader_ElementClosing(xml);

		AssertXmlReader_ElementClosing(xml);
		AssertXmlReader_EndOfFile(xml);
	}
}
#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\TreeQuery.h"
#include "..\..\Library\Collections\Operation.h"

using namespace vl;
using namespace vl::entities;
using namespace vl::collections;
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
		book->attributes.Add(L"name", name);
		book->attributes.Add(L"id", L"id_"+name);
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

	bool IsBookWithName(const WString& name, Ptr<ITreeQuerable> book)
	{
		return book->IsAttributeExists(L"name") && book->GetAttribute(L"name")==name;
	}
}
using namespace TreeNodeInterfaceImplemenatationHelper;

TEST_CASE(TreeNodeInterfaceImplemenatation)
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
			TEST_ASSERT(elements[i]->GetAttribute(L"name")==WString((wchar_t)(L'A'+i)));
			TEST_ASSERT(elements[i]->IsAttributeExists(L"id")==true);
			TEST_ASSERT(elements[i]->GetAttribute(L"id")==L"id_"+WString((wchar_t)(L'A'+i)));
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
			TEST_ASSERT(elements[i*2]->GetAttribute(L"name")==WString((wchar_t)(L'A'+i)));
			
			TEST_ASSERT(elements[i*2+1]->IsTextNode()==true);
			TEST_ASSERT(elements[i*2+1]->GetText()==L"text_"+WString((wchar_t)(L'A'+i)));
		}
	}
}
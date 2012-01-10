#include "..\..\GUI\GraphicsElement\GuiGraphicsTextElement.h"
#include "..\..\GUI\Controls\ExtendedControls\GuiTreeViewControls.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::elements::text;
using namespace vl::presentation::controls;
using namespace vl::presentation::controls::tree;

/***********************************************************************
Framework
***********************************************************************/

#define TEST_CASE(NAME)\
	void TestExecution_##NAME(void);\
	class TestCase_##NAME\
	{\
	public:\
		TestCase_##NAME()\
		{\
			TestExecution_##NAME();\
		}\
	} instance_##NAME;\
	void TestExecution_##NAME(void)

#define TEST_ASSERT(CONDITION) do{if(!(CONDITION)) throw 0;}while(false)

/***********************************************************************
Text
***********************************************************************/

void AssertEmpty(TextLine line)
{
	TEST_ASSERT(line.text==0);
	TEST_ASSERT(line.att==0);
	TEST_ASSERT(line.bufferLength==0);
	TEST_ASSERT(line.dataLength==0);
	TEST_ASSERT(line.availableOffsetCount==0);
}

void AssertText(TextLine line, const WString& text)
{
	int length=text.Length();
	int buffer=0;
	if(length%TextLine::BlockSize)
	{
		buffer=(length/TextLine::BlockSize+1)*TextLine::BlockSize;
	}
	if(buffer==0)
	{
		buffer=TextLine::BlockSize;
	}

	TEST_ASSERT(wcsncmp(line.text,text.Buffer(), length)==0);
	TEST_ASSERT(line.att!=0);
	TEST_ASSERT(line.bufferLength==buffer);
	TEST_ASSERT(line.dataLength==length);
	TEST_ASSERT(line.availableOffsetCount==0);
}

TEST_CASE(SingleTextLine)
{
	TextLine line;
	AssertEmpty(line);

	line.Initialize();
	AssertText(line, L"");

	TEST_ASSERT(line.Modify(-1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, 0, 0, 0)==true);
	TEST_ASSERT(line.Modify(0, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 1, 0, 0)==false);
	AssertText(line, L"");

	TEST_ASSERT(line.Modify(0, 0, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26)==true);
	AssertText(line, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 0, L"abcdefghijklmnopqrstuvwxyz", 26));
	AssertText(line, L"ABCDEFGHIJKLMabcdefghijklmnopqrstuvwxyzNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 26, 0, 0));
	AssertText(line, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(0, 7, 0, 0));
	AssertText(line, L"HIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 6, 0, 0));
	AssertText(line, L"HIJKLMNOPQRST");

	TEST_ASSERT(line.Modify(0, 13, 0, 0));
	AssertText(line, L"");

	line.Finalize();
	AssertEmpty(line);
}

TEST_CASE(MultipleTextLine)
{
	{
		TextLine line1, line2;
		line1.Initialize();
		line2.Initialize();
		line1.Modify(0, 0, L"ABCDEFG", 7);
		line2.Modify(0, 0, L"HIJKLMN", 7);
		line1.AppendAndFinalize(line2);
		AssertText(line1, L"ABCDEFGHIJKLMN");
		AssertEmpty(line2);
		line1.Finalize();
	}
	{
		TextLine line1;
		line1.Initialize();
		line1.Modify(0, 0, L"ABCDEFGHIJKLMN", 14);
		TextLine line2=line1.Split(7);
		AssertText(line1, L"ABCDEFG");
		AssertText(line2, L"HIJKLMN");
		line1.Finalize();
		line2.Finalize();
	}
}

TEST_CASE(TextLines)
{
	TextLines lines;
	{
		TEST_ASSERT(lines.GetCount()==1);
		AssertText(lines.GetLine(0), L"");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 0), L"")==TextPos(0, 0));
	{
		TEST_ASSERT(lines.GetCount()==1);
		AssertText(lines.GetLine(0), L"");
		TEST_ASSERT(lines.GetText()==L"");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 0), L"ABCDEFG")==TextPos(0, 7));
	{
		TEST_ASSERT(lines.GetCount()==1);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 7), TextPos(0, 7), L"\r\nHIJKLMN")==TextPos(1, 7));
	{
		TEST_ASSERT(lines.GetCount()==2);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJKLMN");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 7), TextPos(1, 7), L"\r\n")==TextPos(2, 0));
	{
		TEST_ASSERT(lines.GetCount()==3);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJKLMN");
		AssertText(lines.GetLine(2), L"");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN\r\n");
	}

	TEST_ASSERT(lines.Modify(TextPos(2, 0), TextPos(2, 0), L"OPQRST\r\nUVWXYZ")==TextPos(3, 6));
	{
		TEST_ASSERT(lines.GetCount()==4);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJKLMN");
		AssertText(lines.GetLine(2), L"OPQRST");
		AssertText(lines.GetLine(3), L"UVWXYZ");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 3), TextPos(2, 3), L"vczh")==TextPos(1, 7));
	{
		TEST_ASSERT(lines.GetCount()==3);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJvczhRST");
		AssertText(lines.GetLine(2), L"UVWXYZ");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJvczhRST\r\nUVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 3), TextPos(1, 7), L"KLMN\r\nOPQ")==TextPos(2, 3));
	{
		TEST_ASSERT(lines.GetCount()==4);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJKLMN");
		AssertText(lines.GetLine(2), L"OPQRST");
		AssertText(lines.GetLine(3), L"UVWXYZ");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 3), TextPos(3, 3), L"vczh")==TextPos(0, 7));
	{
		TEST_ASSERT(lines.GetCount()==1);
		AssertText(lines.GetLine(0), L"ABCvczhXYZ");
		TEST_ASSERT(lines.GetText()==L"ABCvczhXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 10), L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ")==TextPos(3, 6));
	{
		TEST_ASSERT(lines.GetCount()==4);
		AssertText(lines.GetLine(0), L"ABCDEFG");
		AssertText(lines.GetLine(1), L"HIJKLMN");
		AssertText(lines.GetLine(2), L"OPQRST");
		AssertText(lines.GetLine(3), L"UVWXYZ");
		TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(3, 6), L"12345\r\n67890")==TextPos(1, 5));
	{
		TEST_ASSERT(lines.GetCount()==2);
		AssertText(lines.GetLine(0), L"12345");
		AssertText(lines.GetLine(1), L"67890");
		TEST_ASSERT(lines.GetText()==L"12345\r\n67890");
	}

	lines.Clear();
	{
		TEST_ASSERT(lines.GetCount()==1);
		AssertText(lines.GetLine(0), L"");
		TEST_ASSERT(lines.GetText()==L"");
	}
}

namespace TextMeasurementHelper
{
	const int CharSize=5;

	class TestCharMeasurer : public Object, public CharMeasurer
	{
	protected:
		int MeasureWidthInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
		{
			return CharSize;
		}

		int GetRowHeightInternal(IGuiGraphicsRenderTarget* renderTarget)
		{
			return CharSize;
		}
	public:
		TestCharMeasurer()
			:CharMeasurer(CharSize)
		{
		}
	};
}
using namespace TextMeasurementHelper;

TEST_CASE(TextMeasurement)
{
	TextLines lines;
	TestCharMeasurer charMeasurer;
	lines.SetCharMeasurer(&charMeasurer);
	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 0), L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ")==TextPos(3, 6));
	TEST_ASSERT(lines.GetText()==L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

	TEST_ASSERT(lines.GetRowHeight()==CharSize);
	for(int i=-1;i<5;i++)
	{
		if(i==-1 || i==4)
		{
			TEST_ASSERT(lines.GetRowWidth(i)==-1);
		}
		else
		{
			TEST_ASSERT(lines.GetRowWidth(i)==lines.GetLine(i).dataLength*CharSize);
		}
	}
	TEST_ASSERT(lines.GetMaxHeight()==lines.GetCount()*CharSize);
	TEST_ASSERT(lines.GetMaxWidth()==lines.GetLine(0).dataLength*CharSize);

	for(int y=-1;y<5;y++)
	{
		for(int x=-1;x<9;x++)
		{
			if(y==-1 || y==4)
			{
				TEST_ASSERT(lines.GetRectFromTextPos(TextPos(y, x))==Rect(-1, -1, -1, -1));
			}
			else
			{
				int c=lines.GetLine(y).dataLength;
				if(x==-1 || x>c)
				{
					TEST_ASSERT(lines.GetRectFromTextPos(TextPos(y, x))==Rect(-1, -1, -1, -1));
				}
				else if(x==c)
				{
					TEST_ASSERT(lines.GetRectFromTextPos(TextPos(y, x))==Rect(Point(x*CharSize, y*CharSize), Size(CharSize/2, CharSize)));
				}
				else
				{
					TEST_ASSERT(lines.GetRectFromTextPos(TextPos(y, x))==Rect(Point(x*CharSize, y*CharSize), Size(CharSize, CharSize)));
				}
			}

			for(int dy=0;dy<CharSize;dy++)
			{
				for(int dx=0;dx<CharSize;dx++)
				{
					Point point(x*CharSize+dx, y*CharSize+dy);
					TextPos pos=lines.GetTextPosFromPoint(point);

					int ay=y;
					if(y==-1)
					{
						ay=0;
					}
					else if(y==4)
					{
						ay=3;
					}

					int c=lines.GetLine(ay).dataLength;
					if(x==-1)
					{
						TEST_ASSERT(pos==TextPos(ay, 0));
					}
					else if(x>=c)
					{
						TEST_ASSERT(pos==TextPos(ay, c));
					}
					else
					{
						TEST_ASSERT(pos==TextPos(ay, x));
					}
				}
			}
		}
	}
}

/***********************************************************************
TreeView
***********************************************************************/

namespace MemoryNodeProviderHelper
{
	class TestCompareData
	{
	public:
		int							data;
		List<Ptr<TestCompareData>>	child;
	};

	Ptr<TestCompareData>& operator,(Ptr<TestCompareData>& a, const Ptr<TestCompareData>& b)
	{
		a->child.Add(b);
		return a;
	}

	Ptr<TestCompareData> Node(int data)
	{
		Ptr<TestCompareData> node=new TestCompareData;
		node->data=data;
		return node;
	}

	Ptr<TestCompareData> Root()
	{
		return Node(-1);
	}

	int AssertTree(MemoryNodeProvider* node, Ptr<TestCompareData> data)
	{
		int total=1;
		TEST_ASSERT(node->GetChildCount()==data->child.Count());
		if(data->data==-1)
		{
			TEST_ASSERT(dynamic_cast<MemoryNodeRootProvider*>(node)!=0);
		}
		else
		{
			TEST_ASSERT(node->GetData().Cast<ObjectBox<int>>()->Unbox()==data->data);
		}
		for(int i=0;i<node->GetChildCount();i++)
		{
			INodeProvider* provider=node->RequestChild(i);
			MemoryNodeProvider* child=dynamic_cast<MemoryNodeProvider*>(provider);
			TEST_ASSERT(provider!=0);
			TEST_ASSERT(child!=0);
			TEST_ASSERT(node==child->GetParent());
			int childTotal=AssertTree(child, data->child[i]);
			if(node->GetExpanding())
			{
				total+=childTotal;
			}
		}
		TEST_ASSERT(total==node->CalculateTotalVisibleNodes());
		return total;
	}

	void AssertItemsInternal(NodeItemProvider* items, INodeProvider* node, int& index)
	{
		if(index==-1)
		{
			TEST_ASSERT(items->GetRoot()->GetRootNode()==node);
		}
		else
		{
			INodeItemView* view=dynamic_cast<INodeItemView*>(items->RequestView(INodeItemView::Identifier));
			INodeProvider* indexNode=view->RequestNode(index);
			TEST_ASSERT(indexNode==node);
			view->ReleaseNode(indexNode);
			items->ReleaseView(view);
		}
		index++;

		if(node->GetExpanding())
		{
			for(int i=0;i<node->GetChildCount();i++)
			{
				INodeProvider* child=node->RequestChild(i);
				AssertItemsInternal(items, child, index);
				node->ReleaseChild(child);
			}
		}
	}

	void AssertItems(NodeItemProvider* items)
	{
		int index=-1;
		AssertItemsInternal(items, items->GetRoot()->GetRootNode(), index);
	}

	int callbackStart=0;
	int callbackCount=0;
	int callbackNewCount=0;
	bool callbackAvailable=false;

	void AssertCallback(int start, int count, int newCount)
	{
		TEST_ASSERT(callbackAvailable==true);
		TEST_ASSERT(callbackStart==start);
		TEST_ASSERT(callbackCount==count);
		TEST_ASSERT(callbackNewCount==newCount);
		callbackAvailable=false;
	}

	class ItemProviderCallback : public Object, public virtual GuiListControl::IItemProviderCallback
	{
	public:
		void OnAttached(GuiListControl::IItemProvider* provider)
		{
		}

		void OnItemModified(int start, int count, int newCount)
		{
			TEST_ASSERT(callbackAvailable==false);
			callbackStart=start;
			callbackCount=count;
			callbackNewCount=newCount;
			callbackAvailable=true;
		}
	};
}
using namespace MemoryNodeProviderHelper;

TEST_CASE(MemoryNodeProvider)
{
	MemoryNodeRootProvider* root=new MemoryNodeRootProvider;
	NodeItemProvider items(root);
	ItemProviderCallback callback;
	items.AttachCallback(&callback);

	AssertTree(root, Root());
	AssertItems(&items);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==0+1);

	for(int i=0;i<5;i++)
	{
		Ptr<MemoryNodeProvider> node=new MemoryNodeProvider(new ObjectBox<int>(i*10));
		root->Children().Add(node);
		AssertCallback(i, 0, 1);
		for(int j=1;j<=i;j++)
		{
			node->Children().Add(new MemoryNodeProvider(new ObjectBox<int>(i*10+j)));
		}
	}

	Ptr<TestCompareData> testRoot=
		(Root()
		,	Node(0)
		,	(Node(10)
			, Node(11)
			)
		,	(Node(20)
			, Node(21)
			, Node(22)
			)
		,	(Node(30)
			, Node(31)
			, Node(32)
			, Node(33)
			)
		,	(Node(40)
			, Node(41)
			, Node(42)
			, Node(43)
			, Node(44)
			)
		);
	AssertTree(root, testRoot);
	AssertItems(&items);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==5+1);

	root->Children()[0]->SetData(new ObjectBox<int>(100));
	testRoot->child[0]->data=100;
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(0, 1, 1);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==5+1);

	root->Children()[3]->SetExpanding(true);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(4, 0, 3);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==8+1);

	root->Children()[4]->SetExpanding(true);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(8, 0, 4);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==12+1);

	root->Children()[3]->SetExpanding(false);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(4, 3, 0);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==9+1);

	root->Children()[4]->SetExpanding(false);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(5, 4, 0);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==5+1);

	root->Children()[3]->SetExpanding(true);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(4, 0, 3);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==8+1);

	Ptr<MemoryNodeProvider> p3=root->Children()[3];
	Ptr<TestCompareData> t3=testRoot->child[3];
	root->Children().RemoveAt(3);
	testRoot->child.RemoveAt(3);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(3, 4, 0);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==4+1);

	root->Children().RemoveAt(3);
	testRoot->child.RemoveAt(3);
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(3, 1, 0);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==3+1);

	root->Children()[2]->Children().RemoveRange(0, 2);
	testRoot->child[2]->child.RemoveRange(0, 2);
	AssertTree(root, testRoot);
	AssertItems(&items);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==3+1);

	root->Children().Set(0, p3);
	testRoot->child[0]=t3;
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(0, 1, 4);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==6+1);

	root->Children().Clear();
	testRoot->child.Clear();
	AssertTree(root, testRoot);
	AssertItems(&items);
	AssertCallback(0, 6, 0);
	TEST_ASSERT(root->CalculateTotalVisibleNodes()==0+1);
}

/***********************************************************************
Reflection
***********************************************************************/

namespace ReflectionVirtualInheritanceHelper
{
	class A{};
	class B:public A{};
	class C:public A{};
	class D:public B, public C{};
	class E:virtual public A{};
	class F:virtual public A{};
	class G:public E, public F{};
}
using namespace ReflectionVirtualInheritanceHelper;

TEST_CASE(ReflectionVirtualInheritance)
{
	D d;
	A& da1=static_cast<B&>(d);
	A& da2=static_cast<C&>(d);
	TEST_ASSERT(&da1!=&da2);
	
	G g;
	A& ga1=static_cast<E&>(g);
	A& ga2=static_cast<F&>(g);
	TEST_ASSERT(&ga1==&ga2);
}
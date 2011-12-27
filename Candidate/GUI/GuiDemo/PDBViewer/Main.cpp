#include "..\..\GUI\GacUI.h"
#include "dia2.h"

#pragma comment(lib, "diaguids.lib")

extern tree::INodeRootProvider* CreateProviderFromDiaSymbol(IDiaSymbol* symbol);

tree::INodeRootProvider* CreateProvider()
{
    IDiaDataSource* pSource=0;
    IDiaSession* pSession=0;
    IDiaSymbol* pSymbol=0;
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(
        CLSID_DiaSource,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDiaDataSource,
        (void**) &pSource
        );
    if(SUCCEEDED(hr))
	if(SUCCEEDED(pSource->loadDataFromPdb(L"..\\Debug\\GuiDemo.pdb")))
    if(SUCCEEDED(pSource->openSession(&pSession)))
	if(SUCCEEDED(pSession->get_globalScope(&pSymbol)))
	{
		return CreateProviderFromDiaSymbol(pSymbol);
	}
	return 0;
}

void GuiMain()
{
	GuiWindow window(new win7::Win7WindowStyle);
#ifdef GUI_GRAPHICS_RENDERER_GDI
	window.SetText(L"PDB Viewer (GDI)");
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	window.SetText(L"PDB Viewer (Direct2D)");
#endif
	{
		window.GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider, CreateProvider());
		treeControl->GetBoundsComposition()->SetPreferredMinSize(Size(400, 600));
		treeControl->GetBoundsComposition()->SetAlignmentToParent(Margin(3, 3, 3, 3));
		window.GetBoundsComposition()->AddChild(treeControl->GetBoundsComposition());
	}

	window.SetClientSize(Size(800, 600));
	INativeScreen* screen=window.GetRelatedScreen();
	Rect windowBounds=window.GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window.SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	GetApplication()->Run(&window);
}

//---------------------------------------------------------------------------------

class DiaSymbolProvider : public Object, public virtual tree::INodeProvider
{
protected:
	INodeProvider*								parent;
	bool										expanding;
	int											totalVisibleNodes;
	IDiaSymbol*									diaSymbol;
	tree::INodeProviderCallback*				callbackProxy;
	collections::List<Ptr<DiaSymbolProvider>>	children;

	void EnsureChildren()
	{
	}

	void OnChildTotalVisibleNodesChanged(int offset)
	{
		totalVisibleNodes+=offset;
		if(DiaSymbolProvider* provider=dynamic_cast<DiaSymbolProvider*>(parent))
		{
			provider->OnChildTotalVisibleNodesChanged(offset);
		}
	}
public:
	DiaSymbolProvider(INodeProvider* _parent, IDiaSymbol* _symbol, tree::INodeProviderCallback* _callbackProxy)
		:parent(_parent)
		,totalVisibleNodes(1)
		,diaSymbol(_symbol)
		,callbackProxy(_callbackProxy)
	{
	}

	~DiaSymbolProvider()
	{
	}

	bool GetExpanding()override
	{
		return expanding;
	}

	void SetExpanding(bool value)override
	{
		if(expanding!=value)
		{
			expanding=value;
			int offset=0;
			EnsureChildren();
			for(int i=0;i<children.Count();i++)
			{
				offset+=children[i]->totalVisibleNodes;
			}

			OnChildTotalVisibleNodesChanged(expanding?offset:-offset);
			if(expanding)
			{
				callbackProxy->OnItemExpanded(this);
			}
			else
			{
				callbackProxy->OnItemCollapsed(this);
			}
		}
	}

	int CalculateTotalVisibleNodes()override
	{
		return totalVisibleNodes;
	}

	int GetChildCount()override
	{
		EnsureChildren();
		return children.Count();
	}

	INodeProvider* GetParent()override
	{
		return parent;
	}

	INodeProvider* RequestChild(int index)override
	{
		EnsureChildren();
		if(0<=index && index<children.Count())
		{
			return children[index].Obj();
		}
		else
		{
			return 0;
		}
	}

	void ReleaseChild(INodeProvider* node)override
	{
	}
};

class DiaSymbolRootProvider: public Object, public tree::NodeRootProviderBase, public tree::INodeProvider
{
protected:
	Ptr<DiaSymbolProvider>			exeSymbol;
public:
	DiaSymbolRootProvider(IDiaSymbol* _symbol)
	{
		exeSymbol=new DiaSymbolProvider(this, _symbol, this);
	}

	~DiaSymbolRootProvider()
	{
	}

	tree::INodeProvider* GetRootNode()
	{
		return exeSymbol.Obj();
	}

	bool GetExpanding()override
	{
		return true;
	}

	void SetExpanding(bool value)override
	{
	}

	int CalculateTotalVisibleNodes()override
	{
		return 1+exeSymbol->CalculateTotalVisibleNodes();
	}

	int GetChildCount()override
	{
		return 1;
	}

	INodeProvider* GetParent()override
	{
		return 0;
	}

	INodeProvider* RequestChild(int index)override
	{
		return index==0?exeSymbol.Obj():0;
	}

	void ReleaseChild(INodeProvider* node)override
	{
	}

	Interface* RequestView(const WString& identifier)override
	{
		return tree::NodeRootProviderBase::RequestView(identifier);
	}

	void ReleaseView(Interface* view)override
	{
		tree::NodeRootProviderBase::ReleaseView(view);
	}
};

tree::INodeRootProvider* CreateProviderFromDiaSymbol(IDiaSymbol* symbol)
{
	return new DiaSymbolRootProvider(symbol);
}
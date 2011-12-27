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
		treeControl->GetBoundsComposition()->SetPreferredMinSize(Size(100, 100));
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

#define PROCESS(TAG) case TAG: return L#TAG;
 
const wchar_t* GetTagName(enum SymTagEnum symTag)
{
    switch(symTag)
    {
	PROCESS(SymTagNull)
	PROCESS(SymTagExe)
	PROCESS(SymTagCompiland)
	PROCESS(SymTagCompilandDetails)
	PROCESS(SymTagCompilandEnv)
	PROCESS(SymTagFunction)
	PROCESS(SymTagBlock)
	PROCESS(SymTagData)
	PROCESS(SymTagAnnotation)
	PROCESS(SymTagLabel)
	PROCESS(SymTagPublicSymbol)
	PROCESS(SymTagUDT)
	PROCESS(SymTagEnum)
	PROCESS(SymTagFunctionType)
	PROCESS(SymTagPointerType)
	PROCESS(SymTagArrayType)
	PROCESS(SymTagBaseType)
	PROCESS(SymTagTypedef)
	PROCESS(SymTagBaseClass)
	PROCESS(SymTagFriend)
	PROCESS(SymTagFunctionArgType)
	PROCESS(SymTagFuncDebugStart)
	PROCESS(SymTagFuncDebugEnd)
	PROCESS(SymTagUsingNamespace)
	PROCESS(SymTagVTableShape)
	PROCESS(SymTagVTable)
	PROCESS(SymTagCustom)
	PROCESS(SymTagThunk)
	PROCESS(SymTagCustomType)
	PROCESS(SymTagManagedType)
	PROCESS(SymTagDimension)
	PROCESS(SymTagCallSite)
    PROCESS(SymTagMax)
    }
    return L"";
}

//---------------------------------------------------------------------------------

class DiaSymbolProvider : public Object, public virtual tree::INodeProvider
{
protected:
	INodeProvider*								parent;
	bool										expanding;
	int											totalVisibleNodes;
	IDiaSymbol*									diaSymbolLazyLoaded;
	int											diaSymbolIndex;
	tree::INodeProviderCallback*				callbackProxy;
	collections::Array<Ptr<DiaSymbolProvider>>	children;
	bool										childrenFilled;

	void EnsureChildren()
	{
		if(!childrenFilled)
		{
			childrenFilled=true;
			IDiaEnumSymbols* pEnum=0;
			GetDiaSymbol()->findChildren(SymTagNull, NULL, nsNone, &pEnum);
			if(pEnum)
			{
				LONG count=0;
				pEnum->get_Count(&count);
				pEnum->Release();
				children.Resize((int)count);
			}
		}
	}

	void OnChildTotalVisibleNodesChanged(int offset)
	{
		totalVisibleNodes+=offset;
		if(DiaSymbolProvider* provider=dynamic_cast<DiaSymbolProvider*>(parent))
		{
			provider->OnChildTotalVisibleNodesChanged(offset);
		}
	}

	IDiaSymbol* GetDiaSymbol()
	{
		if(!diaSymbolLazyLoaded)
		{
			IDiaEnumSymbols* pEnum=0;
			IDiaSymbol* parentSymbol=dynamic_cast<DiaSymbolProvider*>(parent)->GetDiaSymbol();
			parentSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnum);
			if(pEnum)
			{
				pEnum->Item(diaSymbolIndex, &diaSymbolLazyLoaded);
				pEnum->Release();
			}
		}
		return diaSymbolLazyLoaded;
	}
public:
	DiaSymbolProvider(INodeProvider* _parent, int _diaSymbolIndex, tree::INodeProviderCallback* _callbackProxy)
		:parent(_parent)
		,expanding(false)
		,totalVisibleNodes(1)
		,diaSymbolLazyLoaded(0)
		,diaSymbolIndex(_diaSymbolIndex)
		,callbackProxy(_callbackProxy)
		,childrenFilled(false)
	{
	}

	DiaSymbolProvider(INodeProvider* _parent, IDiaSymbol* _diaSymbol, tree::INodeProviderCallback* _callbackProxy)
		:parent(_parent)
		,expanding(false)
		,totalVisibleNodes(1)
		,diaSymbolLazyLoaded(_diaSymbol)
		,diaSymbolIndex(-1)
		,callbackProxy(_callbackProxy)
		,childrenFilled(false)
	{
	}

	~DiaSymbolProvider()
	{
		if(diaSymbolLazyLoaded) diaSymbolLazyLoaded->Release();
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
				offset+=children[i]?children[i]->totalVisibleNodes:1;
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
			if(!children[index])
			{
				children[index]=new DiaSymbolProvider(this, index, callbackProxy);
			}
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

	WString GetName()
	{
		BSTR name=0;
		GetDiaSymbol()->get_name(&name);
		WString symbolName=name?(wchar_t*)name:L"";

		enum SymTagEnum tag=SymTagNull;
		GetDiaSymbol()->get_symTag((DWORD*)&tag);
		WString tagName=GetTagName(tag);

		return tagName+L": "+symbolName;
	}
};

class DiaSymbolRootProvider
	: public Object
	, public tree::NodeRootProviderBase
	, public tree::INodeProvider
	, protected virtual tree::ITreeViewItemView
{
protected:
	Ptr<DiaSymbolProvider>			exeSymbol;

	Ptr<GuiImageData> GetNodeImage(INodeProvider* node)
	{
		return 0;
	}

	WString GetNodeText(INodeProvider* node)
	{
		DiaSymbolProvider* provider=dynamic_cast<DiaSymbolProvider*>(node);
		return provider?provider->GetName():L"";
	}
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
		return this;
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
		if(identifier==tree::ITreeViewItemView::Identifier)
		{
			return (tree::ITreeViewItemView*)this;
		}
		else
		{
			return NodeRootProviderBase::RequestView(identifier);
		}
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
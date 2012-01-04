#include "..\..\GUI\GacUI.h"
#include "dia2.h"

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
 
const wchar_t* GetBasicTypeName(enum BasicType type)
{
    switch(type)
    {
	PROCESS(btNoType)
    PROCESS(btVoid)
    PROCESS(btChar)
    PROCESS(btWChar)
    PROCESS(btInt)
    PROCESS(btUInt)
    PROCESS(btFloat)
    PROCESS(btBCD)
    PROCESS(btBool)
    PROCESS(btLong)
    PROCESS(btULong)
    PROCESS(btCurrency)
    PROCESS(btDate)
    PROCESS(btVariant)
    PROCESS(btComplex)
    PROCESS(btBit)
    PROCESS(btBSTR)
    PROCESS(btHresult)
	}
	return L"";
}
 
const wchar_t* GetDataKindName(enum DataKind kind)
{
    switch(kind)
    {
	PROCESS(DataIsUnknown)
	PROCESS(DataIsLocal)
	PROCESS(DataIsStaticLocal)
	PROCESS(DataIsParam)
	PROCESS(DataIsObjectPtr)
	PROCESS(DataIsFileStatic)
	PROCESS(DataIsGlobal)
	PROCESS(DataIsMember)
	PROCESS(DataIsStaticMember)
	PROCESS(DataIsConstant)
	}
	return L"";
}

const wchar_t* GetAccessName(enum CV_access_e access)
{
    switch(access)
    {
	PROCESS(CV_private)
	PROCESS(CV_protected)
	PROCESS(CV_public)
	}
	return L"";
}

const wchar_t* GetCallingConversionName(enum CV_call_e callconv)
{
    switch(callconv)
    {
	PROCESS(CV_CALL_NEAR_C)
	PROCESS(CV_CALL_NEAR_FAST)
	PROCESS(CV_CALL_NEAR_STD)
	PROCESS(CV_CALL_NEAR_SYS)
	PROCESS(CV_CALL_THISCALL)
	PROCESS(CV_CALL_CLRCALL)
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
	collections::SortedList<int>				expandedChildrenIndices;
	WString										namePrefix;

	void EnsureChildren()
	{
		if(!childrenFilled)
		{
			childrenFilled=true;
			IDiaEnumSymbols* pEnum=0;
			GetDiaSymbol()->findChildren(SymTagNull, NULL, nsNone, &pEnum);
			if(pEnum)
			{
				enum SymTagEnum tag=SymTagNull;
				GetDiaSymbol()->get_symTag((DWORD*)&tag);
				if(tag==SymTagExe)
				{
					collections::List<Ptr<DiaSymbolProvider>> compilands;
					DWORD celt=0;
					while(true)
					{
						IDiaSymbol* child=0;
						if(pEnum->Next(1, &child, &celt)!=S_OK) break;
						if(celt==0 || !child) break;

						child->get_symTag((DWORD*)&tag);
						if(tag==SymTagCompiland)
						{
							DiaSymbolProvider* provider=new DiaSymbolProvider(this, compilands.Count(), callbackProxy);
							provider->diaSymbolLazyLoaded=child;
							compilands.Add(provider);
						}
						else
						{
							child->Release();
						}
					}
					collections::CopyFrom(children.Wrap(), compilands.Wrap());
				}
				else
				{
					LONG count=0;
					pEnum->get_Count(&count);

					IDiaSymbol* typeSymbol=0;
					GetDiaSymbol()->get_type(&typeSymbol);
					if(typeSymbol)
					{
						children.Resize((int)count+1);
						DiaSymbolProvider* typeProvider=new DiaSymbolProvider(this, (int)count, callbackProxy);
						typeProvider->namePrefix=L"<SYMBOL-TYPE>";
						typeProvider->diaSymbolLazyLoaded=typeSymbol;
						children[count]=typeProvider;
					}
					else
					{
						children.Resize((int)count);
					}
				}
				pEnum->Release();
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

	DiaSymbolProvider* RequestChildInternal(int index)
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

			DiaSymbolProvider* provider=dynamic_cast<DiaSymbolProvider*>(parent);
			if(provider)
			{
				if(expanding)
				{
					provider->expandedChildrenIndices.Add(diaSymbolIndex);
				}
				else
				{
					provider->expandedChildrenIndices.Remove(diaSymbolIndex);
				}
			}

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
		return RequestChildInternal(index);
	}

	void ReleaseChild(INodeProvider* node)override
	{
	}

	WString GetName()
	{
		WString symbolName, tagName;

		enum SymTagEnum tag=SymTagNull;
		GetDiaSymbol()->get_symTag((DWORD*)&tag);
		tagName=GetTagName(tag);
		if(tag==SymTagBaseType)
		{
			enum BasicType type;
			GetDiaSymbol()->get_baseType((DWORD*)&type);
			symbolName=GetBasicTypeName(type);

			ULONGLONG length=0;
			GetDiaSymbol()->get_length(&length);
			symbolName+=L" ("+vl::u64tow(length)+L" bytes)";
		}
		else
		{
			BSTR name=0;
			GetDiaSymbol()->get_name(&name);
			symbolName=name?(wchar_t*)name:L"";
			if(tag==SymTagData)
			{
				enum DataKind kind;
				GetDiaSymbol()->get_dataKind((DWORD*)&kind);
				symbolName+=L" ("+WString(GetDataKindName(kind))+L")";
			}
		}
		{
			BOOL boolValue=FALSE;
			if(SUCCEEDED(GetDiaSymbol()->get_constType(&boolValue)))
			{
				if(boolValue)
				{
					symbolName=L"<CONST> "+symbolName;
				}
			}
			if(SUCCEEDED(GetDiaSymbol()->get_reference(&boolValue)))
			{
				if(boolValue)
				{
					symbolName=L"<L-REF> "+symbolName;
				}
			}
			if(SUCCEEDED(GetDiaSymbol()->get_RValueReference(&boolValue)))
			{
				if(boolValue)
				{
					symbolName=L"<R-REF> "+symbolName;
				}
			}
			if(SUCCEEDED(GetDiaSymbol()->get_volatileType(&boolValue)))
			{
				if(boolValue)
				{
					symbolName=L"<VOLATILE> "+symbolName;
				}
			}
		}
		if(tag==SymTagFunctionType)
		{
			enum CV_call_e callconv;
			if(SUCCEEDED(GetDiaSymbol()->get_callingConvention((DWORD*)&callconv)))
			{
				switch(callconv)
				{
				case CV_CALL_NEAR_C:
				case CV_CALL_NEAR_FAST:
				case CV_CALL_NEAR_STD:
				case CV_CALL_NEAR_SYS:
				case CV_CALL_THISCALL:
				case CV_CALL_CLRCALL:
					symbolName=L"<"+WString(GetCallingConversionName(callconv))+L"> "+symbolName;
					break;
				}
			}
		}
		{
			enum CV_access_e access;
			if(SUCCEEDED(GetDiaSymbol()->get_access((DWORD*)&access)))
			{
				switch(access)
				{
				case CV_private:
				case CV_protected:
				case CV_public:
					symbolName=L"<"+WString(GetAccessName(access))+L"> "+symbolName;
					break;
				}
			}
		}

		return namePrefix+tagName+L": "+symbolName;
	}

	DiaSymbolProvider* GetNodeByVisibleIndex(int index)
	{
		if(index<=0)
		{
			return this;
		}
		else
		{
			EnsureChildren();
			index-=1;
			int totalExpandedCount=0;
			for(int i=0;i<expandedChildrenIndices.Count();i++)
			{
				int expandedIndex=expandedChildrenIndices[i];
				int expandedCount=children[expandedIndex]->totalVisibleNodes;
				int startIndex=expandedIndex+totalExpandedCount;
				if(index<startIndex)
				{
					return RequestChildInternal(index-totalExpandedCount);
				}
				else if(index<startIndex+expandedCount)
				{
					return children[expandedIndex]->GetNodeByVisibleIndex(index-startIndex);
				}
				totalExpandedCount+=expandedCount-1;
			}
			return RequestChildInternal(index-totalExpandedCount);
		}
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

	WString GetPrimaryTextViewText(INodeProvider* node)
	{
		return GetNodeText(node);
	}

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

	bool CanGetNodeByVisibleIndex()
	{
		return true;
	}

	tree::INodeProvider* GetNodeByVisibleIndex(int index)
	{
		return exeSymbol->GetNodeByVisibleIndex(index-1);
	};

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
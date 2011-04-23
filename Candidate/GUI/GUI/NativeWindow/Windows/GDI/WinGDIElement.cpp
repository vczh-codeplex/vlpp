#include "WinGDIElement.h"
#include "WinGDIApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace vl::presentation::windows;

/***********************************************************************
WinGDIElement
***********************************************************************/

			WinGDIElement::WinGDIElement(WinGDIElementEnvironment* _environment)
				:environment(_environment)
			{
			}

			WinGDIElement::~WinGDIElement()
			{
			}

/***********************************************************************
WinGDIElementEnvironment
***********************************************************************/

			WinGDIElementEnvironment::WinGDIElementEnvironment(INativeWindow* _window)
				:window(_window)
			{
			}

			WinGDIElementEnvironment::~WinGDIElementEnvironment()
			{
			}

			WinGDIElementEnvironment:: ResourceMap& WinGDIElementEnvironment::Resources()
			{
				return resources;
			}

			Ptr<Object> WinGDIElementEnvironment::Resource(const WString& name)
			{
				int index=resources.Keys().IndexOf(name);
				return index==-1?0:resources.Values()[index];
			}

			WinDC* WinGDIElementEnvironment::GetEnvironmentDC()
			{
				return GetNativeWindowDC(window);
			}
			
			Ptr<WinGDIElement> WinGDIElementEnvironment::GetRootElement()
			{
				return rootElement;
			}

			void WinGDIElementEnvironment::SetRootElement(Ptr<WinGDIElement> element)
			{
				rootElement=element;
			}

			void WinGDIElementEnvironment::Paint()
			{
				if(rootElement)
				{
					rootElement->Paint(Size(0, 0), GetEnvironmentDC());
				}
			}

/***********************************************************************
WinGDIClipElement
***********************************************************************/

			WinGDIClipElement::WinGDIClipElement(WinGDIElementEnvironment* _environment)
				:WinGDIElement(_environment)
			{
			}

			WinGDIClipElement::~WinGDIClipElement()
			{
			}

			WinGDIClipElement::ElementList& WinGDIClipElement::Children()
			{
				return children;
			}

			Rect WinGDIClipElement::GetBounds()
			{
				return bounds;
			}

			void WinGDIClipElement::SetBounds(Rect value)
			{
				bounds=value;
			}

			void WinGDIClipElement::Paint(Size offset, windows::WinDC* dc)
			{
				if(children.Count()>0)
				{
					Rect clipBounds(bounds.LeftTop()+offset, bounds.GetSize());
					WinRegion::Ptr clip=dc->GetClipRegion();
					dc->ClipRegion(new WinRegion(clipBounds.Left(), clipBounds.Top(), clipBounds.Right(), clipBounds.Bottom(), true));
					Size newOffset(clipBounds.Left(), clipBounds.Top());
					for(int i=0;i<children.Count();i++)
					{
						children[i]->Paint(newOffset, dc);
					}
					dc->ClipRegion(clip);
				}
			}
		}
	}
}
/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION

#include "WinGDI.h"
#include "..\WinNativeWindow.h"
#include "..\..\..\..\..\..\Library\Collections\Dictionary.h"
#include "..\..\..\GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class WinGDIElement;
			class WinGDIElementEnvironment;
		}

		namespace windows
		{
			extern WinDC*									GetNativeWindowDC(INativeWindow* window);
			extern elements::WinGDIElementEnvironment*		GetNativeWindowGDIElementEnvironment(INativeWindow* window);
		}

		namespace elements
		{

/***********************************************************************
基础建设
***********************************************************************/

			class WinGDIElement : public Object
			{
			protected:
				WinGDIElementEnvironment*		environment;

			public:
				WinGDIElement(WinGDIElementEnvironment* _environment);
				~WinGDIElement();

				virtual void					Paint(Size offset, windows::WinDC* dc)=0;
			};

			class WinGDIElementEnvironment : public Object
			{
				typedef collections::Dictionary<WString, Ptr<Object>>	ResourceMap;
			protected:
				INativeWindow*					window;
				Ptr<WinGDIElement>				rootElement;
				ResourceMap						resources;
			public:
				WinGDIElementEnvironment(INativeWindow* _window);
				~WinGDIElementEnvironment();

				ResourceMap&					Resources();
				Ptr<Object>						Resource(const WString& name);
				windows::WinDC*					GetEnvironmentDC();
				Ptr<WinGDIElement>				GetRootElement();
				void							SetRootElement(Ptr<WinGDIElement> element);
				void							Paint();
			};

/***********************************************************************
基本元素
***********************************************************************/

			class WinGDIClipElement : public WinGDIElement
			{
				typedef collections::List<Ptr<WinGDIElement>>			ElementList;
			protected:
				ElementList						children;
				Rect							bounds;
			public:
				WinGDIClipElement(WinGDIElementEnvironment* _environment);
				~WinGDIClipElement();
				
				ElementList&					Children();
				Rect							GetBounds();
				void							SetBounds(Rect value);
				void							Paint(Size offset, windows::WinDC* dc);
			};

/***********************************************************************
基本皮肤
***********************************************************************/

#define DEFINE_BUILDER(SKIN)\
			public:\
				class Builder : public Object, public IGuiSkinBuilder\
				{\
				public:\
					Ptr<IGuiSkin> Build(INativeWindow* window)\
					{\
						return new SKIN(window);\
					}\
				}

#define BUILDER_OF_SKIN(SKIN) SKIN::Builder

			class WinGDISkinBase : public Object, public virtual IGuiSkin
			{
				template<typename ISkinInterface>
				friend class WinGDISkin;
			protected:
				virtual Ptr<WinGDIClipElement>				GetContainerElement()=0;
				virtual int									GetTopLevelElementCount()=0;
				virtual void								InsertElements(int index, Ptr<WinGDIClipElement> containerElement)=0;
			};

			template<typename ISkinInterface>
			class WinGDISkin : public WinGDISkinBase, public ISkinInterface
			{
			protected:
				INativeWindow*								window;
				WinGDIElementEnvironment*					environment;
				IGuiSkinListener*							skinListener;
				collections::List<WinGDISkinBase*>			childSkins;

			public:
				WinGDISkin(INativeWindow* _window)
					:window(_window)
					,environment(windows::GetNativeWindowGDIElementEnvironment(_window))
					,skinListener(0)
				{
				}

				IGuiSkinListener* GetListener()
				{
					return skinListener;
				}

				void AttachListener(IGuiSkinListener* listener)
				{
					skinListener=listener;
				}

				Size GetClientSizeFromBoundsSize(Size value)
				{
					Ptr<WinGDIClipElement> container=GetContainerElement();
					if(container)
					{
						return value-(GetBounds().GetSize()-container->GetBounds().GetSize());
					}
					else
					{
						return value;
					}
				}

				Size GetBoundsSizeFromClientSize(Size value)
				{
					Ptr<WinGDIClipElement> container=GetContainerElement();
					if(container)
					{
						return value+(GetBounds().GetSize()-container->GetBounds().GetSize());
					}
					else
					{
						return value;
					}
				}

				bool ContainsPoint(Point value)
				{
					return GetBounds().Contains(value);
				}

				void RemoveChild(IGuiSkin* child)
				{
					WinGDISkinBase* skin=dynamic_cast<WinGDISkinBase*>(child);
					if(skin)
					{
						int index=childSkins.IndexOf(skin);
						if(index>=0)
						{
							int previousCount=0;
							for(int i=0;i<index;i++)
							{
								previousCount=childSkins[i]->GetTopLevelElementCount();
							}

							int totalCount=skin->GetTopLevelElementCount();
							collections::List<Ptr<WinGDIElement>>& elements=GetContainerElement()->Children();
							for(int i=0;i<totalCount;i++)
							{
								elements.RemoveAt(previousCount);
							}
							childSkins.RemoveAt(index);
							skinListener->RequireRedraw();
						}
					}
				}

				void InsertChild(int index, IGuiSkin* child)
				{
					WinGDISkinBase* skin=dynamic_cast<WinGDISkinBase*>(child);
					if(skin && !childSkins.Contains(skin))
					{
						int previousCount=0;
						for(int i=0;i<index;i++)
						{
							previousCount=childSkins[i]->GetTopLevelElementCount();
						}

						skin->InsertElements(previousCount, GetContainerElement());

						childSkins.Insert(index, skin);
						skinListener->RequireRedraw();
					}
				}

				IGuiSkin* GetChild(int index)
				{
					return childSkins[index];
				}

				int ChildCount()
				{
					return childSkins.Count();
				}
			};
		}
	}
}

#endif
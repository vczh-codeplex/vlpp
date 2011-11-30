/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{

/***********************************************************************
ListView Style Provider
***********************************************************************/

/***********************************************************************
ListView Data Source
***********************************************************************/

				class ListViewItem
				{
				public:
					Ptr<GuiImageData>							smallImage;
					Ptr<GuiImageData>							largeImage;
					WString										text;
					collections::List<WString>					subItems;
				};

				class ListViewItemProvider : public ListProvider<Ptr<ListViewItem>>
				{
				public:
					ListViewItemProvider();
					~ListViewItemProvider();

					Interface*									RequestView(const WString& identifier)override;
					void										ReleaseView(Interface* view)override;
				};
			}

/***********************************************************************
ListView Control
***********************************************************************/

			class GuiListView : public GuiSelectableListControl
			{
			protected:
				list::ListViewItemProvider*							items;
			public:
				GuiListView(IStyleProvider* _styleProvider);
				~GuiListView();
			};
		}
	}
}

#endif
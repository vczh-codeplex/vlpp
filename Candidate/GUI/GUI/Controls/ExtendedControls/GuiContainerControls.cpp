#include "GuiContainerControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
/***********************************************************************
GuiTabPage
***********************************************************************/

			GuiTabPage::GuiTabPage(GuiTab* _owner, GuiControl::IStyleController* _styleController)
				:container(new GuiControl(_styleController))
				,owner(_owner)
			{
				TextChanged.SetAssociatedComposition(container->GetBoundsComposition());
			}

			GuiTabPage::~GuiTabPage()
			{
				if(!container->GetParent())
				{
					delete container;
				}
			}

			GuiControl* GuiTabPage::GetContainer()
			{
				return container;
			}

			GuiTab* GuiTabPage::GetOwnerTab()
			{
				return owner;
			}

			const WString& GuiTabPage::GetText()
			{
				return text;
			}

			void GuiTabPage::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					owner->styleController->SetTabText(owner->tabPages.IndexOf(this), text);
					TextChanged.Execute(container->GetNotifyEventArguments());
				}
			}

			bool GuiTabPage::GetSelected()
			{
				return owner->GetSelectedPage()==this;
			}

/***********************************************************************
GuiTab
***********************************************************************/

			GuiTab::CommandExecutor::CommandExecutor(GuiTab* _tab)
				:tab(_tab)
			{
			}

			GuiTab::CommandExecutor::~CommandExecutor()
			{
			}

			void GuiTab::CommandExecutor::ShowTab(int index)
			{
				tab->SetSelectedPage(tab->GetPages()[index]);
			}

			GuiTab::GuiTab(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,selectedPage(0)
			{
				commandExecutor=new CommandExecutor(this);
				styleController->SetCommandExecutor(commandExecutor.Obj());
			}

			GuiTab::~GuiTab()
			{
				for(int i=0;i<tabPages.Count();i++)
				{
					delete tabPages[i];
				}
			}

			GuiTabPage* GuiTab::CreatePage(int index)
			{
				if(index>=0 && index>=tabPages.Count())
				{
					index=tabPages.Count()-1;
				}
				else if(index<-1)
				{
					index=-1;
				}

				GuiTabPage* page=new GuiTabPage(this, styleController->CreateTabPageStyleController());
				index=index==-1?tabPages.Add(page):tabPages.Insert(index, page);
				GetContainerComposition()->AddChild(page->GetContainer()->GetBoundsComposition());
				styleController->InsertTab(index);
				
				if(!selectedPage)
				{
					SetSelectedPage(page);
				}
				page->GetContainer()->SetVisible(page==selectedPage);
				return page;
			}

			bool GuiTab::RemovePage(GuiTabPage* value)
			{
				if(value->GetOwnerTab()==this)
				{
					int index=tabPages.IndexOf(value);
					styleController->RemoveTab(index);
					GetContainerComposition()->RemoveChild(value->GetContainer()->GetBoundsComposition());
					tabPages.RemoveAt(index);
					if(tabPages.Count()==0)
					{
						SetSelectedPage(0);
						return 0;
					}
					else if(selectedPage==value)
					{
						SetSelectedPage(tabPages[0]);
					}
					return true;
				}
				else
				{
					return false;
				}
			}

			const collections::IReadonlyList<GuiTabPage*>& GuiTab::GetPages()
			{
				return tabPages.Wrap();
			}

			GuiTabPage* GuiTab::GetSelectedPage()
			{
				return selectedPage;
			}

			bool GuiTab::SetSelectedPage(GuiTabPage* value)
			{
				if(value->GetOwnerTab()==this)
				{
					if(selectedPage!=value)
					{
						selectedPage=value;
						for(int i=0;i<tabPages.Count();i++)
						{
							bool selected=tabPages[i]==value;
							tabPages[i]->GetContainer()->SetVisible(selected);
							if(selected)
							{
								styleController->SetSelectedTab(i);
							}
						}
						SelectedPageChanged.Execute(GetNotifyEventArguments());
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
}
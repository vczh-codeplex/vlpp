#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

/***********************************************************************
GuiControl
***********************************************************************/

			void GuiControl::OnChildInserted(GuiControl* control)
			{
				GuiControl* oldParent=control->parent;
				children.Add(control);
				control->parent=this;
				control->OnParentChanged(oldParent, control->parent);
				control->UpdateVisuallyEnabled();
			}

			void GuiControl::OnChildRemoved(GuiControl* control)
			{
				GuiControl* oldParent=control->parent;
				control->parent=0;
				children.Remove(control);
				control->OnParentChanged(oldParent, control->parent);
			}

			void GuiControl::OnParentChanged(GuiControl* oldParent, GuiControl* newParent)
			{
				OnParentLineChanged();
			}

			void GuiControl::OnParentLineChanged()
			{
				for(int i=0;i<children.Count();i++)
				{
					children[i]->OnParentLineChanged();
				}
			}

			void GuiControl::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
			}

			void GuiControl::UpdateVisuallyEnabled()
			{
				bool newValue=isEnabled && (parent==0?true:parent->GetVisuallyEnabled());
				if(isVisuallyEnabled!=newValue)
				{
					isVisuallyEnabled=newValue;
					styleController->SetVisuallyEnabled(isVisuallyEnabled);
					VisuallyEnabledChanged.Execute(GetNotifyEventArguments());

					for(int i=0;i<children.Count();i++)
					{
						children[i]->UpdateVisuallyEnabled();
					}
				}
			}

			void GuiControl::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				if(focusableComposition!=value)
				{
					focusableComposition=value;
					styleController->SetFocusableComposition(focusableComposition);
				}
			}

			GuiControl::GuiControl(IStyleController* _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,focusableComposition(0)
				,eventReceiver(_styleController->GetBoundsComposition()->GetEventReceiver())
				,isEnabled(true)
				,isVisuallyEnabled(true)
				,isVisible(true)
				,parent(0)
			{
				boundsComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(boundsComposition);
				EnabledChanged.SetAssociatedComposition(boundsComposition);
				VisuallyEnabledChanged.SetAssociatedComposition(boundsComposition);
				TextChanged.SetAssociatedComposition(boundsComposition);
				FontChanged.SetAssociatedComposition(boundsComposition);

				font=GetCurrentController()->GetDefaultFont();
				styleController->SetFont(font);
				styleController->SetText(text);
				styleController->SetVisuallyEnabled(isVisuallyEnabled);
			}

			GuiControl::~GuiControl()
			{
				if(parent || !styleController)
				{
					for(int i=0;i<children.Count();i++)
					{
						delete children[i];
					}
				}
				else
				{
					for(int i=children.Count()-1;i>=0;i--)
					{
						GuiControl* child=children[i];
						child->GetBoundsComposition()->GetParent()->RemoveChild(child->GetBoundsComposition());
						delete child;
					}
					delete boundsComposition;
				}
			}

			elements::GuiEventArgs GuiControl::GetNotifyEventArguments()
			{
				return GuiEventArgs(boundsComposition);
			}

			GuiControl::IStyleController* GuiControl::GetStyleController()
			{
				return styleController.Obj();
			}

			elements::GuiBoundsComposition* GuiControl::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* GuiControl::GetContainerComposition()
			{
				return styleController->GetContainerComposition();
			}

			elements::GuiGraphicsComposition* GuiControl::GetFocusableComposition()
			{
				return focusableComposition;
			}

			elements::GuiGraphicsEventReceiver* GuiControl::GetEventReceiver()
			{
				return eventReceiver;
			}

			GuiControl* GuiControl::GetParent()
			{
				return parent;
			}

			int GuiControl::GetChildrenCount()
			{
				return children.Count();
			}

			GuiControl* GuiControl::GetChild(int index)
			{
				return children[index];
			}

			bool GuiControl::GetVisuallyEnabled()
			{
				return isVisuallyEnabled;
			}

			bool GuiControl::GetEnabled()
			{
				return isEnabled;
			}

			void GuiControl::SetEnabled(bool value)
			{
				if(isEnabled!=value)
				{
					isEnabled=value;
					EnabledChanged.Execute(GetNotifyEventArguments());
					UpdateVisuallyEnabled();
				}
			}

			bool GuiControl::GetVisible()
			{
				return isVisible;
			}

			void GuiControl::SetVisible(bool value)
			{
				boundsComposition->SetVisible(value);
				if(isVisible!=value)
				{
					isVisible=value;
					VisibleChanged.Execute(GetNotifyEventArguments());
				}
			}

			const WString& GuiControl::GetText()
			{
				return text;
			}

			void GuiControl::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					styleController->SetText(text);
					TextChanged.Execute(GetNotifyEventArguments());
				}
			}

			const FontProperties& GuiControl::GetFont()
			{
				return font;
			}

			void GuiControl::SetFont(const FontProperties& value)
			{
				if(font!=value)
				{
					font=value;
					styleController->SetFont(font);
					FontChanged.Execute(GetNotifyEventArguments());
				}
			}

			Interface* GuiControl::QueryService(const WString& identifier)
			{
				if(parent)
				{
					return parent->QueryService(identifier);
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
GuiComponent
***********************************************************************/
			
			GuiComponent::GuiComponent()
				:controlHost(0)
			{
			}

			GuiComponent::~GuiComponent()
			{
			}

			GuiControlHost* GuiComponent::GetControlHost()
			{
				return controlHost;
			}

			void GuiComponent::SetControlHost(GuiControlHost* value)
			{
				controlHost=value;
			}

/***********************************************************************
GuiImageData
***********************************************************************/

			GuiImageData::GuiImageData()
				:frameIndex(-1)
			{
			}

			GuiImageData::GuiImageData(Ptr<INativeImage> _image, int _frameIndex)
				:image(_image)
				,frameIndex(_frameIndex)
			{
			}

			GuiImageData::~GuiImageData()
			{
			}

			Ptr<INativeImage> GuiImageData::GetImage()
			{
				return image;
			}

			int GuiImageData::GetFrameIndex()
			{
				return frameIndex;
			}

/***********************************************************************
GuiButton
***********************************************************************/

			void GuiButton::UpdateControlState()
			{
				ControlState newControlState=Normal;
				if(mousePressing)
				{
					if(mouseHoving)
					{
						newControlState=Pressed;
					}
					else
					{
						newControlState=Active;
					}
				}
				else
				{
					if(mouseHoving)
					{
						newControlState=Active;
					}
					else
					{
						newControlState=Normal;
					}
				}
				if(controlState!=newControlState)
				{
					controlState=newControlState;
					styleController->Transfer(controlState);
				}
			}

			void GuiButton::OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mousePressing=true;
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
					UpdateControlState();
				}
			}

			void GuiButton::OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mousePressing=false;
					UpdateControlState();
				}
				if(GetVisuallyEnabled())
				{
					if(mouseHoving)
					{
						Clicked.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiButton::OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mouseHoving=true;
					UpdateControlState();
				}
			}

			void GuiButton::OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mouseHoving=false;
					UpdateControlState();
				}
			}

			GuiButton::GuiButton(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,mousePressing(false)
				,mouseHoving(false)
				,controlState(Normal)
			{
				Clicked.SetAssociatedComposition(boundsComposition);
				styleController->Transfer(Normal);
				styleController->SetFocusableComposition(boundsComposition);

				GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiButton::OnLeftButtonDown);
				GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiButton::OnLeftButtonUp);
				GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiButton::OnMouseEnter);
				GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiButton::OnMouseLeave);
			}

			GuiButton::~GuiButton()
			{
			}

/***********************************************************************
GuiSelectableButton::GroupController
***********************************************************************/

			GuiSelectableButton::GroupController::GroupController()
			{
			}

			GuiSelectableButton::GroupController::~GroupController()
			{
			}

			void GuiSelectableButton::GroupController::Attach(GuiSelectableButton* button)
			{
				if(!buttons.Contains(button))
				{
					buttons.Add(button);
				}
			}

			void GuiSelectableButton::GroupController::Detach(GuiSelectableButton* button)
			{
				buttons.Remove(button);
			}

/***********************************************************************
GuiSelectableButton::MutexGroupController
***********************************************************************/

			GuiSelectableButton::MutexGroupController::MutexGroupController()
				:suppress(false)
			{
			}

			GuiSelectableButton::MutexGroupController::~MutexGroupController()
			{
			}

			void GuiSelectableButton::MutexGroupController::OnSelectedChanged(GuiSelectableButton* button)
			{
				if(!suppress)
				{
					suppress=true;
					for(int i=0;i<buttons.Count();i++)
					{
						buttons[i]->SetSelected(buttons[i]==button);
					}
					suppress=false;
				}
			}

/***********************************************************************
GuiSelectableButton
***********************************************************************/

			void GuiSelectableButton::OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(autoSelection)
				{
					SetSelected(!GetSelected());
				}
			}

			GuiSelectableButton::GuiSelectableButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,groupController(0)
				,autoSelection(true)
				,isSelected(false)
			{
				GroupControllerChanged.SetAssociatedComposition(boundsComposition);
				AutoSelectionChanged.SetAssociatedComposition(boundsComposition);
				SelectedChanged.SetAssociatedComposition(boundsComposition);

				Clicked.AttachMethod(this, &GuiSelectableButton::OnClicked);
				styleController->SetSelected(isSelected);
			}
			
			GuiSelectableButton::~GuiSelectableButton()
			{
			}

			GuiSelectableButton::GroupController* GuiSelectableButton::GetGroupController()
			{
				return groupController;
			}

			void GuiSelectableButton::SetGroupController(GroupController* value)
			{
				if(groupController)
				{
					groupController->Detach(this);
				}
				groupController=value;
				if(groupController)
				{
					groupController->Attach(this);
				}
				GroupControllerChanged.Execute(GetNotifyEventArguments());
			}

			bool GuiSelectableButton::GetAutoSelection()
			{
				return autoSelection;
			}

			void GuiSelectableButton::SetAutoSelection(bool value)
			{
				if(autoSelection!=value)
				{
					autoSelection=value;
					AutoSelectionChanged.Execute(GetNotifyEventArguments());
				}
			}

			bool GuiSelectableButton::GetSelected()
			{
				return isSelected;
			}

			void GuiSelectableButton::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					styleController->SetSelected(isSelected);
					if(groupController)
					{
						groupController->OnSelectedChanged(this);
					}
					SelectedChanged.Execute(GetNotifyEventArguments());
				}
			}

/***********************************************************************
GuiScroll::CommandExecutor
***********************************************************************/

			GuiScroll::CommandExecutor::CommandExecutor(GuiScroll* _scroll)
				:scroll(_scroll)
			{
			}

			GuiScroll::CommandExecutor::~CommandExecutor()
			{
			}

			void GuiScroll::CommandExecutor::SmallDecrease()
			{
				scroll->SetPosition(scroll->GetPosition()-scroll->GetSmallMove());
			}

			void GuiScroll::CommandExecutor::SmallIncrease()
			{
				scroll->SetPosition(scroll->GetPosition()+scroll->GetSmallMove());
			}

			void GuiScroll::CommandExecutor::BigDecrease()
			{
				scroll->SetPosition(scroll->GetPosition()-scroll->GetBigMove());
			}

			void GuiScroll::CommandExecutor::BigIncrease()
			{
				scroll->SetPosition(scroll->GetPosition()+scroll->GetBigMove());
			}
			
			void GuiScroll::CommandExecutor::SetTotalSize(int value)
			{
				scroll->SetTotalSize(value);
			}

			void GuiScroll::CommandExecutor::SetPageSize(int value)
			{
				scroll->SetPageSize(value);
			}

			void GuiScroll::CommandExecutor::SetPosition(int value)
			{
				scroll->SetPosition(value);
			}

/***********************************************************************
GuiScroll
***********************************************************************/

			GuiScroll::GuiScroll(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,totalSize(100)
				,pageSize(10)
				,position(0)
				,smallMove(1)
				,bigMove(10)
			{
				TotalSizeChanged.SetAssociatedComposition(boundsComposition);
				PageSizeChanged.SetAssociatedComposition(boundsComposition);
				PositionChanged.SetAssociatedComposition(boundsComposition);
				SmallMoveChanged.SetAssociatedComposition(boundsComposition);
				BigMoveChanged.SetAssociatedComposition(boundsComposition);

				commandExecutor=new CommandExecutor(this);
				styleController->SetCommandExecutor(commandExecutor.Obj());
				styleController->SetPageSize(pageSize);
				styleController->SetTotalSize(totalSize);
				styleController->SetPosition(position);
			}

			GuiScroll::~GuiScroll()
			{
			}

			int GuiScroll::GetTotalSize()
			{
				return totalSize;
			}

			void GuiScroll::SetTotalSize(int value)
			{
				if(totalSize!=value && 0<value)
				{
					totalSize=value;
					if(pageSize>totalSize)
					{
						SetPageSize(totalSize);
					}
					if(position>GetMaxPosition())
					{
						SetPosition(GetMaxPosition());
					}
					styleController->SetTotalSize(totalSize);
					TotalSizeChanged.Execute(GetNotifyEventArguments());
				}
			}

			int GuiScroll::GetPageSize()
			{
				return pageSize;
			}

			void GuiScroll::SetPageSize(int value)
			{
				if(pageSize!=value && 0<=value && value<=totalSize)
				{
					pageSize=value;
					if(position>GetMaxPosition())
					{
						SetPosition(GetMaxPosition());
					}
					styleController->SetPageSize(pageSize);
					PageSizeChanged.Execute(GetNotifyEventArguments());
				}
			}

			int GuiScroll::GetPosition()
			{
				return position;
			}

			void GuiScroll::SetPosition(int value)
			{
				int min=GetMinPosition();
				int max=GetMaxPosition();
				int newPosition=
					value<min?min:
					value>max?max:
					value;
				if(position!=newPosition)
				{
					position=newPosition;
					styleController->SetPosition(position);
					PositionChanged.Execute(GetNotifyEventArguments());
				}
			}

			int GuiScroll::GetSmallMove()
			{
				return smallMove;
			}

			void GuiScroll::SetSmallMove(int value)
			{
				if(value>0 && smallMove!=value)
				{
					smallMove=value;
					SmallMoveChanged.Execute(GetNotifyEventArguments());
				}
			}

			int GuiScroll::GetBigMove()
			{
				return bigMove;
			}

			void GuiScroll::SetBigMove(int value)
			{
				if(value>0 && bigMove!=value)
				{
					bigMove=value;
					BigMoveChanged.Execute(GetNotifyEventArguments());
				}
			}

			int GuiScroll::GetMinPosition()
			{
				return 0;
			}

			int GuiScroll::GetMaxPosition()
			{
				return totalSize-pageSize;
			}

/***********************************************************************
GuiScrollView::StyleController
***********************************************************************/

			void GuiScrollView::StyleController::UpdateTable()
			{
				if(horizontalScroll->GetEnabled() || horizontalAlwaysVisible)
				{
					tableComposition->SetRowOption(1, GuiCellOption::AbsoluteOption(styleProvider->GetDefaultScrollSize()));
				}
				else
				{
					tableComposition->SetRowOption(1, GuiCellOption::AbsoluteOption(0));
				}
				if(verticalScroll->GetEnabled() || verticalAlwaysVisible)
				{
					tableComposition->SetColumnOption(1, GuiCellOption::AbsoluteOption(styleProvider->GetDefaultScrollSize()));
				}
				else
				{
					tableComposition->SetColumnOption(1, GuiCellOption::AbsoluteOption(0));
				}
				tableComposition->UpdateCellBounds();
			}

			GuiScrollView::StyleController::StyleController(IStyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
				,scrollView(0)
				,horizontalAlwaysVisible(true)
				,verticalAlwaysVisible(true)
			{
				horizontalScroll=new GuiScroll(styleProvider->CreateHorizontalScrollStyle());
				horizontalScroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				horizontalScroll->SetEnabled(false);
				verticalScroll=new GuiScroll(styleProvider->CreateVerticalScrollStyle());
				verticalScroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				verticalScroll->SetEnabled(false);

				boundsComposition=new GuiBoundsComposition;
				GuiGraphicsComposition* tableContainerComposition=styleProvider->InstallBackground(boundsComposition);

				tableComposition=new GuiTableComposition;
				tableContainerComposition->AddChild(tableComposition);
				tableComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				tableComposition->SetRowsAndColumns(2, 2);
				tableComposition->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
				tableComposition->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				UpdateTable();
				{
					GuiCellComposition* cell=new GuiCellComposition;
					tableComposition->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);
					cell->AddChild(horizontalScroll->GetBoundsComposition());
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					tableComposition->AddChild(cell);
					cell->SetSite(0, 1, 1, 1);
					cell->AddChild(verticalScroll->GetBoundsComposition());
				}
				
				containerCellComposition=new GuiCellComposition;
				tableComposition->AddChild(containerCellComposition);
				containerCellComposition->SetSite(0, 0, 1, 1);

				containerComposition=new GuiBoundsComposition;
				containerComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				containerCellComposition->AddChild(containerComposition);

				styleProvider->AssociateStyleController(this);
			}

			GuiScrollView::StyleController::~StyleController()
			{
			}

			void GuiScrollView::StyleController::SetScrollView(GuiScrollView* _scrollView)
			{
				scrollView=_scrollView;
			}

			void GuiScrollView::StyleController::AdjustView(Size fullSize)
			{
				Size viewSize=containerComposition->GetBounds().GetSize();
				if(fullSize.x<viewSize.x)
				{
					horizontalScroll->SetEnabled(false);
					horizontalScroll->SetPosition(0);
				}
				else
				{
					horizontalScroll->SetEnabled(true);
					horizontalScroll->SetTotalSize(fullSize.x);
					horizontalScroll->SetPageSize(viewSize.x);
				}
				if(fullSize.y<viewSize.y)
				{
					verticalScroll->SetEnabled(false);
					verticalScroll->SetPosition(0);
				}
				else
				{
					verticalScroll->SetEnabled(true);
					verticalScroll->SetTotalSize(fullSize.y);
					verticalScroll->SetPageSize(viewSize.y);
				}
				UpdateTable();
			}

			GuiScrollView::IStyleProvider* GuiScrollView::StyleController::GetStyleProvider()
			{
				return styleProvider.Obj();
			}

			GuiScroll* GuiScrollView::StyleController::GetHorizontalScroll()
			{
				return horizontalScroll;
			}

			GuiScroll* GuiScrollView::StyleController::GetVerticalScroll()
			{
				return verticalScroll;
			}

			elements::GuiTableComposition* GuiScrollView::StyleController::GetInternalTableComposition()
			{
				return tableComposition;
			}

			elements::GuiBoundsComposition* GuiScrollView::StyleController::GetInternalContainerComposition()
			{
				return containerComposition;
			}

			bool GuiScrollView::StyleController::GetHorizontalAlwaysVisible()
			{
				return horizontalAlwaysVisible;
			}

			void GuiScrollView::StyleController::SetHorizontalAlwaysVisible(bool value)
			{
				if(horizontalAlwaysVisible!=value)
				{
					horizontalAlwaysVisible=value;
					scrollView->CalculateView();
				}
			}

			bool GuiScrollView::StyleController::GetVerticalAlwaysVisible()
			{
				return verticalAlwaysVisible;
			}

			void GuiScrollView::StyleController::SetVerticalAlwaysVisible(bool value)
			{
				if(verticalAlwaysVisible!=value)
				{
					verticalAlwaysVisible=value;
					scrollView->CalculateView();
				}
			}

			elements::GuiBoundsComposition* GuiScrollView::StyleController::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* GuiScrollView::StyleController::GetContainerComposition()
			{
				return containerComposition;
			}

			void GuiScrollView::StyleController::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				styleProvider->SetFocusableComposition(value);
			}

			void GuiScrollView::StyleController::SetText(const WString& value)
			{
				styleProvider->SetText(value);
			}

			void GuiScrollView::StyleController::SetFont(const FontProperties& value)
			{
				styleProvider->SetFont(value);
			}

			void GuiScrollView::StyleController::SetVisuallyEnabled(bool value)
			{
				styleProvider->SetVisuallyEnabled(value);
			}

/***********************************************************************
GuiScrollView
***********************************************************************/

			void GuiScrollView::OnContainerBoundsChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				CalculateView();
			}

			void GuiScrollView::OnHorizontalScroll(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(!supressScrolling)
				{
					CallUpdateView();
				}
			}

			void GuiScrollView::OnVerticalScroll(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(!supressScrolling)
				{
					CallUpdateView();
				}
			}

			void GuiScrollView::CallUpdateView()
			{
				Rect viewBounds=GetViewBounds();
				UpdateView(viewBounds);
			}

			void GuiScrollView::Initialize()
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());
				styleController->SetScrollView(this);

				styleController->GetInternalContainerComposition()->BoundsChanged.AttachMethod(this, &GuiScrollView::OnContainerBoundsChanged);
				styleController->GetHorizontalScroll()->PositionChanged.AttachMethod(this, &GuiScrollView::OnHorizontalScroll);
				styleController->GetVerticalScroll()->PositionChanged.AttachMethod(this, &GuiScrollView::OnVerticalScroll);
			}

			GuiScrollView::GuiScrollView(StyleController* _styleController)
				:GuiControl(_styleController)
				,supressScrolling(false)
			{
				Initialize();
			}

			GuiScrollView::GuiScrollView(IStyleProvider* styleProvider)
				:GuiControl(new StyleController(styleProvider))
				,supressScrolling(false)
			{
				Initialize();
			}
			
			GuiScrollView::~GuiScrollView()
			{
			}

			void GuiScrollView::CalculateView()
			{
				if(!supressScrolling)
				{
					Size fullSize=QueryFullSize();
					while(true)
					{
						styleController->AdjustView(fullSize);
						styleController->AdjustView(fullSize);
						supressScrolling=true;
						CallUpdateView();
						supressScrolling=false;

						Size newSize=QueryFullSize();
						if(fullSize==newSize)
						{
							break;
						}
						else
						{
							fullSize=newSize;
						}
					}
				}
			}

			Size GuiScrollView::GetViewSize()
			{
				Size viewSize=styleController->GetInternalContainerComposition()->GetBounds().GetSize();
				return viewSize;
			}

			Rect GuiScrollView::GetViewBounds()
			{
				Point viewPosition=
					Point(
						styleController->GetHorizontalScroll()->GetPosition(),
						styleController->GetVerticalScroll()->GetPosition()
						);
				Size viewSize=GetViewSize();
				return Rect(viewPosition, viewSize);
			}

			GuiScroll* GuiScrollView::GetHorizontalScroll()
			{
				return styleController->GetHorizontalScroll();
			}

			GuiScroll* GuiScrollView::GetVerticalScroll()
			{
				return styleController->GetVerticalScroll();
			}

			bool GuiScrollView::GetHorizontalAlwaysVisible()
			{
				return styleController->GetHorizontalAlwaysVisible();
			}

			void GuiScrollView::SetHorizontalAlwaysVisible(bool value)
			{
				styleController->SetHorizontalAlwaysVisible(value);
			}

			bool GuiScrollView::GetVerticalAlwaysVisible()
			{
				return styleController->GetVerticalAlwaysVisible();
			}

			void GuiScrollView::SetVerticalAlwaysVisible(bool value)
			{
				styleController->SetVerticalAlwaysVisible(value);
			}

/***********************************************************************
GuiScrollContainer::StyleController
***********************************************************************/

			GuiScrollContainer::StyleController::StyleController(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView::StyleController(styleProvider)
			{
				controlContainerComposition=new GuiBoundsComposition;
				controlContainerComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				GetInternalContainerComposition()->AddChild(controlContainerComposition);
			}

			GuiScrollContainer::StyleController::~StyleController()
			{
			}

			elements::GuiGraphicsComposition* GuiScrollContainer::StyleController::GetContainerComposition()
			{
				return controlContainerComposition;
			}

			void GuiScrollContainer::StyleController::MoveContainer(Point leftTop)
			{
				controlContainerComposition->SetBounds(Rect(leftTop, Size(0, 0)));
			}

/***********************************************************************
GuiScrollContainer
***********************************************************************/

			void GuiScrollContainer::OnControlContainerBoundsChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				CalculateView();
			}

			Size GuiScrollContainer::QueryFullSize()
			{
				return styleController->GetContainerComposition()->GetBounds().GetSize();
			}

			void GuiScrollContainer::UpdateView(Rect viewBounds)
			{
				styleController->MoveContainer(Point(-viewBounds.x1, -viewBounds.y1));
			}

			GuiScrollContainer::GuiScrollContainer(GuiScrollContainer::IStyleProvider* styleProvider)
				:GuiScrollView(new StyleController(styleProvider))
				,styleController(0)
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());
				GuiBoundsComposition* composition=dynamic_cast<GuiBoundsComposition*>(styleController->GetContainerComposition());
				composition->BoundsChanged.AttachMethod(this, &GuiScrollContainer::OnControlContainerBoundsChanged);
			}

			GuiScrollContainer::~GuiScrollContainer()
			{
			}
		}
	}
}
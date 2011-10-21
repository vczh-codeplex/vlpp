#include <math.h>
#include "GuiCommonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace common_styles
		{
			using namespace elements;
			using namespace controls;

/***********************************************************************
CommonScrollStyle
***********************************************************************/

			void CommonScrollStyle::UpdateHandle()
			{
				double handleRatio=(double)position/totalSize;
				double handlePageSize=(double)pageSize/totalSize;
				switch(direction)
				{
				case Horizontal:
					handleComposition->SetWidthRatio(handleRatio);
					handleComposition->SetWidthPageSize(handlePageSize);
					break;
				case Vertical:
					handleComposition->SetHeightRatio(handleRatio);
					handleComposition->SetHeightPageSize(handlePageSize);
					break;
				}
			}

			void CommonScrollStyle::OnDecreaseButtonClicked(elements::GuiGraphicsComposition* sender,elements::GuiEventArgs& arguments)
			{
				if(commandExecutor)
				{
					commandExecutor->SmallDecrease();
				}
			}

			void CommonScrollStyle::OnIncreaseButtonClicked(elements::GuiGraphicsComposition* sender,elements::GuiEventArgs& arguments)
			{
				if(commandExecutor)
				{
					commandExecutor->SmallIncrease();
				}
			}

			void CommonScrollStyle::OnHandleMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				if(commandExecutor && handleButton->GetVisuallyEnabled())
				{
					draggingHandle=true;
					draggingStartLocation=Point(arguments.x, arguments.y);
				}
			}

			void CommonScrollStyle::OnHandleMouseMove(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				if(draggingHandle)
				{
					int totalPixels=0;
					int currentOffset=0;
					int newOffset=0;
					switch(direction)
					{
					case Horizontal:
						totalPixels=handleComposition->GetParent()->GetBounds().Width();
						currentOffset=handleComposition->GetBounds().Left();
						newOffset=currentOffset+(arguments.x-draggingStartLocation.x);
						break;
					case Vertical:
						totalPixels=handleComposition->GetParent()->GetBounds().Height();
						currentOffset=handleComposition->GetBounds().Top();
						newOffset=currentOffset+(arguments.y-draggingStartLocation.y);
						break;
					}

					double ratio=(double)newOffset/totalPixels;
					int newPosition=(int)(ratio*totalSize);

					int offset1=(int)(((double)newPosition/totalSize)*totalPixels);
					int offset2=int(((double)(newPosition+1)/totalSize)*totalPixels);
					int delta1=abs(offset1-newOffset);
					int delta2=abs(offset2-newOffset);
					if(delta1<delta2)
					{
						commandExecutor->SetPosition(newPosition);
					}
					else
					{
						commandExecutor->SetPosition(newPosition+1);
					}
				}
			}

			void CommonScrollStyle::OnHandleMouseUp(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				draggingHandle=false;
			}

			void CommonScrollStyle::OnBigMoveMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				if(commandExecutor && handleButton->GetVisuallyEnabled())
				{
					if(arguments.eventSource==arguments.compositionSource)
					{
						Rect handleBounds=handleComposition->GetBounds();
						switch(direction)
						{
						case Horizontal:
							if(arguments.x<handleBounds.x1)
							{
								commandExecutor->BigDecrease();
							}
							else if(arguments.x>=handleBounds.x2)
							{
								commandExecutor->BigIncrease();
							}
							break;
						case Vertical:
							if(arguments.y<handleBounds.y1)
							{
								commandExecutor->BigDecrease();
							}
							else if(arguments.y>=handleBounds.y2)
							{
								commandExecutor->BigIncrease();
							}
							break;
						}
					}
				}
			}

			void CommonScrollStyle::BuildStyle(int defaultSize, int arrowSize)
			{
				boundsComposition=new GuiBoundsComposition;
				InstallBackground(boundsComposition, direction);
				{
					GuiBoundsComposition* handleBoundsComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(handleBoundsComposition);
					switch(direction)
					{
					case Horizontal:
						handleBoundsComposition->SetAlignmentToParent(Margin(defaultSize, 0, defaultSize, 0));
						break;
					case Vertical:
						handleBoundsComposition->SetAlignmentToParent(Margin(0, defaultSize, 0, defaultSize));
						break;
					}
					handleBoundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &CommonScrollStyle::OnBigMoveMouseDown);

					handleComposition=new GuiPartialViewComposition;
					handleBoundsComposition->AddChild(handleComposition);
					
					handleButton=new GuiButton(CreateHandleButtonStyle(direction));
					handleButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					handleComposition->AddChild(handleButton->GetBoundsComposition());

					handleButton->GetBoundsComposition()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &CommonScrollStyle::OnHandleMouseDown);
					handleButton->GetBoundsComposition()->GetEventReceiver()->mouseMove.AttachMethod(this, &CommonScrollStyle::OnHandleMouseMove);
					handleButton->GetBoundsComposition()->GetEventReceiver()->leftButtonUp.AttachMethod(this, &CommonScrollStyle::OnHandleMouseUp);
				}
				{
					FontProperties font;
					font.fontFamily=L"Wingdings 3";
					font.size=arrowSize;

					decreaseButton=new GuiButton(CreateDecreaseButtonStyle(direction));
					decreaseButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					decreaseButton->Clicked.AttachMethod(this, &CommonScrollStyle::OnDecreaseButtonClicked);
					decreaseButton->SetFont(font);
					
					increaseButton=new GuiButton(CreateIncreaseButtonStyle(direction));
					increaseButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					increaseButton->Clicked.AttachMethod(this, &CommonScrollStyle::OnIncreaseButtonClicked);
					increaseButton->SetFont(font);
				}
				{
					GuiSideAlignedComposition* decreaseComposition=new GuiSideAlignedComposition;
					decreaseComposition->SetMaxLength(defaultSize);
					decreaseComposition->SetMaxRatio(0.5);
					decreaseComposition->AddChild(decreaseButton->GetBoundsComposition());
					boundsComposition->AddChild(decreaseComposition);

					GuiSideAlignedComposition* increaseComposition=new GuiSideAlignedComposition;
					increaseComposition->SetMaxLength(defaultSize);
					increaseComposition->SetMaxRatio(0.5);
					increaseComposition->AddChild(increaseButton->GetBoundsComposition());
					boundsComposition->AddChild(increaseComposition);

					switch(direction)
					{
					case Horizontal:
						{
							decreaseComposition->SetDirection(GuiSideAlignedComposition::Left);
							decreaseButton->SetText((wchar_t)0x7C);
							increaseComposition->SetDirection(GuiSideAlignedComposition::Right);
							increaseButton->SetText((wchar_t)0x7D);
						}
						break;
					case Vertical:
						{
							decreaseComposition->SetDirection(GuiSideAlignedComposition::Top);
							decreaseButton->SetText((wchar_t)0x7E);
							increaseComposition->SetDirection(GuiSideAlignedComposition::Bottom);
							increaseButton->SetText((wchar_t)0xF080);
						}
						break;
					}
				}
			}

			CommonScrollStyle::CommonScrollStyle(Direction _direction)
				:direction(_direction)
				,commandExecutor(0)
				,decreaseButton(0)
				,increaseButton(0)
				,boundsComposition(0)
				,totalSize(1)
				,pageSize(1)
				,position(0)
				,draggingHandle(false)
			{
			}

			CommonScrollStyle::~CommonScrollStyle()
			{
			}

			elements::GuiBoundsComposition* CommonScrollStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* CommonScrollStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void CommonScrollStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void CommonScrollStyle::SetText(const WString& value)
			{
			}

			void CommonScrollStyle::SetFont(const FontProperties& value)
			{
			}

			void CommonScrollStyle::SetVisuallyEnabled(bool value)
			{
			}

			void CommonScrollStyle::SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)
			{
				commandExecutor=value;
			}

			void CommonScrollStyle::SetTotalSize(int value)
			{
				if(totalSize!=value)
				{
					totalSize=value;
					UpdateHandle();
				}
			}

			void CommonScrollStyle::SetPageSize(int value)
			{
				if(pageSize!=value)
				{
					pageSize=value;
					UpdateHandle();
				}
			}

			void CommonScrollStyle::SetPosition(int value)
			{
				if(position!=value)
				{
					position=value;
					UpdateHandle();
				}
			}

/***********************************************************************
CommonTrackStyle
***********************************************************************/

			void CommonTrackStyle::UpdateHandle()
			{
				int maxSize=totalSize-pageSize;
				if(maxSize<1) maxSize=1;
				double ratio=(double)position/maxSize;
				switch(direction)
				{
				case Horizontal:
					handleComposition->SetColumnOption(0, GuiCellOption::PercentageOption(ratio));
					handleComposition->SetColumnOption(2, GuiCellOption::PercentageOption(1-ratio));
					break;
				case Vertical:
					handleComposition->SetRowOption(0, GuiCellOption::PercentageOption(1-ratio));
					handleComposition->SetRowOption(2, GuiCellOption::PercentageOption(ratio));
					break;
				}
				handleComposition->UpdateCellBounds();
			}

			void CommonTrackStyle::OnHandleMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				if(commandExecutor && handleButton->GetVisuallyEnabled())
				{
					draggingHandle=true;
					draggingStartLocation=Point(arguments.x, arguments.y);
				}
			}

			void CommonTrackStyle::OnHandleMouseMove(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				if(draggingHandle)
				{
					int totalPixels=0;
					int currentOffset=0;
					int newOffset=0;

					Rect handleArea=handleComposition->GetBounds();
					Rect handleBounds=handleButton->GetBoundsComposition()->GetParent()->GetBounds();
					switch(direction)
					{
					case Horizontal:
						totalPixels=handleArea.Width()-handleBounds.Width();
						currentOffset=handleBounds.Left();
						newOffset=currentOffset+(arguments.x-draggingStartLocation.x);
						break;
					case Vertical:
						totalPixels=handleArea.Height()-handleBounds.Height();
						currentOffset=handleArea.Height()-handleBounds.Bottom();
						newOffset=currentOffset-(arguments.y-draggingStartLocation.y);
						break;
					}

					double ratio=(double)newOffset/totalPixels;
					int maxSize=totalSize-pageSize;
					if(maxSize<1) maxSize=1;
					int newPosition=(int)(ratio*maxSize);

					int offset1=(int)(((double)newPosition/maxSize)*totalPixels);
					int offset2=int(((double)(newPosition+1)/maxSize)*totalPixels);
					int delta1=abs(offset1-newOffset);
					int delta2=abs(offset2-newOffset);
					if(delta1<delta2)
					{
						commandExecutor->SetPosition(newPosition);
					}
					else
					{
						commandExecutor->SetPosition(newPosition+1);
					}
				}
			}

			void CommonTrackStyle::OnHandleMouseUp(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments)
			{
				draggingHandle=false;
			}

			void CommonTrackStyle::BuildStyle(int trackThickness, int trackPadding, int handleLong, int handleShort)
			{
				boundsComposition=new GuiBoundsComposition;
				InstallBackground(boundsComposition, direction);
				{
					GuiTableComposition* table=new GuiTableComposition;
					boundsComposition->AddChild(table);
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetRowsAndColumns(3, 3);
					table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					switch(direction)
					{
					case Horizontal:
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::AbsoluteOption(handleLong));
						table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
						table->SetColumnOption(0, GuiCellOption::AbsoluteOption(trackPadding));
						table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
						table->SetColumnOption(2, GuiCellOption::AbsoluteOption(trackPadding));
						break;
					case Vertical:
						table->SetRowOption(0, GuiCellOption::AbsoluteOption(trackPadding));
						table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
						table->SetRowOption(2, GuiCellOption::AbsoluteOption(trackPadding));
						table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetColumnOption(1, GuiCellOption::AbsoluteOption(handleLong));
						table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
						break;
					}
					{
						GuiCellComposition* contentCell=new GuiCellComposition;
						table->AddChild(contentCell);
						contentCell->SetSite(1, 1, 1, 1);
						{
							GuiTableComposition* trackTable=new GuiTableComposition;
							trackTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
							contentCell->AddChild(trackTable);
							GuiCellComposition* trackCell=new GuiCellComposition;
							trackTable->AddChild(trackCell);

							switch(direction)
							{
							case Horizontal:
								trackTable->SetRowsAndColumns(3, 1);
								trackTable->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
								trackTable->SetRowOption(1, GuiCellOption::AbsoluteOption(trackThickness));
								trackTable->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
								trackCell->SetSite(1, 0, 1, 1);
								break;
							case Vertical:
								trackTable->SetRowsAndColumns(1, 3);
								trackTable->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
								trackTable->SetColumnOption(1, GuiCellOption::AbsoluteOption(trackThickness));
								trackTable->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
								trackCell->SetSite(0, 1, 1, 1);
								break;
							}
							InstallTrack(trackCell, direction);
						}
						{
							handleComposition=new GuiTableComposition;
							handleComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
							contentCell->AddChild(handleComposition);
							GuiCellComposition* handleCell=new GuiCellComposition;
							handleComposition->AddChild(handleCell);

							switch(direction)
							{
							case Horizontal:
								handleComposition->SetRowsAndColumns(1, 3);
								handleComposition->SetColumnOption(0, GuiCellOption::PercentageOption(0.0));
								handleComposition->SetColumnOption(1, GuiCellOption::AbsoluteOption(handleShort));
								handleComposition->SetColumnOption(2, GuiCellOption::PercentageOption(1.0));
								handleCell->SetSite(0, 1, 1, 1);
								break;
							case Vertical:
								handleComposition->SetRowsAndColumns(3, 1);
								handleComposition->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
								handleComposition->SetRowOption(1, GuiCellOption::AbsoluteOption(handleShort));
								handleComposition->SetRowOption(2, GuiCellOption::PercentageOption(0.0));
								handleCell->SetSite(1, 0, 1, 1);
								break;
							}

							handleButton=new GuiButton(CreateHandleButtonStyle(direction));
							handleButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
							handleCell->AddChild(handleButton->GetBoundsComposition());

							handleButton->GetBoundsComposition()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &CommonTrackStyle::OnHandleMouseDown);
							handleButton->GetBoundsComposition()->GetEventReceiver()->mouseMove.AttachMethod(this, &CommonTrackStyle::OnHandleMouseMove);
							handleButton->GetBoundsComposition()->GetEventReceiver()->leftButtonUp.AttachMethod(this, &CommonTrackStyle::OnHandleMouseUp);
						}
					}
				}
			}

			CommonTrackStyle::CommonTrackStyle(Direction _direction)
				:direction(_direction)
				,commandExecutor(0)
				,totalSize(1)
				,pageSize(1)
				,position(0)
				,draggingHandle(false)
			{
			}

			CommonTrackStyle::~CommonTrackStyle()
			{
			}

			elements::GuiBoundsComposition* CommonTrackStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* CommonTrackStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void CommonTrackStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void CommonTrackStyle::SetText(const WString& value)
			{
			}

			void CommonTrackStyle::SetFont(const FontProperties& value)
			{
			}

			void CommonTrackStyle::SetVisuallyEnabled(bool value)
			{
			}

			void CommonTrackStyle::SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)
			{
				commandExecutor=value;
				if(value)
				{
					value->SetPageSize(0);
				}
			}

			void CommonTrackStyle::SetTotalSize(int value)
			{
				if(totalSize!=value)
				{
					totalSize=value;
					UpdateHandle();
				}
			}

			void CommonTrackStyle::SetPageSize(int value)
			{
				if(pageSize!=value)
				{
					pageSize=value;
					UpdateHandle();
				}
			}

			void CommonTrackStyle::SetPosition(int value)
			{
				if(position!=value)
				{
					position=value;
					UpdateHandle();
				}
			}
		}
	}
}
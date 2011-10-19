/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES
#define VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace common_styles
		{

/***********************************************************************
Scrolls
***********************************************************************/

			class CommonScrollStyle : public Object, public controls::GuiScroll::IStyleController
			{
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				Direction											direction;
				controls::GuiScroll::ICommandExecutor*				commandExecutor;
				controls::GuiButton*								decreaseButton;
				controls::GuiButton*								increaseButton;
				controls::GuiButton*								handleButton;
				elements::GuiPartialViewComposition*				handleComposition;
				elements::GuiBoundsComposition*						boundsComposition;

				int													totalSize;
				int													pageSize;
				int													position;
				Point												draggingStartLocation;
				bool												draggingHandle;

				void												UpdateHandle();
				void												OnDecreaseButtonClicked(elements::GuiGraphicsComposition* sender,elements::GuiEventArgs& arguments);
				void												OnIncreaseButtonClicked(elements::GuiGraphicsComposition* sender,elements::GuiEventArgs& arguments);
				void												OnHandleMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);
				void												OnHandleMouseMove(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);
				void												OnHandleMouseUp(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);;
				void												OnBigMoveMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);

				virtual controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction)=0;
				virtual controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction)=0;
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				virtual void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				void												BuildStyle(int defaultSize, int arrowSize);
			public:
				CommonScrollStyle(Direction _direction);
				~CommonScrollStyle();

				elements::GuiBoundsComposition*						GetBoundsComposition();
				elements::GuiGraphicsComposition*					GetContainerComposition();
				void												SetText(const WString& value);
				void												SetFont(const FontProperties& value);
				void												SetVisuallyEnabled(bool value);
				void												SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value);
				void												SetTotalSize(int value);
				void												SetPageSize(int value);
				void												SetPosition(int value);
			};

			class CommonTrackStyle : public Object, public controls::GuiScroll::IStyleController
			{
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				Direction											direction;
				controls::GuiScroll::ICommandExecutor*				commandExecutor;
				elements::GuiBoundsComposition*						boundsComposition;
				controls::GuiButton*								handleButton;
				elements::GuiTableComposition*						handleComposition;

				int													totalSize;
				int													pageSize;
				int													position;
				Point												draggingStartLocation;
				bool												draggingHandle;

				void												UpdateHandle();
				void												OnHandleMouseDown(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);
				void												OnHandleMouseMove(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);
				void												OnHandleMouseUp(elements::GuiGraphicsComposition* sender,elements::GuiMouseEventArgs& arguments);
				
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				virtual void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				virtual void										InstallTrack(elements::GuiGraphicsComposition* trackComposition, Direction direction)=0;
				void												BuildStyle(int trackThickness, int trackPadding, int handleLong, int handleShort);
			public:
				CommonTrackStyle(Direction _direction);
				~CommonTrackStyle();

				elements::GuiBoundsComposition*						GetBoundsComposition();
				elements::GuiGraphicsComposition*					GetContainerComposition();
				void												SetText(const WString& value);
				void												SetFont(const FontProperties& value);
				void												SetVisuallyEnabled(bool value);
				void												SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value);
				void												SetTotalSize(int value);
				void												SetPageSize(int value);
				void												SetPosition(int value);
			};
		}
	}
}

#endif
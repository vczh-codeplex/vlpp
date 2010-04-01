/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Layout

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_LAYOUTHOST
#define VCZH_PRESENTATION_LAYOUT_LAYOUTHOST

#include "..\Core\GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		class LayoutHost : public Object
		{
		protected:
			Size					minSize;			// priority 1
			Size					maxSize;			// priority 1
			Margin					margin;				// priority 2
			Rect					relativeBounds;		// priority 2
			Rect					bounds;				// priority 3
			Rect					realBounds;

			void					CalculateBound(int min, int max, int start, int length, int marginStart, int marginEnd, int relativeStart, int relativeEnd, int& resultStart, int& resultEnd);
			void					UpdateRealBounds();
		public:
			LayoutHost();
			~LayoutHost();

			Size					GetMinSize();
			Size					GetMaxSize();
			Margin					GetMargin();
			Point					GetPosition();
			Size					GetSize();
			Rect					GetBounds();
			Rect					GetMarginRelativeBounds();
			Rect					GetRealBounds();

			void					SetMinSize(Size _size);
			void					SetMaxSize(Size _size);
			void					SetMargin(Margin _margin);
			void					SetPosition(Point _position);
			void					SetSize(Size _size);
			void					SetBounds(Rect _bounds);
			void					SetMarginRelativeBounds(Rect _bounds);
		};
	}
}

#endif
/***********************************************************************
Vczh Library++ 3.0
Developer: ≥¬Ë˜Â´(vczh)
GUI::Layout

Classes:
  LayoutHost								£∫≈≈∞Ê∂‘œÛ
  LayoutBase								£∫≈≈∞Êª˘¿‡

  FreeLayout								£∫◊‘”…≈≈∞Ê
  TableLayout								£∫±Ì∏Ò≈≈∞Ê
  DockLayout								£∫Õ£øø≈≈∞Ê
  StackLayout								£∫∂—’ª≈≈∞Ê
  WrapLayout								£∫ªª––≈≈∞Ê
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_LAYOUTHOST
#define VCZH_PRESENTATION_LAYOUT_LAYOUTHOST

#include "..\Core\GuiTypes.h"
#include "..\..\..\..\Library\Collections\List.h"

namespace vl
{
	namespace presentation
	{
		const int LayoutMaxSize=65536;

		class LayoutHost : public Object
		{
		protected:
			Size										minSize;			// priority 1
			Size										maxSize;			// priority 1
			Margin										margin;				// priority 2
			Rect										relativeBounds;		// priority 2
			Rect										bounds;				// priority 3
			Rect										realBounds;

			void										CalculateBound(int min, int max, int start, int length, int marginStart, int marginEnd, int relativeStart, int relativeEnd, int& resultStart, int& resultEnd);
			void										GetBound(int marginStart, int marginEnd, int min, int& bound);
			void										UpdateRealBounds();
		public:
			LayoutHost();
			~LayoutHost();

			Size										GetMinSize();
			Size										GetMaxSize();
			Margin										GetMargin();
			Point										GetPosition();
			Size										GetSize();
			Rect										GetBounds();
			Rect										GetMarginRelativeBounds();

			Rect										GetRealBounds();
			Size										GetMinBounds();
			Size										GetMaxBounds();
			Size										GetSuggestedBounds();

			void										SetMinSize(Size _size);
			void										SetMaxSize(Size _size);
			void										SetMargin(Margin _margin);
			void										SetPosition(Point _position);
			void										SetSize(Size _size);
			void										SetBounds(Rect _bounds);
			void										SetMarginRelativeBounds(Rect _bounds);
		};

		class LayoutBase : public Object
		{
		protected:
			Size										size;
			Size										minSize;
			Size										maxSize;
		public:
			LayoutBase();
			~LayoutBase();

			Size										GetSize();
			void										SetSize(Size _size);
			Size										GetMinSize();
			Size										GetMaxSize();

			virtual void								AdjustHosts()=0;
			virtual void								CalculateMinMax()=0;
			virtual int									GetHostCount()=0;
			virtual LayoutHost*							GetHost(int index)=0;
		};
	}
}

#endif
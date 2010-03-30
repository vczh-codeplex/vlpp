/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Common

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CORE_GUITYPES
#define VCZH_PRESENTATION_CORE_GUITYPES

#include "..\..\..\..\Library\Basic.h"
#include "..\..\..\..\Library\String.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Point
***********************************************************************/

		struct Point
		{
			int			x;
			int			y;

			Point()
				:x(0) ,y(0)
			{
			}

			Point(int _x, int _y)
				:x(_x) ,y(_y)
			{
			}
		};

/***********************************************************************
Size
***********************************************************************/

		struct Size
		{
			int			x;
			int			y;

			Size()
				:x(0) ,y(0)
			{
			}

			Size(int _x, int _y)
				:x(_x) ,y(_y)
			{
			}
		};

/***********************************************************************
Rectangle
***********************************************************************/

		struct Rect
		{
			int		x1, y1, x2, y2;

			Rect()
				:x1(0), y1(0), x2(0), y2(0)
			{
			}

			Rect(int _x1, int _y1, int _x2, int _y2)
				:x1(_x1), y1(_y1), x2(_x2), y2(_y2)
			{
			}

			Rect(Point p, Size s)
				:x1(p.x), y1(p.y), x2(p.x+s.x), y2(p.y+s.y)
			{
			}

			Point LeftTop()const
			{
				return Point(x1, y1);
			}

			Point RightBottom()const
			{
				return Point(x2, y2);
			}

			Size GetSize()const
			{
				return Size(x2-x1, y2-y1);
			}

			int Left()const
			{
				return x1;
			}

			int Right()const
			{
				return x2;
			}

			int Width()const
			{
				return x2-x1;
			}

			int Top()const
			{
				return y1;
			}

			int Bottom()const
			{
				return y2;
			}

			int Height()const
			{
				return y2-y1;
			}

			void Expand(int x, int y)
			{
				x1-=x;
				y1-=y;
				x2+=x;
				y2+=y;
			}

			void Expand(Size s)
			{
				x1-=s.x;
				y1-=s.y;
				x2+=s.x;
				y2+=s.y;
			}

			void Move(int x, int y)
			{
				x1+=x;
				y1+=y;
				x2+=x;
				y2+=y;
			}

			void Move(Size s)
			{
				x1+=s.x;
				y1+=s.y;
				x2+=s.x;
				y2+=s.y;
			}
		};

/***********************************************************************
2D operations
***********************************************************************/

		inline Point operator+(Point p, Size s)
		{
			return Point(p.x+s.x, p.y+s.y);
		}

		inline Point operator+(Size s, Point p)
		{
			return Point(p.x+s.x, p.y+s.y);
		}

		inline Size operator-(Point p1, Point p2)
		{
			return Size(p1.x-p2.x, p1.y-p2.y);
		}

		inline Size operator+(Size s1, Size s2)
		{
			return Size(s1.x+s2.x, s1.y+s2.y);
		}

		inline Size operator-(Size s1, Size s2)
		{
			return Size(s1.x-s2.x, s1.y-s2.y);
		}

/***********************************************************************
Color
***********************************************************************/

		struct Color
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;

			Color()
				:r(0), g(0), b(0), a(255)
			{
			}
		};
	}
}

#endif
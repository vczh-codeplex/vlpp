/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Common Types

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUITYPES
#define VCZH_PRESENTATION_GUITYPES

#include "..\GacVlppReferences.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Enumerations
***********************************************************************/

		namespace Alignment
		{
			enum Type
			{
				Left=0,
				Top=0,
				Center=1,
				Right=2,
				Bottom=2,
			};
		}

/***********************************************************************
TextPos
***********************************************************************/

		struct TextPos
		{
			int			row;
			int			column;

			TextPos()
				:row(0) ,column(0)
			{
			}

			TextPos(int _row, int _column)
				:row(_row) ,column(_column)
			{
			}

			int Compare(const TextPos& value)const
			{
				if(row<value.row) return -1;
				if(row>value.row) return 1;
				if(column<value.column) return -1;
				if(column>value.column) return 1;
				return 0;
			}

			bool operator==(const TextPos& value)const {return Compare(value)==0;}
			bool operator!=(const TextPos& value)const {return Compare(value)!=0;}
			bool operator<(const TextPos& value)const {return Compare(value)<0;}
			bool operator<=(const TextPos& value)const {return Compare(value)<=0;}
			bool operator>(const TextPos& value)const {return Compare(value)>0;}
			bool operator>=(const TextPos& value)const {return Compare(value)>=0;}
		};

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

			bool operator==(Point point)const
			{
				return x==point.x && y==point.y;
			}

			bool operator!=(Point point)const
			{
				return x!=point.x || y!=point.y;
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

			bool operator==(Size size)const
			{
				return x==size.x && y==size.y;
			}

			bool operator!=(Size size)const
			{
				return x!=size.x || y!=size.y;
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

			bool operator==(Rect rect)const
			{
				return x1==rect.x1 && y1==rect.y1 && x2==rect.x2 && y2==rect.y2;
			}

			bool operator!=(Rect rect)const
			{
				return x1!=rect.x1 || y1!=rect.y1 || x2!=rect.x2 || y2!=rect.y2;
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

			bool Contains(Point p)
			{
				return x1<=p.x && p.x<x2 && y1<=p.y && p.y<y2;
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

		inline Point operator-(Point p, Size s)
		{
			return Point(p.x-s.x, p.y-s.y);
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

		inline Size operator*(Size s, int i)
		{
			return Size(s.x*i, s.y*i);
		}

		inline Size operator/(Size s, int i)
		{
			return Size(s.x/i, s.y/i);
		}

		inline Point operator+=(Point& s1, Size s2)
		{
			s1.x+=s2.x;
			s1.y+=s2.y;
			return s1;
		}

		inline Point operator-=(Point& s1, Size s2)
		{
			s1.x-=s2.x;
			s1.y-=s2.y;
			return s1;
		}

		inline Size operator+=(Size& s1, Size s2)
		{
			s1.x+=s2.x;
			s1.y+=s2.y;
			return s1;
		}

		inline Size operator-=(Size& s1, Size s2)
		{
			s1.x-=s2.x;
			s1.y-=s2.y;
			return s1;
		}

/***********************************************************************
Color
***********************************************************************/

		struct Color
		{
			union
			{
				struct
				{
					unsigned char r;
					unsigned char g;
					unsigned char b;
					unsigned char a;
				};
				unsigned __int32 value;
			};

			Color()
				:r(0), g(0), b(0), a(255)
			{
			}

			Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a=255)
				:r(_r), g(_g), b(_b), a(_a)
			{
			}

			int Compare(Color color)const
			{
				return value-color.value;
			}

			bool operator==(Color color)const {return Compare(color)==0;}
			bool operator!=(Color color)const {return Compare(color)!=0;}
			bool operator<(Color color)const {return Compare(color)<0;}
			bool operator<=(Color color)const {return Compare(color)<=0;}
			bool operator>(Color color)const {return Compare(color)>0;}
			bool operator>=(Color color)const {return Compare(color)>=0;}
		};

/***********************************************************************
Margin
***********************************************************************/

		struct Margin
		{
			int		left, top, right, bottom;

			Margin()
				:left(0), top(0), right(0), bottom(0)
			{
			}

			Margin(int _left, int _top, int _right, int _bottom)
				:left(_left), top(_top), right(_right), bottom(_bottom)
			{
			}

			bool operator==(Margin margin)const
			{
				return left==margin.left && top==margin.top && right==margin.right && bottom==margin.bottom;
			}

			bool operator!=(Margin margin)const
			{
				return left!=margin.left || top!=margin.top || right!=margin.right || bottom!=margin.bottom;
			}
		};

/***********************************************************************
Resources
***********************************************************************/

		struct FontProperties
		{
			WString				fontFamily;
			int					size;
			bool				bold;
			bool				italic;
			bool				underline;
			bool				strikeline;
			bool				antialias;

			FontProperties()
				:size(0)
				,bold(false)
				,italic(false)
				,underline(false)
				,strikeline(false)
				,antialias(true)
			{
			}
			
			int Compare(const FontProperties& value)const
			{
				int result=0;
				
				result=WString::Compare(fontFamily, value.fontFamily);
				if(result!=0) return result;

				result=size-value.size;
				if(result!=0) return result;

				result=(int)bold-(int)value.bold;
				if(result!=0) return result;

				result=(int)italic-(int)value.italic;
				if(result!=0) return result;

				result=(int)underline-(int)value.underline;
				if(result!=0) return result;

				result=(int)strikeline-(int)value.strikeline;
				if(result!=0) return result;

				result=(int)antialias-(int)value.antialias;
				if(result!=0) return result;

				return 0;
			}

			bool operator==(const FontProperties& value)const {return Compare(value)==0;}
			bool operator!=(const FontProperties& value)const {return Compare(value)!=0;}
			bool operator<(const FontProperties& value)const {return Compare(value)<0;}
			bool operator<=(const FontProperties& value)const {return Compare(value)<=0;}
			bool operator>(const FontProperties& value)const {return Compare(value)>0;}
			bool operator>=(const FontProperties& value)const {return Compare(value)>=0;}
		};
	}
}

#endif
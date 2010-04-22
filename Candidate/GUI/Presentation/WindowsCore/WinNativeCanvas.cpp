#include "WinNativeCanvas.h"
#include "WinGDI.h"

#undef CreateFont

using namespace vl::windows;

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
GdiPen
***********************************************************************/

			class GdiPen : public Object, public INativePen
			{
			public:
				Ptr<WinPen>					pen;
				Color						color;

				GdiPen(Color _color)
					:color(_color)
				{
					pen=new WinPen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
				}

				Color GetColor()
				{
					return color;
				}
			};

/***********************************************************************
GdiBrush
***********************************************************************/

			class GdiBrush : public Object, public INativeBrush
			{
			public:
				Ptr<WinPen>					pen;
				Ptr<WinBrush>				brush;
				Color						color;

				GdiBrush(Color _color)
					:color(_color)
				{
					pen=new WinPen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
					brush=new WinBrush(RGB(color.r, color.g, color.b));
				}

				Color GetColor()
				{
					return color;
				}
			};

/***********************************************************************
GdiFont
***********************************************************************/

			class GdiFont : public Object, public INativeFont
			{
			public:
				Ptr<WinFont>				font;
				WString						name;
				int							size;
				bool						bold, italic, underline;

				GdiFont(const WString& _name, int _size, bool _bold, bool _italic, bool _underline)
					:name(_name)
					,size(_size)
					,bold(_bold)
					,italic(_italic)
					,underline(_underline)
				{
					font=new WinFont(name, size, 0, 0, 0, bold?400:700, italic, underline, false, true);
				}

				WString GetName()
				{
					return name;
				}

				bool GetBold()
				{
					return bold;
				}

				bool GetItalic()
				{
					return italic;
				}

				bool GetUnderline()
				{
					return underline;
				}

				int GetSize()
				{
					return size;
				}
			};

/***********************************************************************
GdiString
***********************************************************************/

			class GdiString : public Object, public INativeString
			{
			public:
				GdiFont						font;
				GdiBrush					brush;
				WString						string;

				GdiString(const WString& _string, GdiFont* _font, GdiBrush* _brush)
					:string(_string)
					,font(_font->name, _font->size, _font->bold, _font->italic, _font->underline)
					,brush(_brush->color)
				{
				}

				INativeFont* GetFont()
				{
					return &font;
				}

				INativeBrush* GetBrush()
				{
					return &brush;
				}

				const WString& GetString()
				{
					return string;
				}
			};

/***********************************************************************
GdiBitmapBase
***********************************************************************/

			class GdiBitmapBase : public Object, public INativeBitmap
			{
			public:
				Ptr<WinBitmap>				bitmap;

				GdiBitmapBase(const WString& path)
				{
					bitmap=new WinBitmap(path, true, true);
				}

				GdiBitmapBase(int width, int height)
				{
					bitmap=new WinBitmap(width, height, WinBitmap::vbb32Bits, true);
				}

				int GetWidth()
				{
					return bitmap->GetWidth();
				}

				int GetHeight()
				{
					return bitmap->GetHeight();
				}
			};

/***********************************************************************
GdiCanvas
***********************************************************************/

			class GdiCanvas : public Object, public INativeCanvas, public IGdiCanvas
			{
			public:
				WinDC*						dc;
				Ptr<WinBrush>				emptyBrush;

				GdiCanvas()
					:dc(0)
				{
					emptyBrush=new WinBrush();
				}

				void Initialize()
				{
					dc->SetBackTransparent(true);
				}

				HDC GetDeviceContext()
				{
					return dc->GetHandle();
				}

				void DrawLine(Point a, Point b, INativePen* pen)
				{
					GdiPen* gdiPen=dynamic_cast<GdiPen*>(pen);
					if(gdiPen)
					{
						dc->SetPen(gdiPen->pen);
						dc->MoveTo(a.x, a.y);
						dc->LineTo(b.x, b.y);
					}
				}

				void DrawRectangle(Rect bounds, INativePen* pen, INativeBrush* brush)
				{
					GdiPen* gdiPen=dynamic_cast<GdiPen*>(pen);
					GdiBrush* gdiBrush=dynamic_cast<GdiBrush*>(brush);
					if(gdiPen && gdiBrush)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(gdiBrush->brush);
						dc->Rectangle(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
					else if(gdiPen)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(emptyBrush);
						dc->Rectangle(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
					else if(gdiBrush)
					{
						dc->SetBrush(gdiBrush->brush);
						dc->FillRect(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
				}

				void DrawEllipse(Rect bounds, INativePen* pen, INativeBrush* brush)
				{
					GdiPen* gdiPen=dynamic_cast<GdiPen*>(pen);
					GdiBrush* gdiBrush=dynamic_cast<GdiBrush*>(brush);
					if(gdiPen && gdiBrush)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(gdiBrush->brush);
						dc->Ellipse(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
					else if(gdiPen)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(emptyBrush);
						dc->Ellipse(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
					else if(gdiBrush)
					{
						dc->SetPen(gdiBrush->pen);
						dc->SetBrush(gdiBrush->brush);
						dc->Ellipse(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1);
					}
				}

				void DrawRoundRectangle(Rect bounds, Size ellipseSize, INativePen* pen, INativeBrush* brush)
				{
					GdiPen* gdiPen=dynamic_cast<GdiPen*>(pen);
					GdiBrush* gdiBrush=dynamic_cast<GdiBrush*>(brush);
					if(gdiPen && gdiBrush)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(gdiBrush->brush);
						dc->RoundRect(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1, ellipseSize.x, ellipseSize.y);
					}
					else if(gdiPen)
					{
						dc->SetPen(gdiPen->pen);
						dc->SetBrush(emptyBrush);
						dc->RoundRect(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1, ellipseSize.x, ellipseSize.y);
					}
					else if(gdiBrush)
					{
						dc->SetPen(gdiBrush->pen);
						dc->SetBrush(gdiBrush->brush);
						dc->RoundRect(bounds.x1, bounds.y1, bounds.x2-1, bounds.y2-1, ellipseSize.x, ellipseSize.y);
					}
				}

				void DrawString(const WString& string, Point position, INativeFont* font, INativeBrush* brush)
				{
					GdiFont* gdiFont=dynamic_cast<GdiFont*>(font);
					GdiBrush* gdiBrush=dynamic_cast<GdiBrush*>(brush);
					if(gdiFont && gdiBrush)
					{
						dc->SetFont(gdiFont->font);
						dc->SetTextColor(RGB(gdiBrush->color.r, gdiBrush->color.g, gdiBrush->color.b));
						dc->DrawString(position.x, position.y, string);
					}
				}

				void DrawString(INativeString* string, Point position)
				{
					GdiString* gdiString=dynamic_cast<GdiString*>(string);
					if(gdiString)
					{
						dc->SetFont(gdiString->font.font);
						dc->SetTextColor(RGB(gdiString->brush.color.r, gdiString->brush.color.g, gdiString->brush.color.b));
						dc->DrawString(position.x, position.y, gdiString->string);
					}
				}

				void DrawBitmap(INativeBitmap* bitmap, Rect dstBounds, Rect srcBounds)
				{
					GdiBitmapBase* gdiBitmap=dynamic_cast<GdiBitmapBase*>(bitmap);
					if(gdiBitmap)
					{
						RECT dst={dstBounds.x1, dstBounds.y1, dstBounds.x2, dstBounds.y2};
						RECT src={srcBounds.x1, srcBounds.y1, srcBounds.x2, srcBounds.y2};
						dc->Draw(dst, gdiBitmap->bitmap, src);
					}
				}

				Size MeasureString(const WString& string, INativeFont* font)
				{
					GdiFont* gdiFont=dynamic_cast<GdiFont*>(font);
					if(gdiFont)
					{
						dc->SetFont(gdiFont->font);
						SIZE size=dc->MeasureString(string);
						return Size(size.cx, size.cy);
					}
					else
					{
						return Size();
					}
				}

				Size MeasureString(INativeString* string)
				{
					GdiString* gdiString=dynamic_cast<GdiString*>(string);
					if(gdiString)
					{
						dc->SetFont(gdiString->font.font);
						SIZE size=dc->MeasureString(gdiString->string);
						return Size(size.cx, size.cy);
					}
					else
					{
						return Size();
					}
				}
			};

/***********************************************************************
GdiBitmap
***********************************************************************/

			class GdiBitmap : public GdiBitmapBase
			{
			public:
				GdiCanvas					canvas;

				GdiBitmap(const WString& path)
					:GdiBitmapBase(path)
				{
					canvas.dc=bitmap->GetWinDC();
				}

				GdiBitmap(int width, int height)
					:GdiBitmapBase(width, height)
				{
					canvas.dc=bitmap->GetWinDC();
				}

				INativeCanvas* Lock()
				{
					return &canvas;
				}

				void Unlock(INativeCanvas* canvas)
				{
				}
			};

/***********************************************************************
GdiGraphics
***********************************************************************/

			class GdiGraphics : public Object, public INativeGraphics
			{
			public:
				INativePen* CreatePen(Color color)
				{
					return new GdiPen(color);
				}

				INativeBrush* CreateBrush(Color color)
				{
					return new GdiBrush(color);
				}

				INativeFont* CreateFont(const WString& name, int size, bool bold, bool italic, bool underline)
				{
					return new GdiFont(name, size, bold, italic, underline);
				}

				INativeString* CreateString(const WString& string, INativeFont* font, INativeBrush* brush)
				{
					GdiFont* gdiFont=dynamic_cast<GdiFont*>(font);
					GdiBrush* gdiBrush=dynamic_cast<GdiBrush*>(brush);
					return new GdiString(string, gdiFont, gdiBrush);
				}

				INativeBitmap* CreateBitmap(const WString& path)
				{
					return new GdiBitmap(path);
				}

				INativeBitmap* CreateBitmap(int width, int height)
				{
					return new GdiBitmap(width, height);
				}
				
				void Destroy(INativePen* pen)
				{
					delete pen;
				}

				void Destroy(INativeBrush* brush)
				{
					delete brush;
				}

				void Destroy(INativeFont* font)
				{
					delete font;
				}

				void Destroy(INativeString* string)
				{
					delete string;
				}

				void Destroy(INativeBitmap* bitmap)
				{
					delete bitmap;
				}
			};

/***********************************************************************
Windows Platform GDI Graphics
***********************************************************************/

			INativeGraphics* CreateWindowsGdiGraphics()
			{
				return new GdiGraphics();
			}

			IGdiCanvas* GetGdiCanvas(INativeCanvas* canvas)
			{
				return dynamic_cast<GdiCanvas*>(canvas);
			}

			void DestroyWindowsGdiGraphics(INativeGraphics* graphics)
			{
				delete graphics;
			}
		}
	}
}
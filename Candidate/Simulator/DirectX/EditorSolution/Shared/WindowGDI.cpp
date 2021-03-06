#include <math.h>
#include "WindowGDI.h"
#include "..\..\..\..\..\Library\Stream\FileStream.h"
using namespace vl::stream;

#pragma comment(lib, "Msimg32.lib")

namespace vl
{
	namespace directx
	{
/*********************************************************************************************************
WinRegion
*********************************************************************************************************/

		bool IsEqual(WinRegion::Ptr Region1, WinRegion::Ptr Region2)
		{
			return EqualRgn(Region1->GetHandle(), Region2->GetHandle())!=0;
		}

		WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, bool Rectangle)
		{
			if(Rectangle)
			{
				FHandle=CreateRectRgn(Left, Top, Right, Bottom);
			}
			else
			{
				FHandle=CreateEllipticRgn(Left, Top, Right, Bottom);
			}
		}

		WinRegion::WinRegion(RECT Rect, bool Rectangle)
		{
			if(Rectangle)
			{
				FHandle=CreateRectRgnIndirect(&Rect);
			}
			else
			{
				FHandle=CreateEllipticRgnIndirect(&Rect);
			}
		}

		WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
		{
			FHandle=CreateRoundRectRgn(Left, Top, Right, Bottom, EllipseWidth, EllipseHeight);
		}

		WinRegion::WinRegion(POINT* Points, int Count, bool Alternate)
		{
			FHandle=CreatePolygonRgn(Points, Count, Alternate?ALTERNATE:WINDING);
		}

		WinRegion::WinRegion(WinRegion::Ptr Region)
		{
			FHandle=CreateRectRgn(0, 0, 1, 1);
			CombineRgn(FHandle, Region->GetHandle(), Region->GetHandle(), RGN_COPY);
		}

		WinRegion::WinRegion(WinRegion::Ptr Region1, WinRegion::Ptr Region2, int CombineMode)
		{
			FHandle=CreateRectRgn(0, 0, 1, 1);
			CombineRgn(FHandle, Region1->GetHandle(), Region2->GetHandle(), CombineMode);
		}

		WinRegion::WinRegion(HRGN RegionHandle)
		{
			FHandle=RegionHandle;
		}

		WinRegion::~WinRegion()
		{
			DeleteObject(FHandle);
		}

		HRGN WinRegion::GetHandle()
		{
			return FHandle;
		}

		bool WinRegion::ContainPoint(POINT Point)
		{
			return PtInRegion(FHandle, Point.x, Point.y)!=0;
		}

		bool WinRegion::ContainRect(RECT Rect)
		{
			return RectInRegion(FHandle, &Rect)!=0;
		}

		RECT WinRegion::GetBoundRect()
		{
			RECT Rect={0, 0, 0, 0};
			GetRgnBox(FHandle, &Rect);
			return Rect;
		}

		void WinRegion::Move(int OffsetX, int OffsetY)
		{
			OffsetRgn(FHandle, OffsetX, OffsetY);
		}

/*********************************************************************************************************
WinTransform
*********************************************************************************************************/
	
		WinTransform::WinTransform(XFORM Transform)
		{
			FTransform=Transform;
		}

		WinTransform::WinTransform(const WinTransform& Transform)
		{
			FTransform=Transform.FTransform;
		}

		WinTransform& WinTransform::operator=(const WinTransform& Transform)
		{
			FTransform=Transform.FTransform;
			return *this;
		}

		WinTransform WinTransform::operator*(const WinTransform& Transform)
		{
			XFORM Result;
			CombineTransform(&Result, GetHandle(), Transform.GetHandle());
			return Result;
		}

		const XFORM* WinTransform::GetHandle()const
		{
			return &FTransform;
		}

		/*------------------------------------------------------------------------------*/

		WinTransform WinTransform::Translate(float OffsetX, float OffsetY)
		{
			XFORM Transform;
			Transform.eM11=1.0f;
			Transform.eM12=0.0f;
			Transform.eM21=0.0f;
			Transform.eM22=1.0f;
			Transform.eDx=OffsetX;
			Transform.eDy=OffsetY;
			return Transform;
		}

		WinTransform WinTransform::Scale(float ScaleX, float ScaleY)
		{
			XFORM Transform;
			Transform.eM11=ScaleX;
			Transform.eM12=0.0f;
			Transform.eM21=0.0f;
			Transform.eM22=ScaleY;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::Rotate(float Angle)
		{
			XFORM Transform;
			Transform.eM11=cos(Angle);
			Transform.eM12=sin(Angle);
			Transform.eM21=-sin(Angle);
			Transform.eM22=cos(Angle);
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::Rotate(float Cos, float Sin)
		{
			XFORM Transform;
			Transform.eM11=Cos;
			Transform.eM12=Sin;
			Transform.eM21=-Sin;
			Transform.eM22=Cos;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::ReflectX()
		{
			XFORM Transform;
			Transform.eM11=1.0f;
			Transform.eM12=0.0f;
			Transform.eM21=0.0f;
			Transform.eM22=-1.0f;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::ReflectY()
		{
			XFORM Transform;
			Transform.eM11=-1.0f;
			Transform.eM12=0.0f;
			Transform.eM21=0.0f;
			Transform.eM22=1.0f;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::Reflect(float VectorX, float VectorY)
		{
			float Len=sqrt(VectorX*VectorX+VectorY*VectorY);
			float Cos=VectorX/Len;
			float Sin=VectorY/Len;

			return Rotate(Cos, -Sin)*ReflectX()*Rotate(Cos, Sin);
		}

		WinTransform WinTransform::Reflect(float OriginX, float OriginY, float VectorX, float VectorY)
		{
			float Len=sqrt(VectorX*VectorX+VectorY*VectorY);
			float Cos=VectorX/Len;
			float Sin=VectorY/Len;

			return Translate(-OriginX, -OriginY)*Rotate(Cos, -Sin)*ReflectX()*Rotate(Cos, Sin)*Translate(OriginX, OriginY);
		}

		WinTransform WinTransform::AxisV(float Xx, float Xy, float Yx, float Yy)
		{
			XFORM Transform;
			Transform.eM11=Xx;
			Transform.eM12=Xy;
			Transform.eM21=Yx;
			Transform.eM22=Yy;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

		WinTransform WinTransform::AxisA(float AngleX, float LenX, float AngleY, float LenY)
		{
			XFORM Transform;
			Transform.eM11=cos(AngleX)*LenX;
			Transform.eM12=sin(AngleX)*LenX;
			Transform.eM21=cos(AngleY)*LenY;
			Transform.eM22=sin(AngleY)*LenY;
			Transform.eDx=0.0f;
			Transform.eDy=0.0f;
			return Transform;
		}

/*********************************************************************************************************
WinMetaFileBuilder
*********************************************************************************************************/

		void WinMetaFileBuilder::Create(int Width, int Height)
		{
			HDC hdcRef=GetDC(NULL);
			int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE); 
			int iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE); 
			int iWidthPels = GetDeviceCaps(hdcRef, HORZRES); 
			int iHeightPels = GetDeviceCaps(hdcRef, VERTRES); 
			ReleaseDC(NULL, hdcRef);

			RECT Rect;
			Rect.left=0;
			Rect.top=0;
			Rect.right = (Width*iWidthMM*100)/iWidthPels;
			Rect.bottom = (Height*iHeightMM*100)/iHeightPels;

			HDC Handle=CreateEnhMetaFile(NULL, NULL, &Rect, L"VczhLibrary++GDI\0Enhanced Meta File\0");
			FDC->Initialize(Handle);

			FWidth=Width;
			FHeight=Height;
		}

		void WinMetaFileBuilder::Draw(HENHMETAFILE Handle)
		{
			RECT Rect;
			Rect.left=0;
			Rect.top=0;
			Rect.right=FWidth;
			Rect.bottom=FHeight;
			PlayEnhMetaFile(FDC->GetHandle(), Handle, &Rect);
		}

		void WinMetaFileBuilder::Destroy()
		{
			DeleteEnhMetaFile(CloseEnhMetaFile(FDC->GetHandle()));
		}

		WinMetaFileBuilder::WinMetaFileBuilder(int Width, int Height)
		{
			FDC=new WinProxyDC();
			Create(Width, Height);
		}

		WinMetaFileBuilder::~WinMetaFileBuilder()
		{
			Destroy();
			delete FDC;
		}

		void WinMetaFileBuilder::LoadFrom(WinMetaFile* File)
		{
			Destroy();
			Create(File->GetWidth(), File->GetHeight());
			Draw(File->GetHandle());
		}

		void WinMetaFileBuilder::SaveTo(WinMetaFile* File)
		{
			HENHMETAFILE Handle=CloseEnhMetaFile(FDC->GetHandle());
			if(File->FHandle)
			{
				DeleteEnhMetaFile(File->FHandle);
			}
			File->FHandle=Handle;
			File->FWidth=FWidth;
			File->FHeight=FHeight;
			Create(FWidth, FHeight);
			Draw(Handle);
		}

		void WinMetaFileBuilder::LoadFrom(WString FileName)
		{
			WinMetaFile File(FileName);
			Destroy();
			Create(File.GetWidth(), File.GetHeight());
			Draw(File.GetHandle());
		}

		void WinMetaFileBuilder::SaveTo(WString FileName)
		{
			HENHMETAFILE Handle=CloseEnhMetaFile(FDC->GetHandle());
			HENHMETAFILE NewHandle=CopyEnhMetaFile(Handle, FileName.Buffer());
			DeleteEnhMetaFile(NewHandle);
			Create(FWidth, FHeight);
			Draw(Handle);
			DeleteEnhMetaFile(Handle);
		}

		WinDC* WinMetaFileBuilder::GetWinDC()
		{
			return FDC;
		}

		int WinMetaFileBuilder::GetWidth()
		{
			return FWidth;
		}

		int WinMetaFileBuilder::GetHeight()
		{
			return FHeight;
		}

/*********************************************************************************************************
WinMetaFile
*********************************************************************************************************/

		WinMetaFile::WinMetaFile(WString FileName)
		{
			FHandle=GetEnhMetaFile(FileName.Buffer());
			ENHMETAHEADER Header;
			GetEnhMetaFileHeader(FHandle, sizeof(Header), &Header);
			FWidth=(Header.rclFrame.right-Header.rclFrame.left)*Header.szlDevice.cx/(Header.szlMillimeters.cx*100);
			FHeight=(Header.rclFrame.bottom-Header.rclFrame.top)*Header.szlDevice.cy/(Header.szlMillimeters.cy*100);
		}

		WinMetaFile::WinMetaFile(WinMetaFileBuilder* Builder)
		{
			FHandle=NULL;
			Builder->SaveTo(this);
		}

		WinMetaFile::~WinMetaFile()
		{
			DeleteEnhMetaFile(FHandle);
		}

		HENHMETAFILE WinMetaFile::GetHandle()
		{
			return FHandle;
		}

		int WinMetaFile::GetWidth()
		{
			return FWidth;
		}

		int WinMetaFile::GetHeight()
		{
			return FHeight;
		}

/*********************************************************************************************************
WinBitmap
*********************************************************************************************************/

		int WinBitmap::GetBitsFromBB(BitmapBits BB)
		{
			switch(BB)
			{
			case vbb32Bits:
				return 32;
			case vbb24Bits:
				return 24;
			default:
				return 1;
			}
		}

		int WinBitmap::GetLineBytes(int Width, BitmapBits BB)
		{
			int Bits=GetBitsFromBB(BB);
			int LineBits=Width*Bits;
			int AlignBits=sizeof(DWORD)*8;
			LineBits+=(AlignBits-LineBits%AlignBits)%AlignBits;
			return LineBits/8;
		}

		void WinBitmap::FillBitmapInfoHeader(int Width, int Height, BitmapBits Bits, BITMAPINFOHEADER* Header)
		{
			Header->biSize=sizeof(BITMAPINFOHEADER);
			Header->biWidth=Width;
			Header->biHeight=-Height;
			Header->biPlanes=1;
			Header->biBitCount=GetBitsFromBB(Bits);
			Header->biCompression=BI_RGB;
			Header->biSizeImage=0;
			Header->biXPelsPerMeter=0;
			Header->biYPelsPerMeter=0;
			Header->biClrUsed=(Bits==vbb2Bits?2:0);
			Header->biClrImportant=0;
		}

		HBITMAP WinBitmap::CreateDDB(int Width, int Height, BitmapBits Bits)
		{
			if(Bits==vbb2Bits)
			{
				return CreateBitmap(Width, Height, 2, GetBitsFromBB(Bits), NULL);
			}
			else
			{
				WinBitmap Bitmap(1, 1, Bits, true);
				return CreateCompatibleBitmap(Bitmap.GetWinDC()->GetHandle(), Width, Height);
			}
		}

		HBITMAP WinBitmap::CreateDIB(int Width, int Height, BitmapBits Bits, BYTE**& ScanLines)
		{
			BITMAPINFO* Info=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+2*sizeof(RGBQUAD));
			FillBitmapInfoHeader(Width, Height, Bits, &Info->bmiHeader);
			Info->bmiColors[0].rgbBlue=0;
			Info->bmiColors[0].rgbGreen=0;
			Info->bmiColors[0].rgbRed=0;
			Info->bmiColors[0].rgbReserved=0;
			Info->bmiColors[1].rgbBlue=255;
			Info->bmiColors[1].rgbGreen=255;
			Info->bmiColors[1].rgbRed=255;
			Info->bmiColors[1].rgbReserved=255;

			BYTE* FirstLine=0;
			HBITMAP Handle=CreateDIBSection(FDC->GetHandle(), Info, DIB_RGB_COLORS, (void**)&FirstLine, NULL, 0);
			ScanLines=new BYTE*[Height];
			int LineBytes=GetLineBytes(Width, Bits);
			for(int i=0;i<Height;i++)
			{
				ScanLines[i]=FirstLine+LineBytes*i;
			}
			free(Info);
			return Handle;
		}

		void WinBitmap::Constructor(int Width, int Height, BitmapBits Bits, bool DIBSections)
		{
			FDC=new WinImageDC();
			if(DIBSections)
			{
				FHandle=CreateDIB(Width, Height, Bits, FScanLines);
			}
			else
			{
				FHandle=CreateDDB(Width, Height, Bits);
				FScanLines=0;
			}
			FWidth=Width;
			FHeight=Height;
			FBits=Bits;
			FAlphaChannelBuilt=false;
			HGDIOBJ Object=SelectObject(FDC->GetHandle(), FHandle);
			if(Object)
			{
				DeleteObject(Object);
			}
		}
	
		WinBitmap::WinBitmap(int Width, int Height, BitmapBits Bits, bool DIBSections)
		{
			Constructor(Width, Height, Bits, DIBSections);
		}

		WinBitmap::WinBitmap(WString FileName, bool Use32Bits, bool DIBSections)
		{
			FBits=Use32Bits?vbb32Bits:vbb24Bits;
		
			HBITMAP	TempBmp=(HBITMAP)LoadImage(NULL, FileName.Buffer(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			HDC		TempDC=CreateCompatibleDC(NULL);
			BITMAP	TempRec;

			GetObject(TempBmp, sizeof(BITMAP), &TempRec);
			DeleteObject(SelectObject(TempDC, TempBmp));

			Constructor(TempRec.bmWidth, TempRec.bmHeight, FBits, DIBSections);
			BitBlt(FDC->GetHandle(), 0, 0, TempRec.bmWidth, TempRec.bmHeight, TempDC, 0, 0, SRCCOPY);

			DeleteObject(TempDC);
			DeleteObject(TempBmp);
		}

		WinBitmap::~WinBitmap()
		{
			if(FScanLines)
			{
				delete[] FScanLines;
			}
			if(FHandle)
			{
				DeleteObject(FHandle);
			}
			delete FDC;
		}

		void WinBitmap::SaveToFile(WString FileName)
		{
			if(FScanLines)
			{
				BITMAPFILEHEADER Header1;
				BITMAPV5HEADER Header2;
				{
					Header1.bfType='M'*256+'B';
					Header1.bfSize=sizeof(Header1)+sizeof(Header2)+GetLineBytes()*FHeight;
					Header1.bfReserved1=0;
					Header1.bfReserved2=0;
					Header1.bfOffBits=sizeof(Header2)+sizeof(Header1);
				}
				{
					memset(&Header2, 0, sizeof(Header2));
					Header2.bV5Size=sizeof(Header2);
					Header2.bV5Width=FWidth;
					Header2.bV5Height=-FHeight;
					Header2.bV5Planes=1;
					Header2.bV5BitCount=GetBitsFromBB(FBits);
					Header2.bV5Compression=BI_RGB;
					Header2.bV5CSType=LCS_sRGB;
					Header2.bV5Intent=LCS_GM_GRAPHICS;
				}
				FileStream Output(FileName, FileStream::WriteOnly);
				Output.Write(&Header1, sizeof(Header1));
				Output.Write(&Header2, sizeof(Header2));
				for(int i=0;i<FHeight;i++)
				{
					Output.Write(FScanLines[i], GetLineBytes());
				}
			}
			else
			{
				WinBitmap Temp(FWidth, FHeight, FBits, true);
				Temp.GetWinDC()->Copy(0, 0, FWidth, FHeight, FDC, 0, 0);
				Temp.SaveToFile(FileName);
			}
		}

		WinDC* WinBitmap::GetWinDC()
		{
			return FDC;
		}

		int WinBitmap::GetWidth()
		{
			return FWidth;
		}

		int WinBitmap::GetHeight()
		{
			return FHeight;
		}

		int WinBitmap::GetLineBytes()
		{
			return GetLineBytes(FWidth, FBits);
		}

		BYTE** WinBitmap::GetScanLines()
		{
			return FScanLines;
		}

		HBITMAP WinBitmap::GetBitmap()
		{
			return FHandle;
		}

		WinBitmap::BitmapBits WinBitmap::GetBitmapBits()
		{
			return FBits;
		}
	
		void WinBitmap::FillCompatibleHeader(BITMAPINFOHEADER* Header)
		{
			FillBitmapInfoHeader(FWidth, FHeight, FBits, Header);
		}

		bool WinBitmap::CanBuildAlphaChannel()
		{
			return FScanLines!=0 && FBits==vbb32Bits;
		}

		bool WinBitmap::IsAlphaChannelBuilt()
		{
			return FAlphaChannelBuilt;
		}

		void WinBitmap::BuildAlphaChannel()
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				FAlphaChannelBuilt=true;
				for(int i=0;i<FHeight;i++)
				{
					BYTE* Colors=FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						BYTE Alpha=Colors[3];
						Colors[0]=Colors[0]*Alpha/255;
						Colors[1]=Colors[1]*Alpha/255;
						Colors[2]=Colors[2]*Alpha/255;
						Colors+=4;
					}
				}
			}
		}

		void WinBitmap::GenerateTrans(COLORREF Color)
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				for(int i=0;i<FHeight;i++)
				{
					COLORREF* Colors=(COLORREF*)FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						COLORREF Dest=*Colors & 0x00FFFFFF;
						*Colors = Dest | (0xFF000000 * (Dest!=Color));
						Colors++;
					}
				}
			}
		}

		void WinBitmap::GenerateAlpha(BYTE Alpha)
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				for(int i=0;i<FHeight;i++)
				{
					BYTE* Colors=FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						Colors[3]=Alpha;
						Colors+=4;
					}
				}
			}
		}

		void WinBitmap::GenerateTransAlpha(COLORREF Color, BYTE Alpha)
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				COLORREF A=Alpha<<24;
				for(int i=0;i<FHeight;i++)
				{
					COLORREF* Colors=(COLORREF*)FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						COLORREF Dest=*Colors & 0x00FFFFFF;
						*Colors = Dest | (A * (Dest!=Color));
						Colors++;
					}
				}
			}
		}

		void WinBitmap::GenerateLuminance()
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				for(int i=0;i<FHeight;i++)
				{
					COLORREF* Colors=(COLORREF*)FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						COLORREF Dest=*Colors & 0x00FFFFFF;
						*Colors = Dest | ((GetRValue(Dest)*77 + GetGValue(Dest)*151 + GetBValue(Dest)*28) & 0x0000FF00)<<16;
						Colors++;
					}
				}
			}
		}

		void WinBitmap::GenerateGrayLevel()
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				for(int i=0;i<FHeight;i++)
				{
					COLORREF* Colors=(COLORREF*)FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						COLORREF Dest=*Colors & 0x00FFFFFF;
						*Colors = Dest | ((GetRValue(Dest)+GetGValue(Dest)+GetBValue(Dest))/3)<<24;
						Colors++;
					}
				}
			}
		}

		void WinBitmap::Generate(BYTE(*Function)(COLORREF))
		{
			if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
			{
				for(int i=0;i<FHeight;i++)
				{
					COLORREF* Colors=(COLORREF*)FScanLines[i];
					int j=FWidth;
					while(j--)
					{
						COLORREF Dest= *Colors & 0x00FFFFFF;
						*Colors = Dest | Function(Dest)<<24;
						Colors++;
					}
				}
			}
		}
			
/*********************************************************************************************************
WinBrush
*********************************************************************************************************/

		WinBrush::WinBrush()
		{
			FDIBMemory=0;
			LOGBRUSH lb;
			lb.lbColor=0;
			lb.lbStyle=BS_NULL;
			lb.lbHatch=0;
			FHandle=CreateBrushIndirect(&lb);
		}

		WinBrush::WinBrush(COLORREF Color)
		{
			FDIBMemory=0;
			FHandle=CreateSolidBrush(Color);
		}

		WinBrush::WinBrush(int Hatch, COLORREF Color)
		{
			FDIBMemory=0;
			FHandle=CreateHatchBrush(Hatch, Color);
		}

		WinBrush::WinBrush(WinBitmap::Ptr DIB)
		{
			WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::vbb24Bits, true);
			Temp.GetWinDC()->Draw(0, 0, DIB);
			int HeaderSize=sizeof(BITMAPINFOHEADER);
			FDIBMemory=new unsigned char[HeaderSize+Temp.GetHeight()*Temp.GetLineBytes()];
			Temp.FillCompatibleHeader((BITMAPINFOHEADER*)FDIBMemory);
			memcpy(FDIBMemory+HeaderSize, Temp.GetScanLines()[0], Temp.GetHeight()*Temp.GetLineBytes());

			FHandle=CreateDIBPatternBrushPt(FDIBMemory, DIB_RGB_COLORS);
			DWORD Error=GetLastError();
		}

		WinBrush::~WinBrush()
		{
			DeleteObject(FHandle);
			if(FDIBMemory)
			{
				delete[] FDIBMemory;
			}
		}

		HBRUSH WinBrush::GetHandle()
		{
			return FHandle;
		}

/*********************************************************************************************************
WinPen
*********************************************************************************************************/

		WinPen::WinPen(int Style, int Width, COLORREF Color)
		{
			FDIBMemory=0;
			FHandle=CreatePen(Style, Width, Color);
		}

		WinPen::WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color)
		{
			FDIBMemory=0;
			LOGBRUSH Brush;
			Brush.lbColor=Color;
			Brush.lbStyle=BS_SOLID;
			Brush.lbHatch=0;
			FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join, Width, &Brush, 0, 0);
		}

		WinPen::WinPen(int Style, int EndCap, int Join, int Hatch, int Width, COLORREF Color)
		{
			FDIBMemory=0;
			LOGBRUSH Brush;
			Brush.lbColor=Color;
			Brush.lbStyle=BS_HATCHED;
			Brush.lbHatch=Hatch;
			FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join, Width, &Brush, 0, 0);
		}

		WinPen::WinPen(WinBitmap::Ptr DIB, int Style, int EndCap, int Join, int Width)
		{
			WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::vbb24Bits, true);
			Temp.GetWinDC()->Draw(0, 0, DIB);
			int HeaderSize=sizeof(BITMAPINFOHEADER);
			FDIBMemory=new unsigned char[HeaderSize+Temp.GetHeight()*Temp.GetLineBytes()];
			Temp.FillCompatibleHeader((BITMAPINFOHEADER*)FDIBMemory);
			memcpy(FDIBMemory+HeaderSize, Temp.GetScanLines()[0], Temp.GetHeight()*Temp.GetLineBytes());
		
			LOGBRUSH Brush;
			Brush.lbColor=RGB(0, 0, 0);
			Brush.lbStyle=BS_DIBPATTERNPT;
			Brush.lbHatch=(LONG)FDIBMemory;
			FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join, Width, &Brush, 0, 0);
		}

		WinPen::~WinPen()
		{
			DeleteObject(FHandle);
			if(FDIBMemory)
			{
				delete[] FDIBMemory;
			}
		}

		HPEN WinPen::GetHandle()
		{
			return FHandle;
		}

/*********************************************************************************************************
WinFont
*********************************************************************************************************/

		WinFont::WinFont(WString Name, int Height, int Width, int Escapement, int Orientation, int Weight, bool Italic, bool Underline, bool StrikeOut, bool Antialise)
		{
			FFontInfo.lfHeight=Height;
			FFontInfo.lfWidth=Width;
			FFontInfo.lfEscapement=Escapement;
			FFontInfo.lfOrientation=Orientation;
			FFontInfo.lfWeight=Weight;
			FFontInfo.lfItalic=Italic?TRUE:FALSE;
			FFontInfo.lfUnderline=Underline?TRUE:FALSE;
			FFontInfo.lfStrikeOut=StrikeOut?TRUE:FALSE;
			FFontInfo.lfCharSet=DEFAULT_CHARSET;
			FFontInfo.lfOutPrecision=OUT_DEFAULT_PRECIS;
			FFontInfo.lfClipPrecision=CLIP_DEFAULT_PRECIS;
			FFontInfo.lfQuality=Antialise?CLEARTYPE_QUALITY:NONANTIALIASED_QUALITY;
			FFontInfo.lfPitchAndFamily=DEFAULT_PITCH | FF_DONTCARE;
			wcsncpy_s(FFontInfo.lfFaceName, sizeof(FFontInfo.lfFaceName)/sizeof(*FFontInfo.lfFaceName), Name.Buffer(), LF_FACESIZE-1);
			FHandle=CreateFontIndirect(&FFontInfo);
		}

		WinFont::WinFont(LOGFONT* FontInfo)
		{
			FFontInfo=*FontInfo;
			FHandle=CreateFontIndirect(&FFontInfo);
		}

		WinFont::~WinFont()
		{
			DeleteObject(FHandle);
		}

		HFONT WinFont::GetHandle()
		{
			return FHandle;
		}

		LOGFONT* WinFont::GetInfo()
		{
			return &FFontInfo;
		}

/*********************************************************************************************************
IWinResourceService
*********************************************************************************************************/

		WinBrush::Ptr CreateDefaultBrush()
		{
			return new WinBrush(RGB(255, 255, 255));
		}

		WinPen::Ptr CreateDefaultPen()
		{
			return new WinPen(PS_SOLID, 0, RGB(0, 0, 0));
		}

		WinFont::Ptr CreateDefaultFont()
		{
			NONCLIENTMETRICS NonClientMetrics;
			NonClientMetrics.cbSize=sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, NonClientMetrics.cbSize, &NonClientMetrics, 0);
			if(!*NonClientMetrics.lfMessageFont.lfFaceName)
			{
				NonClientMetrics.cbSize=sizeof(NONCLIENTMETRICS)-sizeof(NonClientMetrics.iPaddedBorderWidth);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, NonClientMetrics.cbSize, &NonClientMetrics, 0);
			}
			return new WinFont(&NonClientMetrics.lfMessageFont);
		}

		class DefaultResourceService : public Object, public IWinResourceService
		{
		public:
			static IWinResourceService* _DefaultResourceService;

			WinPen::Ptr GetDefaultPen()
			{
				return CreateDefaultPen();
			}

			WinBrush::Ptr GetDefaultBrush()
			{
				return CreateDefaultBrush();
			}

			WinFont::Ptr GetDefaultFont()
			{
				return CreateDefaultFont();
			}
		} _DRS;
		IWinResourceService* DefaultResourceService::_DefaultResourceService=&_DRS;

		IWinResourceService* GetDefaultResourceService()
		{
			return DefaultResourceService::_DefaultResourceService;
		}

		void SetDefaultResourceService(IWinResourceService* Service)
		{
			DefaultResourceService::_DefaultResourceService=Service;
		}

/*********************************************************************************************************
WinDC
*********************************************************************************************************/

		void WinDC::Init()
		{
			FPen=GetDefaultResourceService()->GetDefaultPen();
			FOldPen=(HPEN)SelectObject(FHandle, FPen->GetHandle());

			FBrush=GetDefaultResourceService()->GetDefaultBrush();
			FOldBrush=(HBRUSH)SelectObject(FHandle, FBrush->GetHandle());

			FFont=GetDefaultResourceService()->GetDefaultFont();
			FOldFont=(HFONT)SelectObject(FHandle, FFont->GetHandle());

			SetGraphicsMode(FHandle, GM_ADVANCED);
		}

		WinDC::WinDC()
		{
			FHandle=0;
			FOldPen=0;
			FOldBrush=0;
			FOldFont=0;
		}

		WinDC::~WinDC()
		{
			SelectObject(FHandle, FOldFont);
			SelectObject(FHandle, FOldBrush);
			SelectObject(FHandle, FOldPen);
		}

		HDC WinDC::GetHandle()
		{
			return FHandle;
		}

		WinPen::Ptr WinDC::GetPen()
		{
			return FPen;
		}

		WinBrush::Ptr WinDC::GetBrush()
		{
			return FBrush;
		}

		WinFont::Ptr WinDC::GetFont()
		{
			return FFont;
		}

		void WinDC::SetPen(WinPen::Ptr Pen)
		{
			SelectObject(FHandle, Pen->GetHandle());
			FPen=Pen;
		}

		void WinDC::SetBrush(WinBrush::Ptr Brush)
		{
			SelectObject(FHandle, Brush->GetHandle());
			FBrush=Brush;
		}

		void WinDC::SetFont(WinFont::Ptr Font)
		{
			SelectObject(FHandle, Font->GetHandle());
			FFont=Font;
		}

		COLORREF WinDC::GetBackColor()
		{
			return GetBkColor(FHandle);
		}

		void WinDC::SetBackColor(COLORREF Color)
		{
			SetBkColor(FHandle, Color);
		}

		COLORREF WinDC::GetTextColor()
		{
			return ::GetTextColor(FHandle);
		}

		void WinDC::SetTextColor(COLORREF Color)
		{
			::SetTextColor(FHandle, Color);
		}

		bool WinDC::GetBackTransparent()
		{
			return GetBkMode(FHandle)==TRANSPARENT;
		}

		void WinDC::SetBackTransparent(bool Transparent)
		{
			SetBkMode(FHandle, Transparent?TRANSPARENT:OPAQUE);
		}

		POINT WinDC::GetBrushOrigin()
		{
			POINT Point;
			GetBrushOrgEx(FHandle, &Point);
			return Point;
		}

		void WinDC::SetBrushOrigin(POINT Point)
		{
			SetBrushOrgEx(FHandle, Point.x, Point.y, NULL);
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::DrawBuffer(int X, int Y, const wchar_t* Text, int CharCount)
		{
			TextOut(FHandle, X, Y, Text, CharCount);
		}

		void WinDC::DrawBuffer(int X, int Y, const wchar_t* Text, int CharCount, int TabWidth, int TabOriginX)
		{
			TabbedTextOut(FHandle, X, Y, Text, CharCount, 1, &TabWidth, TabOriginX);
		}

		void WinDC::DrawString(int X, int Y, WString Text)
		{
			TextOut(FHandle, X, Y, Text.Buffer(), Text.Length());
		}

		void WinDC::DrawString(int X, int Y, WString Text, int TabWidth, int TabOriginX)
		{
			TabbedTextOut(FHandle, X, Y, Text.Buffer(), Text.Length(), 1, &TabWidth, TabOriginX);
		}

		SIZE WinDC::MeasureString(WString Text, int TabSize)
		{
			return MeasureBuffer(Text.Buffer(), Text.Length(), TabSize);
		}

		SIZE WinDC::MeasureBuffer(const wchar_t* Text, int CharCount, int TabSize)
		{
			SIZE Size;
			if(TabSize==-1)
			{
				GetTextExtentPoint32(FHandle, Text, CharCount, &Size);
			}
			else
			{
				DWORD Result=GetTabbedTextExtent(FHandle, Text, CharCount, 1, &TabSize);
				Size.cx=LOWORD(Result);
				Size.cy=HIWORD(Result);
			}
			return Size;
		}

		SIZE WinDC::MeasureBuffer(const wchar_t* Text, int TabSize)
		{
			return MeasureBuffer(Text, wcslen(Text), TabSize);
		}

		void WinDC::FillRegion(WinRegion::Ptr Region)
		{
			FillRgn(FHandle, Region->GetHandle(), FBrush->GetHandle());
		}

		void WinDC::FrameRegion(WinRegion::Ptr Region, int BlockWidth, int BlockHeight)
		{
			FrameRgn(FHandle, Region->GetHandle(), FBrush->GetHandle(), BlockWidth, BlockHeight);
		}

		void WinDC::MoveTo(int X, int Y)
		{
			::MoveToEx(FHandle, X, Y, NULL);
		}

		void WinDC::LineTo(int X, int Y)
		{
			::LineTo(FHandle, X, Y);
		}

		void WinDC::Rectangle(int Left, int Top, int Right, int Bottom)
		{
			::Rectangle(FHandle, Left, Top, Right, Bottom);
		}

		void WinDC::Rectangle(RECT Rect)
		{
			::Rectangle(FHandle, Rect.left, Rect.top, Rect.right, Rect.bottom);
		}

		void WinDC::FocusRectangle(int Left, int Top, int Right, int Bottom)
		{
			RECT Rect;
			Rect.left=Left;
			Rect.top=Top;
			Rect.right=Right;
			Rect.bottom=Bottom;
			::DrawFocusRect(FHandle, &Rect);
		}

		void WinDC::FocusRectangle(RECT Rect)
		{
			::DrawFocusRect(FHandle, &Rect);
		}

		void WinDC::FillRect(int Left, int Top, int Right, int Bottom)
		{
			RECT Rect;
			Rect.left=Left;
			Rect.top=Top;
			Rect.right=Right;
			Rect.bottom=Bottom;
			::FillRect(FHandle, &Rect, FBrush->GetHandle());
		}

		void WinDC::FillRect(RECT Rect)
		{
			::FillRect(FHandle, &Rect, FBrush->GetHandle());
		}

		void WinDC::Ellipse(int Left, int Top, int Right, int Bottom)
		{
			::Ellipse(FHandle, Left, Top, Right, Bottom);
		}

		void WinDC::Ellipse(RECT Rect)
		{
			::Ellipse(FHandle, Rect.left, Rect.top, Rect.right, Rect.bottom);
		}

		void WinDC::RoundRect(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
		{
			::RoundRect(FHandle, Left, Top, Right, Bottom, EllipseWidth, EllipseHeight);
		}

		void WinDC::RoundRect(RECT Rect, int EllipseWidth, int EllipseHeight)
		{
			::RoundRect(FHandle, Rect.left, Rect.top, Rect.right, Rect.bottom, EllipseWidth, EllipseHeight);
		}

		void WinDC::PolyLine(const POINT* Points, int Count)
		{
			::Polyline(FHandle, Points, Count);
		}

		void WinDC::PolyLineTo(const POINT* Points, int Count)
		{
			::PolylineTo(FHandle, Points, Count);
		}

		void WinDC::PolyGon(const POINT* Points, int Count)
		{
			::Polygon(FHandle, Points, Count);
		}

		void WinDC::PolyBezier(const POINT* Points, int Count)
		{
			::PolyBezier(FHandle, Points, Count);
		}

		void WinDC::PolyBezierTo(const POINT* Points, int Count)
		{
			::PolyBezierTo(FHandle, Points, Count);
		}

		void WinDC::PolyDraw(const POINT* Points, const BYTE* Actions, int PointCount)
		{
			::PolyDraw(FHandle, Points, Actions, PointCount);
		}

		void WinDC::Arc(RECT Bound, POINT Start, POINT End)
		{
			::Arc(FHandle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
		}

		void WinDC::Arc(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
		{
			::Arc(FHandle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
		}

		void WinDC::ArcTo(RECT Bound, POINT Start, POINT End)
		{
			::ArcTo(FHandle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
		}

		void WinDC::ArcTo(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
		{
			::ArcTo(FHandle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
		}

		void WinDC::AngleArc(int X, int Y, int Radius, float StartAngle, float SweepAngle)
		{
			::AngleArc(FHandle, X, Y, Radius, StartAngle, SweepAngle);
		}

		void WinDC::AngleArc(int X, int Y, int Radius, double StartAngle, double SweepAngle)
		{
			::AngleArc(FHandle, X, Y, Radius, (float)StartAngle, (float)SweepAngle);
		}

		void WinDC::Chord(RECT Bound, POINT Start, POINT End)
		{
			::Chord(FHandle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
		}

		void WinDC::Chord(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
		{
			::Chord(FHandle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
		}

		void WinDC::Pie(RECT Bound, POINT Start, POINT End)
		{
			::Pie(FHandle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
		}

		void WinDC::Pie(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
		{
			::Pie(FHandle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
		}

		void WinDC::GradientTriangle(TRIVERTEX* Vertices, int VerticesCount, GRADIENT_TRIANGLE* Triangles, int TriangleCount)
		{
			GradientFill(FHandle, Vertices, VerticesCount, Triangles, TriangleCount, GRADIENT_FILL_TRIANGLE);
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::BeginPath()
		{
			::BeginPath(FHandle);
		}

		void WinDC::EndPath()
		{
			::EndPath(FHandle);
		}

		void WinDC::ClosePath()
		{
			::CloseFigure(FHandle);
		}

		void WinDC::WidenPath()
		{
			::WidenPath(FHandle);
		}

		void WinDC::DiscardPath()
		{
			::AbortPath(FHandle);
		}

		void WinDC::DrawPath()
		{
			::StrokePath(FHandle);
		}

		void WinDC::FillPath()
		{
			::FillPath(FHandle);
		}

		void WinDC::DrawAndFillPath()
		{
			::StrokeAndFillPath(FHandle);
		}

		WinRegion::Ptr WinDC::RegionFromPath()
		{
			return new WinRegion(::PathToRegion(FHandle));
		}
	
		/*------------------------------------------------------------------------------*/

		bool WinDC::PointInClip(POINT Point)
		{
			return PtVisible(FHandle, Point.x, Point.y)==TRUE;
		}

		bool WinDC::RectInClip(RECT Rect)
		{
			return RectVisible(FHandle, &Rect)==TRUE;
		}

		void WinDC::ClipPath(int CombineMode)
		{
			SelectClipPath(FHandle, CombineMode);
		}

		void WinDC::ClipRegion(WinRegion::Ptr Region)
		{
			SelectClipRgn(FHandle, Region->GetHandle());
		}

		void WinDC::RemoveClip()
		{
			SelectClipRgn(FHandle, NULL);
		}

		void WinDC::MoveClip(int OffsetX, int OffsetY)
		{
			OffsetClipRgn(FHandle, OffsetX, OffsetY);
		}

		void WinDC::CombineClip(WinRegion::Ptr Region, int CombineMode)
		{
			ExtSelectClipRgn(FHandle, Region->GetHandle(), CombineMode);
		}

		void WinDC::IntersetClipRect(RECT Rect)
		{
			::IntersectClipRect(FHandle, Rect.left, Rect.top, Rect.right, Rect.bottom);
		}

		void WinDC::ExcludeClipRect(RECT Rect)
		{
			::ExcludeClipRect(FHandle, Rect.left, Rect.top, Rect.right, Rect.bottom);
		}

		WinRegion::Ptr WinDC::GetClipRegion()
		{
			HRGN Handle=CreateRectRgn(0, 0, 1, 1);
			GetClipRgn(FHandle, Handle);
			return new WinRegion(Handle);
		}

		RECT WinDC::GetClipBoundRect()
		{
			RECT Rect;
			GetClipBox(FHandle, &Rect);
			return Rect;
		}

		/*------------------------------------------------------------------------------*/

		WinTransform WinDC::GetTransform()
		{
			XFORM Transform;
			GetWorldTransform(FHandle, &Transform);
			return Transform;
		}

		void WinDC::SetTransform(const WinTransform& Transform)
		{
			SetWorldTransform(FHandle, Transform.GetHandle());
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, DWORD DrawROP)
		{
			HDC SourceHandle=Source?Source->GetHandle():0;
			BitBlt(FHandle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, DrawROP);
		}

		void WinDC::Copy(RECT dstRect, WinDC* Source, POINT srcPos, DWORD DrawROP)
		{
			HDC SourceHandle=Source?Source->GetHandle():0;
			BitBlt(FHandle, dstRect.left, dstRect.top, dstRect.right-dstRect.left, dstRect.bottom-dstRect.top, SourceHandle, srcPos.x, srcPos.y, DrawROP);
		}

		void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY , int srcW, int srcH, DWORD DrawROP)
		{
			HDC SourceHandle=Source?Source->GetHandle():0;
			StretchBlt(FHandle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, srcW, srcH, DrawROP);
		}

		void WinDC::Copy(RECT dstRect, WinDC* Source, RECT srcRect, DWORD DrawROP)
		{
			HDC SourceHandle=Source?Source->GetHandle():0;
			StretchBlt(	FHandle		, dstRect.left, dstRect.top, dstRect.right-dstRect.left, dstRect.bottom-dstRect.top, 
						SourceHandle, srcRect.left, srcRect.top, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top, 
						DrawROP);
		}

		void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC* Source, int srcX, int srcY, int srcW, int srcH)
		{
			POINT Pt[3];
			Pt[0]=UpperLeft;
			Pt[1]=UpperRight;
			Pt[2]=LowerLeft;
			PlgBlt(FHandle, Pt, Source->GetHandle(), srcX, srcY, srcW, srcH, 0, 0, 0);
		}

		void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC*Source, RECT srcRect)
		{
			POINT Pt[3];
			Pt[0]=UpperLeft;
			Pt[1]=UpperRight;
			Pt[2]=LowerLeft;
			PlgBlt(FHandle, Pt, Source->GetHandle(), srcRect.left, srcRect.top, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top, 0, 0, 0);
		}

		void WinDC::CopyTrans(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY , int srcW, int srcH, COLORREF Color)
		{
			TransparentBlt(FHandle, dstX, dstY, dstW, dstH, Source->GetHandle(), srcX, srcY, srcW, srcH, Color);
		}

		void WinDC::CopyTrans(RECT dstRect, WinDC* Source, RECT srcRect, COLORREF Color)
		{
			TransparentBlt(	FHandle				, dstRect.left, dstRect.top, dstRect.right-dstRect.left, dstRect.bottom-dstRect.top, 
							Source->GetHandle()	, srcRect.left, srcRect.top, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top, 
							Color);
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::Draw(int dstX, int dstY, WinMetaFile* MetaFile)
		{
			Draw(dstX, dstY, MetaFile->GetWidth(), MetaFile->GetHeight(), MetaFile);
		}

		void WinDC::Draw(POINT Pos, WinMetaFile* MetaFile)
		{
			Draw(Pos.x, Pos.y, MetaFile->GetWidth(), MetaFile->GetHeight(), MetaFile);
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinMetaFile* MetaFile)
		{
			RECT Rect;
			Rect.left=dstX;
			Rect.top=dstY;
			Rect.right=dstX+dstW;
			Rect.bottom=dstY+dstH;
			Draw(Rect, MetaFile);
		}

		void WinDC::Draw(RECT Rect, WinMetaFile* MetaFile)
		{
			PlayEnhMetaFile(FHandle, MetaFile->GetHandle(), &Rect);
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap)
		{
			int dstW=Bitmap->GetWidth();
			int dstH=Bitmap->GetHeight();
			int srcX=0;
			int srcY=0;
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				BitBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
			}
			else
			{
				int srcW=dstW;
				int srcH=dstH;
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(POINT Pos, WinBitmap::Ptr Bitmap)
		{
			int dstX=Pos.x;
			int dstY=Pos.y;
			int dstW=Bitmap->GetWidth();
			int dstH=Bitmap->GetHeight();
			int srcX=0;
			int srcY=0;
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				BitBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
			}
			else
			{
				int srcW=dstW;
				int srcH=dstH;
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap)
		{
			int srcX=0;
			int srcY=0;
			int srcW=Bitmap->GetWidth();
			int srcH=Bitmap->GetHeight();
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				StretchBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
			}
			else
			{
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap)
		{
			int dstX=Rect.left;
			int dstY=Rect.top;
			int dstW=Rect.right-Rect.left;
			int dstH=Rect.bottom-Rect.top;
			int srcX=0;
			int srcY=0;
			int srcW=Bitmap->GetWidth();
			int srcH=Bitmap->GetHeight();
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				StretchBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
			}
			else
			{
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY)
		{
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				BitBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
			}
			else
			{
				int srcW=dstW;
				int srcH=dstH;
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos)
		{
			int dstX=Rect.left;
			int dstY=Rect.top;
			int dstW=Rect.right-Rect.left;
			int dstH=Rect.bottom-Rect.top;
			int srcX=Pos.x;
			int srcY=Pos.y;
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				BitBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
			}
			else
			{
				int srcW=dstW;
				int srcH=dstH;
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH)
		{
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				StretchBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
			}
			else
			{
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		void WinDC::Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect)
		{
			int dstX=dstRect.left;
			int dstY=dstRect.top;
			int dstW=dstRect.right-dstRect.left;
			int dstH=dstRect.bottom-dstRect.top;
			int srcX=srcRect.left;
			int srcY=srcRect.top;
			int srcW=srcRect.right-srcRect.left;
			int srcH=srcRect.bottom-srcRect.top;
			if(!Bitmap->IsAlphaChannelBuilt())
			{
				StretchBlt(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
			}
			else
			{
				BLENDFUNCTION Blend;
				Blend.BlendOp=AC_SRC_OVER;
				Blend.BlendFlags=0;
				Blend.SourceConstantAlpha=255;
				Blend.AlphaFormat=AC_SRC_ALPHA;
				AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
			}
		}

		/*------------------------------------------------------------------------------*/

		void WinDC::Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap, unsigned char Alpha)
		{
			int dstW=Bitmap->GetWidth();
			int dstH=Bitmap->GetHeight();
			int srcX=0;
			int srcY=0;
			int srcW=dstW;
			int srcH=dstH;

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(POINT Pos, WinBitmap::Ptr Bitmap, unsigned char Alpha)
		{
			int dstX=Pos.x;
			int dstY=Pos.y;
			int dstW=Bitmap->GetWidth();
			int dstH=Bitmap->GetHeight();
			int srcX=0;
			int srcY=0;
			int srcW=dstW;
			int srcH=dstH;

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, unsigned char Alpha)
		{
			int srcX=0;
			int srcY=0;
			int srcW=Bitmap->GetWidth();
			int srcH=Bitmap->GetHeight();

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, unsigned char Alpha)
		{
			int dstX=Rect.left;
			int dstY=Rect.top;
			int dstW=Rect.right-Rect.left;
			int dstH=Rect.bottom-Rect.top;
			int srcX=0;
			int srcY=0;
			int srcW=Bitmap->GetWidth();
			int srcH=Bitmap->GetHeight();

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, unsigned char Alpha)
		{
			int srcW=dstW;
			int srcH=dstH;

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos, unsigned char Alpha)
		{
			int dstX=Rect.left;
			int dstY=Rect.top;
			int dstW=Rect.right-Rect.left;
			int dstH=Rect.bottom-Rect.top;
			int srcX=Pos.x;
			int srcY=Pos.y;
			int srcW=dstW;
			int srcH=dstH;

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH, unsigned char Alpha)
		{
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

		void WinDC::Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect, unsigned char Alpha)
		{
			int dstX=dstRect.left;
			int dstY=dstRect.top;
			int dstW=dstRect.right-dstRect.left;
			int dstH=dstRect.bottom-dstRect.top;
			int srcX=srcRect.left;
			int srcY=srcRect.top;
			int srcW=srcRect.right-srcRect.left;
			int srcH=srcRect.bottom-srcRect.top;

			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=Alpha;
			Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
			AlphaBlend(FHandle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
		}

/*********************************************************************************************************
WinControlDC
*********************************************************************************************************/

		WinControlDC::WinControlDC(HWND Handle)
		{
			FControlHandle=Handle;
			FHandle=GetDC(FControlHandle);
			Init();
		}

		WinControlDC::~WinControlDC()
		{
			ReleaseDC(FControlHandle, FHandle);
		}

/*********************************************************************************************************
WinProxyDC
*********************************************************************************************************/

		WinProxyDC::WinProxyDC()
		{
			FHandle=NULL;
		}

		WinProxyDC::~WinProxyDC()
		{
		}

		void WinProxyDC::Initialize(HDC Handle)
		{
			FHandle=Handle;
			Init();
		}

/*********************************************************************************************************
WinImageDC
*********************************************************************************************************/

		WinImageDC::WinImageDC()
		{
			FHandle=CreateCompatibleDC(NULL);
			Init();
		}

		WinImageDC::~WinImageDC()
		{
			DeleteDC(FHandle);
		}
	}
}

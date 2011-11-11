#include "WinNativeWindow.h"

#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "WindowsCodecs.lib")

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace collections;
				
			bool WinIsKeyPressing(int code)
			{
				return (GetKeyState(code)&0xF0)!=0;
			}

			bool WinIsKeyToggled(int code)
			{
				return (GetKeyState(code)&0x0F)!=0;
			}

/***********************************************************************
WindowsClass
***********************************************************************/

			class WinClass : public Object
			{
			protected:
				WString					name;
				WNDCLASSEX				windowClass;
				ATOM					windowAtom;
			public:
				WinClass(WString _name, bool shadow, bool ownDC, WNDPROC procedure, HINSTANCE hInstance)
				{
					name=_name;
					windowClass.cbSize=sizeof(windowClass);
					windowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | (shadow?CS_DROPSHADOW:0) | (ownDC?CS_OWNDC:0);
					windowClass.lpfnWndProc=procedure;
					windowClass.cbClsExtra=0;
					windowClass.cbWndExtra=0;
					windowClass.hInstance=hInstance;
					windowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
					windowClass.hCursor=NULL;//LoadCursor(NULL,IDC_ARROW);
					windowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
					windowClass.lpszMenuName=NULL;
					windowClass.lpszClassName=name.Buffer();
					windowClass.hIconSm=NULL;
					windowAtom=RegisterClassEx(&windowClass);
				}

				bool IsAvailable()
				{
					return windowAtom!=0;
				}

				WString GetName()
				{
					return name;
				}

				ATOM GetClassAtom()
				{
					return windowAtom;
				}
			};

/***********************************************************************
WindowsScreen
***********************************************************************/

			class WindowsScreen : public Object, public INativeScreen
			{
				friend class WindowsController;
			protected:
				HMONITOR					monitor;
			public:
				WindowsScreen()
				{
					monitor=NULL;
				}

				Rect GetBounds()
				{
					MONITORINFOEX info;
					info.cbSize=sizeof(MONITORINFOEX);
					GetMonitorInfo(monitor, &info);
					return Rect(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
				}

				Rect GetClientBounds()
				{
					MONITORINFOEX info;
					info.cbSize=sizeof(MONITORINFOEX);
					GetMonitorInfo(monitor, &info);
					return Rect(info.rcWork.left, info.rcWork.top, info.rcWork.right, info.rcWork.bottom);
				}

				WString GetName()
				{
					MONITORINFOEX info;
					info.cbSize=sizeof(MONITORINFOEX);
					GetMonitorInfo(monitor, &info);
					
					wchar_t buffer[sizeof(info.szDevice)/sizeof(*info.szDevice)+1];
					memset(buffer, 0, sizeof(buffer));
					memcpy(buffer, info.szDevice, sizeof(info.szDevice));
					return buffer;
				}

				bool IsPrimary()
				{
					MONITORINFOEX info;
					info.cbSize=sizeof(MONITORINFOEX);
					GetMonitorInfo(monitor, &info);
					return info.dwFlags==MONITORINFOF_PRIMARY;
				}
			};

/***********************************************************************
WindowsCursor
***********************************************************************/

			class WindowsCursor : public Object, public INativeCursor
			{
			protected:
				HCURSOR								handle;
				bool								isSystemCursor;
				SystemCursorType					systemCursorType;
			public:
				WindowsCursor(HCURSOR _handle)
					:handle(_handle)
					,isSystemCursor(false)
					,systemCursorType(INativeCursor::Arrow)
				{
				}

				WindowsCursor(SystemCursorType type)
					:handle(NULL)
					,isSystemCursor(true)
					,systemCursorType(type)
				{
					LPWSTR id=NULL;
					switch(type)
					{
					case SmallWaiting:
						id=IDC_APPSTARTING;
						break;
					case LargeWaiting:
						id=IDC_WAIT;
						break;
					case Arrow:
						id=IDC_ARROW;
						break;
					case Cross:
						id=IDC_CROSS;
						break;
					case Hand:
						id=IDC_HAND;
						break;
					case Help:
						id=IDC_HELP;
						break;
					case IBeam:
						id=IDC_IBEAM;
						break;
					case SizeAll:
						id=IDC_SIZEALL;
						break;
					case SizeNESW:
						id=IDC_SIZENESW;
						break;
					case SizeNS:
						id=IDC_SIZENS;
						break;
					case SizeNWSE:
						id=IDC_SIZENWSE;
						break;
					case SizeWE:
						id=IDC_SIZEWE;
						break;
					}
					handle=(HCURSOR)LoadImage(NULL, id, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
				}
				
				bool IsSystemCursor()
				{
					return isSystemCursor;
				}

				SystemCursorType GetSystemCursorType()
				{
					return systemCursorType;
				}

				HCURSOR GetCursorHandle()
				{
					return handle;
				}
			};

/***********************************************************************
WindowsClipboard
***********************************************************************/

			class WindowsClipboard : public Object, public INativeClipboard
			{
			protected:
				HWND			ownerHandle;
			public:
				WindowsClipboard()
					:ownerHandle(NULL)
				{
				}

				void SetOwnerHandle(HWND handle)
				{
					ownerHandle=handle;
				}

				bool ContainsText()
				{
					if(OpenClipboard(ownerHandle))
					{
						UINT format=0;
						bool contains=false;
						while(format=EnumClipboardFormats(format))
						{
							if(format==CF_TEXT || format==CF_UNICODETEXT)
							{
								contains=true;
								break;
							}
						}
						CloseClipboard();
						return contains;
					}
					return false;
				}

				WString GetText()
				{
					if(OpenClipboard(ownerHandle))
					{
						WString result;
						HANDLE handle=GetClipboardData(CF_UNICODETEXT);
						if(handle!=0)
						{
							wchar_t* buffer=(wchar_t*)GlobalLock(handle);
							result=buffer;
							GlobalUnlock(handle);
						}
						CloseClipboard();
						return result;
					}
					return L"";
				}

				bool SetText(const WString& value)
				{
					if(OpenClipboard(ownerHandle))
					{
						EmptyClipboard();
						int size=(value.Length()+1)*sizeof(wchar_t);
						HGLOBAL data=GlobalAlloc(GMEM_MOVEABLE, size);
						wchar_t* buffer=(wchar_t*)GlobalLock(data);
						memcpy(buffer, value.Buffer(), size);
						GlobalUnlock(data);
						SetClipboardData(CF_UNICODETEXT, data);
						CloseClipboard();
						return true;
					}
					return false;
				}
			};

/***********************************************************************
WindowsImage
***********************************************************************/

			class WindowsImageFrame : public Object, public INativeImageFrame
			{
			protected:
				INativeImage*									image;
				ComPtr<IWICBitmapFrameDecode>					frameDecode;
				ComPtr<IWICBitmap>								frameBitmap;
				Dictionary<void*, Ptr<INativeImageFrameCache>>	caches;
			public:
				WindowsImageFrame(INativeImage* _image, IWICBitmapFrameDecode* _frameDecode)
					:image(_image)
					,frameDecode(_frameDecode)
				{
					IWICImagingFactory* factory=GetWICImagingFactory();

					ComPtr<IWICFormatConverter> converter;
					{
						IWICFormatConverter* formatConverter=0;
						HRESULT hr=factory->CreateFormatConverter(&formatConverter);
						if(SUCCEEDED(hr))
						{
							converter=formatConverter;
							converter->Initialize(
								frameDecode.Obj(),
								GUID_WICPixelFormat32bppPBGRA,
								WICBitmapDitherTypeNone,
								NULL,
								0.0f,
								WICBitmapPaletteTypeCustom);
						}
					}

					IWICBitmap* bitmap=0;
					IWICBitmapSource* bitmapSource=0;
					if(converter)
					{
						bitmapSource=converter.Obj();
					}
					else
					{
						bitmapSource=frameDecode.Obj();
					}
					HRESULT hr=factory->CreateBitmapFromSource(bitmapSource, WICBitmapCacheOnLoad, &bitmap);
					if(SUCCEEDED(hr))
					{
						frameBitmap=bitmap;
					}
				}

				~WindowsImageFrame()
				{
					for(int i=0;i<caches.Count();i++)
					{
						caches.Values()[i]->OnDetach(this);
					}
				}

				INativeImage* GetImage()
				{
					return image;
				}

				Size GetSize()
				{
					UINT width=0;
					UINT height=0;
					frameBitmap->GetSize(&width, &height);
					return Size(width, height);
				}

				bool SetCache(void* key, Ptr<INativeImageFrameCache> cache)
				{
					int index=caches.Keys().IndexOf(key);
					if(index!=-1)
					{
						return false;
					}
					caches.Add(key, cache);
					cache->OnAttach(this);
					return true;
				}

				Ptr<INativeImageFrameCache> GetCache(void* key)
				{
					int index=caches.Keys().IndexOf(key);
					return index==-1?0:caches.Values()[index];
				}

				Ptr<INativeImageFrameCache> RemoveCache(void* key)
				{
					int index=caches.Keys().IndexOf(key);
					if(index==-1)
					{
						return 0;
					}
					Ptr<INativeImageFrameCache> cache=caches.Values()[index];
					cache->OnDetach(this);
					caches.Remove(key);
					return cache;
				}

				IWICBitmapFrameDecode* GetFrameDecode()
				{
					return frameDecode.Obj();
				}

				IWICBitmap* GetFrameBitmap()
				{
					return frameBitmap.Obj();
				}
			};

			class WindowsImage : public Object, public INativeImage
			{
			protected:
				INativeImageProvider*					provider;
				ComPtr<IWICBitmapDecoder>				bitmapDecoder;
				Array<Ptr<WindowsImageFrame>>			frames;
			public:
				WindowsImage(INativeImageProvider* _provider, IWICBitmapDecoder* _bitmapDecoder)
					:provider(_provider)
					,bitmapDecoder(_bitmapDecoder)
				{
					UINT count=0;
					bitmapDecoder->GetFrameCount(&count);
					frames.Resize(count);
				}

				~WindowsImage()
				{
				}

				INativeImageProvider* GetProvider()
				{
					return provider;
				}

				FormatType GetFormat()
				{
					GUID formatGUID;
					HRESULT hr=bitmapDecoder->GetContainerFormat(&formatGUID);
					if(SUCCEEDED(hr))
					{
						if(formatGUID==GUID_ContainerFormatBmp)
						{
							return INativeImage::Bmp;
						}
						else if(formatGUID==GUID_ContainerFormatPng)
						{
							return INativeImage::Png;
						}
						else if(formatGUID==GUID_ContainerFormatGif)
						{
							return INativeImage::Gif;
						}
						else if(formatGUID==GUID_ContainerFormatJpeg)
						{
							return INativeImage::Jpeg;
						}
						else if(formatGUID==GUID_ContainerFormatIco)
						{
							return INativeImage::Icon;
						}
						else if(formatGUID==GUID_ContainerFormatTiff)
						{
							return INativeImage::Tiff;
						}
						else if(formatGUID==GUID_ContainerFormatWmp)
						{
							return INativeImage::Wmp;
						}
					}
					return INativeImage::Unknown;
				}

				int GetFrameCount()
				{
					return frames.Count();
				}

				INativeImageFrame* GetFrame(int index)
				{
					if(0<=index && index<GetFrameCount())
					{
						Ptr<WindowsImageFrame>& frame=frames[index];
						if(!frame)
						{
							IWICBitmapFrameDecode* frameDecode=0;
							HRESULT hr=bitmapDecoder->GetFrame(index, &frameDecode);
							if(SUCCEEDED(hr))
							{
								frame=new WindowsImageFrame(this, frameDecode);
							}
						}
						return frame.Obj();
					}
					else
					{
						return 0;
					}
				}

				IWICBitmapDecoder* GetBitmapDecoder()
				{
					return bitmapDecoder.Obj();
				}
			};

			class WindowsImageProvider : public Object, public INativeImageProvider
			{
			protected:
				ComPtr<IWICImagingFactory>				imagingFactory;
			public:
				WindowsImageProvider()
				{
					IWICImagingFactory* factory=0;
					HRESULT hr = CoCreateInstance(
						CLSID_WICImagingFactory,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_IWICImagingFactory,
						(LPVOID*)&factory
						);
					if(SUCCEEDED(hr))
					{
						imagingFactory=factory;
					}
				}

				~WindowsImageProvider()
				{
				}

				Ptr<INativeImage> CreateImageFromFile(const WString& path)
				{
					IWICBitmapDecoder* bitmapDecoder=0;
					HRESULT hr=imagingFactory->CreateDecoderFromFilename(
						path.Buffer(),
						NULL,
						GENERIC_READ,
						WICDecodeMetadataCacheOnDemand,
						&bitmapDecoder);
					if(SUCCEEDED(hr))
					{
						return new WindowsImage(this, bitmapDecoder);
					}
					else
					{
						return 0;
					}
				}

				IWICImagingFactory* GetImagingFactory()
				{
					return imagingFactory.Obj();
				}
			};

			IWICImagingFactory* GetWICImagingFactory()
			{
				return dynamic_cast<WindowsImageProvider*>(GetCurrentController()->GetImageProvider())->GetImagingFactory();
			}

			IWICBitmapDecoder* GetWICBitmapDecoder(INativeImage* image)
			{
				return dynamic_cast<WindowsImage*>(image)->GetBitmapDecoder();
			}

			IWICBitmap* GetWICBitmap(INativeImageFrame* frame)
			{
				return dynamic_cast<WindowsImageFrame*>(frame)->GetFrameBitmap();
			}

/***********************************************************************
WindowsForm
***********************************************************************/

			class WindowsForm : public Object, public INativeWindow, public IWindowsForm
			{
			protected:
				HWND								handle;
				WString								title;
				WindowsCursor*						cursor;
				Point								caretPoint;
				INativeWindow*						parentWindow;
				List<INativeWindowListener*>		listeners;
				int									mouseLastX;
				int									mouseLastY;
				int									mouseHoving;
				IWindowsFormGraphicsHandler*		graphicsHandler;
				
				DWORD InternalGetExStyle()
				{
					return GetWindowLongPtr(handle,GWL_EXSTYLE);
				}

				void InternalSetExStyle(DWORD exStyle)
				{
					LONG result=SetWindowLongPtr(handle,GWL_EXSTYLE,exStyle);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				bool GetExStyle(DWORD exStyle)
				{
					LONG_PTR Long=InternalGetExStyle();
					return (Long & exStyle) != 0;
				}

				void SetExStyle(DWORD exStyle, bool available)
				{
					LONG_PTR Long=InternalGetExStyle();
					if(available)
					{
						Long|=exStyle;
					}
					else
					{
						Long&=~exStyle;
					}
					InternalSetExStyle(Long);
				}

				bool GetStyle(DWORD style)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					return (Long & style) != 0;
				}

				void SetStyle(DWORD style, bool available)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					if(available)
					{
						Long|=style;
					}
					else
					{
						Long&=~style;
					}
					SetWindowLongPtr(handle,GWL_STYLE,Long);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				NativeWindowMouseInfo ConvertMouse(WPARAM wParam, LPARAM lParam, bool wheelMessage)
				{
					NativeWindowMouseInfo info;
					if(wheelMessage)
					{
						info.wheel=GET_WHEEL_DELTA_WPARAM(wParam);
						wParam=GET_KEYSTATE_WPARAM(wParam);
					}
					else
					{
						info.wheel=0;
					}
					info.ctrl=(wParam & MK_CONTROL)!=0;
					info.shift=(wParam & MK_SHIFT)!=0;
					info.left=(wParam & MK_LBUTTON)!=0;
					info.middle=(wParam & MK_MBUTTON)!=0;
					info.right=(wParam & MK_RBUTTON)!=0;
					POINTS Point=MAKEPOINTS(lParam);
					info.x=Point.x;
					info.y=Point.y;
					return info;
				}

				NativeWindowKeyInfo ConvertKey(WPARAM wParam, LPARAM lParam)
				{
					NativeWindowKeyInfo info;
					info.code=wParam;
					info.ctrl=WinIsKeyPressing(VK_CONTROL);
					info.shift=WinIsKeyPressing(VK_SHIFT);
					info.alt=WinIsKeyPressing(VK_MENU);
					info.capslock=WinIsKeyToggled(VK_CAPITAL);
					return info;
				}

				NativeWindowCharInfo ConvertChar(WPARAM wParam)
				{
					NativeWindowCharInfo info;
					info.code=wParam;
					info.ctrl=WinIsKeyPressing(VK_CONTROL);
					info.shift=WinIsKeyPressing(VK_SHIFT);
					info.alt=WinIsKeyPressing(VK_MENU);
					info.capslock=WinIsKeyToggled(VK_CAPITAL);
					return info;
				}

				void TrackMouse(bool enable)
				{
					TRACKMOUSEEVENT trackMouseEvent;
					trackMouseEvent.cbSize=sizeof(trackMouseEvent);
					trackMouseEvent.hwndTrack=handle;
					trackMouseEvent.dwFlags=(enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
					trackMouseEvent.dwHoverTime=HOVER_DEFAULT;
					TrackMouseEvent(&trackMouseEvent);
				}

				void UpdateCompositionForContent()
				{
					HIMC imc = ImmGetContext(handle);
					COMPOSITIONFORM cf;
					cf.dwStyle = CFS_POINT;
					cf.ptCurrentPos.x = caretPoint.x;
					cf.ptCurrentPos.y = caretPoint.y;
					ImmSetCompositionWindow(imc, &cf);
					ImmReleaseContext(handle, imc);
				}

			public:
				WindowsForm(HWND parent, WString className, HINSTANCE hInstance)
					:cursor(0)
					,parentWindow(0)
					,mouseLastX(-1)
					,mouseLastY(-1)
					,mouseHoving(false)
					,graphicsHandler(0)
				{
					DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
					DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
					handle=CreateWindowEx(exStyle, className.Buffer(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, hInstance, NULL);
				}

				~WindowsForm()
				{
					List<INativeWindowListener*> copiedListeners;
					CopyFrom(copiedListeners.Wrap(), listeners.Wrap());
					for(int i=0;i<copiedListeners.Count();i++)
					{
						INativeWindowListener* listener=copiedListeners[i];
						if(listeners.Contains(listener))
						{
							listener->Destroyed();
						}
					}
					DestroyWindow(handle);
				}

				void InvokeDestroying()
				{
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->Destroying();
					}
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					switch(uMsg)
					{
					case WM_MOVING:case WM_SIZING:
						{
							LPRECT rawBounds=(LPRECT)lParam;
							Rect bounds(rawBounds->left, rawBounds->top, rawBounds->right, rawBounds->bottom);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moving(bounds, false);
							}
							if(		rawBounds->left!=bounds.Left()
								||	rawBounds->top!=bounds.Top()
								||	rawBounds->right!=bounds.Right()
								||	rawBounds->bottom!=bounds.Bottom())
							{
								rawBounds->left=bounds.Left();
								rawBounds->top=bounds.Top();
								rawBounds->right=bounds.Right();
								rawBounds->bottom=bounds.Bottom();
								result=TRUE;
							}
						}
						break;
					case WM_MOVE:case WM_SIZE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moved();
							}
						}
						break;
					case WM_ENABLE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								if(wParam==TRUE)
								{
									listeners[i]->Enabled();
								}
								else
								{
									listeners[i]->Disabled();
								}
							}
						}
						break;
					case WM_SETFOCUS:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->GotFocus();
							}
						}
						break;
					case WM_KILLFOCUS:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LostFocus();
							}
						}
						break;
					case WM_ACTIVATE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								if(wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE)
								{
									listeners[i]->Activated();
								}
								else
								{
									listeners[i]->Deactivated();
								}
							}
						}
						break;
					case WM_SHOWWINDOW:
						{
							if(wParam==TRUE)
							{
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Opened();
								}
							}
							else
							{
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Closed();
								}
							}
						}
						break;
					case WM_CLOSE:
						{
							bool cancel=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Closing(cancel);
							}
							return cancel;
						}
						break;
					case WM_LBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDown(info);
							}
						}
						break;
					case WM_LBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonUp(info);
							}
						}
						break;
					case WM_LBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDoubleClick(info);
							}
						}
						break;
					case WM_RBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDown(info);
							}
						}
						break;
					case WM_RBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonUp(info);
							}
						}
						break;
					case WM_RBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDoubleClick(info);
							}
						}
						break;
					case WM_MBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDown(info);
							}
						}
						break;
					case WM_MBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonUp(info);
							}
						}
						break;
					case WM_MBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDoubleClick(info);
							}
						}
						break;
					case WM_MOUSEHWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->HorizontalWheel(info);
							}
						}
						break;
					case WM_MOUSEWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->VerticalWheel(info);
							}
						}
						break;
					case WM_MOUSEMOVE:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							if(info.x!=mouseLastX || info.y!=mouseLastY)
							{
								if(!mouseHoving)
								{
									mouseHoving=true;
									for(int i=0;i<listeners.Count();i++)
									{
										listeners[i]->MouseEntered();
									}
									TrackMouse(true);
								}
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->MouseMoving(info);
								}
							}
							if(GetCursor()!=cursor->GetCursorHandle())
							{
								SetCursor(cursor->GetCursorHandle());
							}
						}
						break;
					case WM_MOUSELEAVE:
						{
							mouseLastX=-1;
							mouseLastY=-1;
							mouseHoving=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MouseLeaved();
							}
						}
						break;
					case WM_MOUSEHOVER:
						{
							TrackMouse(true);
						}
						break;
					case WM_KEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyUp(info);
							}
						}
						break;
					case WM_KEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyDown(info);
							}
						}
						break;
					case WM_SYSKEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyUp(info);
							}
						}
						break;
					case WM_SYSKEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyDown(info);
							}
						}
						break;
					case WM_CHAR:
						{
							NativeWindowCharInfo info=ConvertChar(wParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Char(info);
							}
						}
						break;
					case WM_PAINT:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Paint();
							}
						}
						break;
					case WM_ERASEBKGND:
						return true;
					case WM_IME_STARTCOMPOSITION:
						UpdateCompositionForContent();
						break;
					}
					return false;
				}

				HWND GetWindowHandle()
				{
					return handle;
				}

				IWindowsFormGraphicsHandler* GetGraphicsHandler()
				{
					return graphicsHandler;
				}

				void SetGraphicsHandler(IWindowsFormGraphicsHandler* handler)
				{
					graphicsHandler=handler;
				}

				Rect GetBounds()
				{
					RECT rect;
					GetWindowRect(handle, &rect);
					return Rect(rect.left, rect.top, rect.right, rect.bottom);
				}

				void SetBounds(const Rect& bounds)
				{
					Rect newBounds=bounds;
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->Moving(newBounds, true);
					}
					MoveWindow(handle, newBounds.Left(), newBounds.Top(), newBounds.Width(), newBounds.Height(), FALSE);
				}

				Size GetClientSize()
				{
					return GetClientBoundsInScreen().GetSize();
				}

				void SetClientSize(Size size)
				{
					RECT required={0,0,size.x,size.y};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					SetBounds(Rect(Point(bounds.left, bounds.top), Size(required.right-required.left, required.bottom-required.top)));
				}

				Rect GetClientBoundsInScreen()
				{
					RECT required={0,0,0,0};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					return Rect(
						Point(
							(bounds.left-required.left),
							(bounds.top-required.top)
							),
						Size(
							(bounds.right-bounds.left)-(required.right-required.left),
							(bounds.bottom-bounds.top)-(required.bottom-required.top)
							)
						);
				}

				WString GetTitle()
				{
					return title;
				}

				void SetTitle(WString _title)
				{
					title=_title;
					SetWindowText(handle, title.Buffer());
				}

				INativeCursor* GetWindowCursor()
				{
					return cursor;
				}

				void SetWindowCursor(INativeCursor* _cursor)
				{
					WindowsCursor* newCursor=dynamic_cast<WindowsCursor*>(_cursor);
					if(newCursor && cursor!=newCursor)
					{
						cursor=newCursor;
						if(mouseHoving && IsVisible())
						{
							SetCursor(cursor->GetCursorHandle());
						}
					}
				}
				
				Point GetCaretPoint()
				{
					return caretPoint;
				}

				void SetCaretPoint(Point point)
				{
					caretPoint=point;
					UpdateCompositionForContent();
				}

				INativeWindow* GetParent()
				{
					return parentWindow;
				}

				void SetParent(INativeWindow* parent)
				{
					parentWindow=parent;
					if(parentWindow)
					{
						HWND parentHandle=dynamic_cast<WindowsForm*>(parent)->handle;
						SetWindowLongPtr(handle, GWL_HWNDPARENT, (LONG_PTR)parentHandle);
					}
					else
					{
						SetWindowLongPtr(handle, GWL_HWNDPARENT, NULL);
					}
				}

				void Show()
				{
					ShowWindow(handle, SW_SHOWNORMAL);
				}

				void ShowDeactivated()
				{
					ShowWindow(handle, SW_SHOWNOACTIVATE);
				}

				void ShowRestored()
				{
					ShowWindow(handle, SW_RESTORE);
				}

				void ShowMaximized()
				{
					ShowWindow(handle, SW_SHOWMAXIMIZED);
				}

				void ShowMinimized()
				{
					ShowWindow(handle, SW_SHOWMINIMIZED);
				}

				void Hide()
				{
					SendMessage(handle, WM_CLOSE, NULL, NULL);
				}

				bool IsVisible()
				{
					return IsWindowVisible(handle)!=0;
				}

				void Enable()
				{
					EnableWindow(handle, TRUE);
				}

				void Disable()
				{
					EnableWindow(handle, FALSE);
				}

				bool IsEnabled()
				{
					return IsWindowEnabled(handle)!=0;
				}

				void SetFocus()
				{
					::SetFocus(handle);
				}

				bool IsFocused()
				{
					return GetFocus()==handle;
				}

				void SetActivate()
				{
					SetActiveWindow(handle);
				}

				bool IsActivated()
				{
					return GetActiveWindow()==handle;
				}

				void ShowInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, true);
				}

				void HideInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, false);
				}

				bool IsAppearedInTaskBar()
				{
					return GetExStyle(WS_EX_APPWINDOW);
				}

				void EnableActivate()
				{
					SetExStyle(WS_EX_NOACTIVATE, false);
				}

				void DisableActivate()
				{
					SetExStyle(WS_EX_NOACTIVATE, true);
				}

				bool IsEnabledActivate()
				{
					return !GetExStyle(WS_EX_NOACTIVATE);
				}

				bool RequireCapture()
				{
					SetCapture(handle);
					return true;
				}

				bool ReleaseCapture()
				{
					::ReleaseCapture();
					return true;
				}

				bool GetMaximizedBox()
				{
					return GetStyle(WS_MAXIMIZEBOX);
				}

				void SetMaximizedBox(bool visible)
				{
					SetStyle(WS_MAXIMIZEBOX, visible);
				}

				bool GetMinimizedBox()
				{
					return GetStyle(WS_MINIMIZEBOX);
				}

				void SetMinimizedBox(bool visible)
				{
					SetStyle(WS_MINIMIZEBOX, visible);
				}

				bool GetBorder()
				{
					return GetStyle(WS_BORDER);
				}

				void SetBorder(bool visible)
				{
					SetStyle(WS_BORDER, visible);
				}

				bool GetSizeBox()
				{
					return GetStyle(WS_SIZEBOX);
				}

				void SetSizeBox(bool visible)
				{
					SetStyle(WS_SIZEBOX, visible);
				}

				bool GetIconVisible()
				{
					return GetStyle(WS_SYSMENU);
				}

				void SetIconVisible(bool visible)
				{
					SetStyle(WS_SYSMENU, visible);
				}

				bool GetTitleBar()
				{
					return GetStyle(WS_CAPTION);
				}

				void SetTitleBar(bool visible)
				{
					SetStyle(WS_CAPTION, visible);
				}

				bool GetTopMost()
				{
					return GetExStyle(WS_EX_TOPMOST);
				}

				void SetTopMost(bool topmost)
				{
					SetWindowPos(handle,(topmost?HWND_TOPMOST:HWND_NOTOPMOST),0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
				}

				bool InstallListener(INativeWindowListener* listener)
				{
					if(listeners.Contains(listener))
					{
						return false;
					}
					else
					{
						listeners.Add(listener);
						return true;
					}
				}

				bool UninstallListener(INativeWindowListener* listener)
				{
					if(listeners.Contains(listener))
					{
						listeners.Remove(listener);
						return true;
					}
					else
					{
						return false;
					}
				}

				void RedrawContent()
				{
					if(graphicsHandler)
					{
						graphicsHandler->RedrawContent();
						SendMessage(this->handle, WM_PAINT, NULL, NULL);
					}
				}
			};

/***********************************************************************
WindowsController
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK MouseProc(int nCode , WPARAM wParam , LPARAM lParam);

			class WindowsController : public Object, public INativeController
			{
			protected:
				WinClass							windowClass;
				WinClass							godClass;
				HINSTANCE							hInstance;
				HWND								godWindow;
				Dictionary<HWND, WindowsForm*>		windows;
				List<INativeControllerListener*>	listeners;
				INativeWindow*						mainWindow;
				WindowsClipboard					clipboard;
				WindowsImageProvider				imageProvider;

				HHOOK								mouseHook;
				bool								isTimerEnabled;

				Array<Ptr<WindowsCursor>>			systemCursors;
				List<Ptr<WindowsScreen>>			screens;
				FontProperties						defaultFont;
			public:
				WindowsController(HINSTANCE _hInstance)
					:hInstance(_hInstance)
					,windowClass(L"VczhWindow", false, false, WndProc, _hInstance)
					,godClass(L"GodWindow", false, false, GodProc, _hInstance)
					,mainWindow(0)
					,mouseHook(NULL)
					,isTimerEnabled(false)
				{
					godWindow=CreateWindowEx(WS_EX_CONTROLPARENT, godClass.GetName().Buffer(), L"GodWindow", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
					AddClipboardFormatListener(godWindow);
					clipboard.SetOwnerHandle(godWindow);

					{
						NONCLIENTMETRICS metrics;
						metrics.cbSize=sizeof(NONCLIENTMETRICS);
						SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
						if(!*metrics.lfMessageFont.lfFaceName)
						{
							metrics.cbSize=sizeof(NONCLIENTMETRICS)-sizeof(metrics.iPaddedBorderWidth);
							SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
						}
						defaultFont.fontFamily=metrics.lfMessageFont.lfFaceName;
						defaultFont.size=metrics.lfMessageFont.lfHeight;
						if(defaultFont.size<0)
						{
							defaultFont.size=-defaultFont.size;
						}
					}

					systemCursors.Resize(INativeCursor::SystemCursorCount);
					for(int i=0;i<systemCursors.Count();i++)
					{
						systemCursors[i]=new WindowsCursor((INativeCursor::SystemCursorType)i);
					}
				}

				~WindowsController()
				{
					StopTimer();
					StopHookMouse();
					RemoveClipboardFormatListener(godWindow);
					DestroyWindow(godWindow);
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool skipDefaultProcedure=false;
					int index=windows.Keys().IndexOf(hwnd);
					if(index!=-1)
					{
						WindowsForm* window=windows.Values()[index];
						skipDefaultProcedure=window->HandleMessage(hwnd, uMsg, wParam, lParam, result);
						switch(uMsg)
						{
						case WM_CLOSE:
							if(!skipDefaultProcedure)
							{
								ShowWindow(window->GetWindowHandle(), SW_HIDE);
								if(window!=mainWindow)
								{
									skipDefaultProcedure=true;
								}
							}
							break;
						case WM_DESTROY:
							DestroyNativeWindow(window);
							if(window==mainWindow)
							{
								for(int i=0;i<windows.Count();i++)
								{
									if(windows.Values()[i]->IsVisible())
									{
										windows.Values()[i]->Hide();
									}
								}
								while(windows.Count())
								{
									DestroyNativeWindow(windows.Values()[0]);
								}
								PostQuitMessage(0);
							}
							break;
						}
					}
					return skipDefaultProcedure;
				}

				INativeCursor* GetSystemCursor(INativeCursor::SystemCursorType type)
				{
					int index=(int)type;
					if(0<=index && index<systemCursors.Count())
					{
						return systemCursors[index].Obj();
					}
					else
					{
						return 0;
					}
				}

				INativeCursor* GetDefaultSystemCursor()
				{
					return GetSystemCursor(INativeCursor::Arrow);
				}

				INativeWindow* CreateNativeWindow()
				{
					WindowsForm* window=new WindowsForm(godWindow, windowClass.GetName(), hInstance);
					windows.Add(window->GetWindowHandle(), window);
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->NativeWindowCreated(window);
					}
					window->SetWindowCursor(GetDefaultSystemCursor());
					return window;
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
					WindowsForm* windowsForm=dynamic_cast<WindowsForm*>(window);
					windowsForm->InvokeDestroying();
					if(windowsForm!=0 && windows.Keys().Contains(windowsForm->GetWindowHandle()))
					{
						for(int i=0;i<listeners.Count();i++)
						{
							listeners[i]->NativeWindowDestroying(window);
						}
						windows.Remove(windowsForm->GetWindowHandle());
						delete windowsForm;
					}
				}

				INativeWindow* GetMainWindow()
				{
					return mainWindow;
				}

				void Run(INativeWindow* window)
				{
					mainWindow=window;
					mainWindow->Show();
					MSG message;
					while(GetMessage(&message, NULL, 0, 0))
					{
						TranslateMessage(&message);
						DispatchMessage(&message);
					}
				}

				FontProperties GetDefaultFont()
				{
					return defaultFont;
				}

				void SetDefaultFont(const FontProperties& value)
				{
					defaultFont=value;
				}

				bool InstallListener(INativeControllerListener* listener)
				{
					if(listeners.Contains(listener))
					{
						return false;
					}
					else
					{
						listeners.Add(listener);
						return true;
					}
				}

				bool UninstallListener(INativeControllerListener* listener)
				{
					if(listeners.Contains(listener))
					{
						listeners.Remove(listener);
						return true;
					}
					else
					{
						return false;
					}
				}

				//=======================================================================

				void InvokeMouseHook(WPARAM message, Point location)
				{
					switch(message)
					{
					case WM_LBUTTONDOWN:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDown(location);
							}
						}
						break;
					case WM_LBUTTONUP:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonUp(location);
							}
						}
						break;
					case WM_RBUTTONDOWN:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDown(location);
							}
						}
						break;
					case WM_RBUTTONUP:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonUp(location);
							}
						}
						break;
					case WM_MOUSEMOVE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MouseMoving(location);
							}
						}
						break;
					}
				}

				void StartHookMouse()
				{
					if(!IsHookingMouse())
					{
						mouseHook=SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, NULL);
					}
				}

				void StopHookMouse()
				{
					if(IsHookingMouse())
					{
						UnhookWindowsHookEx(mouseHook);
						mouseHook=NULL;
					}
				}

				bool IsHookingMouse()
				{
					return mouseHook!=NULL;
				}

				//=======================================================================

				void InvokeGlobalTimer()
				{
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->GlobalTimer();
					}
				}

				void StartTimer()
				{
					if(!IsTimerEnabled())
					{
						SetTimer(godWindow, 1, 16, NULL);
						isTimerEnabled=true;
					}
				}

				void StopTimer()
				{
					if(IsTimerEnabled())
					{
						KillTimer(godWindow, 1);
						isTimerEnabled=false;
					}
				}

				bool IsTimerEnabled()
				{
					return isTimerEnabled;
				}

				//=======================================================================

				void InvokeClipboardUpdated()
				{
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->ClipboardUpdated();
					}
				}

				INativeClipboard* GetClipboard()
				{
					return &clipboard;
				}

				INativeImageProvider* GetImageProvider()
				{
					return &imageProvider;
				}

				//=======================================================================

				struct MonitorEnumProcData
				{
					WindowsController*		controller;
					int						currentScreen;
				};

				static BOOL CALLBACK MonitorEnumProc(
				  HMONITOR hMonitor,
				  HDC hdcMonitor,
				  LPRECT lprcMonitor,
				  LPARAM dwData
				)
				{
					MonitorEnumProcData* data=(MonitorEnumProcData*)dwData;
					if(data->currentScreen==data->controller->screens.Count())
					{
						data->controller->screens.Add(new WindowsScreen());
					}
					data->controller->screens[data->currentScreen]->monitor=hMonitor;
					data->currentScreen++;
					return TRUE;
				}

				void RefreshScreenInformation()
				{
					for(int i=0;i<screens.Count();i++)
					{
						screens[i]->monitor=NULL;
					}
					MonitorEnumProcData data;
					data.controller=this;
					data.currentScreen=0;
					EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)(&data));
				}
				
				int GetScreenCount()
				{
					RefreshScreenInformation();
					return GetSystemMetrics(SM_CMONITORS);
				}

				INativeScreen* GetScreen(int index)
				{
					RefreshScreenInformation();
					return screens[index].Obj();
				}

				INativeScreen* GetScreen(INativeWindow* window)
				{
					RefreshScreenInformation();
					IWindowsForm* windowsForm=GetWindowsForm(window);
					if(windowsForm)
					{
						HMONITOR monitor=MonitorFromWindow(windowsForm->GetWindowHandle(), MONITOR_DEFAULTTONULL);
						if(monitor!=NULL)
						{
							for(int i=0;i<screens.Count();i++)
							{
								if(screens[i]->monitor==monitor)
								{
									return screens[i].Obj();
								}
							}
						}
					}
					return 0;
				}

				//=======================================================================
				
				bool IsKeyPressing(int code)
				{
					return WinIsKeyPressing(code);
				}

				bool IsKeyToggled(int code)
				{
					return WinIsKeyToggled(code);
				}
			};

/***********************************************************************
Windows Procedure
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					LRESULT result=0;
					if(controller->HandleMessage(hwnd, uMsg, wParam, lParam, result))
					{
						return result;
					}
				}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					switch(uMsg)
					{
					case WM_TIMER:
						controller->InvokeGlobalTimer();
						break;
					case WM_CLIPBOARDUPDATE:
						controller->InvokeClipboardUpdated();
						break;
					}
				}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK MouseProc(int nCode , WPARAM wParam , LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					MSLLHOOKSTRUCT* mouseHookStruct=(MSLLHOOKSTRUCT*)lParam;
					Point location(mouseHookStruct->pt.x, mouseHookStruct->pt.y);
					controller->InvokeMouseHook(wParam, location);
				}
				return CallNextHookEx(NULL,nCode,wParam,lParam);
			}

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			INativeController* CreateWindowsNativeController(HINSTANCE hInstance)
			{
				return new WindowsController(hInstance);
			}

			IWindowsForm* GetWindowsForm(INativeWindow* window)
			{
				return dynamic_cast<WindowsForm*>(window);
			}

			void DestroyWindowsNativeController(INativeController* controller)
			{
				delete controller;
			}
		}
	}
}
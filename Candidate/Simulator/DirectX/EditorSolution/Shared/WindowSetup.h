#ifndef VCZH_WINDOWSETUP
#define VCZH_WINDOWSETUP

#include <Windows.h>

namespace vl
{
	namespace directx
	{
		struct WindowMouseInfo
		{
			bool						ctrl;
			bool						shift;
			bool						left;
			bool						middle;
			bool						right;
			int							x;
			int							y;
			int							wheel;

			WindowMouseInfo(WPARAM wParam, LPARAM lParam, bool wheelMessage);
		};

		struct WindowKeyInfo
		{
			DWORD						key;
			bool						alt;

			WindowKeyInfo(WPARAM wParam, LPARAM lParam);
		};

		extern void							WindowTrackMouse(HWND handle, bool enable);
		extern SIZE							WindowGetClient(HWND handle);

		/*
		WM_LBUTTONDOWN
			SetCapture
			ReleaseCapture
		WM_LBUTTONUP
		WM_LBUTTONDBLCLK
		WM_RBUTTONDOWN
		WM_RBUTTONUP
		WM_RBUTTONDBLCLK
		WM_MBUTTONDOWN
		WM_MBUTTONUP
		WM_MBUTTONDBLCLK
		WM_MOUSEHWHEEL
		WM_MOUSEWHEEL
		WM_MOUSEMOVE
			TrackMouse(true);
		WM_MOUSELEAVE
		WM_MOUSEHOVER
		WM_KEYUP
		WM_KEYDOWN
		*/
	}
}

#endif
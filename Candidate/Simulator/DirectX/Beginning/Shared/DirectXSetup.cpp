#include "DirectXSetup.h"

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <Windows.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

using namespace vl;
using namespace vl::directx;

/***********************************************************************
DirectXSetup
***********************************************************************/

HRESULT SetupDirectXEnvironment(DirectXEnvironment* env, HWND outputWindow, int clientWidth, int clientHeight)
{
	//=========================================
	// define device
	ZeroMemory(env, sizeof(*env));
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = clientWidth;
	sd.BufferDesc.Height = clientHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = outputWindow;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	
	//=========================================
	// create device
	D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_10_1;
    HRESULT hr = S_OK;
    if(FAILED(hr = D3D11CreateDeviceAndSwapChain(
		NULL, 
        D3D_DRIVER_TYPE_HARDWARE,
        NULL, 
        0,
        &featureLevels, 
        1, 
        D3D11_SDK_VERSION, 
        &sd, 
		&env->swapChain, 
		&env->device, 
        &env->featureLevel,
		&env->context
		))
	)
    {
        return hr;
    }
	
	//=========================================
	// create rasterizer state
	{
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		if(FAILED(hr=env->device->CreateRasterizerState(&rasterDesc, &env->rasterizerState)))
			return hr;
		env->context->RSSetState(env->rasterizerState);
	}
	
	//=========================================
	// succeeded
	return S_OK;
}

void ReleaseDirectXEnvironment(DirectXEnvironment* env)
{
	env->rasterizerState->Release();
	env->swapChain->Release();
	env->device->Release();
	env->context->Release();
}

/***********************************************************************
Interface
***********************************************************************/

namespace vl
{
	namespace directx
	{
		DirectXEnvironment* directXEnvironment=0;

		const DirectXEnvironment* CreateDirectXEnvironment(HWND mainWindowHandle)
		{
			if(!directXEnvironment)
			{
				directXEnvironment=new DirectXEnvironment;
				SIZE client=WindowGetClient(mainWindowHandle);

				SetupDirectXEnvironment(directXEnvironment, mainWindowHandle, client.cx, client.cy);
			}
			return directXEnvironment;
		}

		const void DestroyDirectXEnvironment()
		{
			if(directXEnvironment)
			{
				ReleaseDirectXEnvironment(directXEnvironment);
				delete directXEnvironment;
				directXEnvironment=0;
			}
		}
	}
}
using namespace vl;
using namespace vl::directx;

/***********************************************************************
Window
***********************************************************************/

extern LRESULT CALLBACK DirectXProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& callDefWindowProc);
extern void CALLBACK DirectXProcIdle();

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool callDefWindowProc=true;
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		callDefWindowProc=false;
		break;
	}

	LRESULT result=DirectXProc(hwnd, uMsg, wParam, lParam, callDefWindowProc);
	if(!callDefWindowProc)
	{
		return result;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK PrimaryMonitorSelector(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	MONITORINFOEX info;
	info.cbSize=sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &info);
	if(info.dwFlags==MONITORINFOF_PRIMARY)
	{
		HMONITOR* pMonitor=(HMONITOR*)dwData;
		*pMonitor=hMonitor;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void RunWindow(HINSTANCE hInstance, int clientWidth, int clientHeight)
{
	//=========================================
	// global variables
	WNDCLASSEX windowClass;
	HWND mainWindowHandle;

	//=========================================
	// create class
	{
		windowClass.cbSize=sizeof(windowClass);
		windowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
		windowClass.lpfnWndProc=WndProc;
		windowClass.cbClsExtra=0;
		windowClass.cbWndExtra=0;
		windowClass.hInstance=hInstance;
		windowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		windowClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		windowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
		windowClass.lpszMenuName=NULL;
		windowClass.lpszClassName=L"VczhDirectXWindowClass";
		windowClass.hIconSm=NULL;
		RegisterClassEx(&windowClass);
	}

	//=========================================
	// create window
	{
		DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
		DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		mainWindowHandle=CreateWindowEx(exStyle, windowClass.lpszClassName, L"Vczh DirectX Window", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	}
	
	//=========================================
	// calculate position
	{
		HMONITOR primaryMonitor=0;
		EnumDisplayMonitors(NULL, NULL, PrimaryMonitorSelector, (LPARAM)(&primaryMonitor));
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize=sizeof(MONITORINFOEX);
		GetMonitorInfo(primaryMonitor, &monitorInfo);
	
		RECT required={0, 0, clientWidth, clientHeight};
		AdjustWindowRect(&required, GetWindowLongPtr(mainWindowHandle, GWL_STYLE), FALSE);

		int width=required.right-required.left;
		int height=required.bottom-required.top;
		int left=monitorInfo.rcWork.left+(monitorInfo.rcWork.right-monitorInfo.rcWork.left-width)/2;
		int top=monitorInfo.rcWork.top+(monitorInfo.rcWork.bottom-monitorInfo.rcWork.top-height)/2;
		MoveWindow(mainWindowHandle, left, top, width, height, TRUE);
	}
	
	//=========================================
	// run
	{
		ShowWindow(mainWindowHandle, SW_SHOWNORMAL);
		MSG message;
		while(true)
		{
			if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				if(message.message==WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			DirectXProcIdle();
		}
	}
	
	//=========================================
	// exit
	DestroyWindow(mainWindowHandle);
}

/***********************************************************************
WinMain
***********************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	RunWindow(hInstance, 800, 600);
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

/***********************************************************************
DirectXHelper
***********************************************************************/

namespace vl
{
	namespace directx
	{
		void CreatePixelShader(
			const DirectXEnvironment* env,
			const WString& fileName,
			const WString& functionName,
			ID3D11PixelShader** pixelShader
			)
		{
			ID3D10Blob* psBlob=0;
			*pixelShader=0;

			D3DX11CompileFromFile(fileName.Buffer(), 0, 0, wtoa(functionName).Buffer(), "ps_4_0", 0, 0, 0, &psBlob, 0, 0);
			env->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, pixelShader);
		}

		void CreateVertexShader(
			const DirectXEnvironment* env,
			const WString& fileName,
			const WString& functionName,
			ID3D11VertexShader** vertexShader,
			D3D11_INPUT_ELEMENT_DESC* inputElementDescs,
			int inputElementDescsCount,
			ID3D11InputLayout** inputLayout
			)
		{
			ID3D10Blob* vsBlob=0;
			*vertexShader=0;
			D3DX11CompileFromFile(fileName.Buffer(), 0, 0, wtoa(functionName).Buffer(), "vs_4_0", 0, 0, 0, &vsBlob, 0, 0);
			env->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, vertexShader);

			if(inputLayout)
			{
				*inputLayout=0;
				env->device->CreateInputLayout(inputElementDescs, inputElementDescsCount, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout);
			}
		}

		void WriteBuffer(
			const DirectXEnvironment* env,
			ID3D11Buffer* buffer,
			void* data,
			int size
			)
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			env->context->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, data, size);
			env->context->Unmap(buffer, NULL);
		}

		void CreateBuffer(
			const DirectXEnvironment* env,
			int size,
			UINT type,
			ID3D11Buffer** buffer
			)
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = size;
			bd.BindFlags = type;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			env->device->CreateBuffer(&bd, NULL, buffer);
		}

		void CreateVertexBuffer(
			const DirectXEnvironment* env,
			int size,
			ID3D11Buffer** buffer
			)
		{
			CreateBuffer(env, size, D3D11_BIND_VERTEX_BUFFER, buffer);
		}

		void CreateIndexBuffer(
			const DirectXEnvironment* env,
			int size,
			ID3D11Buffer** buffer
			)
		{
			CreateBuffer(env, size, D3D11_BIND_INDEX_BUFFER, buffer);
		}

		void CreateConstantBuffer(
			const DirectXEnvironment* env,
			int size,
			ID3D11Buffer** buffer
			)
		{
			CreateBuffer(env, size, D3D11_BIND_CONSTANT_BUFFER, buffer);
		}

/***********************************************************************
WindowMessageHelper
***********************************************************************/

		WindowMouseInfo::WindowMouseInfo(WPARAM wParam, LPARAM lParam, bool wheelMessage)
		{
			if(wheelMessage)
			{
				this->wheel=GET_WHEEL_DELTA_WPARAM(wParam);
				wParam=GET_KEYSTATE_WPARAM(wParam);
			}
			else
			{
				this->wheel=0;
			}
			this->ctrl=(wParam & MK_CONTROL)!=0;
			this->shift=(wParam & MK_SHIFT)!=0;
			this->left=(wParam & MK_LBUTTON)!=0;
			this->middle=(wParam & MK_MBUTTON)!=0;
			this->right=(wParam & MK_RBUTTON)!=0;
			POINTS Point=MAKEPOINTS(lParam);
			this->x=Point.x;
			this->y=Point.y;
		}

		WindowKeyInfo::WindowKeyInfo(WPARAM wParam, LPARAM lParam)
		{
			this->key=wParam;
			this->alt=(lParam>>29)%1==1;
		}

		void WindowTrackMouse(HWND handle, bool enable)
		{
			TRACKMOUSEEVENT trackMouseEvent;
			trackMouseEvent.cbSize=sizeof(trackMouseEvent);
			trackMouseEvent.hwndTrack=handle;
			trackMouseEvent.dwFlags=(enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
			trackMouseEvent.dwHoverTime=HOVER_DEFAULT;
			TrackMouseEvent(&trackMouseEvent);
		}

		SIZE WindowGetClient(HWND handle)
		{
			RECT required={0,0,0,0};
			RECT bounds;
			GetWindowRect(handle, &bounds);
			AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
			int width=(bounds.right-bounds.left)-(required.right-required.left);
			int height=(bounds.bottom-bounds.top)-(required.bottom-required.top);

			SIZE size;
			size.cx=width;
			size.cy=height;
			return size;
		}
	}
}
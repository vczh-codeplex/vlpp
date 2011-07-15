#ifndef VCZH_DIRECTXSETUP
#define VCZH_DIRECTXSETUP

#include "..\..\..\..\..\Library\String.h"

#include <D3D11.h>
#include <d3dx11.h>
#include <d3dx10.h>

namespace vl
{
	namespace directx
	{
		struct DirectXEnvironment
		{
			IDXGISwapChain*					swapChain;
			ID3D11Device*					device;
			ID3D11DeviceContext*			context;
			D3D_FEATURE_LEVEL				featureLevel;
			ID3D11RasterizerState*			rasterizerState;
		};
		extern const DirectXEnvironment*	CreateDirectXEnvironment(HWND mainWindowHandle);
		extern const void					DestroyDirectXEnvironment();

		extern void							CreatePixelShader(
												const DirectXEnvironment* env,
												const WString& fileName,
												const WString& functionName,
												ID3D11PixelShader** pixelShader
												);
		extern void							CreateVertexShader(
												const DirectXEnvironment* env,
												const WString& fileName,
												const WString& functionName,
												ID3D11VertexShader** vertexShader,
												D3D11_INPUT_ELEMENT_DESC* inputElementDescs=0,
												int inputElementDescsCount=0,
												ID3D11InputLayout** inputLayout=0
												);
		extern void							WriteBuffer(
												const DirectXEnvironment* env,
												ID3D11Buffer* buffer,
												void* data,
												int size
												);
		
		extern void							CreateBuffer(
												const DirectXEnvironment* env,
												int size,
												UINT type,
												ID3D11Buffer** buffer
												);
		extern void							CreateVertexBuffer(
												const DirectXEnvironment* env,
												int size,
												ID3D11Buffer** buffer
												);
		extern void							CreateIndexBuffer(
												const DirectXEnvironment* env,
												int size,
												ID3D11Buffer** buffer
												);
		extern void							CreateConstantBuffer(
												const DirectXEnvironment* env,
												int size,
												ID3D11Buffer** buffer
												);

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
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
			HWND							outputWindow;
			IDXGISwapChain*					swapChain;
			ID3D11Device*					device;
			ID3D11DeviceContext*			context;
			D3D_FEATURE_LEVEL				featureLevel;
			ID3D11RasterizerState*			rasterizerState;

			DirectXEnvironment(HWND _outputWindow, int clientWidth, int clientHeight);
			~DirectXEnvironment();

			void							Resize(int clientWidth, int clientHeight);
		};

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
	}
}

#endif
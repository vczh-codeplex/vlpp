#include "DirectXSetup.h"

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
using namespace vl;
using namespace vl::directx;

namespace vl
{
	namespace directx
	{

/***********************************************************************
DirectXEnvironment
***********************************************************************/

		DirectXEnvironment::DirectXEnvironment(HWND outputWindow, int clientWidth, int clientHeight)
		{
			SetupDirectXEnvironment(this, outputWindow, clientWidth, clientHeight);
		}

		DirectXEnvironment::~DirectXEnvironment()
		{
			ReleaseDirectXEnvironment(this);
		}

/***********************************************************************
DirectXHelper
***********************************************************************/

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
	}
}
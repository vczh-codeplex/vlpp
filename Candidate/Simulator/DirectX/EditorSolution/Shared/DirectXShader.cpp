#include "DirectXShader.h"

namespace vl
{
	namespace directx
	{

/***********************************************************************
DirectXTextureBuffer
***********************************************************************/

		DirectXTextureBuffer::DirectXTextureBuffer(const DirectXEnvironment* _env)
			:env(_env)
			,texture(0)
			,shaderResourceView(0)
		{
		}

		DirectXTextureBuffer::~DirectXTextureBuffer()
		{
			if(texture) texture->Release();
			if(shaderResourceView) shaderResourceView->Release();
		}
			
		void DirectXTextureBuffer::VSBindToRegisterTN(int index)
		{
			env->context->VSSetShaderResources(index, 1, &shaderResourceView);
		}

		void DirectXTextureBuffer::PSBindToRegisterTN(int index)
		{
			env->context->PSSetShaderResources(index, 1, &shaderResourceView);
		}

		void DirectXTextureBuffer::Update(const WString& fileName)
		{
			if(texture)
			{
				texture->Release();
				texture=0;
			}
			if(shaderResourceView)
			{
				shaderResourceView->Release();
				shaderResourceView=0;
			}
			D3DX11CreateShaderResourceViewFromFile(env->device, fileName.Buffer(), NULL, NULL, &shaderResourceView, NULL);
		}

		void DirectXTextureBuffer::Update(int width, int height, int arraySize, bool forCubeMap, DXGI_FORMAT format)
		{
			if(texture)
			{
				texture->Release();
				texture=0;
			}
			if(shaderResourceView)
			{
				shaderResourceView->Release();
				shaderResourceView=0;
			}
			HRESULT hr=S_OK;
			{
				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));
				textureDesc.Width = width;
				textureDesc.Height = height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = arraySize==-1?1:arraySize;
				textureDesc.Format = format;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = forCubeMap?D3D11_RESOURCE_MISC_TEXTURECUBE:0;
				if(FAILED(hr=env->device->CreateTexture2D(&textureDesc, NULL, &texture)))
					return;
			}
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
				shaderResourceViewDesc.Format = format;
				if(arraySize==-1)
				{
					shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
					shaderResourceViewDesc.Texture2D.MipLevels = 1;
				}
				else if(!forCubeMap)
				{
					shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
					shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
					shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					shaderResourceViewDesc.Texture2DArray.ArraySize = arraySize;
				}
				if(FAILED(hr=env->device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView)))
					return;
			}
		}

		void DirectXTextureBuffer::UpdateUint(int width, int height)
		{
			Update(width, height, -1, false, DXGI_FORMAT_R8G8B8A8_UINT);
		}

/***********************************************************************
DirectXTextureBuffer
***********************************************************************/

		DirectXCubeMapReference::DirectXCubeMapReference(const DirectXEnvironment* _env)
			:env(_env)
			,shaderResourceView(0)
		{
		}

		DirectXCubeMapReference::~DirectXCubeMapReference()
		{
			if(shaderResourceView) shaderResourceView->Release();
		}
			
		void DirectXCubeMapReference::VSBindToRegisterTN(int index)
		{
			env->context->VSSetShaderResources(index, 1, &shaderResourceView);
		}

		void DirectXCubeMapReference::PSBindToRegisterTN(int index)
		{
			env->context->PSSetShaderResources(index, 1, &shaderResourceView);
		}

		void DirectXCubeMapReference::Update(DirectXTextureBuffer* textureArray)
		{
			if(shaderResourceView)
			{
				shaderResourceView->Release();
				shaderResourceView=0;
			}
			
			HRESULT hr=S_OK;
			D3D11_TEXTURE2D_DESC textureDesc;
			textureArray->RawTexture()->GetDesc(&textureDesc);
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
			shaderResourceViewDesc.TextureCube.MipLevels = 1;
			if(FAILED(hr=env->device->CreateShaderResourceView(textureArray->RawTexture(), &shaderResourceViewDesc, &shaderResourceView)))
				return;
		}

/***********************************************************************
SamplerBuffer
***********************************************************************/

		DirectXSamplerBuffer::DirectXSamplerBuffer(const DirectXEnvironment* _env)
			:env(_env)
			,sampler(0)
		{
		}

		DirectXSamplerBuffer::~DirectXSamplerBuffer()
		{
			if(sampler) sampler->Release();
		}

		void DirectXSamplerBuffer::VSBindToRegisterSN(int index)
		{
			env->context->VSSetSamplers(index, 1, &sampler);
		}

		void DirectXSamplerBuffer::PSBindToRegisterSN(int index)
		{
			env->context->PSSetSamplers(index, 1, &sampler);
		}

		void DirectXSamplerBuffer::Update(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, D3DXCOLOR borderColor)
		{
			if(sampler)
			{
				sampler->Release();
				sampler=0;
			}

			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));

			samplerDesc.Filter = filter;
			samplerDesc.AddressU = mode;
			samplerDesc.AddressV = mode;
			samplerDesc.AddressW = mode;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = borderColor.operator FLOAT *()[0];
			samplerDesc.BorderColor[1] = borderColor.operator FLOAT *()[1];
			samplerDesc.BorderColor[2] = borderColor.operator FLOAT *()[2];
			samplerDesc.BorderColor[3] = borderColor.operator FLOAT *()[3];
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			env->device->CreateSamplerState(&samplerDesc, &sampler);
		}

/***********************************************************************
DirectXDepthBuffer
***********************************************************************/

		DirectXDepthBuffer::DirectXDepthBuffer(const DirectXEnvironment* _env)
			:env(_env)
			,depthStencilBuffer(0)
			,depthStencilState(0)
			,depthStencilView(0)
		{
		}

		DirectXDepthBuffer::~DirectXDepthBuffer()
		{
			if(depthStencilView) depthStencilView->Release();
			if(depthStencilState) depthStencilState->Release();
			if(depthStencilBuffer) depthStencilBuffer->Release();
		}

		void DirectXDepthBuffer::Update(int width, int height)
		{
			if(depthStencilView) depthStencilView->Release();
			if(depthStencilState) depthStencilState->Release();
			if(depthStencilBuffer) depthStencilBuffer->Release();
			depthStencilView=0;
			depthStencilState=0;
			depthStencilBuffer=0;

			HRESULT hr=S_OK;
			//=========================================
			// create depth buffer
			{
				D3D11_TEXTURE2D_DESC depthBufferDesc;
				ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
				depthBufferDesc.Width = width;
				depthBufferDesc.Height = height;
				depthBufferDesc.MipLevels = 1;
				depthBufferDesc.ArraySize = 1;
				depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthBufferDesc.SampleDesc.Count = 1;
				depthBufferDesc.SampleDesc.Quality = 0;
				depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthBufferDesc.CPUAccessFlags = 0;
				depthBufferDesc.MiscFlags = 0;
				if(FAILED(hr=env->device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer)))
					return;
			}
	
			//=========================================
			// create depth stencil state
			{
				D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
				ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

				depthStencilDesc.StencilEnable = false;
				depthStencilDesc.StencilReadMask = 0xFF;
				depthStencilDesc.StencilWriteMask = 0xFF;

				depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
				depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
				depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				if(FAILED(hr=env->device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
					return;
				env->context->OMSetDepthStencilState(depthStencilState, 1);
			}
	
			//=========================================
			// create depth stencil view
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
				ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

				depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;

				if(FAILED(hr=env->device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView)))
					return;
			}
		}

		void DirectXDepthBuffer::Clear()
		{
			env->context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

/***********************************************************************
DirectXRenderTarget
***********************************************************************/

		DirectXRenderTarget::DirectXRenderTarget(const DirectXEnvironment* _env)
			:env(_env)
			,renderTargetView(0)
		{
		}

		DirectXRenderTarget::~DirectXRenderTarget()
		{
			if(renderTargetView) renderTargetView->Release();
		}

		void DirectXRenderTarget::Clear(D3DXCOLOR color)
		{
			env->context->ClearRenderTargetView(renderTargetView, color);
		}

/***********************************************************************
DirectXWindowRenderTarget
***********************************************************************/

		DirectXWindowRenderTarget::DirectXWindowRenderTarget(const DirectXEnvironment* _env)
			:DirectXRenderTarget(_env)
		{
			HRESULT hr=S_OK;
			//=========================================
			// create render target
			ID3D11Texture2D* texture2DBackBuffer=0;
			env->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture2DBackBuffer);
			if(FAILED(hr = env->device->CreateRenderTargetView(texture2DBackBuffer, NULL, &renderTargetView)))
				return;
			texture2DBackBuffer->Release();
		}

		DirectXWindowRenderTarget::~DirectXWindowRenderTarget()
		{
		}

/***********************************************************************
DirectXTextureRenderTarget
***********************************************************************/

		DirectXTextureRenderTarget::DirectXTextureRenderTarget(const DirectXEnvironment* _env)
			:DirectXRenderTarget(_env)
		{
		}

		DirectXTextureRenderTarget::~DirectXTextureRenderTarget()
		{
		}

		void DirectXTextureRenderTarget::Update(DirectXTextureBuffer* textureBuffer, int arrayIndex)
		{
			if(renderTargetView)
			{
				renderTargetView->Release();
				renderTargetView=0;
			}

			HRESULT hr=S_OK;
			D3D11_TEXTURE2D_DESC textureDesc;
			textureBuffer->RawTexture()->GetDesc(&textureDesc);

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			renderTargetViewDesc.Format = textureDesc.Format;
			if(arrayIndex==-1)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
			}
			else
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				renderTargetViewDesc.Texture2DArray.MipSlice = 0;
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = arrayIndex;
				renderTargetViewDesc.Texture2DArray.ArraySize = 1;
			}
			if(FAILED(hr=env->device->CreateRenderTargetView(textureBuffer->RawTexture(), &renderTargetViewDesc, &renderTargetView)))
				return;
		}

/***********************************************************************
DirectXRenderer
***********************************************************************/

		DirectXRenderer::DirectXRenderer(const DirectXEnvironment* _env)
			:env(_env)
		{
		}

		DirectXRenderer::~DirectXRenderer()
		{
		}

		void DirectXRenderer::SetRenderTarget(DirectXRenderTarget* target, DirectXDepthBuffer* depthBuffer)
		{
			ID3D11RenderTargetView* renderTargetView=target?target->RawRenderTargetView():NULL;
			env->context->OMSetRenderTargets((target?1:0), &renderTargetView, (depthBuffer?depthBuffer->RawDepthStencilView():NULL));
		}

/***********************************************************************
DirectXViewport
***********************************************************************/

		DirectXViewport::DirectXViewport(const DirectXEnvironment* _env)
			:env(_env)
		{
		}

		DirectXViewport::~DirectXViewport()
		{
		}

		void DirectXViewport::SetViewport(int left, int top, int width, int height, float fieldOfView, float screenNear, float screenFar)
		{
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
			viewport.TopLeftX = (FLOAT)left;
			viewport.TopLeftY = (FLOAT)top;
			viewport.Width = (FLOAT)width;
			viewport.Height = (FLOAT)height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			env->context->RSSetViewports(1, &viewport);

			float screenAspect = (float)width / (float)height;
			D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenFar);
		}
	}
}
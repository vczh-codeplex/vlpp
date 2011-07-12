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
		{
		}

		DirectXTextureBuffer::~DirectXTextureBuffer()
		{
			if(texture) texture->Release();
		}
			
		void DirectXTextureBuffer::VSBindToRegisterTN(int index)
		{
			env->context->VSSetShaderResources(index, 1, &texture);
		}

		void DirectXTextureBuffer::PSBindToRegisterTN(int index)
		{
			env->context->PSSetShaderResources(index, 1, &texture);
		}

		void DirectXTextureBuffer::Update(const WString& fileName)
		{
			if(texture)
			{
				texture->Release();
				texture=0;
			}
			D3DX11CreateShaderResourceViewFromFile(env->device, fileName.Buffer(), NULL, NULL, &texture, NULL);
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
	}
}
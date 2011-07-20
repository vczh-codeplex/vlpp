/***********************************************************************
DirectX Helper

Classes:
	DirectXShader<VertexType>
	DirectXVertexBuffer<VertexType>
	DirectXConstantBuffer<ContentType>
	DirectXTextureBuffer
	DirectXCubeMapReference
	DirectXSamplerBuffer
	DirectXDepthBuffer
	DirectXRenderTarget
		DirectXWindowRenderTarget
		DirectXTextureRenderTarget
	DirectXRenderer
	DirectXViewport
***********************************************************************/
#ifndef VCZH_DIRECTXSHADER
#define VCZH_DIRECTXSHADER

#include "DirectXSetup.h"
#include "..\..\..\..\..\Library\Collections\List.h"
#include "..\..\..\..\..\Library\Collections\Pair.h"

namespace vl
{
	namespace directx
	{

/***********************************************************************
Type Format Lookup
***********************************************************************/

		template<typename T>
		struct DirectXTypeFormatLookup{};

		template<>
		struct DirectXTypeFormatLookup<D3DXVECTOR2>{static const DXGI_FORMAT Format = DXGI_FORMAT_R32G32_FLOAT;};

		template<>
		struct DirectXTypeFormatLookup<D3DXVECTOR3>{static const DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;};

		template<>
		struct DirectXTypeFormatLookup<D3DXVECTOR4>{static const DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;};

		template<>
		struct DirectXTypeFormatLookup<D3DXCOLOR>{static const DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;};

/***********************************************************************
Shader
***********************************************************************/

		template<typename T>
		class DirectXShader : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11VertexShader*			vertexShader;
			ID3D11InputLayout*			vertexLayout;
			ID3D11PixelShader*			pixelShader;
		public:
			class Creater
			{
				typedef collections::List<collections::Pair<AString, DXGI_FORMAT>> DescList;
			protected:
				DirectXShader<T>*		shader;
				WString					fileName, vertexShaderName, pixelShaderName;
				DescList				descriptions;
			public:
				Creater(DirectXShader<T>* _shader, const WString& _fileName, const WString& _vertexShaderName, const WString& _pixelShaderName)
					:shader(_shader)
					,fileName(_fileName)
					,vertexShaderName(_vertexShaderName)
					,pixelShaderName(_pixelShaderName)
				{
				}

				~Creater()
				{
					D3D11_INPUT_ELEMENT_DESC* ied=new D3D11_INPUT_ELEMENT_DESC[descriptions.Count()];
					for(int i=0;i<descriptions.Count();i++)
					{
						D3D11_INPUT_ELEMENT_DESC desc={descriptions[i].key.Buffer(), 0, descriptions[i].value, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
						ied[i]=desc;
					}

					CreatePixelShader(shader->env, fileName, pixelShaderName, &shader->pixelShader);
					CreateVertexShader(shader->env, fileName, vertexShaderName, &shader->vertexShader, ied, descriptions.Count(), &shader->vertexLayout);
					delete[] ied;
				}

				template<typename FieldType>
				Creater& Field(const WString& name, FieldType T::* field)
				{
					descriptions.Add(collections::Pair<AString, DXGI_FORMAT>(wtoa(name), DirectXTypeFormatLookup<FieldType>::Format));
					return *this;
				}
			};
		public:
			DirectXShader(const DirectXEnvironment* _env)
				:env(_env)
				,vertexShader(0)
				,vertexLayout(0)
				,pixelShader(0)
			{
			}

			~DirectXShader()
			{
				if(vertexShader) vertexShader->Release();
				if(vertexLayout) vertexLayout->Release();
				if(pixelShader) pixelShader->Release();
			}

			Creater Fill(const WString& fileName, const WString& vertexShaderName, const WString& pixelShaderName)
			{
				return Creater(this, fileName, vertexShaderName, pixelShaderName);
			}

			void SetCurrent()
			{
				env->context->PSSetShader(pixelShader, 0, 0);
				env->context->VSSetShader(vertexShader, 0, 0);
				env->context->IASetInputLayout(vertexLayout);
			}

			ID3D11VertexShader* RawVertexShader(){return vertexShader;}
			ID3D11InputLayout* RawVertexLayout(){return vertexLayout;}
			ID3D11PixelShader* RawPixelShader(){return pixelShader;}
		};

/***********************************************************************
Vertex Buffer
***********************************************************************/

		template<typename T>
		class DirectXVertexBuffer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11Buffer*				vertexBuffer;
			ID3D11Buffer*				indexBuffer;
			int							indexBufferLength;
		public:
			DirectXVertexBuffer(const DirectXEnvironment* _env)
				:env(_env)
				,vertexBuffer(0)
				,indexBuffer(0)
				,indexBufferLength(0)
			{
			}

			~DirectXVertexBuffer()
			{
				if(vertexBuffer) vertexBuffer->Release();
				if(indexBuffer) indexBuffer->Release();
			}

			void Fill(T* vertices, int vertexCount, unsigned int* indices, int indexCount)
			{
				if(!vertexBuffer) CreateVertexBuffer(env, sizeof(T)*vertexCount, &vertexBuffer);
				if(!indexBuffer) CreateIndexBuffer(env, sizeof(unsigned int)*indexCount, &indexBuffer);

				WriteBuffer(env, vertexBuffer, vertices, sizeof(T)*vertexCount);
				WriteBuffer(env, indexBuffer, indices, sizeof(unsigned int)*indexCount);

				indexBufferLength=indexCount;
			}

			template<int VertexCount, int IndexCount>
			void Fill(T (&vertices)[VertexCount], unsigned int (&indices)[IndexCount])
			{
				Fill(vertices, VertexCount, indices, IndexCount);
			}

			void SetCurrent(D3D11_PRIMITIVE_TOPOLOGY topology=D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			{
				UINT stride=sizeof(T);
				UINT offset=0;
				env->context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
				env->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
				env->context->IASetPrimitiveTopology(topology);
			}

			void Render()
			{
				env->context->DrawIndexed(indexBufferLength, 0, 0);
			}

			void Render(int indexStart, int indexCount)
			{
				env->context->DrawIndexed(indexCount, indexStart, 0);
			}

			void SetCurrentAndRender(DirectXShader<T>* shader, D3D11_PRIMITIVE_TOPOLOGY topology=D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			{
				shader->SetCurrent();
				SetCurrent(topology);
				Render();
			}

			ID3D11Buffer* RawVertexBuffer(){return vertexBuffer;}
			ID3D11Buffer* RawIndexBuffer(){return indexBuffer;}
		};

/***********************************************************************
Constant Buffer
***********************************************************************/

		template<typename T>
		class DirectXConstantBuffer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			T							value;
			ID3D11Buffer*				buffer;
		public:
			DirectXConstantBuffer(const DirectXEnvironment* _env)
				:env(_env)
				,buffer(0)
			{
				int size=sizeof(T);
				if(size%16) size=(size/16+1)*16;
				CreateConstantBuffer(env, size, &buffer);
			}

			~DirectXConstantBuffer()
			{
				if(buffer) buffer->Release();
			}

			T* operator->()
			{
				return &value;
			}

			void Update()
			{
				WriteBuffer(env, buffer, &value, sizeof(T));
			}

			void VSBindToRegisterBN(int index)
			{
				env->context->VSSetConstantBuffers(index, 1, &buffer);
			}

			void PSBindToRegisterBN(int index)
			{
				env->context->PSSetConstantBuffers(index, 1, &buffer);
			}

			ID3D11Buffer* RawBuffer(){return buffer;}
		};

/***********************************************************************
Texture Buffer
***********************************************************************/

		class DirectXTextureBuffer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11Texture2D*			texture;
			ID3D11ShaderResourceView*	shaderResourceView;
		public:
			DirectXTextureBuffer(const DirectXEnvironment* _env);
			~DirectXTextureBuffer();

			ID3D11Texture2D*			RawTexture(){return texture;}
			ID3D11ShaderResourceView*	RawResourceView(){return shaderResourceView;}
			
			void						VSBindToRegisterTN(int index);
			void						PSBindToRegisterTN(int index);

			void						Update(const WString& fileName);
			void						Update(int width, int height, int arraySize=-1, bool forCubeMap=false);
		};

/***********************************************************************
Cube Map
***********************************************************************/

		class DirectXCubeMapReference : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11ShaderResourceView*	shaderResourceView;
		public:
			DirectXCubeMapReference(const DirectXEnvironment* _env);
			~DirectXCubeMapReference();

			ID3D11ShaderResourceView*	RawResourceView(){return shaderResourceView;}
			
			void						VSBindToRegisterTN(int index);
			void						PSBindToRegisterTN(int index);

			void						Update(DirectXTextureBuffer* textureArray);
		};

/***********************************************************************
Sampler Buffer
***********************************************************************/

		class DirectXSamplerBuffer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11SamplerState*			sampler;
		public:
			DirectXSamplerBuffer(const DirectXEnvironment* _env);
			~DirectXSamplerBuffer();

			ID3D11SamplerState*			RawSampler(){return sampler;}

			void						VSBindToRegisterSN(int index);
			void						PSBindToRegisterSN(int index);

			void						Update(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, D3DXCOLOR borderColor);
		};

/***********************************************************************
Depth Buffer
***********************************************************************/

		class DirectXDepthBuffer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11Texture2D*			depthStencilBuffer;
			ID3D11DepthStencilState*	depthStencilState;
			ID3D11DepthStencilView*		depthStencilView;
		public:
			DirectXDepthBuffer(const DirectXEnvironment* _env);
			~DirectXDepthBuffer();

			ID3D11Texture2D*			RawDepthStencilBuffer(){return depthStencilBuffer;}
			ID3D11DepthStencilState*	RawDepthStencilState(){return depthStencilState;}
			ID3D11DepthStencilView*		RawDepthStencilView(){return depthStencilView;}

			void						Update(int width, int height);
			void						Clear();
		};

/***********************************************************************
Render Target
***********************************************************************/

		class DirectXRenderTarget : public Object
		{
		protected:
			const DirectXEnvironment*	env;
			ID3D11RenderTargetView*		renderTargetView;
		public:
			DirectXRenderTarget(const DirectXEnvironment* _env);
			~DirectXRenderTarget();

			ID3D11RenderTargetView*		RawRenderTargetView(){return renderTargetView;}

			void						Clear(D3DXCOLOR color);
		};

		class DirectXWindowRenderTarget : public DirectXRenderTarget
		{
		public:
			DirectXWindowRenderTarget(const DirectXEnvironment* _env);
			~DirectXWindowRenderTarget();
		};

		class DirectXTextureRenderTarget : public DirectXRenderTarget
		{
		protected:
		public:
			DirectXTextureRenderTarget(const DirectXEnvironment* _env);
			~DirectXTextureRenderTarget();

			void						Update(DirectXTextureBuffer* textureBuffer, int arrayIndex=-1);
		};

/***********************************************************************
Renderer
***********************************************************************/

		class DirectXRenderer : public Object
		{
		protected:
			const DirectXEnvironment*	env;
		public:
			DirectXRenderer(const DirectXEnvironment* _env);
			~DirectXRenderer();

			void						SetRenderTarget(DirectXRenderTarget* target, DirectXDepthBuffer* depthBuffer);
		};

/***********************************************************************
Viewport
***********************************************************************/

		class DirectXViewport : public Object
		{
		protected:
			const DirectXEnvironment*	env;
		public:
			DirectXViewport(const DirectXEnvironment* _env);
			~DirectXViewport();

			D3DXMATRIX					projectionMatrix;

			void						SetViewport(int left, int top, int width, int height, float fieldOfView, float screenNear, float screenFar);
		};
	}
}

#endif
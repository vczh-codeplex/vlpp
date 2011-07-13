#include "..\Shared\DirectXSetup.h"
#include "..\Shared\DirectXShader.h"

using namespace vl;
using namespace vl::directx;

struct ConstantBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct ColorVertex
{
    D3DXVECTOR3 Position;
    D3DXCOLOR Color;
};

struct TextureVertex
{
    D3DXVECTOR3 Position;
    D3DXVECTOR2 Texcoord0;
};

struct World
{
private:
	const DirectXEnvironment*					env;
	D3DXMATRIX									viewMatrix, worldMatrix[3];

	DirectXConstantBuffer<ConstantBufferType>	constantBuffer;
	
	DirectXVertexBuffer<ColorVertex>			lightGeometry;
	DirectXVertexBuffer<ColorVertex>			cube1;
	DirectXVertexBuffer<TextureVertex>			cube2;

	DirectXShader<ColorVertex>					colorShader;
	DirectXShader<TextureVertex>				textureShader;
	DirectXTextureBuffer						texture;
	DirectXSamplerBuffer						sampler;

	void WriteConstantBuffer(int worldMatrixIndex)
	{
		D3DXMatrixTranspose(&constantBuffer->world, &worldMatrix[worldMatrixIndex]);
		D3DXMatrixTranspose(&constantBuffer->view, &viewMatrix);
		D3DXMatrixTranspose(&constantBuffer->projection, &env->projectionMatrix);
		constantBuffer.Update();
	}
public:
	World(const DirectXEnvironment* _env)
		:env(_env)
		,constantBuffer(_env)
		,lightGeometry(_env)
		,cube1(_env)
		,cube2(_env)
		,colorShader(_env)
		,textureShader(_env)
		,texture(_env)
		,sampler(_env)
	{
		{
			ColorVertex vertices[] =
			{
				{D3DXVECTOR3( 0,  1,  0),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3( 1,  0,  0),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3( 0,  0,  1),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3(-1,  0,  0),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3( 0,  0, -1),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3( 0, -1,  0),	D3DXCOLOR(1, 1, 1, 1)},
			};

			unsigned int indices[] =
			{
				0, 1, 4,
				0, 4, 3,
				0, 3, 2,
				0, 2, 1,
				5, 1, 2,
				5, 2, 3,
				5, 3, 4,
				5, 4, 1,
			};

			lightGeometry.Fill(vertices, indices);
		}
		{
			ColorVertex vertices[] =
			{
				{D3DXVECTOR3(-1, -1, -1),	D3DXCOLOR(0, 0, 0, 1)},
				{D3DXVECTOR3( 1, -1, -1),	D3DXCOLOR(1, 0, 0, 1)},
				{D3DXVECTOR3( 1,  1, -1),	D3DXCOLOR(1, 1, 0, 1)},
				{D3DXVECTOR3(-1,  1, -1),	D3DXCOLOR(0, 1, 0, 1)},
				{D3DXVECTOR3(-1, -1,  1),	D3DXCOLOR(0, 0, 1, 1)},
				{D3DXVECTOR3( 1, -1,  1),	D3DXCOLOR(1, 0, 1, 1)},
				{D3DXVECTOR3( 1,  1,  1),	D3DXCOLOR(1, 1, 1, 1)},
				{D3DXVECTOR3(-1,  1,  1),	D3DXCOLOR(0, 1, 1, 1)},
			};

			unsigned int indices[] = 
			{
				0, 3, 2,
				0, 2, 1,
				1, 2, 6,
				1, 6, 5,
				5, 6, 7,
				5, 7, 4,
				4, 7, 3,
				4, 3, 0,
				3, 7, 6,
				3, 6, 2,
				4, 0, 1,
				4, 1, 5,
			};

			cube1.Fill(vertices, indices);
		}
		{
			TextureVertex vertices[] =
			{
				{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR2(0, 0)},

				{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR2(0, 0)},

				{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR2(0, 0)},

				{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR2(0, 0)},

				{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR2(0, 0)},

				{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR2(0, 1)},
				{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR2(1, 1)},
				{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR2(1, 0)},
				{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR2(0, 0)},
			};

			unsigned int indices[] =
			{
				0, 3, 2,
				0, 2, 1,
				4, 7, 6,
				4, 6, 5,
				8, 11,10,
				8, 10,9,
				12,15,14,
				12,14,13,
				16,19,18,
				16,18,17,
				20,23,22,
				20,22,21,
			};

			cube2.Fill(vertices, indices);
		}
		{
			colorShader.Fill(L"ColorVertexShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &ColorVertex::Position)
				.Field(L"COLOR", &ColorVertex::Color)
				;

			textureShader.Fill(L"TextureVertexShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &TextureVertex::Position)
				.Field(L"TEXCOORD", &TextureVertex::Texcoord0)
				;

			texture.Update(L"TextureColumn.jpg");
			sampler.Update(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3DXCOLOR(1, 1, 1, 1));
		}
		{
			{
				D3DXMATRIX matrix;
				D3DXMatrixScaling(&worldMatrix[2], 0.2f, 0.2f, 0.2f);

				D3DXMatrixTranslation(&matrix, 3.5f, 0, 0);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);

				D3DXMatrixRotationY(&matrix, (float)D3DX_PI/4);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);

				D3DXMatrixRotationX(&matrix, (float)D3DX_PI/4);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);
			}
			D3DXMatrixTranslation(&worldMatrix[0], -2, 0, 0);
			D3DXMatrixTranslation(&worldMatrix[1], 2, 0, 0);
			D3DXMatrixTranslation(&viewMatrix, 0, 0, 10.0);
		}
	}

	~World()
	{
	}

	void Render()
	{
		env->context->ClearRenderTargetView(env->renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		env->context->ClearDepthStencilView(env->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		{
			constantBuffer.VSBindToRegisterBN(0);

			WriteConstantBuffer(0);
			cube1.SetCurrentAndRender(&colorShader);
			
			WriteConstantBuffer(1);
			texture.PSBindToRegisterTN(0);
			sampler.PSBindToRegisterSN(0);
			cube2.SetCurrentAndRender(&textureShader);

			WriteConstantBuffer(2);
			lightGeometry.SetCurrentAndRender(&colorShader);
		}
		env->swapChain->Present(0, 0);
	}

	void Rotate(float x, float y)
	{
		D3DXMATRIX rotation;
		D3DXMatrixRotationYawPitchRoll(&rotation, -x, -y, 0);
		D3DXMatrixMultiply(&worldMatrix[0], &worldMatrix[0], &rotation);
		D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &rotation);
	}
};
World* world=0;
int oldX=0;
int oldY=0;
bool mouseTracking=false;

void CALLBACK DirectXProcIdle()
{
	if(world)
	{
		world->Render();
	}
}

LRESULT CALLBACK DirectXProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& callDefWindowProc)
{
	switch(uMsg)
	{
	case WM_SHOWWINDOW:
		{
			if(wParam==TRUE)
			{
				if(!world)
				{
					const DirectXEnvironment* env=CreateDirectXEnvironment(hwnd, 0.1f, 100.0f);
					world=new World(env);
				}
			}
		}
		break;
	case WM_DESTROY:
		{
			if(world)
			{
				delete world;
				world=0;
				DestroyDirectXEnvironment();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetCapture(hwnd);
			WindowMouseInfo info(wParam, lParam, false);
			oldX=info.x;
			oldY=info.y;
			mouseTracking=true;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(mouseTracking)
			{
				WindowMouseInfo info(wParam, lParam, false);

				int offsetX=info.x-oldX;
				int offsetY=info.y-oldY;
				float rotateX=(float)D3DX_PI*offsetX/200;
				float rotateY=(float)D3DX_PI*offsetY/200;
				world->Rotate(rotateX, rotateY);

				oldX=info.x;
				oldY=info.y;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			mouseTracking=false;
		}
		break;
	}
	return 0;
}
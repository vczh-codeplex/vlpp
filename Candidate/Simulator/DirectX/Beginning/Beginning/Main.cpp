#include "..\Shared\WindowSetup.h"
#include "ModelBuilder.h"

using namespace vl;

struct ConstantBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXVECTOR4 lightPosition;
	D3DXCOLOR lightColor;
	float lightMinimunDistanceSquare;
	float lightMinimumStrenght;
	float unused0[2];
	D3DXCOLOR environmentColor;
};

class World : public Object
{
public:
	virtual void Render()=0;
	virtual void Rotate(float x, float y)=0;
};

World* CreateCubeAndSphereWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight);
World* CreateSmdWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight);

World* world=0;
DirectXEnvironment* dxenv=0;
int oldX=0;
int oldY=0;
bool mouseTracking=false;

/***********************************************************************
Message Handler
***********************************************************************/

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
					SIZE size=WindowGetClient(hwnd);
					dxenv=new DirectXEnvironment(hwnd, size.cx, size.cy);
					world=CreateCubeAndSphereWorld(dxenv, size.cx, size.cy);
				}
			}
		}
		break;
	case WM_DESTROY:
		{
			if(world)
			{
				delete world;
				delete dxenv;
				world=0;
				dxenv=0;
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

/***********************************************************************
CubeAndSphereWorld
***********************************************************************/

class CubeAndSphereWorld : public World
{
private:
	const DirectXEnvironment*					env;
	int											clientWidth, clientHeight;
	D3DXMATRIX									viewMatrix, worldMatrix[4];

	DirectXConstantBuffer<ConstantBufferType>	constantBuffer;
	DirectXDepthBuffer							depthBuffer;
	DirectXWindowRenderTarget					windowRenderTarget;
	DirectXRenderer								renderer;
	DirectXViewport								viewport;

	DirectXTextureBuffer						cubeMapTextures;
	Ptr<DirectXTextureRenderTarget>				cubeMapRenderTargets[6];
	DirectXDepthBuffer							cubeMapDepthBuffer;
	DirectXCubeMapReference						cubeMap;
	
	DirectXVertexBuffer<LightVertex>			lightGeometry;
	DirectXVertexBuffer<ColorVertex>			cube1;
	DirectXVertexBuffer<TextureVertex>			cube2, sphere;

	DirectXShader<LightVertex>					lightShader;
	DirectXShader<ColorVertex>					colorShader;
	DirectXShader<TextureVertex>				textureShader;
	DirectXTextureBuffer						textureColumn;
	DirectXTextureBuffer						textureEarth;
	DirectXSamplerBuffer						textureSampler;
	DirectXShader<TextureVertex>				cubeShader;

	void WriteConstantBuffer(int worldMatrixIndex)
	{
		D3DXMatrixTranspose(&constantBuffer->world, &worldMatrix[worldMatrixIndex]);
		D3DXMatrixTranspose(&constantBuffer->view, &viewMatrix);
		D3DXMatrixTranspose(&constantBuffer->projection, &viewport.projectionMatrix);
		constantBuffer.Update();
	}
public:
	CubeAndSphereWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight)
		:env(_env)
		,clientWidth(_clientWidth), clientHeight(_clientHeight)
		,constantBuffer(_env)
		,depthBuffer(_env), windowRenderTarget(_env), renderer(_env), viewport(_env)
		,cubeMapTextures(_env), cubeMapDepthBuffer(_env), cubeMap(_env)
		,lightGeometry(_env) ,cube1(_env) ,cube2(_env) ,sphere(_env)
		,lightShader(_env) ,colorShader(_env) ,textureShader(_env)
		,textureColumn(_env) ,textureEarth(_env) ,textureSampler(_env)
		,cubeShader(_env)
	{
		{
			depthBuffer.Update(clientWidth, clientHeight);
			cubeMapDepthBuffer.Update(512, 512);
			cubeMapTextures.Update(512, 512, 6, true);
			for(int i=0;i<6;i++)
			{
				cubeMapRenderTargets[i]=new DirectXTextureRenderTarget(_env);
				cubeMapRenderTargets[i]->Update(&cubeMapTextures, i);
			}
			cubeMap.Update(&cubeMapTextures);
		}
		BuildLightGeometry(lightGeometry);
		BuildColorCube(cube1);
		BuildTextureCube(cube2);
		BuildTextureSphere(sphere);
		{
			lightShader.Fill(L"Shaders/LightShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &LightVertex::Position)
				;

			colorShader.Fill(L"Shaders/ColorShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &ColorVertex::Position)
				.Field(L"NORMAL", &ColorVertex::Normal)
				.Field(L"COLOR", &ColorVertex::Color)
				;

			textureShader.Fill(L"Shaders/TextureShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &TextureVertex::Position)
				.Field(L"NORMAL", &TextureVertex::Normal)
				.Field(L"TEXCOORD", &TextureVertex::Texcoord0)
				;

			textureColumn.Update(L"Shaders/TextureColumn.jpg");
			textureEarth.Update(L"Shaders/earth.bmp");
			textureSampler.Update(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3DXCOLOR(1, 1, 1, 1));

			cubeShader.Fill(L"Shaders/CubeShader.txt", L"VShader", L"PShader")
				.Field(L"POSITION", &TextureVertex::Position)
				.Field(L"NORMAL", &TextureVertex::Normal)
				.Field(L"TEXCOORD", &TextureVertex::Texcoord0)
				;
		}
		{
			{
				D3DXMATRIX scaling;
				D3DXMatrixScaling(&scaling, 1.4f, 1.4f, 1.4f);

				D3DXMatrixTranslation(&worldMatrix[0], -2, 0, 0);
				D3DXMatrixMultiply(&worldMatrix[0], &worldMatrix[0], &scaling);
				D3DXMatrixTranslation(&worldMatrix[1], 2, 0, 0);
				D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &scaling);
			}
			{
				D3DXMATRIX matrix;
				D3DXMatrixScaling(&worldMatrix[2], 0.2f, 0.2f, 0.2f);

				D3DXMatrixTranslation(&matrix, 4.7f, 0, 0);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);

				D3DXMatrixRotationY(&matrix, (float)D3DX_PI/4);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);

				D3DXMatrixRotationX(&matrix, (float)D3DX_PI/5);
				D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &matrix);
			}
			{
				D3DXMatrixScaling(&worldMatrix[3], 1.0f, 1.0f, 1.0f);
			}
		}
		{
			constantBuffer->lightPosition=D3DXVECTOR4(0, 0, 0, 1);
			D3DXVec4Transform(&constantBuffer->lightPosition, &constantBuffer->lightPosition, &worldMatrix[2]);
			constantBuffer->lightColor=D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
			constantBuffer->lightMinimunDistanceSquare=9;
			constantBuffer->lightMinimumStrenght=3;
			constantBuffer->environmentColor=D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		}
	}

	~CubeAndSphereWorld()
	{
	}

	void Render()
	{
		{
			D3DXVECTOR3 ats[]=
			{
				D3DXVECTOR3( 1,  0,  0),
				D3DXVECTOR3(-1,  0,  0),
				D3DXVECTOR3( 0,  1,  0),
				D3DXVECTOR3( 0, -1,  0),
				D3DXVECTOR3( 0,  0,  1),
				D3DXVECTOR3( 0,  0, -1),
			};
			D3DXVECTOR3 ups[]=
			{
				D3DXVECTOR3( 0,  1,  0),
				D3DXVECTOR3( 0,  1,  0),
				D3DXVECTOR3( 0,  0, -1),
				D3DXVECTOR3( 0,  0,  1),
				D3DXVECTOR3( 0,  1,  0),
				D3DXVECTOR3( 0,  1,  0),
			};
			for(int i=0;i<6;i++)
			{
				D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0, 0, 0), &ats[i], &ups[i]);
				renderer.SetRenderTarget(cubeMapRenderTargets[i].Obj(), &cubeMapDepthBuffer);
				viewport.SetViewport(0, 0, 512, 512, (float)D3DX_PI/2, 0.1f, 100.0f);
				cubeMapRenderTargets[i]->Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
				cubeMapDepthBuffer.Clear();

				constantBuffer.VSBindToRegisterBN(0);
				constantBuffer.PSBindToRegisterBN(0);

				WriteConstantBuffer(0);
				cube1.SetCurrentAndRender(&colorShader);
			
				WriteConstantBuffer(1);
				textureColumn.PSBindToRegisterTN(0);
				textureSampler.PSBindToRegisterSN(0);
				cube2.SetCurrentAndRender(&textureShader);

				WriteConstantBuffer(2);
				lightGeometry.SetCurrentAndRender(&lightShader);
			}
		}
		{
			D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0, 0, -10), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
			renderer.SetRenderTarget(&windowRenderTarget, &depthBuffer);
			viewport.SetViewport(0, 0, clientWidth, clientHeight, (float)D3DX_PI/4, 0.1f, 100.0f);
			windowRenderTarget.Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
			depthBuffer.Clear();

			constantBuffer.VSBindToRegisterBN(0);
			constantBuffer.PSBindToRegisterBN(0);

			WriteConstantBuffer(0);
			cube1.SetCurrentAndRender(&colorShader);
			
			WriteConstantBuffer(1);
			textureColumn.PSBindToRegisterTN(0);
			textureSampler.PSBindToRegisterSN(0);
			cube2.SetCurrentAndRender(&textureShader);

			WriteConstantBuffer(2);
			lightGeometry.SetCurrentAndRender(&lightShader);
			
			WriteConstantBuffer(3);
			textureEarth.PSBindToRegisterTN(0);
			textureSampler.PSBindToRegisterSN(0);
			cubeMap.PSBindToRegisterTN(1);
			sphere.SetCurrentAndRender(&cubeShader);
		}
		env->swapChain->Present(0, 0);
	}

	void Rotate(float x, float y)
	{
		D3DXMATRIX rotation;
		D3DXMatrixRotationYawPitchRoll(&rotation, -x, -y, 0);
		D3DXMatrixMultiply(&worldMatrix[0], &worldMatrix[0], &rotation);
		D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &rotation);
		D3DXMatrixMultiply(&worldMatrix[3], &worldMatrix[3], &rotation);
	}
};

World* CreateCubeAndSphereWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight)
{
	return new CubeAndSphereWorld(_env, _clientWidth, _clientHeight);
}

/***********************************************************************
SmdWorld
***********************************************************************/

class SmdWorld : public World
{
private:
	const DirectXEnvironment*					env;
	int											clientWidth, clientHeight;
	D3DXMATRIX									viewMatrix, worldMatrix;

	DirectXConstantBuffer<ConstantBufferType>	constantBuffer;
	DirectXDepthBuffer							depthBuffer;
	DirectXWindowRenderTarget					windowRenderTarget;
	DirectXRenderer								renderer;
	DirectXViewport								viewport;
	
	SmdModel									smdModel;
	DirectXShader<TextureVertex>				textureShader;
	DirectXSamplerBuffer						textureSampler;

	void WriteConstantBuffer()
	{
		D3DXMatrixTranspose(&constantBuffer->world, &worldMatrix);
		D3DXMatrixTranspose(&constantBuffer->view, &viewMatrix);
		D3DXMatrixTranspose(&constantBuffer->projection, &viewport.projectionMatrix);
		constantBuffer.Update();
	}
public:
	SmdWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight)
		:env(_env)
		,clientWidth(_clientWidth), clientHeight(_clientHeight)
		,constantBuffer(_env)
		,depthBuffer(_env), windowRenderTarget(_env), renderer(_env), viewport(_env)
		,smdModel(_env) ,textureShader(_env) ,textureSampler(_env)
	{
		depthBuffer.Update(clientWidth, clientHeight);
		smdModel.Load(L"Shaders/SmdFemale/", L"female_01_reference.smd");
		textureShader.Fill(L"Shaders/TextureShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &TextureVertex::Position)
			.Field(L"NORMAL", &TextureVertex::Normal)
			.Field(L"TEXCOORD", &TextureVertex::Texcoord0)
			;
		textureSampler.Update(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3DXCOLOR(1, 1, 1, 1));

		D3DXMatrixTranslation(&worldMatrix, 0.0f, 0.0f, -30.0f);
		constantBuffer->lightPosition=D3DXVECTOR4(0, 0, 0, 1);
		constantBuffer->lightColor=D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		constantBuffer->lightMinimunDistanceSquare=900;
		constantBuffer->lightMinimumStrenght=3;
		constantBuffer->environmentColor=D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		{
			D3DXMATRIX matrix, lightMatrix;
			D3DXMatrixScaling(&lightMatrix, 0.2f, 0.2f, 0.2f);

			D3DXMatrixTranslation(&matrix, 40.0f, 0, 0);
			D3DXMatrixMultiply(&lightMatrix, &lightMatrix, &matrix);

			D3DXMatrixRotationY(&matrix, (float)D3DX_PI/3);
			D3DXMatrixMultiply(&lightMatrix, &lightMatrix, &matrix);

			D3DXMatrixRotationX(&matrix, (float)D3DX_PI/6);
			D3DXMatrixMultiply(&lightMatrix, &lightMatrix, &matrix);

			D3DXVec4Transform(&constantBuffer->lightPosition, &constantBuffer->lightPosition, &lightMatrix);
		}
	}

	~SmdWorld()
	{
	}

	void Render()
	{
		{
			D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0, 0, -120), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
			renderer.SetRenderTarget(&windowRenderTarget, &depthBuffer);
			viewport.SetViewport(0, 0, clientWidth, clientHeight, (float)D3DX_PI/4, 0.1f, 1000.0f);
			windowRenderTarget.Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
			depthBuffer.Clear();

			constantBuffer.VSBindToRegisterBN(0);
			constantBuffer.PSBindToRegisterBN(0);
			
			WriteConstantBuffer();
			textureSampler.PSBindToRegisterSN(0);
			textureShader.SetCurrent();
			smdModel.SetCurrentAndRender();
		}
		env->swapChain->Present(0, 0);
	}

	void Rotate(float x, float y)
	{
		D3DXMATRIX rotation;
		D3DXMatrixRotationYawPitchRoll(&rotation, -x, -y, 0);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotation);
	}
};

World* CreateSmdWorld(const DirectXEnvironment* _env, int _clientWidth, int _clientHeight)
{
	return new SmdWorld(_env, _clientWidth, _clientHeight);
}
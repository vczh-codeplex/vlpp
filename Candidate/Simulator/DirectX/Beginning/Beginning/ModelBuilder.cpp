#include "ModelBuilder.h"

void BuildLightGeometry(DirectXVertexBuffer<LightVertex>& lightGeometry)
{
	LightVertex vertices[] =
	{
		{D3DXVECTOR3( 0,  1,  0)},
		{D3DXVECTOR3( 1,  0,  0)},
		{D3DXVECTOR3( 0,  0,  1)},
		{D3DXVECTOR3(-1,  0,  0)},
		{D3DXVECTOR3( 0,  0, -1)},
		{D3DXVECTOR3( 0, -1,  0)},
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

void BuildColorCube(DirectXVertexBuffer<ColorVertex>& colorCube)
{
	ColorVertex vertices[] =
	{
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(0, 0, 0, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 0, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(0, 1, 0, 1)},

		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 0, 1)},

		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(0, 1, 1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 0, 0, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 1, 0, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 1, 1, 1)},

		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(0, 1, 0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 0, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(0, 1, 1, 1)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(0, 0, 0, 1)},
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

	colorCube.Fill(vertices, indices);
}

void BuildTextureCube(DirectXVertexBuffer<TextureVertex>& textureCube)
{
	TextureVertex vertices[] =
	{
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(0, 0)},
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

	textureCube.Fill(vertices, indices);
}

void BuildTextureSphere(DirectXVertexBuffer<TextureVertex>& textureSphere)
{
	const int levels = 25;
	const int vertexCount = (levels+1)*(levels+1);
	const int triangleCount = 2*levels*(levels-1);
	TextureVertex* vertices = new TextureVertex[vertexCount];
	unsigned int* indices = new unsigned int[triangleCount*3];

	for(int i=0;i<=levels;i++)
	{
		float ty = i*(float)D3DX_PI/levels;
		float y = cosf(ty);
		float r = sinf(ty);
		for(int j=0;j<=levels;j++)
		{
			float tx = j*(float)D3DX_PI*2/levels;
			float x = -cosf(tx)*r;
			float z = -sinf(tx)*r;
			float ix = (float)j/levels;
			float iy = (float)i/levels;

			TextureVertex* vertex=&vertices[i*(levels+1)+j];
			vertex->Position=D3DXVECTOR3(x, y, z);
			vertex->Normal=D3DXVECTOR3(x, y, z);
			vertex->Texcoord0=D3DXVECTOR2(ix, iy);
		}
	}

	int currentTriangle=0;
	for(int i=0;i<levels;i++)
	{
		for(int j=0;j<levels;j++)
		{
			if(i<levels-1)
			{
				indices[currentTriangle*3+0] = i*(levels+1)+j;
				indices[currentTriangle*3+1] = (i+1)*(levels+1)+j+1;
				indices[currentTriangle*3+2] = (i+1)*(levels+1)+j;
				currentTriangle++;
			}
			if(i>0)
			{
				indices[currentTriangle*3+0] = i*(levels+1)+j;
				indices[currentTriangle*3+1] = i*(levels+1)+j+1;
				indices[currentTriangle*3+2] = (i+1)*(levels+1)+j+1;
				currentTriangle++;
			}
		}
	}

	textureSphere.Fill(vertices, vertexCount, indices, triangleCount*3);
	delete[] vertices;
	delete[] indices;
}
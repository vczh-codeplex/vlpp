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
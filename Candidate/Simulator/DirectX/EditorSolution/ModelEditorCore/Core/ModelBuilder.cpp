#include "ModelBuilder.h"

namespace modeleditor
{
	void BuildCube(Model* model)
	{
		VertexObject vertices[] =
		{
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1)},

			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},

			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},

			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},

			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},

			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1)},
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1)},
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

		model->vertices.Resize(sizeof(vertices)/sizeof(*vertices));
		memcpy(&model->vertices[0], vertices, sizeof(vertices));
		model->indices.Resize(sizeof(indices)/sizeof(*indices));
		memcpy(&model->indices[0], indices, sizeof(indices));
		model->Update();
	}
}
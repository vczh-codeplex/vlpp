#include "ModelBuilder.h"

namespace modeleditor
{
	void CreateNormalNoSmooth(VertexObject* vertices, int count)
	{
		for(int i=0;i<count;i+=3)
		{
			VertexObject* p1=&vertices[i];
			VertexObject* p2=&vertices[i+1];
			VertexObject* p3=&vertices[i+2];

			D3DXVECTOR3 x = p3->position-p2->position;
			D3DXVECTOR3 y = p1->position-p2->position;
			D3DXVECTOR3 z;
			D3DXVec3Cross(&z, &x, &y);
			D3DXVec3Normalize(&z, &z);

			p1->normal=z;
			p2->normal=z;
			p3->normal=z;
		}
	}

	void BuildAxis(Model* model)
	{
		VertexObject vertices[36];
		unsigned int indices[36];

		vertices[ 0+ 0].position=D3DXVECTOR3(5, 0, 0);
		vertices[ 0+12].position=D3DXVECTOR3(0, 5, 0);
		vertices[ 0+24].position=D3DXVECTOR3(0, 0, 5);
		vertices[ 1+ 0].position=D3DXVECTOR3(0, 0, 0);
		vertices[ 1+12].position=D3DXVECTOR3(0, 0, 0);
		vertices[ 1+24].position=D3DXVECTOR3(0, 0, 0);
		vertices[ 2+ 0].position=D3DXVECTOR3(1, 1, 0);
		vertices[ 2+12].position=D3DXVECTOR3(0, 1, 1);
		vertices[ 2+24].position=D3DXVECTOR3(1, 0, 1);

		vertices[ 3+ 0].position=D3DXVECTOR3(5, 0, 0);
		vertices[ 3+12].position=D3DXVECTOR3(0, 5, 0);
		vertices[ 3+24].position=D3DXVECTOR3(0, 0, 5);
		vertices[ 4+ 0].position=D3DXVECTOR3(1, 0, 1);
		vertices[ 4+12].position=D3DXVECTOR3(1, 1, 0);
		vertices[ 4+24].position=D3DXVECTOR3(0, 1, 1);
		vertices[ 5+ 0].position=D3DXVECTOR3(0, 0, 0);
		vertices[ 5+12].position=D3DXVECTOR3(0, 0, 0);
		vertices[ 5+24].position=D3DXVECTOR3(0, 0, 0);

		vertices[ 6+ 0].position=D3DXVECTOR3(5, 0, 0);
		vertices[ 6+12].position=D3DXVECTOR3(0, 5, 0);
		vertices[ 6+24].position=D3DXVECTOR3(0, 0, 5);
		vertices[ 7+ 0].position=D3DXVECTOR3(1, 1, 1);
		vertices[ 7+12].position=D3DXVECTOR3(1, 1, 1);
		vertices[ 7+24].position=D3DXVECTOR3(1, 1, 1);
		vertices[ 8+ 0].position=D3DXVECTOR3(1, 0, 1);
		vertices[ 8+12].position=D3DXVECTOR3(1, 1, 0);
		vertices[ 8+24].position=D3DXVECTOR3(0, 1, 1);

		vertices[ 9+ 0].position=D3DXVECTOR3(5, 0, 0);
		vertices[ 9+12].position=D3DXVECTOR3(0, 5, 0);
		vertices[ 9+24].position=D3DXVECTOR3(0, 0, 5);
		vertices[10+ 0].position=D3DXVECTOR3(1, 1, 0);
		vertices[10+12].position=D3DXVECTOR3(0, 1, 1);
		vertices[10+24].position=D3DXVECTOR3(1, 0, 1);
		vertices[11+ 0].position=D3DXVECTOR3(1, 1, 1);
		vertices[11+12].position=D3DXVECTOR3(1, 1, 1);
		vertices[11+24].position=D3DXVECTOR3(1, 1, 1);

		CreateNormalNoSmooth(vertices, sizeof(vertices)/sizeof(*vertices));
		for(int i=0;i<12;i++)
		{
			vertices[i].color=D3DXCOLOR(1, 0, 0, 1);
			vertices[i+12].color=D3DXCOLOR(0, 1, 0, 1);
			vertices[i+24].color=D3DXCOLOR(0, 0, 1, 1);
			vertices[i].id=0;
		}

		for(int i=0;i<sizeof(indices)/sizeof(*indices);i++)
		{
			indices[i]=i;
		}

		model->vertices.Resize(sizeof(vertices)/sizeof(*vertices));
		memcpy(&model->vertices[0], vertices, sizeof(vertices));
		model->indices.Resize(sizeof(indices)/sizeof(*indices));
		memcpy(&model->indices[0], indices, sizeof(indices));
	}
	void BuildCube(Model* model)
	{
		VertexObject vertices[] =
		{
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},

			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},

			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},

			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},

			{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},

			{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
			{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
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
	}
}
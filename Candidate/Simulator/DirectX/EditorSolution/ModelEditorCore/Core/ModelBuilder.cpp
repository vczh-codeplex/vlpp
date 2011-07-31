#include "ModelBuilder.h"

namespace modeleditor
{
	void BuildCube(Model* model)
	{
		//VertexObject vertices[] =
		//{
		//	{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 1, 1), 0},

		//	{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},

		//	{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1), 0},

		//	{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(1, 1, 1, 1), 0},

		//	{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1), 0},

		//	{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//	{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 1, 1, 1), 0},
		//};

		//unsigned int indices[] =
		//{
		//	0, 3, 2,
		//	0, 2, 1,
		//	4, 7, 6,
		//	4, 6, 5,
		//	8, 11,10,
		//	8, 10,9,
		//	12,15,14,
		//	12,14,13,
		//	16,19,18,
		//	16,18,17,
		//	20,23,22,
		//	20,22,21,
		//};

		//model->vertexBufferVertices.Resize(sizeof(vertices)/sizeof(*vertices));
		//memcpy(&model->vertexBufferVertices[0], vertices, sizeof(vertices));
		//model->vertexBufferIndices.Resize(sizeof(indices)/sizeof(*indices));
		//memcpy(&model->vertexBufferIndices[0], indices, sizeof(indices));

		D3DXVECTOR3 vertices[] = 
		{
			D3DXVECTOR3( 1, -1, -1),
			D3DXVECTOR3( 1, -1,  1),
			D3DXVECTOR3(-1, -1,  1),
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3( 1,  1, -1),
			D3DXVECTOR3( 1,  1,  1),
			D3DXVECTOR3(-1,  1,  1),
			D3DXVECTOR3(-1,  1, -1),
		};

		int faces[][4] = 
		{
			{5, 1, 0, 4},
			{6, 2, 1, 5},
			{7, 3, 2, 6},
			{4, 0, 3, 7},
			{6, 5, 4, 7},
			{1, 2, 3, 0},
		};

		model->modelVertices.Clear();
		model->modelFaces.Clear();

		for(int i=0;i<8;i++)
		{
			Model::Vertex* vertex=new Model::Vertex;
			vertex->position=vertices[i];
			vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
			model->modelVertices.Add(vertex);
		}

		for(int i=0;i<6;i++)
		{
			Model::Face* face=new Model::Face;
			for(int j=0;j<4;j++)
			{
				face->vertexIndices.Add(faces[i][j]);
			}
			model->modelFaces.Add(face);
		}

		model->RebuildVertexBuffer();
	}
}
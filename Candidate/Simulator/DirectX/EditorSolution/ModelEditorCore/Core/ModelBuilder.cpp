#include "ModelBuilder.h"

namespace modeleditor
{
	void BuildCube(Model* model)
	{
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
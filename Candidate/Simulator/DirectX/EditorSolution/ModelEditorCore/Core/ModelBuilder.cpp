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

	void BuildSphere(Model* model, int rows, int cols)
	{
		for(int i=1;i<rows;i++)
		{
			float r=(float)D3DX_PI*i/rows;
			float sinr=sinf(r);
			float cosr=cosf(r);
			for(int j=0;j<cols;j++)
			{
				float c=(float)D3DX_PI*2*j/cols;
				float sinc=sinf(c);
				float cosc=cosf(c);

				Model::Vertex* vertex=new Model::Vertex;
				vertex->position=D3DXVECTOR3(cosc*sinr, cosr, sinc*sinr);
				vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
				model->modelVertices.Add(vertex);
			}
		}
		{
			Model::Vertex* vertex=new Model::Vertex;
			vertex->position=D3DXVECTOR3(0, 1, 0);
			vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
			model->modelVertices.Add(vertex);
		}
		{
			Model::Vertex* vertex=new Model::Vertex;
			vertex->position=D3DXVECTOR3(0, -1, 0);
			vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
			model->modelVertices.Add(vertex);
		}
		int topVertex=(rows-1)*cols;
		int bottomVertex=(rows-1)*cols+1;

		for(int i=0;i<rows;i++)
		{
			int r1=(i-1)*cols;
			int r2=i*cols;
			for(int j=0;j<cols;j++)
			{
				int c1=j;
				int c2=(j+1)%cols;
				Model::Face* face=new Model::Face;
				if(i==0)
				{
					face->vertexIndices.Add(topVertex);
					face->vertexIndices.Add(r2+c2);
					face->vertexIndices.Add(r2+c1);
				}
				else if(i==rows-1)
				{
					face->vertexIndices.Add(bottomVertex);
					face->vertexIndices.Add(r1+c1);
					face->vertexIndices.Add(r1+c2);
				}
				else
				{
					face->vertexIndices.Add(r1+c1);
					face->vertexIndices.Add(r1+c2);
					face->vertexIndices.Add(r2+c2);
					face->vertexIndices.Add(r2+c1);
				}
				model->modelFaces.Add(face);
			}
		}

		model->RebuildVertexBuffer();
	}

	void BuildCylinder(Model* model, int rows, int cols)
	{
		for(int i=0;i<=rows;i++)
		{
			float height=1.0f-2.0f*i/rows;
			for(int j=0;j<cols;j++)
			{
				float c=(float)D3DX_PI*2*j/cols;
				float sinc=sinf(c);
				float cosc=cosf(c);

				Model::Vertex* vertex=new Model::Vertex;
				vertex->position=D3DXVECTOR3(cosc, height, sinc);
				vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
				model->modelVertices.Add(vertex);
			}
		}

		for(int i=0;i<rows;i++)
		{
			int r1=i*cols;
			int r2=(i+1)*cols;
			for(int j=0;j<cols;j++)
			{
				int c1=j;
				int c2=(j+1)%cols;
				Model::Face* face=new Model::Face;
				{
					face->vertexIndices.Add(r1+c1);
					face->vertexIndices.Add(r1+c2);
					face->vertexIndices.Add(r2+c2);
					face->vertexIndices.Add(r2+c1);
				}
				model->modelFaces.Add(face);
			}
		}
		{
			Model::Face* face=new Model::Face;
			for(int i=0;i<cols;i++)
			{
				face->vertexIndices.Add(cols-i-1);
			}
			model->modelFaces.Add(face);
		}
		{
			Model::Face* face=new Model::Face;
			for(int i=0;i<cols;i++)
			{
				face->vertexIndices.Add(rows*cols+i);
			}
			model->modelFaces.Add(face);
		}

		model->RebuildVertexBuffer();
	}

	void BuildCone(Model* model, int rows, int cols)
	{
		for(int i=1;i<=rows;i++)
		{
			float height=1.0f-2.0f*i/rows;
			float radius=1.0f*i/rows;
			for(int j=0;j<cols;j++)
			{
				float c=(float)D3DX_PI*2*j/cols;
				float sinc=sinf(c);
				float cosc=cosf(c);

				Model::Vertex* vertex=new Model::Vertex;
				vertex->position=D3DXVECTOR3(cosc*radius, height, sinc*radius);
				vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
				model->modelVertices.Add(vertex);
			}
		}
		{
			Model::Vertex* vertex=new Model::Vertex;
			vertex->position=D3DXVECTOR3(0, 1, 0);
			vertex->diffuse=D3DXCOLOR(1, 1, 1, 1);
			model->modelVertices.Add(vertex);
		}
		int topVertex=rows*cols;

		for(int i=0;i<rows;i++)
		{
			int r1=(i-1)*cols;
			int r2=i*cols;
			for(int j=0;j<cols;j++)
			{
				int c1=j;
				int c2=(j+1)%cols;
				Model::Face* face=new Model::Face;
				if(i==0)
				{
					face->vertexIndices.Add(topVertex);
					face->vertexIndices.Add(r2+c2);
					face->vertexIndices.Add(r2+c1);
				}
				else
				{
					face->vertexIndices.Add(r1+c1);
					face->vertexIndices.Add(r1+c2);
					face->vertexIndices.Add(r2+c2);
					face->vertexIndices.Add(r2+c1);
				}
				model->modelFaces.Add(face);
			}
		}
		{
			Model::Face* face=new Model::Face;
			for(int i=0;i<cols;i++)
			{
				face->vertexIndices.Add((rows-1)*cols+i);
			}
			model->modelFaces.Add(face);
		}

		model->RebuildVertexBuffer();
	}
}
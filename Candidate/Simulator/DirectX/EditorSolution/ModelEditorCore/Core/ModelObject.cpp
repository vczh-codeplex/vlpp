#include "ModelObject.h"

namespace modeleditor
{

/***********************************************************************
Model
***********************************************************************/

	Model::EditorInfo::EditorInfo()
		:selected(false)
	{
		D3DXMatrixIdentity(&worldMatrix);
	}
	bool Model::EditorInfo::IsGeneralSelected()
	{
		return selected || selectedFaces.Count()>0 || selectedVertices.Count()>0;
	}

	Model::Model(DirectXEnvironment* _env)
		:env(0)
		,geometry(0)
	{
		Rebuild(_env);
	}

	Model::~Model()
	{
		delete geometry;
	}

	void Model::RebuildVertexBuffer()
	{
		for(int i=0;i<modelVertices.Count();i++)
		{
			Vertex* vertex=modelVertices[i].Obj();
			vertex->referencedFaces.Clear();
			vertex->referencedVertexBufferVertices.Clear();
		}

		int currentVertexBufferVertex=0;
		for(int i=0;i<modelFaces.Count();i++)
		{
			Face* face=modelFaces[i].Obj();
			face->referencedStartVertexBufferVertex=currentVertexBufferVertex;
			for(int j=0;j<face->vertexIndices.Count();j++)
			{
				Vertex* vertex=modelVertices[face->vertexIndices[j]].Obj();
				vertex->referencedFaces.Add(i);
			}
			currentVertexBufferVertex+=(face->vertexIndices.Count()-2)*3;
		}

		int totalCount=currentVertexBufferVertex;
		vertexBufferVertices.Resize(totalCount);
		vertexBufferIndices.Resize(totalCount);
		for(int i=0;i<totalCount;i++)
		{
			vertexBufferIndices[i]=i;
		}
		
		currentVertexBufferVertex=0;
		for(int i=0;i<modelFaces.Count();i++)
		{
			Face* face=modelFaces[i].Obj();
			int triangleCount=face->vertexIndices.Count()-2;
			for(int j=0;j<triangleCount;j++)
			{
				int triangleVertices[]={0, j+1, j+2};
				for(int k=0;k<sizeof(triangleVertices)/sizeof(*triangleVertices);k++)
				{
					VertexObject* vertexObject=&vertexBufferVertices[currentVertexBufferVertex];
					Vertex* vertex=modelVertices[face->vertexIndices[triangleVertices[k]]].Obj();
					vertex->referencedVertexBufferVertices.Add(currentVertexBufferVertex);
					currentVertexBufferVertex++;

					vertexObject->position=vertex->position;
					vertexObject->color=vertex->diffuse;
				}
			}
		}
		CreateNormalNoSmooth(&vertexBufferVertices[0], totalCount);

		UpdateVertexBuffer();
	}

	void Model::UpdateVertexBuffer()
	{
		if(vertexBufferVertices.Count()>0 && vertexBufferIndices.Count()>0)
		{
			geometry->Fill(&vertexBufferVertices[0], vertexBufferVertices.Count(), &vertexBufferIndices[0], vertexBufferIndices.Count());
		}
	}

	void Model::Rebuild(DirectXEnvironment* _env)
	{
		env=_env;
		delete geometry;
		geometry=new DirectXVertexBuffer<VertexObject>(env);
		UpdateVertexBuffer();
	}

	DirectXVertexBuffer<VertexObject>* Model::Geometry()
	{
		return geometry;
	}

/***********************************************************************
CreateNormalNoSmooth
***********************************************************************/

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
}
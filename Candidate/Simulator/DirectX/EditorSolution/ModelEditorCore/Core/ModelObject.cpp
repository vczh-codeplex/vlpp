#include "ModelObject.h"

namespace modeleditor
{

/***********************************************************************
Model
***********************************************************************/

	Model::Model(DirectXEnvironment* _env)
		:env(0)
		,geometry(0)
		,selected(false)
		,mainSelected(false)
	{
		D3DXMatrixIdentity(&worldMatrix);
		Rebuild(_env);
	}

	Model::~Model()
	{
		delete geometry;
	}

	void Model::RebuildVertexBuffer()
	{
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
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

	void Model::Update()
	{
		if(vertices.Count()>0 && indices.Count()>0)
		{
			geometry->Fill(&vertices[0], vertices.Count(), &indices[0], indices.Count());
		}
	}

	void Model::Rebuild(DirectXEnvironment* _env)
	{
		env=_env;
		delete geometry;
		geometry=new DirectXVertexBuffer<VertexObject>(env);
		Update();
	}

	DirectXVertexBuffer<VertexObject>* Model::Geometry()
	{
		return geometry;
	}
}
#ifndef VCZH_DIRECTX_MODELOBJECT
#define VCZH_DIRECTX_MODELOBJECT

#include "ModelDataStructure.h"

using namespace vl::collections;

namespace modeleditor
{
	class Model
	{
	protected:
		DirectXEnvironment*						env;
		DirectXVertexBuffer<VertexObject>*		geometry;
	public:
		Array<VertexObject>						vertices;
		Array<unsigned int>						indices;
		D3DXMATRIX								worldMatrix;
		bool									selected;
		bool									mainSelected;

		Model(DirectXEnvironment* _env);
		~Model();

		void									Update();
		void									Rebuild(DirectXEnvironment* _env);
		DirectXVertexBuffer<VertexObject>*		Geometry();
	};
}

#endif
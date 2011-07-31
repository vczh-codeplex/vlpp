#ifndef VCZH_DIRECTX_MODELOBJECT
#define VCZH_DIRECTX_MODELOBJECT

#include "ModelDataStructure.h"
#include "..\..\..\..\..\..\Library\Pointer.h"

using namespace vl;
using namespace vl::collections;

namespace modeleditor
{
	class Model
	{
	public:
		struct Vertex
		{
			D3DXVECTOR3							position;
			D3DXCOLOR							diffuse;

			List<int>							referencedFaces;
			List<int>							referencedVertexBufferVertices;
		};

		struct Face
		{
			List<int>							vertexIndices;

			int									referencedStartVertexBufferVertex;
		};
	protected:
		DirectXEnvironment*						env;
		DirectXVertexBuffer<VertexObject>*		geometry;
	public:
		List<Ptr<Vertex>>						modelVertices;
		List<Ptr<Face>>							modelFaces;

		Array<VertexObject>						vertexBufferVertices;
		Array<unsigned int>						vertexBufferIndices;

		D3DXMATRIX								worldMatrix;
		bool									selected;
		bool									mainSelected;

		Model(DirectXEnvironment* _env);
		~Model();

		void									RebuildVertexBuffer();
		void									UpdateVertexBuffer();
		void									Rebuild(DirectXEnvironment* _env);
		DirectXVertexBuffer<VertexObject>*		Geometry();
	};

	extern void									CreateNormalNoSmooth(VertexObject* vertices, int count);
}

#endif
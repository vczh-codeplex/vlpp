#ifndef VCZH_DIRECTX_MODELDATASTRUCTURE
#define VCZH_DIRECTX_MODELDATASTRUCTURE

#include "..\..\Shared\DirectXSetup.h"
#include "..\..\Shared\DirectXShader.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{
	struct VertexAxis
	{
		D3DXVECTOR3								position;
		D3DXCOLOR								color;
	};

	struct VertexImage
	{
		D3DXVECTOR3								position;
		D3DXVECTOR2								texcoord;
	};
	
	struct VertexObject
	{
		D3DXVECTOR3								position;
		D3DXVECTOR3								normal;
		D3DXCOLOR								color;
		unsigned __int32						id;

		bool operator==(const VertexObject& vertex){return false;}
		bool operator!=(const VertexObject& vertex){return true;}
	};

	struct ConstantBuffer
	{
		D3DXMATRIX								worldMatrix;
		D3DXMATRIX								viewMatrix;
		D3DXMATRIX								projectionMatrix;
	};
}

template<typename T>
void DeleteAndZero(T*& object)
{
	delete object;
	object=0;
}

#endif
#ifndef VCZH_BEGINNING_MODELBUILDER
#define VCZH_BEGINNING_MODELBUILDER

#include "..\Shared\DirectXSetup.h"
#include "..\Shared\DirectXShader.h"
#include "..\..\..\..\..\Library\Pointer.h"

using namespace vl;
using namespace vl::directx;
using namespace vl::collections;

struct LightVertex
{
    D3DXVECTOR3 Position;
};

struct ColorVertex
{
    D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
    D3DXCOLOR Color;
};

struct TextureVertex
{
    D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
    D3DXVECTOR2 Texcoord0;
};

extern void			BuildLightGeometry(DirectXVertexBuffer<LightVertex>& lightGeometry);
extern void			BuildColorCube(DirectXVertexBuffer<ColorVertex>& colorCube);
extern void			BuildTextureCube(DirectXVertexBuffer<TextureVertex>& textureCube);
extern void			BuildTextureSphere(DirectXVertexBuffer<TextureVertex>& textureSphere);

struct SmdModel
{
	List<Ptr<DirectXTextureBuffer>>				textures;
	List<Pair<int, int>>						indices; //startIndex, textureIndex
	Ptr<DirectXVertexBuffer<TextureVertex>>		vertexBuffer;
	int											totalIndices;

	SmdModel(const DirectXEnvironment* _env);
	~SmdModel();

	void										SetCurrentAndRender();
};

#endif
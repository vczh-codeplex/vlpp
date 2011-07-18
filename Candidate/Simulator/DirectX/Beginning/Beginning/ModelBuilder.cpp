#include "ModelBuilder.h"
#include "..\..\..\..\..\Library\Collections\Dictionary.h"
#include "..\..\..\..\..\Library\Stream\Accessor.h"
#include "..\..\..\..\..\Library\Stream\CharFormat.h"
#include "..\..\..\..\..\Library\Stream\FileStream.h"

using namespace vl::stream;
using namespace vl::collections;

void BuildLightGeometry(DirectXVertexBuffer<LightVertex>& lightGeometry)
{
	LightVertex vertices[] =
	{
		{D3DXVECTOR3( 0,  1,  0)},
		{D3DXVECTOR3( 1,  0,  0)},
		{D3DXVECTOR3( 0,  0,  1)},
		{D3DXVECTOR3(-1,  0,  0)},
		{D3DXVECTOR3( 0,  0, -1)},
		{D3DXVECTOR3( 0, -1,  0)},
	};

	unsigned int indices[] =
	{
		0, 1, 4,
		0, 4, 3,
		0, 3, 2,
		0, 2, 1,
		5, 1, 2,
		5, 2, 3,
		5, 3, 4,
		5, 4, 1,
	};

	lightGeometry.Fill(vertices, indices);
}

void BuildColorCube(DirectXVertexBuffer<ColorVertex>& colorCube)
{
	ColorVertex vertices[] =
	{
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(0, 0, 0, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(1, 1, 0, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXCOLOR(0, 1, 0, 1)},

		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 1, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXCOLOR(1, 1, 0, 1)},

		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(0, 1, 1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXCOLOR(1, 1, 1, 1)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 0, 0, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 1, 0, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXCOLOR(0, 1, 1, 1)},

		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(0, 1, 0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 0, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(1, 1, 1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXCOLOR(0, 1, 1, 1)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(0, 0, 1, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 0, 1, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(1, 0, 0, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXCOLOR(0, 0, 0, 1)},
	};

	unsigned int indices[] =
	{
		0, 3, 2,
		0, 2, 1,
		4, 7, 6,
		4, 6, 5,
		8, 11,10,
		8, 10,9,
		12,15,14,
		12,14,13,
		16,19,18,
		16,18,17,
		20,23,22,
		20,22,21,
	};

	colorCube.Fill(vertices, indices);
}

void BuildTextureCube(DirectXVertexBuffer<TextureVertex>& textureCube)
{
	TextureVertex vertices[] =
	{
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  0, -1), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 1,  0,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  0,  1), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3(-1,  0,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1,  1, -1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1,  1,  1), D3DXVECTOR3( 0,  1,  0), D3DXVECTOR2(0, 0)},

		{D3DXVECTOR3(-1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, -1,  1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3( 1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3( 0, -1,  0), D3DXVECTOR2(0, 0)},
	};

	unsigned int indices[] =
	{
		0, 3, 2,
		0, 2, 1,
		4, 7, 6,
		4, 6, 5,
		8, 11,10,
		8, 10,9,
		12,15,14,
		12,14,13,
		16,19,18,
		16,18,17,
		20,23,22,
		20,22,21,
	};

	textureCube.Fill(vertices, indices);
}

void BuildTextureSphere(DirectXVertexBuffer<TextureVertex>& textureSphere)
{
	const int levels = 25;
	const int vertexCount = (levels+1)*(levels+1);
	const int triangleCount = 2*levels*(levels-1);
	TextureVertex* vertices = new TextureVertex[vertexCount];
	unsigned int* indices = new unsigned int[triangleCount*3];

	for(int i=0;i<=levels;i++)
	{
		float ty = i*(float)D3DX_PI/levels;
		float y = cosf(ty);
		float r = sinf(ty);
		for(int j=0;j<=levels;j++)
		{
			float tx = j*(float)D3DX_PI*2/levels;
			float x = -cosf(tx)*r;
			float z = -sinf(tx)*r;
			float ix = (float)j/levels;
			float iy = (float)i/levels;

			TextureVertex* vertex=&vertices[i*(levels+1)+j];
			vertex->Position=D3DXVECTOR3(x, y, z);
			vertex->Normal=D3DXVECTOR3(x, y, z);
			vertex->Texcoord0=D3DXVECTOR2(ix, iy);
		}
	}

	int currentTriangle=0;
	for(int i=0;i<levels;i++)
	{
		for(int j=0;j<levels;j++)
		{
			if(i<levels-1)
			{
				indices[currentTriangle*3+0] = i*(levels+1)+j;
				indices[currentTriangle*3+1] = (i+1)*(levels+1)+j+1;
				indices[currentTriangle*3+2] = (i+1)*(levels+1)+j;
				currentTriangle++;
			}
			if(i>0)
			{
				indices[currentTriangle*3+0] = i*(levels+1)+j;
				indices[currentTriangle*3+1] = i*(levels+1)+j+1;
				indices[currentTriangle*3+2] = (i+1)*(levels+1)+j+1;
				currentTriangle++;
			}
		}
	}

	textureSphere.Fill(vertices, vertexCount, indices, triangleCount*3);
	delete[] vertices;
	delete[] indices;
}

/***********************************************************************
SmdModel
***********************************************************************/

SmdModel::SmdModel(const DirectXEnvironment* _env)
	:totalIndices(0)
	,env(_env)
{
}

SmdModel::~SmdModel()
{
}

struct SmdModelBone
{
	int parent;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	bool operator==(const SmdModelBone& bone){return false;}
	bool operator!=(const SmdModelBone& bone){return true;}
};

struct SmdVertexBone
{
	int parent;
	Array<Pair<int, float>> boneWeights;
};

void SmdModel::Load(const WString& folder, const WString& smdFile)
{
	textures.Clear();
	indices.Clear();
	vertexBuffer=0;
	totalIndices=0;

	FileStream fileStream(folder+smdFile, FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader streamReader(decoderStream);

	Dictionary<int, SmdModelBone> bones;
	List<WString> textureNames;
	List<WString> vertexLines;
	Array<Ptr<SmdVertexBone>> vertexBones;
	WString lastTexture;

	while(!streamReader.IsEnd())
	{
		WString section=streamReader.ReadLine();
		if(section==L"nodes")
		{
			while(true)
			{
				WString line=streamReader.ReadLine();
				if(line==L"end")
				{
					break;
				}
				const wchar_t* reading=line.Buffer();
				wchar_t* endptr=0;
				int key=wcstol(reading, &endptr, 10);
				reading=endptr;
				while(*reading++!=L'\"');
				while(*reading++!=L'\"');
				int parent=wcstol(reading, &endptr, 10);

				SmdModelBone bone;
				bone.parent=parent;
				bones.Add(key, bone);
			}
		}
		else if(section==L"time 0")
		{
			while(true)
			{
				WString line=streamReader.ReadLine();
				if(line==L"end")
				{
					break;
				}

				const wchar_t* reading=line.Buffer();
				wchar_t* endptr=0;

				int key=wcstol(reading, &endptr, 10);
				reading=endptr;
				SmdModelBone& bone=const_cast<SmdModelBone&>(bones[key]);

				float values[6];
				for(int i=0;i<6;i++)
				{
					values[i]=(float)wcstod(reading, &endptr);
					reading=endptr;
				}
				bone.pos=D3DXVECTOR3(values[0], values[1], values[2]);
				bone.rot=D3DXVECTOR3(values[3], values[4], values[5]);
			}
		}
		else if(section==L"triangles")
		{
			while(true)
			{
				WString line=streamReader.ReadLine();
				if(line==L"end")
				{
					break;
				}
		
				if(!textureNames.Contains(line))
				{
					textureNames.Add(line);
					DirectXTextureBuffer* texture=new DirectXTextureBuffer(env);
					texture->Update(folder+line.Left(line.Length()-4)+L".jpg");
					textures.Add(texture);
				}
				if(lastTexture!=line)
				{
					lastTexture=line;
					indices.Add(Pair<int, int>(vertexLines.Count(), textureNames.IndexOf(lastTexture)));
				}

				vertexLines.Add(streamReader.ReadLine());
				vertexLines.Add(streamReader.ReadLine());
				vertexLines.Add(streamReader.ReadLine());
			}
		}
	}
	
	totalIndices=vertexLines.Count();
	TextureVertex* vertices=new TextureVertex[totalIndices];
	unsigned int* indices=new unsigned int[totalIndices];
	vertexBones.Resize(totalIndices);

	for(int i=0;i<totalIndices;i++)
	{
		indices[i]=i;
		const wchar_t* reading=vertexLines[i].Buffer();
		wchar_t* endptr=0;
		int parentBone=wcstol(reading, &endptr, 0);
		reading=endptr;

		float values[8];
		for(int j=0;j<8;j++)
		{
			values[j]=(float)wcstod(reading, &endptr);
			reading=endptr;
		}
		vertices[i].Position=D3DXVECTOR3(values[0], values[1], values[2]);
		vertices[i].Normal=D3DXVECTOR3(values[3], values[4], values[5]);
		vertices[i].Texcoord0=D3DXVECTOR2(values[6], -values[7]);

		SmdVertexBone* vertexBone=new SmdVertexBone;
		vertexBones[i]=vertexBone;
		vertexBone->parent=parentBone;

		int links=wcstol(reading, &endptr, 0);
		reading=endptr;
		vertexBone->boneWeights.Resize(links);
		for(int j=0;j<links;j++)
		{
			int bone=wcstol(reading, &endptr, 0);
			reading=endptr;
			float weight=(float)wcstod(reading, &endptr);
			reading=endptr;
			vertexBone->boneWeights[j]=Pair<int, float>(bone, weight);
		}
	}

	vertexBuffer=new DirectXVertexBuffer<TextureVertex>(env);
	vertexBuffer->Fill(vertices, totalIndices, indices, totalIndices);
	delete[] vertices;
	delete[] indices;
}

void SmdModel::SetCurrentAndRender()
{
	vertexBuffer->SetCurrent();
	for(int i=0;i<indices.Count();i++)
	{
		int current=indices[i].key;
		int next=(i==indices.Count()-1?totalIndices:indices[i+1].key);
		int count=next-current;
		textures[indices[i].value]->PSBindToRegisterTN(0);
		vertexBuffer->Render(current, count);
	}
}
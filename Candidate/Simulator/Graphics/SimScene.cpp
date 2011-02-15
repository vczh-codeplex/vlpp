#include "SimGeometries.h"
#include "WinGDI.h"

namespace simulator
{
	using namespace vl::windows;
	
/*********************************************************************************************************
Texture
*********************************************************************************************************/

	Texture::Texture()
		:filter(None)
		,width(0)
		,height(0)
		,colors(0)
	{
	}

	Texture::~Texture()
	{
		if(colors)
		{
			delete[] colors;
		}
	}

	void Texture::Load(const WString& fileName)
	{
		if(colors)
		{
			delete[] colors;
		}
		WinBitmap bitmap(fileName, true, true);

		width=bitmap.GetWidth();
		height=bitmap.GetHeight();
		colors=new Color[width*height];

		Color* currentColor=colors;
		for(int y=0;y<height;y++)
		{
			BYTE* scanline=bitmap.GetScanLines()[y];
			for(int x=0;x<width;x++)
			{
				currentColor->x=(double)(scanline[x*4+2])/255;
				currentColor->y=(double)(scanline[x*4+1])/255;
				currentColor->z=(double)(scanline[x*4+0])/255;
				currentColor++;
			}
		}
	}
	
/*********************************************************************************************************
Material
*********************************************************************************************************/

	Material::Material()
		:color(1, 1, 1)
		,textureDiffuse(0)
		,textureNormal(0)
		,enableReflection(false)
		,reflection(0.8)
		,enableBRDFReflection(false)
		,brLevel(6)
		,brAlpha(1.0)
		,brAngle(PI/2)
		,enableRefraction(false)
		,N(1.6)
		,enableBlinnSpecular(false)
		,blinnC3(1.0)
		,enableAmbientOcclusion(false)
		,aoLevel(6)
	{
	}
	
/*********************************************************************************************************
Geometry
*********************************************************************************************************/

	Geometry::Geometry(Shape _shape)
		:shape(_shape)
	{
		localMatrix=MatrixUnit();
		Update();
	}

	void Geometry::Update()
	{
		localMatrixInverse=MatrixInverse(localMatrix);
		localMatrixNormal=MatrixNormal(localMatrix);
	}

	bool Geometry::Intersect(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
	{
		switch(shape)
		{
		case Sphere:
			return ((geometries::Sphere*)this)->IntersectInternal(ray, plane, diffuseNormal, planeMaterial);
		case Cube:
			return ((geometries::Cube*)this)->IntersectInternal(ray, plane, diffuseNormal, planeMaterial);
		case Cylinder:
			return ((geometries::Cylinder*)this)->IntersectInternal(ray, plane, diffuseNormal, planeMaterial);
		case Triangles:
			return ((geometries::Triangles*)this)->IntersectInternal(ray, plane, diffuseNormal, planeMaterial);
		case Group:
			return ((geometries::GeometryGroup*)this)->IntersectInternal(ray, plane, diffuseNormal, planeMaterial);
		default:
			return false;
		}
	}

	void Geometry::DrawFrame(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
	{
		switch(shape)
		{
		case Sphere:
			((geometries::Sphere*)this)->DrawFrameInternal(context, drawLine);
			break;
		case Cube:
			((geometries::Cube*)this)->DrawFrameInternal(context, drawLine);
			break;
		case Cylinder:
			((geometries::Cylinder*)this)->DrawFrameInternal(context, drawLine);
			break;
		case Triangles:
			((geometries::Triangles*)this)->DrawFrameInternal(context, drawLine);
			break;
		case Group:
			((geometries::GeometryGroup*)this)->DrawFrameInternal(context, drawLine);
			break;
		}
	}

	void Geometry::GetBox(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
	{
		switch(shape)
		{
		case Sphere:
			((geometries::Sphere*)this)->GetBoxInternal(o, x, y, z);
			break;
		case Cube:
			((geometries::Cube*)this)->GetBoxInternal(o, x, y, z);
			break;
		case Cylinder:
			((geometries::Cylinder*)this)->GetBoxInternal(o, x, y, z);
			break;
		case Triangles:
			((geometries::Triangles*)this)->GetBoxInternal(o, x, y, z);
			break;
		case Group:
			((geometries::GeometryGroup*)this)->GetBoxInternal(o, x, y, z);
			break;
		}
	}
	
/*********************************************************************************************************
Observer
*********************************************************************************************************/

	Observer::Observer()
		:eyePosition(0, -500, 0)
		,eyeForward(0, 1, 0)
		,eyeUp(0, 0, 1)
		,eyeDistance(500)
	{
		Update();
	}

	void Observer::Update()
	{
		vY=Normalize(eyeForward, 1.0);
		vZ=Normalize(eyeUp, 1.0);
		vX=Cross(vY, vZ);
	}

	Ray3 Observer::CreateRay(double x, double y, double bw, double bh, double sw, double sh)
	{
		x*=sw/bw;
		y*=sh/bh;
		Vector3 p(x-sw/2, eyeDistance, sh/2-y);
		return Ray3(eyePosition, Scale(vX, p.x)+Scale(vY, p.y)+Scale(vZ, p.z));
	}

	Vector3 Observer::Perspect(const Vector3& p)
	{
		Plane3 plane(eyePosition+Scale(vY, eyeDistance), vY);
		Ray3 ray(eyePosition, p-eyePosition);
		Vector3 vec=Intersect(plane, ray)-plane.position;
		return Vector3(Dot(vec, vX), Dot(vec, vZ), 0.0);
	}
	
/*********************************************************************************************************
Scene
*********************************************************************************************************/

	Scene::Scene()
		:skyGeometry(0)
		,environmentLight(0.2, 0.2, 0.2)
	{
	}

	Scene::~Scene()
	{
		for(int i=0;i<geometries.Count();i++)
		{
			delete geometries[i];
		}
		for(int i=0;i<textures.Count();i++)
		{
			delete textures[i];
		}
		if(skyGeometry)
		{
			delete skyGeometry;
		}
	}

	Texture* Scene::LoadTexture(const WString& fileName)
	{
		Texture* texture=new Texture;
		texture->Load(fileName);
		textures.Add(texture);
		return texture;
	}
}
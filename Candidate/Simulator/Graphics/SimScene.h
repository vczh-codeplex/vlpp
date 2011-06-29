#ifndef VCZHSIM_SIMSCENE
#define VCZHSIM_SIMSCENE

#include "..\..\..\Library\Basic.h"
#include "..\..\..\Library\String.h"
#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\Collections\List.h"
#include "SimMath.h"

namespace simulator
{
	using namespace vl;
	using namespace vl::collections;
	
/*********************************************************************************************************
基础
*********************************************************************************************************/

	typedef Vector3				Color;

	const double PI = 2*asin(1.0);

	struct Texture
	{
		enum FilterType
		{
			None,
			Linear,
			Cubic,
		}						filter;
		int						width;
		int						height;
		Color*					colors;

		Texture();
		~Texture();

		void					Load(const WString& fileName);

		__forceinline Color	GetColorDirectly(double x, double y)
		{
			x*=width;
			y*=height;
			int ix=((int)x%width+width)%width;
			int iy=((int)y%height+height)%height;
			switch(filter)
			{
			case Linear:
				{
					double x2=x-floor(x);
					double y2=y-floor(y);
					double x1=1-x2;
					double y1=1-y2;
					int jx=(ix+1)%width;
					int jy=(iy+1)%height;
					return
						Scale(Scale(colors[iy*width+ix], x1)+Scale(colors[iy*width+jx], x2), y1)+
						Scale(Scale(colors[jy*width+ix], x1)+Scale(colors[jy*width+jx], x2), y2);
				}
			case Cubic:
				{
					double tx=x-floor(x);
					double tx2=tx*tx;
					double tx3=tx2*tx;
					double ty=y-floor(y);
					double ty2=ty*ty;
					double ty3=ty2*ty;

					double fxp0=2*tx3-3*tx2+1;
					double fxm0=tx3-2*tx2+tx;
					double fxp1=-2*tx3+3*tx2;
					double fxm1=tx3-tx2;

					double fyp0=2*ty3-3*ty2+1;
					double fym0=ty3-2*ty2+ty;
					double fyp1=-2*ty3+3*ty2;
					double fym1=ty3-ty2;

					int x1=(ix+width-1)%width;
					int x2=ix;
					int x3=(ix+1)%width;
					int x4=(ix+2)%width;

					int y1=(iy+height-1)%height;
					int y2=iy;
					int y3=(iy+1)%height;
					int y4=(iy+2)%height;

					Color cy1=
						Scale(colors[y1*width+x2], fxp0)+
						Scale(Scale(colors[y1*width+x3]-colors[y1*width+x1], 0.5), fxm0)+
						Scale(colors[y1*width+x3], fxp1)+
						Scale(Scale(colors[y1*width+x4]-colors[y1*width+x2], 0.5), fxm1);

					Color cy2=
						Scale(colors[y2*width+x2], fxp0)+
						Scale(Scale(colors[y2*width+x3]-colors[y2*width+x1], 0.5), fxm0)+
						Scale(colors[y2*width+x3], fxp1)+
						Scale(Scale(colors[y2*width+x4]-colors[y2*width+x2], 0.5), fxm1);

					Color cy3=
						Scale(colors[y3*width+x2], fxp0)+
						Scale(Scale(colors[y3*width+x3]-colors[y3*width+x1], 0.5), fxm0)+
						Scale(colors[y3*width+x3], fxp1)+
						Scale(Scale(colors[y3*width+x4]-colors[y3*width+x2], 0.5), fxm1);

					Color cy4=
						Scale(colors[y4*width+x2], fxp0)+
						Scale(Scale(colors[y4*width+x3]-colors[y4*width+x1], 0.5), fxm0)+
						Scale(colors[y4*width+x3], fxp1)+
						Scale(Scale(colors[y4*width+x4]-colors[y4*width+x2], 0.5), fxm1);

					return
						Scale(cy2, fyp0)+
						Scale(Scale(cy3-cy1, 0.5), fym0)+
						Scale(cy3, fyp1)+
						Scale(Scale(cy4-cy2, 0.5), fym1);
				}
			default:
				return colors[iy*width+ix];
			}
		}
	};

	struct Material
	{
		// color
		Color					color;
		Texture*				textureDiffuse;
		Texture*				textureNormal;
		// reflection
		bool					enableReflection;
		double					reflection;
		bool					enableBRDFReflection;
		int						brLevel;
		double					brAlpha;
		double					brAngle;
		// refraction
		bool					enableRefraction;
		double					N;
		// blinn specular
		bool					enableBlinnSpecular;
		double					blinnC3;
		// ambient occlusion
		bool					enableAmbientOcclusion;
		int						aoLevel;

		Material();
	};

	class Geometry : public Object
	{
	public:
		enum Shape
		{
			Sphere,
			Cube,
			Cylinder,
			Triangles,
			Group,
		}						shape;
		Matrix4	localMatrix, localMatrixInverse, localMatrixNormal;
		Material				material;

	public:
		Geometry(Shape _shape);

		void					Update();
		bool					Intersect(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
		void					DrawFrame(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
		void					GetBox(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
	};

	class Observer
	{
	protected:
		Vector3			vX, vY, vZ;
	public:
		Vector3			eyePosition;
		Vector3			eyeForward;
		Vector3			eyeUp;
		double			eyeDistance;

		Observer();

		void			Update();
		Ray3			CreateRay(double x, double y, double bw, double bh, double sw, double sh);
		Vector3			Perspect(const Vector3& p);
	};
}
	
/*********************************************************************************************************
平行光
*********************************************************************************************************/

namespace simulator
{
	struct ParallelLight
	{
		Color					light;
		Vector3			direction;

		inline bool operator==(const ParallelLight& light)const
		{
			return false;
		}

		inline bool operator!=(const ParallelLight& light)const
		{
			return true;
		}
	};
}
namespace vl
{
	template<>
	struct POD<simulator::ParallelLight>{static const bool Result=true;};
}
	
/*********************************************************************************************************
场景
*********************************************************************************************************/

namespace simulator
{
	class Scene : public Object
	{
	public:
		Geometry*				skyGeometry;
		List<Geometry*>			geometries;
		List<Texture*>			textures;
		Color					environmentLight;
		List<ParallelLight>		parallelLights;

		Scene();
		~Scene();

		Texture*				LoadTexture(const WString& fileName);
	};
	
/*********************************************************************************************************
渲染器
*********************************************************************************************************/

	class Renderer : public Object
	{
	public:
		virtual Color			Render(Observer* observer, Scene* scene, vint screenWidth, vint screenHeight, int x, int y)=0;
		virtual void			Render(Observer* observer, Scene* scene, vint bufferWidth, vint bufferHeight, vint screenWidth, vint screenHeight, int startY, int endY, Color* colors)=0;
	};

	namespace renderers
	{
		class DirectTrayRenderer : public Renderer
		{
		public:
			int					level;

			DirectTrayRenderer(int _level);

			Color				Render(Observer* observer, Scene* scene, vint screenWidth, vint screenHeight, int x, int y);
			void				Render(Observer* observer, Scene* scene, vint bufferWidth, vint bufferHeight, vint screenWidth, vint screenHeight, int startY, int endY, Color* colors);
		};
	}
}

#endif
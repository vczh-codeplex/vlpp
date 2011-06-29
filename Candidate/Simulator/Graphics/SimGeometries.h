#ifndef VCZHSIM_SIMGEOMETRIES
#define VCZHSIM_SIMGEOMETRIES

#include "SimKdObject.h"
	
namespace simulator
{
/*********************************************************************************************************
几何物体（方程）
*********************************************************************************************************/

	namespace geometries
	{
		class Sphere : public Geometry
		{
		public:
			Sphere();

			bool				IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
			void				DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
			void				GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
		};

		class Cube : public Geometry
		{
		public:
			Texture				*px, *nx, *py, *ny, *pz, *nz;

			Cube();

			bool				IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
			void				DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
			void				GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
		};

		class Cylinder : public Geometry
		{
		public:
			Texture				*pzDiffuse, *nzDiffuse, *sideDiffuse;
			Texture				*pzNormal, *nzNormal, *sideNormal;

			Cylinder();

			bool				IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
			void				DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
			void				GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
		};
	}
}
	
/*********************************************************************************************************
几何物体（三角形）
*********************************************************************************************************/

namespace simulator
{
	namespace geometries
	{
		struct Triangle
		{
			union
			{
				struct
				{
					Vectorf3			p1, p2, p3;
				};
				Vectorf3				points[3];
			};

			bool						operator==(const Triangle&)const{return false;}
			bool						operator!=(const Triangle&)const{return true;}
		};
	}
}

namespace vl
{
	template<>
	struct POD<simulator::geometries::Triangle>
	{
		static const bool Result=true;
	};
}

namespace simulator
{
	namespace geometries
	{
		class Triangles : public KdObject<Triangle, Triangles>
		{
		public:
			collections::List<Triangle>			triangles;

			Triangles();

			void								Load(const WString& fileName);
			
			void								CallbackContentIntersect(const Vector3& d, double& scale, Triangle& t, const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
			Triangle&							CallbackGetObject(int index);
			Vectorf3*							CallbackGetObjectPoints(Triangle& t);
			int									CallbackGetObjectPointCount(Triangle& t);
			int									CallbackGetObjectCount();
			void								CallbackFillBox(Vectorf3& min, Vectorf3& max);
			void								DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
			void								GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
		};
	}
}
	
/*********************************************************************************************************
几何物体（复合图形）
*********************************************************************************************************/

namespace simulator
{
	namespace geometries
	{
		struct SubGeometry
		{
			Vectorf3					box[8];
			Geometry*					g;

			bool						operator==(const SubGeometry&)const{return false;}
			bool						operator!=(const SubGeometry&)const{return true;}
		};
	}
}

namespace vl
{
	template<>
	struct POD<simulator::geometries::SubGeometry>
	{
		static const bool Result=true;
	};
}

namespace simulator
{
	namespace geometries
	{
		class GeometryGroup : public KdObject<SubGeometry, GeometryGroup>
		{
		public:
			collections::List<SubGeometry>		geometries;

			GeometryGroup();
			~GeometryGroup();
			
			void								CallbackContentIntersect(const Vector3& d, double& scale, SubGeometry& t, const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial);
			SubGeometry&						CallbackGetObject(int index);
			Vectorf3*							CallbackGetObjectPoints(SubGeometry& t);
			int									CallbackGetObjectPointCount(SubGeometry& t);
			int									CallbackGetObjectCount();
			void								CallbackFillBox(Vectorf3& min, Vectorf3& max);
			void								DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&));
			void								GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z);
		};
	}
}

#endif
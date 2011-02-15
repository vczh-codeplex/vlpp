#include "SimGeometries.h"
#include <math.h>

namespace simulator
{
	namespace geometries
	{
		Triangles::Triangles()
			:KdObject<Triangle, Triangles>(Geometry::Triangles)
		{
		}

		Triangle& Triangles::CallbackGetObject(int index)
		{
			return triangles[index];
		}

		Vectorf3* Triangles::CallbackGetObjectPoints(Triangle& t)
		{
			return t.points;
		}

		int Triangles::CallbackGetObjectPointCount(Triangle& t)
		{
			return 3;
		}

		int Triangles::CallbackGetObjectCount()
		{
			return triangles.Count();
		}

		void Triangles::CallbackContentIntersect(const Vector3& d, double& scale, Triangle& t, const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
		{
			Vector3 _p1=t.p1.D();
			Vector3 _p2=t.p2.D();
			Vector3 _p3=t.p3.D();
			Vector3 _a=_p2-_p1;
			Vector3 _b=_p3-_p1;
			Vector3 _normal=Normalize(Cross(_a, _b), 1.0);
			double _d=-Dot(_normal, _p1);
			Matrix4 _matrixInterpolation=MatrixInverse(MatrixAxis(_a, _b, _normal));

			double NP=Dot(_normal, d);
			const double dmin=0.00001;
			if(NP<-dmin || NP>dmin)
			{
				double s=-(Dot(_normal, ray.position)+_d)/NP;
				if((scale<0 || s<scale) && s>0)
				{
					Vector3 p=ray.position+Scale(d, s);
					Vector3 pt=(p-_p1)*_matrixInterpolation;
					if(0<=pt.x && 0<=pt.y && pt.x+pt.y<=1)
					{
						plane.position=p;
						plane.normal=_normal;
						diffuseNormal=plane.normal;
						planeMaterial=material;
						scale=s;
					}
				}
			}
		}

		void Triangles::CallbackFillBox(Vectorf3& min, Vectorf3& max)
		{
			min=triangles[0].p1;
			max=triangles[0].p1;
			for(int i=0;i<triangles.Count();i++)
			{
				Triangle& t=triangles[i];
				UpdateBox(min, max, t.p1);
				UpdateBox(min, max, t.p2);
				UpdateBox(min, max, t.p3);
			}
		}

		void Triangles::DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
		{
			for(int i=0;i<triangles.Count();i++)
			{
				Triangle& t=triangles[i];
				drawLine(this, context, t.p1.D(), t.p2.D());
				drawLine(this, context, t.p2.D(), t.p3.D());
				drawLine(this, context, t.p3.D(), t.p1.D());
			}
		}

		void Triangles::GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
		{
			Vectorf3 min, max;
			CallbackFillBox(min, max);
			o=Vector3(min.x, min.y, min.z);
			x=Vector3(max.x-min.x, 0, 0);
			y=Vector3(0, max.y-min.y, 0);
			z=Vector3(0, 0, max.z-min.z);
		}
	}
}
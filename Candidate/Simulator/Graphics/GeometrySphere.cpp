#include "SimGeometries.h"

namespace simulator
{
	namespace geometries
	{
		Sphere::Sphere()
			:Geometry(Geometry::Sphere)
		{
		}

		bool Sphere::IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
		{
			Vector3 d=ray.position;
			double a=LengthSquare(ray.direction);
			double b=Dot(ray.direction, d)*2;
			double c=LengthSquare(d)-1;

			double x1, x2, rayScale;
			if(SolveX2(a, b, c, x1, x2) && MinPositive(x1, x2, rayScale))
			{
				plane.position=ray.position+Scale(ray.direction, rayScale);
				plane.normal=plane.position;
				planeMaterial=material;

				if(material.textureDiffuse)
				{
					double ty=asin(-plane.position.z)/PI+0.5;
					double tx=atan2(plane.position.y, plane.position.x)/(2*PI);
					planeMaterial.color=material.textureDiffuse->GetColorDirectly(tx, ty);
				}
				if(material.textureNormal)
				{
					double ty=asin(-plane.position.z)/PI+0.5;
					double tx=atan2(plane.position.y, plane.position.x)/(2*PI);
					Vector3 r=Normalize(Cross(Vector3(0, 0, 1), plane.normal), 1);
					Vector3 g=Cross(r, plane.normal);
					Color color=material.textureNormal->GetColorDirectly(tx, ty);
					diffuseNormal=Scale(r, color.x-0.5)+Scale(g, color.y-0.5)+Scale(plane.normal, color.z-0.5);
				}
				else
				{
					diffuseNormal=plane.normal;
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		Vector3 SphereGetFramePoint(int x, int y)
		{
			double th=PI*x/5.0;
			double tv=PI*y/8;

			double sth=sin(th);
			double cth=cos(th);
			double stv=sin(tv);
			double ctv=cos(tv);

			return Vector3(stv*cth, stv*sth, ctv);
		}

		void Sphere::DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
		{
			for(int i=0;i<8;i++)
			{
				for(int j=0;j<10;j++)
				{
					drawLine(this, context, SphereGetFramePoint(j, i), SphereGetFramePoint(j, i+1));
				}
			}
			for(int i=1;i<8;i++)
			{
				for(int j=0;j<10;j++)
				{
					drawLine(this, context, SphereGetFramePoint(j, i), SphereGetFramePoint((j+1)%10, i));
				}
			}
		}

		void Sphere::GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
		{
			o=Vector3(-1.001, -1.001, -1.001);
			x=Vector3(2.002, 0, 0);
			y=Vector3(0, 2.002, 0);
			z=Vector3(0, 0, 2.002);
		}
	}
}
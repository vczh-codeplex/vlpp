#include "SimGeometries.h"

namespace simulator
{
	namespace geometries
	{
		Cylinder::Cylinder()
			:Geometry(Geometry::Cylinder)
			,pzDiffuse(0),pzNormal(0)
			,nzDiffuse(0),nzNormal(0)
			,sideDiffuse(0),sideNormal(0)
		{
		}

#define PROCESS_CYLINDER(V, TD, TN)\
		i=(V-ray.position.z)/ray.direction.z;\
		if(i>0)\
		{\
			Vector3 p=ray.position+Scale(ray.direction, i);\
			if((scale<0 || i<scale) && (p.x*p.x+p.y*p.y<=1))\
			{\
				plane.position=p;\
				plane.normal=Vector3(0, 0, V);\
				tx=p.x;\
				ty=p.y*-(V);\
				td=TD;\
				tn=TN;\
				r=Vector3(1, 0, 0);\
				g=Vector3(0, -(V), 0);\
				scale=i;\
			}\
		}

		bool Cylinder::IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
		{
			Vector3 d=Normalize(ray.direction, 1.0);
			double scale=-1;
			double i=0;
			double tx=0;
			double ty=0;
			Texture* td=0;
			Texture* tn=0;
			Vector3 r, g;
			const double dmin=0.00001;
			if(d.z<-dmin || d.z>dmin)
			{
				PROCESS_CYLINDER(1, pzDiffuse, pzNormal);
				PROCESS_CYLINDER(-1, nzDiffuse, nzNormal);
			}
			{
				double a=ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y;
				double b=(ray.direction.x*ray.position.x+ray.direction.y*ray.position.y)*2;
				double c=(ray.position.x*ray.position.x+ray.position.y*ray.position.y)-1;
				double x1, x2;
				if(SolveX2(a, b, c, x1, x2) && MinPositive(x1, x2, i))
				{
					Vector3 p=ray.position+Scale(ray.direction, i);
					if((scale<0 || i<scale) && (p.z>=-1 && p.z<1))
					{
						plane.position=p;
						plane.normal=Vector3(p.x, p.y, 0);
						ty=-p.z;
						tx=atan2(plane.position.y, plane.position.x)/PI-1;
						td=sideDiffuse;
						tn=sideNormal;
						g=Vector3(0, 0, -1);
						r=Cross(plane.normal, g);
						scale=i;
					}
				}
			}

			if(scale>0)
			{
				planeMaterial=material;
				if(material.textureDiffuse || td)
				{
					planeMaterial.color=(td?td:material.textureDiffuse)->GetColorDirectly((tx+1)/2, (ty+1)/2);
				}
				if(material.textureNormal || tn)
				{
					Color color=(tn?tn:material.textureNormal)->GetColorDirectly((tx+1)/2, (ty+1)/2);
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

		Vector3 CylinderGetFramePoint(int x, int y)
		{
			double th=PI*x/5.0;
			double sth=sin(th);
			double cth=cos(th);
			return Vector3(cth, sth, 0.25*y-1);
		}

		void Cylinder::DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
		{
			for(int i=0;i<8;i++)
			{
				for(int j=0;j<10;j++)
				{
					drawLine(this, context, CylinderGetFramePoint(j, i), CylinderGetFramePoint(j, i+1));
				}
			}
			for(int i=0;i<=8;i++)
			{
				for(int j=0;j<10;j++)
				{
					drawLine(this, context, CylinderGetFramePoint(j, i), CylinderGetFramePoint((j+1)%10, i));
				}
			}
		}

		void Cylinder::GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
		{
			o=Vector3(-1.001, -1.001, -1.001);
			x=Vector3(2.002, 0, 0);
			y=Vector3(0, 2.002, 0);
			z=Vector3(0, 0, 2.002);
		}
	}
}
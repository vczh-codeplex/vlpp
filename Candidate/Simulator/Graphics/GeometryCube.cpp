#include "SimGeometries.h"

namespace simulator
{
	namespace geometries
	{
		Cube::Cube()
			:Geometry(Geometry::Cube)
			,px(0)
			,nx(0)
			,py(0)
			,ny(0)
			,pz(0)
			,nz(0)
		{
		}

#define PROCESS_CUBE(V, M, X, Y, Z, TX, TY, R, G, T)\
		i=(V-ray.position.M)/ray.direction.M;\
		if(i>0)\
		{\
			Vector3 p=ray.position+Scale(ray.direction, i);\
			if((scale<0 || i<scale) && (-1-dmin<p.x && p.x<1+dmin && -1-dmin<p.y && p.y<1+dmin && -1-dmin<p.z && p.z<1+dmin))\
			{\
				plane.position=p;\
				plane.normal=Vector3(X, Y, Z);\
				tx=p.TX;\
				ty=p.TY;\
				t=T;\
				r=R;\
				g=G;\
				scale=i;\
			}\
		}

		bool Cube::IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
		{
			Vector3 d=Normalize(ray.direction, 1.0);
			double scale=-1;
			double i=0;
			double tx=0;
			double ty=0;
			Texture* t=0;
			Vector3 r, g;
			const double dmin=0.00001;
			if(d.x<-dmin || d.x>dmin)
			{
				PROCESS_CUBE(1, x, 1, 0, 0, y, z*-1, Vector3(0, 1, 0), Vector3(0, 0, -1), px);
				PROCESS_CUBE(-1, x, -1, 0, 0, y*-1, z*-1, Vector3(0,- 1, 0), Vector3(0, 0, -1), nx);
			}
			if(d.y<-dmin || d.y>dmin)
			{
				PROCESS_CUBE(1, y, 0, 1, 0, x*-1, z*-1, Vector3(-1, 0, 0), Vector3(0, 0, -1), py);
				PROCESS_CUBE(-1, y, 0, -1, 0, x, z*-1, Vector3(1, 0, 0), Vector3(0, 0, -1), ny);
			}
			if(d.z<-dmin || d.z>dmin)
			{
				PROCESS_CUBE(1, z, 0, 0, 1, x, y*-1, Vector3(1, 0, 0), Vector3(0, -1, 0), pz);
				PROCESS_CUBE(-1, z, 0, 0, -1, x, y, Vector3(1, 0, 0), Vector3(0, 1, 0), nz);
			}
			if(scale>0)
			{
				planeMaterial=material;
				if(material.textureDiffuse || t)
				{
					planeMaterial.color=(t?t:material.textureDiffuse)->GetColorDirectly((tx+1)/2, (ty+1)/2);
				}
				if(material.textureNormal)
				{
					Color color=material.textureNormal->GetColorDirectly((tx+1)/2, (ty+1)/2);
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

		void Cube::DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
		{
			drawLine(this, context, Vector3(-1, -1, -1), Vector3(-1, -1, 1));
			drawLine(this, context, Vector3(-1, -1, 1), Vector3(1, -1, 1));
			drawLine(this, context, Vector3(1, -1, 1), Vector3(1, -1, -1));
			drawLine(this, context, Vector3(1, -1, -1), Vector3(-1, -1, -1));

			drawLine(this, context, Vector3(-1, 1, -1), Vector3(-1, 1, 1));
			drawLine(this, context, Vector3(-1, 1, 1), Vector3(1, 1, 1));
			drawLine(this, context, Vector3(1, 1, 1), Vector3(1, 1, -1));
			drawLine(this, context, Vector3(1, 1, -1), Vector3(-1, 1, -1));
			
			drawLine(this, context, Vector3(-1, -1, -1), Vector3(-1, 1, -1));
			drawLine(this, context, Vector3(-1, -1, 1), Vector3(-1, 1, 1));
			drawLine(this, context, Vector3(1, -1, 1), Vector3(1, 1, 1));
			drawLine(this, context, Vector3(1, -1, -1), Vector3(1, 1, -1));
		}

		void Cube::GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
		{
			o=Vector3(-1.001, -1.001, -1.001);
			x=Vector3(2.002, 0, 0);
			y=Vector3(0, 2.002, 0);
			z=Vector3(0, 0, 2.002);
		}
	}
}
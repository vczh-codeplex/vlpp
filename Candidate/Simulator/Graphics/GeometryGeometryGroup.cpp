#include "SimGeometries.h"
#include <math.h>

namespace simulator
{
	namespace geometries
	{
		GeometryGroup::GeometryGroup()
			:KdObject<SubGeometry, GeometryGroup>(Geometry::Group)
		{
		}

		GeometryGroup::~GeometryGroup()
		{
			for(int i=0;i<geometries.Count();i++)
			{
				delete geometries[i].g;
			}
		}

		SubGeometry& GeometryGroup::CallbackGetObject(int index)
		{
			return geometries[index];
		}

		Vectorf3* GeometryGroup::CallbackGetObjectPoints(SubGeometry& t)
		{
			return t.box;
		}

		int GeometryGroup::CallbackGetObjectPointCount(SubGeometry& t)
		{
			return 8;
		}

		int GeometryGroup::CallbackGetObjectCount()
		{
			return geometries.Count();
		}

		void GeometryGroup::CallbackContentIntersect(const Vector3& d, double& scale, SubGeometry& t, const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
		{
			Ray3 newRay=ray*t.g->localMatrixInverse;
			Plane3 subPlane;
			Vector3 subDiffuseNormal;
			Material subPlaneMaterial;
			if(t.g->Intersect(newRay, subPlane, subDiffuseNormal, subPlaneMaterial))
			{
				double subScale=
					((subPlane.position.x-newRay.position.x)
					+(subPlane.position.y-newRay.position.y)
					+(subPlane.position.z-newRay.position.z))
					/(newRay.direction.x+newRay.direction.y+newRay.direction.z);
				if((scale<0 || subScale<scale) && subScale>0)
				{
					scale=subScale;
					plane=TransformPlane(subPlane, t.g->localMatrix, t.g->localMatrixNormal);
					diffuseNormal=subDiffuseNormal*t.g->localMatrixNormal;
					planeMaterial=subPlaneMaterial;
				}
			}
		}

		void GeometryGroup::CallbackFillBox(Vectorf3& min, Vectorf3& max)
		{
			for(int i=0;i<geometries.Count();i++)
			{
				SubGeometry& t=geometries[i];

				Vector3 o, x, y, z;
				t.g->GetBox(o, x, y, z);
				t.box[0].D((o)*t.g->localMatrix);
				t.box[1].D((o+x)*t.g->localMatrix);
				t.box[2].D((o+y)*t.g->localMatrix);
				t.box[3].D((o+x+y)*t.g->localMatrix);
				t.box[4].D((o+z)*t.g->localMatrix);
				t.box[5].D((o+x+z)*t.g->localMatrix);
				t.box[6].D((o+y+z)*t.g->localMatrix);
				t.box[7].D((o+x+y+z)*t.g->localMatrix);
			}
			min=geometries[0].box[0];
			max=geometries[0].box[0];
			for(int i=0;i<geometries.Count();i++)
			{
				SubGeometry& t=geometries[i];
				for(int j=0;j<8;j++)
				{
					UpdateBox(min, max, t.box[j]);
				}
			}
		}

		void GeometryGroup::DrawFrameInternal(void* context, void(*drawLine)(Geometry*, void*, const Vector3&, const Vector3&))
		{
			for(int i=0;i<geometries.Count();i++)
			{
				SubGeometry& t=geometries[i];
				Matrix4 matrix=t.g->localMatrix;
				t.g->localMatrix=matrix*localMatrix;
				t.g->Update();
				t.g->DrawFrame(context, drawLine);
				t.g->localMatrix=matrix;
				t.g->Update();
			}
		}

		void GeometryGroup::GetBoxInternal(Vector3& o, Vector3& x, Vector3& y, Vector3& z)
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
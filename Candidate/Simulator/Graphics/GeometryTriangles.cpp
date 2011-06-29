#include "SimGeometries.h"
#include <math.h>
#include <stdio.h>

namespace simulator
{
	namespace geometries
	{
		namespace fuckobj
		{
			struct FaceVertex
			{
				int vid, nid, tid;

				bool operator==(const FaceVertex& fv)
				{
					return false;
				}

				bool operator!=(const FaceVertex& fv)
				{
					return false;
				}
			};
		}
		using namespace fuckobj;

		Triangles::Triangles()
			:KdObject<Triangle, Triangles>(Geometry::Triangles)
		{
		}

		void Triangles::Load(const WString& fileName)
		{
			FILE * f = 0;
			fopen_s(&f, wtoa(fileName).Buffer(), "rt");
			if (f == 0)
				return;
			char buf[200];
			int smoothGroup = 0;
			List<FaceVertex> faceVertices;
			List<Vectorf3> vertices;
			while (!feof(f))
			{
				fscanf_s(f, "%s", buf, 200);
				if (_stricmp(buf, "v") == 0)
				{
					Vectorf3 v;
					fscanf_s(f, "%f %f %f", &v.x, &v.y, &v.z);
					vertices.Add(v);
				}
				else if (_stricmp(buf, "vn") == 0)
				{
					Vectorf3 v;
					fscanf_s(f, "%f %f %f", &v.x, &v.y, &v.z);
					//Normals.Add(v);
				}
				else if (_stricmp(buf, "vt") == 0)
				{
					Vectorf3 v;
					fscanf_s(f, "%f %f", &v.x, &v.y);
					//TexCoords.Add(v);
				}
				else if (_stricmp(buf, "f") == 0)
				{
					fgets(buf, 200, f);
					int len = strlen(buf);
					int startPos = 0;
					faceVertices.Clear();
					for (int i = 0; i<len; i++)
					{
						if (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
						{
							continue;
						}
						else if (i == startPos)
						{
							startPos++;
							continue;
						}
						char str[50];
						memset(str, 0, 50);
						memcpy(str, buf+startPos, i-startPos);
						if (strstr(str, "//"))
						{
							int vid, nid;
							sscanf_s(str, "%d//%d", &vid, &nid);
							FaceVertex vtx;
							vtx.vid = vid - 1;
							vtx.nid = nid - 1;
							vtx.tid = -1;
							faceVertices.Add(vtx);
						}
						else
						{
							int slashCount = 0;
							for (int j = 0; j<i-startPos; j++)
							{
								if (str[j] == '/') slashCount ++;
							}
							if (slashCount == 0)
							{
								FaceVertex vtx;
								vtx.nid = vtx.tid = -1;
								sscanf_s(str, "%d", &vtx.vid);
								vtx.vid --;
								faceVertices.Add(vtx);
							}
							else if (slashCount == 3)
							{
								FaceVertex vtx;
								vtx.nid = -1;
								sscanf_s(str, "%d/%d", &vtx.vid, &vtx.tid);
								vtx.vid --;
								vtx.tid --;
								faceVertices.Add(vtx);
							}
							else
							{
								FaceVertex vtx;
								sscanf_s(str, "%d/%d/%d", &vtx.vid, &vtx.tid, &vtx.nid);
								vtx.vid --;
								vtx.tid --;
								vtx.nid --;
								faceVertices.Add(vtx);
							}
						}
						startPos = i+1;
					}
					// simple triangulation
					for (int i = 2; i<faceVertices.Count(); i++)
					{
						Triangle triangle;
						triangle.p1=vertices[faceVertices[0].vid];
						triangle.p2=vertices[faceVertices[i-1].vid];
						triangle.p3=vertices[faceVertices[i].vid];
						triangles.Add(triangle);
					}
				}
				else if (_stricmp(buf, "s") == 0)
				{
					fscanf_s(f, "%s", buf, 200);
					if (buf[0] >= '0' && buf[0] <= '9')
					{
						sscanf_s(buf, "%d", &smoothGroup);
					}
					else
						smoothGroup = 0;
				}
				else
				{
					while (!feof(f) && fgetc(f) != '\n');
				}
			}
			fclose(f);
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
			if(Dot(_normal, ray.direction)>0)
			{
				_normal=Scale(_normal, -1);
			}
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
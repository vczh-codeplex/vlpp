#include "SimulatorCore.h"

namespace CreateScene5_Helper
{
	Vector3 msVertices[8]=
	{
		Vector3(0, 1, 1),
		Vector3(0, 0, 1),
		Vector3(1, 0, 1),
		Vector3(1, 1, 1),
		Vector3(0, 1, 0),
		Vector3(0, 0, 0),
		Vector3(1, 0, 0),
		Vector3(1, 1, 0),
	};

	int msTriangles[12][3]=
	{
		{0, 1, 2}, {0, 2, 3},
		{0, 4, 5}, {0, 5, 1},
		{1, 5, 6}, {1, 6, 2},
		{2, 6, 7}, {2, 7, 3},
		{3, 7, 4}, {3, 4, 0},
		{5, 4, 7}, {5, 7, 6},
	};

	void CreateMengerSponge(int xi, int yi, int zi, int l, int m, int level, int maxLevel, Triangles* g)
	{
		if(level==maxLevel)
		{
			const double s=2.0/l;
			Vector3 o(xi*s-1, yi*s-1, zi*s-1);
			for(int i=0;i<12;i++)
			{
				if(m&(1<<(i/2)))
				{
					Triangle t;
					t.p1.D(o+Scale(msVertices[msTriangles[i][0]], s));
					t.p2.D(o+Scale(msVertices[msTriangles[i][1]], s));
					t.p3.D(o+Scale(msVertices[msTriangles[i][2]], s));
					g->triangles.Add(t);
				}
			}
		}
		else
		{
			for(int i=0;i<=2;i++)for(int j=0;j<=2;j++)for(int k=0;k<=2;k++)
			{
				if((i&1)+(j&1)+(k&1)<=1)
				{
					int m2=0;

					if(i==0)m2|=(1<<1)&m;
					if(i==2)m2|=(1<<3)&m;
					if(j==0)m2|=(1<<2)&m;
					if(j==2)m2|=(1<<4)&m;
					if(k==0)m2|=(1<<5)&m;
					if(k==2)m2|=(1<<0)&m;
					
					if(i==2&&(j==1 || k==1))m2|=(1<<1);
					if(i==0&&(j==1 || k==1))m2|=(1<<3);
					if(j==2&&(k==1 || i==1))m2|=(1<<2);
					if(j==0&&(k==1 || i==1))m2|=(1<<4);
					if(k==2&&(i==1 || j==1))m2|=(1<<5);
					if(k==0&&(i==1 || j==1))m2|=(1<<0);

					CreateMengerSponge(xi*3+i, yi*3+j, zi*3+k, l*3, m2, level+1, maxLevel, g);
				}
			}
		}
	}

	void CreateMengerSponge(int maxLevel, Triangles* g)
	{
		CreateMengerSponge(0, 0, 0, 1, 0x3F, 0, maxLevel, g);
	}
}

extern "C"
{
	using namespace CreateScene5_Helper;

	SIMULATORCORE_API SceneHandle* __stdcall CreateScene5()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		scene->scene.environmentLight=Color(0.7, 0.7, 0.7);
		{
			ParallelLight light;
			light.light=Color(0.3, 0.3, 0.3);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Triangles* g=new Triangles;
#ifdef NDEBUG
			CreateMengerSponge(4, g);
#else
			CreateMengerSponge(1, g);
#endif
			g->localMatrix=MatrixScale(150, 150, 150)*MatrixRotateXY(-0.5)*MatrixRotateYZ(0.5);
			g->material.color=Color(0.6, 0.8, 1.0);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=18;
			g->BuildKdTree(1.0/32, 1);
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Cube;
			g->localMatrix=MatrixScale(270, 270, 20)*MatrixOffset(0, 0, -170)*MatrixRotateXY(-0.5)*MatrixRotateYZ(0.5);
			g->material.color=Color(1.0, 1.0, 1.0);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=18;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
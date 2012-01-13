#include "SimulatorCore.h"

namespace CreateScene6_Helper
{
	void CreateGreenCube(GeometryGroup* group, double xs, double ys, double zs, double xo, double yo, double zo, Color color, bool BRDF)
	{
		Geometry* g=new Cube;
		g->localMatrix=MatrixScale(xs, ys, zs)*MatrixOffset(xo, yo, zo);
		g->material.color=color;
		g->material.enableAmbientOcclusion=true;
		g->material.aoLevel=13;
		if(BRDF)
		{
			g->material.enableReflection=true;
			g->material.reflection=0.4;
			g->material.enableBRDFReflection=true;
			g->material.brAngle=PI/72;
			g->material.brLevel=3;
		}
		g->Update();

		SubGeometry sg;
		sg.g=g;
		group->geometries.Add(sg);
	}

	void CreateBall(GeometryGroup* group, double r, double xo, double yo, double zo, Color color)
	{
		Geometry* g=new Sphere;
		g->localMatrix=MatrixScale(r, r, r)*MatrixOffset(xo, yo, zo);
		g->material.color=color;
		g->material.enableAmbientOcclusion=true;
		g->material.aoLevel=13;
		g->material.enableReflection=true;
		g->material.reflection=0.3;
		g->material.enableBlinnSpecular=true;
		g->material.blinnC3=0.3;
		g->Update();

		SubGeometry sg;
		sg.g=g;
		group->geometries.Add(sg);
	}
}

extern "C"
{
	using namespace CreateScene6_Helper;

	SIMULATORCORE_API SceneHandle* __stdcall CreateScene6()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		scene->scene.environmentLight=Color(0.6, 0.6, 0.6);
		{
			ParallelLight light;
			light.light=Color(0.05, 0.05, 0.05);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			ParallelLight light;
			light.light=Color(0.05, 0.05, 0.05);
			light.direction=Normalize(Vector3(1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			ParallelLight light;
			light.light=Color(0.05, 0.05, 0.05);
			light.direction=Normalize(Vector3(-1, -1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			ParallelLight light;
			light.light=Color(0.05, 0.05, 0.05);
			light.direction=Normalize(Vector3(1, -1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			GeometryGroup* g=new GeometryGroup;
			CreateGreenCube(g, 
				200, 200, 20, 
				0, 0, -70,
				Color(0.24, 0.6, 0.18), true);
			CreateGreenCube(g, 
				200, 20, 55, 
				0, 180, 5,
				Color(0.24, 0.6, 0.18), false);
			CreateGreenCube(g, 
				20, 180, 55, 
				-180, -20, 5,
				Color(0.24, 0.6, 0.18), false);
			CreateGreenCube(g, 
				20, 20, 20, 
				120, -120, -30,
				Color(0.2, 0.4, 1.0), false);
			CreateBall(g, 70, -90, 90, 20, Color(1.0, 0.2, 0.2));
			CreateBall(g, 70, -90, -50, 20, Color(1.0, 1.0, 0.2));
			CreateBall(g, 70, 31, 20, 20, Color(0.2, 0.2, 1.0));
			//g->BuildKdTree(10, 1);
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
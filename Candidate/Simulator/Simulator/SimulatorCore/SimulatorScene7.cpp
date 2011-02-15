#include "SimulatorCore.h"

namespace CreateScene7_Helper
{
	void CreateGreenCube(SceneHandle* scene, double xs, double ys, double zs, double xo, double yo, double zo, Color color, bool BRDF)
	{
		Geometry* g=new Cube;
		g->localMatrix=MatrixScale(xs, ys, zs)*MatrixOffset(xo, yo, zo);
		g->material.color=color;
		g->material.enableAmbientOcclusion=true;
		g->material.aoLevel=24;
		if(BRDF)
		{
			g->material.enableReflection=true;
			g->material.reflection=0.5;
			g->material.enableBRDFReflection=true;
			g->material.brAngle=PI/72;
			g->material.brLevel=8;
			g->material.brAlpha=3.0;
		}
		g->Update();
		scene->scene.geometries.Add(g);
	}
}

extern "C"
{
	using namespace CreateScene7_Helper;

	SIMULATORCORE_API SceneHandle* __stdcall CreateScene7()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		scene->scene.environmentLight=Color(0.9, 0.9, 0.9);
		{
			ParallelLight light;
			light.light=Color(0.1, 0.1, 0.1);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		CreateGreenCube(scene, 
			200, 200, 20, 
			0, 0, -70,
			Color(1.0, 0.6, 0.3), true);
		CreateGreenCube(scene, 
			80, 80, 80, 
			0, -0, 30,
			Color(1.0, 1.0, 1.0), false);
		return scene;
	}
}
#include "SimulatorCore.h"

extern "C"
{
	SIMULATORCORE_API SceneHandle* __stdcall CreateScene9()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		scene->scene.environmentLight=Color(0.3, 0.3, 0.3);
		{
			ParallelLight light;
			light.light=Color(0.5, 0.5, 0.5);
			light.direction=Normalize(Vector3(-0.4, 1, -0.4), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Triangles* g=new Triangles;
			g->Load(textureFolder+L"conference.obj");
			{
				Vector3 o, x, y, z;
				g->GetBox(o, x, y, z);
				Vector3 center=Scale(x+y+z, 0.5);
				g->localMatrix=MatrixScale(10, 10, 10)*MatrixRotateYZ(1.57)*MatrixOffset(0, -400, -70);
			}
			g->material.color=Color(1.0, 1.0, 1.0);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=6;
			g->material.aoRange=10;
			g->BuildKdTree(0.3, 1);
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
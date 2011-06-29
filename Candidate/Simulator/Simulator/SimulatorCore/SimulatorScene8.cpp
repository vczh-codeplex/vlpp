#include "SimulatorCore.h"

extern "C"
{

	SIMULATORCORE_API SceneHandle* __stdcall CreateScene8()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		scene->scene.environmentLight=Color(0.3, 0.3, 0.3);
		{
			ParallelLight light;
			light.light=Color(0.5, 0.5, 0.5);
			light.direction=Normalize(Vector3(-0.2, 0.2, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Triangles* g=new Triangles;
			g->Load(textureFolder+L"sponza.obj");
			{
				Vector3 o, x, y, z;
				g->GetBox(o, x, y, z);
				Vector3 center=Scale(x+y+z, 0.5);
				//g->localMatrix=MatrixOffset(0, -center.y, 0)*MatrixScale(0.4, 0.4, 0.4);
				g->localMatrix=MatrixRotateZX(1.57)*MatrixRotateYZ(1.57)*MatrixOffset(0, -500, -center.y/4);
			}
			g->material.color=Color(1.0, 1.0, 1.0);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=6;
			g->material.aoRange=100;
			g->BuildKdTree(25, 1);
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
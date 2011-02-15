#include "SimulatorCore.h"

extern "C"
{
	SIMULATORCORE_API SceneHandle* __stdcall CreateScene4()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		Texture* textureEarth=scene->scene.LoadTexture(textureFolder+L"Earth.bmp");
		Texture* textureEarthNormal=scene->scene.LoadTexture(textureFolder+L"EarthNormal.bmp");
		{
			textureEarth->filter=Texture::Cubic;
			textureEarthNormal->filter=Texture::Cubic;
		}
		scene->scene.environmentLight=Color(0.4, 0.4, 0.4);
		{
			ParallelLight light;
			light.light=Color(0.6, 0.6, 0.6);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Cylinder* g=new Cylinder;
			g->localMatrix=MatrixScale(200, 200, 20)*MatrixOffset(0, 0, -100);
			g->material.color=Color(1.0, 0.8, 1.0);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=18;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		for(int i=0;i<10;i++)
		{
			double th=PI*i/5.0;
			double x=cos(th)*160;
			double y=sin(th)*160;
			
			Cylinder* g=new Cylinder;
			g->localMatrix=MatrixScale(20, 20, 82)*MatrixOffset(x, y, 0);
			g->material.color=Color(0.8, 1.0, 0.8);
			g->material.enableAmbientOcclusion=true;
			g->material.aoLevel=18;
			g->sideNormal=textureEarthNormal;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
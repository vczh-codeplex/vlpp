#include "SimulatorCore.h"

extern "C"
{
	SIMULATORCORE_API SceneHandle* __stdcall CreateScene1()
	{
		WString textureFolder=GetTextureFolder();

		SceneHandle* scene=new SceneHandle;
		InstallSky(&scene->scene);
		Texture* textureEarthNormal=scene->scene.LoadTexture(textureFolder+L"EarthNormal.bmp");
		{
			textureEarthNormal->filter=Texture::Cubic;
		}
		scene->scene.environmentLight=Color(0.2, 0.2, 0.2);
		{
			ParallelLight light;
			light.light=Color(0.8, 0.8, 0.8);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Geometry* g=new Sphere;
			g->localMatrix=MatrixScale(100, 100, 100)*MatrixOffset(-80, 0, 0);
			g->material.color=Color(1, 0.5, 0);
			g->material.enableReflection=true;
			g->material.reflection=0.5;
			g->material.textureNormal=textureEarthNormal;
			g->material.enableBlinnSpecular=true;
			g->material.blinnC3=0.4;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Sphere;
			g->localMatrix=MatrixScale(130, 130, 130)*MatrixOffset(50, 0, 0);
			g->material.color=Color(0, 0.5, 1);
			g->material.enableReflection=true;
			g->material.reflection=0.5;
			g->material.textureNormal=textureEarthNormal;
			g->material.enableBlinnSpecular=true;
			g->material.blinnC3=0.4;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Cube;
			g->localMatrix=MatrixScale(200, 10, 150)*MatrixOffset(0, 150, 0);
			g->material.color=Color(0, 1, 0.5);
			g->material.enableReflection=true;
			g->material.reflection=0.5;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Cube;
			g->localMatrix=MatrixScale(50, 10, 50)*MatrixOffset(0, -200, 0)*MatrixRotateXY(0.7);
			g->material.color=Color(1, 1, 1);
			g->material.enableReflection=true;
			g->material.reflection=0.95;
			g->material.enableRefraction=true;
			g->material.N=1.6;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Sphere;
			g->localMatrix=MatrixScale(50, 4, 50)*MatrixOffset(0, -200, 0)*MatrixRotateXY(-0.7);
			g->material.color=Color(1, 1, 1);
			g->material.enableReflection=true;
			g->material.reflection=0.95;
			g->material.enableRefraction=true;
			g->material.N=1.6;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
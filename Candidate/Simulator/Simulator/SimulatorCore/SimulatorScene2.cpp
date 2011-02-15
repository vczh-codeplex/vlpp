#include "SimulatorCore.h"

extern "C"
{
	SIMULATORCORE_API SceneHandle* __stdcall CreateScene2()
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
		scene->scene.environmentLight=Color(0.2, 0.2, 0.2);
		{
			ParallelLight light;
			light.light=Color(0.8, 0.8, 0.8);
			light.direction=Normalize(Vector3(-1, 1, -1), 1.0);
			scene->scene.parallelLights.Add(light);
		}
		{
			Geometry* g=new Sphere;
			g->localMatrix=MatrixScale(100, 100, 100)*MatrixOffset(120, 120, 0);
			g->material.color=Color(1, 1, 1);
			g->material.textureDiffuse=textureEarth;
			g->material.textureNormal=textureEarthNormal;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Sphere;
			g->localMatrix=MatrixScale(100, 100, 100)*MatrixOffset(120, -120, 0);
			g->material.color=Color(1, 1, 1);
			g->material.textureDiffuse=textureEarth;
			g->material.textureNormal=textureEarthNormal;
			g->material.enableBlinnSpecular=true;
			g->material.blinnC3=0.2;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		{
			Geometry* g=new Cube;
			g->localMatrix=MatrixScale(100, 100, 100)*MatrixOffset(-120, 0, 0);
			g->material.color=Color(1, 1, 1);
			g->material.textureDiffuse=textureEarth;
			g->material.textureNormal=textureEarthNormal;
			g->Update();
			scene->scene.geometries.Add(g);
		}
		return scene;
	}
}
#include "SimulatorCore.h"
#include "..\..\..\..\Library\Threading.h"
#include <time.h>

using namespace vl;
using namespace vl::windows;

struct SimulatorHandle
{
	static const int FSAAScale=4;

	struct RenderData
	{
		SceneHandle*		scene;
		Renderer*			renderer;
		Color*				colors;
		int					bufferWidth;
		int					bufferHeight;
		int					screenWidth;
		int					screenHeight;
		int					startY;
		int					endY;
		Semaphore*			threadSemaphore;
		EventObject			eventObject;
	};

	Ptr<WinBitmap>			buffer;
	WinDC*					dc;
	Color*					colors;
	bool					enableFSAA;

	int						cpuCount;
	List<Ptr<Thread>>		threads;
	List<Ptr<RenderData>>	renderDatas;
	Semaphore				threadSemaphore;

	static void RenderProc(Thread* thread, void* data)
	{
		RenderData* rd=(RenderData*)data;
		while(true)
		{
			rd->threadSemaphore->Wait();
			rd->renderer->Render(
				&rd->scene->observer,
				&rd->scene->scene, 
				rd->bufferWidth, 
				rd->bufferHeight,
				rd->screenWidth,
				rd->screenHeight,
				rd->startY, 
				rd->endY,
				rd->colors);
			rd->eventObject.Signal();
		}
	}

	SimulatorHandle(bool _enableFSAA)
		:dc(0), colors(0), enableFSAA(_enableFSAA)
	{
#ifdef _DEBUG
		cpuCount=1;
#endif
#ifdef NDEBUG
		cpuCount=Thread::GetCPUCount();
#endif
		threadSemaphore.Create(0, cpuCount);
		for(int i=0;i<cpuCount;i++)
		{
			RenderData* rd=new RenderData;
			rd->threadSemaphore=&threadSemaphore;
			rd->eventObject.CreateManualUnsignal(false);
			renderDatas.Add(rd);

			Thread* thread=Thread::CreateAndStart(&RenderProc, rd);
			thread->SetCPU(i);
			threads.Add(thread);
		}
	}

	Color Render(SceneHandle* scene, Renderer* renderer, int screenWidth, int screenHeight, int x, int y)
	{
		return renderer->Render(&scene->observer, &scene->scene, screenWidth, screenHeight, x, y);
	}

	void Render(SceneHandle* scene, Renderer* renderer, Color* colors, int bufferWidth, int bufferHeight, int screenWidth, int screenHeight)
	{
		for(int i=0;i<cpuCount;i++)
		{
			RenderData* rd=renderDatas[i].Obj();
			rd->scene=scene;
			rd->renderer=renderer;
			rd->colors=colors;
			rd->bufferWidth=bufferWidth;
			rd->bufferHeight=bufferHeight;
			rd->screenWidth=screenWidth;
			rd->screenHeight=screenHeight;
			rd->startY=i*(bufferHeight/cpuCount);
			rd->endY=(i==cpuCount
				?bufferHeight-1
				:(i+1)*(bufferHeight/cpuCount)-1);
			rd->eventObject.Unsignal();
		}
		threadSemaphore.Release(cpuCount);
		for(int i=0;i<cpuCount;i++)
		{
			RenderData* rd=renderDatas[i].Obj();
			rd->eventObject.Wait();
		}
	}
};

extern "C"
{
	SIMULATORCORE_API SimulatorHandle* __stdcall CreateSimulator(bool enableFSAA)
	{
		srand((unsigned)time(0));
		return new SimulatorHandle(enableFSAA);
	}

	SIMULATORCORE_API void __stdcall DestroySimulator(SimulatorHandle* simulator)
	{
		if(simulator->colors)delete[] simulator->colors;
		delete simulator;
	}

	SIMULATORCORE_API void __stdcall SetSimulatorBuffer(SimulatorHandle* simulator, int width, int height)
	{
		simulator->buffer=new WinBitmap(width, height, WinBitmap::vbb32Bits, true);
		simulator->dc=simulator->buffer->GetWinDC();
		simulator->dc->SetBrush(new WinBrush(RGB(0, 0, 0)));
		simulator->dc->SetPen(new WinPen(PS_SOLID, 1, RGB(255, 255, 255)));
		
		if(simulator->colors)delete[] simulator->colors;
		simulator->colors=new Color[width*height*
			(simulator->enableFSAA?SimulatorHandle::FSAAScale*SimulatorHandle::FSAAScale:1)
		];
	}

	SIMULATORCORE_API void __stdcall ClearSimulatorBuffer(SimulatorHandle* simulator)
	{
		if(simulator->dc)
		{
			simulator->dc->FillRect(0, 0, simulator->buffer->GetWidth(), simulator->buffer->GetHeight());
		}
	}

	SIMULATORCORE_API WindowsHandle* __stdcall GetSimulatorBitmap(SimulatorHandle* simulator)
	{
		return (WindowsHandle*)simulator->buffer->GetBitmap();
	}

	SIMULATORCORE_API void __stdcall RotateScene(SceneHandle* scene, double xy, double yz, double zx)
	{
		Matrix4 m=MatrixRotateXY(xy)*MatrixRotateYZ(yz)*MatrixRotateZX(zx);
		for(int i=0;i<scene->scene.geometries.Count();i++)
		{
			Geometry* g=scene->scene.geometries[i];
			g->localMatrix*=m;
			g->Update();
		}
		if(scene->scene.skyGeometry)
		{
			scene->scene.skyGeometry->localMatrix*=m;
			scene->scene.skyGeometry->Update();
		}
		//Matrix4 p=MatrixInverse(m);
		//Matrix4 n=MatrixNormal(p);
		//for(int i=0;i<scene->scene.parallelLights.Count();i++)
		//{
		//	ParallelLight& light=scene->scene.parallelLights[i];
		//	light.direction*=n;
		//}
		//scene->observer.eyePosition*=p;
		//scene->observer.eyeForward*=n;
		//scene->observer.eyeUp*=n;
		//scene->observer.Update();
	}
	
	SIMULATORCORE_API void __stdcall DestroyScene(SceneHandle* scene)
	{
		delete scene;
	}

	SIMULATORCORE_API int __stdcall GetTriangleCount(SceneHandle* scene)
	{
		int count=0;
		for(int i=0;i<scene->scene.geometries.Count();i++)
		{
			Geometry* g=scene->scene.geometries[i];
			if(g->shape==Geometry::Triangles)
			{
				count+=((Triangles*)g)->triangles.Count();
			}
		}
		return count;
	}

	SIMULATORCORE_API Renderer* __stdcall CreateDirectTrayRenderer(int level)
	{
		return new DirectTrayRenderer(level);
	}

	SIMULATORCORE_API void __stdcall DestroyRenderer(Renderer* renderer)
	{
		delete renderer;
	}

	__forceinline BYTE ConvertColor(double c)
	{
		return
			c<0?0:
			c>1?255:
			(BYTE)(c*255);
	}

	struct SimulatorScenePair
	{
		SimulatorHandle* simulator;
		SceneHandle* scene;
	};

	void DrawFrameLine(Geometry* geometry, void* context, const Vector3& p1, const Vector3& p2)
	{
		SimulatorScenePair* pair=(SimulatorScenePair*)context;
		Vector3 mp1=p1*geometry->localMatrix;
		Vector3 mp2=p2*geometry->localMatrix;
		if(mp1.y>-500 && mp2.y>-500)
		{
			Vector3 v1=pair->scene->observer.Perspect(mp1);
			Vector3 v2=pair->scene->observer.Perspect(mp2);
			int w=pair->simulator->buffer->GetWidth();
			int h=pair->simulator->buffer->GetHeight();
			pair->simulator->dc->MoveTo((int)(v1.x+w/2), (int)(h/2-v1.y));
			pair->simulator->dc->LineTo((int)(v2.x+w/2), (int)(h/2-v2.y));
		}
	}

	SIMULATORCORE_API int __stdcall DebuggerIntersect(SimulatorHandle* simulator, SceneHandle* scene, Renderer* renderer, int x, int y)
	{
		if(renderer==0)
		{
			return 0;
		}
		else
		{
			Color result=simulator->Render(scene, renderer, simulator->buffer->GetWidth(), simulator->buffer->GetHeight(), x, y);
			return RGB(ConvertColor(result.x), ConvertColor(result.y), ConvertColor(result.z));
		}
	}

	SIMULATORCORE_API void __stdcall RenderScene(SimulatorHandle* simulator, SceneHandle* scene, Renderer* renderer)
	{
		if(renderer)
		{
			Color* colors=simulator->colors;
			int scale=(simulator->enableFSAA?SimulatorHandle::FSAAScale:1);
			int bw=simulator->buffer->GetWidth()*scale;
			int bh=simulator->buffer->GetHeight()*scale;
			int sw=simulator->buffer->GetWidth();
			int sh=simulator->buffer->GetHeight();
			simulator->Render(scene, renderer, colors, bw, bh, sw, sh);
			if(simulator->enableFSAA)
			{
				for(int y=0;y<sh;y++)
				{
					BYTE* scanline=simulator->buffer->GetScanLines()[y];
					for(int x=0;x<sw;x++)
					{
						Color color;
						for(int a=0;a<SimulatorHandle::FSAAScale;a++)
							for(int b=0;b<SimulatorHandle::FSAAScale;b++)
								color+=colors[(y*SimulatorHandle::FSAAScale+a)*bw+(x*SimulatorHandle::FSAAScale+b)];
						color=Scale(color, 1.0/(SimulatorHandle::FSAAScale*SimulatorHandle::FSAAScale));

						scanline[x*4+0]=ConvertColor(color.z);
						scanline[x*4+1]=ConvertColor(color.y);
						scanline[x*4+2]=ConvertColor(color.x);
						scanline[x*4+3]=255;
					}
				}
			}
			else
			{
				for(int y=0;y<sh;y++)
				{
					BYTE* scanline=simulator->buffer->GetScanLines()[y];
					for(int x=0;x<sw;x++)
					{
						Color color=*colors++;
						scanline[x*4+0]=ConvertColor(color.z);
						scanline[x*4+1]=ConvertColor(color.y);
						scanline[x*4+2]=ConvertColor(color.x);
						scanline[x*4+3]=255;
					}
				}
			}
		}
		else
		{
			SimulatorScenePair pair={simulator, scene};
			ClearSimulatorBuffer(simulator);
			for(int i=0;i<scene->scene.geometries.Count();i++)
			{
				scene->scene.geometries[i]->DrawFrame(&pair, &DrawFrameLine);
			}
		}
	}
}

void InstallSky(Scene* scene)
{
	WString textureFolder=GetTextureFolder();
	Cube* cube=new Cube();

	cube->px=scene->LoadTexture(textureFolder+L"SkyBox\\+x.bmp");
	cube->px->filter=Texture::Linear;
	cube->nx=scene->LoadTexture(textureFolder+L"SkyBox\\-x.bmp");
	cube->nx->filter=Texture::Linear;

	cube->py=scene->LoadTexture(textureFolder+L"SkyBox\\+y.bmp");
	cube->py->filter=Texture::Linear;
	cube->ny=scene->LoadTexture(textureFolder+L"SkyBox\\-y.bmp");
	cube->ny->filter=Texture::Linear;

	cube->pz=scene->LoadTexture(textureFolder+L"SkyBox\\+z.bmp");
	cube->pz->filter=Texture::Linear;
	cube->nz=scene->LoadTexture(textureFolder+L"SkyBox\\-z.bmp");
	cube->nz->filter=Texture::Linear;

	scene->skyGeometry=cube;
}

WString GetTextureFolder()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	int i=wcslen(buffer)-1;
	while(buffer[i]!=L'\\')i--;
	return WString(buffer, i)+L"\\..\\Textures\\";
}
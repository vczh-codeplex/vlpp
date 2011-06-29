#ifdef SIMULATORCORE_EXPORTS
#define SIMULATORCORE_API __declspec(dllexport)
#else
#define SIMULATORCORE_API __declspec(dllimport)
#endif

#ifdef SIMULATORCORE_EXPORTS
	#include "..\..\Graphics\WinGDI.h"
	#include "..\..\Graphics\SimGeometries.h"

	using namespace simulator;
	using namespace simulator::geometries;
	using namespace simulator::renderers;

	struct SimulatorHandle;
	struct SceneHandle
	{
		Observer				observer;
		Scene					scene;
	};

	extern void					InstallSky(Scene* scene);
	extern WString				GetTextureFolder();
#else
	struct SimulatorHandle;
	struct SceneHandle;
	struct Renderer;
#endif

extern "C"
{
	struct WindowsHandle;

	SIMULATORCORE_API SimulatorHandle*	__stdcall 		CreateSimulator(bool enableFSAA);
	SIMULATORCORE_API void				__stdcall 		DestroySimulator(SimulatorHandle* simulator);
	SIMULATORCORE_API void				__stdcall 		SetSimulatorBuffer(SimulatorHandle* simulator, int width, int height);
	SIMULATORCORE_API void				__stdcall 		ClearSimulatorBuffer(SimulatorHandle* simulator);
	SIMULATORCORE_API WindowsHandle*	__stdcall 		GetSimulatorBitmap(SimulatorHandle* simulator);
	
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene1();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene2();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene3();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene4();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene5();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene6();
	SIMULATORCORE_API SceneHandle*		__stdcall		CreateScene7();
	SIMULATORCORE_API void				__stdcall		RotateScene(SceneHandle* scene, double xy, double yz, double zx);
	SIMULATORCORE_API void				__stdcall		DestroyScene(SceneHandle* scene);
	SIMULATORCORE_API int				__stdcall		GetTriangleCount(SceneHandle* scene);

	SIMULATORCORE_API Renderer*			__stdcall		CreateDirectTrayRenderer(int level);
	SIMULATORCORE_API void				__stdcall		DestroyRenderer(Renderer* renderer);
	SIMULATORCORE_API int				__stdcall		DebuggerIntersect(SimulatorHandle* simulator, SceneHandle* scene, Renderer* renderer, int x, int y);
	SIMULATORCORE_API void				__stdcall		RenderScene(SimulatorHandle* simulator, SceneHandle* scene, Renderer* renderer);
}
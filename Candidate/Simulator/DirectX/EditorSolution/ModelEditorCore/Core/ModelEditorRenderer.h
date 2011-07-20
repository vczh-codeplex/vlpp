#ifndef VCZH_DIRECTX_MODELEDITORRENDERER
#define VCZH_DIRECTX_MODELEDITORRENDERER

#include "..\..\Shared\DirectXSetup.h"
#include "..\..\Shared\DirectXShader.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{
	struct ModelEditorWindow
	{
		HWND							editorControl;
		UINT							subclass;
		SIZE							clientSize;

		DirectXEnvironment*				env;
		DirectXDepthBuffer*				depthBuffer;
		DirectXWindowRenderTarget*		renderTarget;
		DirectXRenderer*				renderer;
		DirectXViewport*				viewport;

		ModelEditorWindow(HWND _editorControl);
		~ModelEditorWindow();

		void							Resize();
		void							Render();
	};
}

#endif
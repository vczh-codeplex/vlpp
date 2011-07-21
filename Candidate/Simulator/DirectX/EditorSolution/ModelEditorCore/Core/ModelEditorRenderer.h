#ifndef VCZH_DIRECTX_MODELEDITORRENDERER
#define VCZH_DIRECTX_MODELEDITORRENDERER

#include "..\..\Shared\DirectXSetup.h"
#include "..\..\Shared\DirectXShader.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{
	struct VertexAxis
	{
		D3DXVECTOR3								position;
	};

	struct ConstantBuffer
	{
		D3DXMATRIX								worldMatrix;
		D3DXMATRIX								viewMatrix;
		D3DXMATRIX								projectionMatrix;
	};

	class ModelEditorWindow
	{
	public:
		HWND									editorControl;
		UINT									subclass;
		SIZE									clientSize;

	protected:
		WString									workingDirectory;
		DirectXEnvironment*						env;
		DirectXDepthBuffer*						depthBuffer;
		DirectXWindowRenderTarget*				renderTarget;
		DirectXRenderer*						renderer;
		DirectXViewport*						viewport;

		DirectXConstantBuffer<ConstantBuffer>*	constantBuffer;
		DirectXVertexBuffer<VertexAxis>*		geometryAxis;
		DirectXShader<VertexAxis>*				shaderAxis;

		void									UpdateConstantBuffer();
		void									UpdateGeometryAxis();

		void									Initialize();
		void									Finalize();
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		void									Resize();
		void									Render();
	};
}

#endif
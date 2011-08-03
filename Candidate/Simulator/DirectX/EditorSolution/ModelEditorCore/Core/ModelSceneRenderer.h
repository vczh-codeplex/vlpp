#ifndef VCZH_DIRECTX_MODELSCENERENDERER
#define VCZH_DIRECTX_MODELSCENERENDERER

#include "ModelDataStructure.h"
#include "..\..\Shared\WindowGDI.h"
#include "..\..\..\..\..\..\Library\Pointer.h"

using namespace vl;

namespace modeleditor
{
	class ModelSceneRenderer
	{
	private:
		static const int						EditorModeTextureSize=512;
	public:
		HWND									editorControl;
		UINT									subclass;
		SIZE									clientSize;

	private:
		WString									workingDirectory;
		DirectXEnvironment*						env;
		DirectXDepthBuffer*						depthBuffer;
		DirectXWindowRenderTarget*				renderTarget;
		DirectXRenderer*						renderer;
		DirectXViewport*						viewport;
		DirectXTextureBuffer*					selectorStagingBuffer;
		DirectXTextureBuffer*					selectorBuffer;
		DirectXTextureRenderTarget*				selectorRenderTarget;

		DirectXConstantBuffer<ConstantBuffer>*	constantBuffer;
		DirectXVertexBuffer<VertexAxis>*		geometryAxisLineGlobal;
		DirectXVertexBuffer<VertexAxis>*		geometryAxisLineLocal;
		DirectXVertexBuffer<VertexObject>*		geometryAxisObject;
		DirectXVertexBuffer<VertexAxis>*		geometryVertexHighlightOuter;
		DirectXVertexBuffer<VertexAxis>*		geometryVertexHighlightInner;
		DirectXVertexBuffer<VertexAxis>*		geometryRanging;
		DirectXShader<VertexAxis>*				shaderAxis;
		DirectXShader<VertexObject>*			shaderObject;
		DirectXShader<VertexObject>*			shaderSelectedObject;
		DirectXShader<VertexObject>*			shaderSelector;

		D3DXVECTOR3								viewAt, viewFront, viewUp, viewLeft;
		float									viewAngleVertical;
		float									viewAngleHorizontal;
		float									viewDistance;

		Ptr<WinBitmap>							editorModeBitmap;
		DirectXTextureBuffer*					editorModeTexture;
		DirectXVertexBuffer<VertexImage>*		editorModeRectangle;
		DirectXShader<VertexImage>*				editorModeShader;
		DirectXSamplerBuffer*					editorModeSampler;

		int										rangingX1, rangingX2, rangingY1, rangingY2;
		bool									rangingVisible;

	private:
		void									ViewCalculateDirection();
		void									ViewCalculateMatrix(D3DXMATRIX& viewMatrix);
		void									UpdateConstantBuffer(const D3DXMATRIX& worldMatrix);
		void									UpdateGeometryAxis();

		void									CreateRectangle(DirectXVertexBuffer<VertexAxis>* geometry, D3DXCOLOR color);
		void									Initialize();
		void									Finalize();

		void									RenderAxisLine();
		void									RenderAxisObject();
		void									UpdateEditorMode();
		void									RenderEditorMode();
		void									RenderVertexHighlights();
		void									RenderRanging();

	protected:
		virtual void							CallbackRebuildModels()=0;
		virtual void							CallbackDrawEditorMode(WinDC* dc)=0;
		virtual void							CallbackRenderModels(bool onlySelected, DirectXShader<VertexObject>* normalObjectShader, DirectXShader<VertexObject>* selectedObjectShader)=0;
		virtual void							CallbackRenderVertexHighlights()=0;
		virtual void							CallbackRenderSelectorSelected()=0;
		virtual bool							CallbackRenderLocalAxis(D3DXMATRIX& worldMatrix)=0;

		void									ToolRenderSelector(bool onlySelected);
		unsigned __int32						ToolGetSelectorResult(int x, int y);
		void									ToolSetWorldMatrix(const D3DXMATRIX& worldMatrix);
		void									ToolDrawEditorMode();
		void									ToolCalculateVertexHighlight(const D3DXMATRIX& worldMatrix, D3DXVECTOR3 vertex, int& x, int& y);
		void									ToolDrawVertexHighlight(const D3DXMATRIX& worldMatrix, D3DXVECTOR3 vertex);

		void									Constructor();
		void									Destructor();
	public:
		ModelSceneRenderer(HWND _editorControl, const WString& _workingDirectory);
		~ModelSceneRenderer();

		DirectXEnvironment*						Env();

		void									Resize();
		void									Render();

		void									ViewReset();
		void									ViewRotateVertical(float angle);
		void									ViewRotateHorizontal(float angle);
		void									ViewMove(float left, float up, float front);

		void									StartRanging(int x, int y);
		void									ContinueRanging(int x, int y);
		void									StopRanging();
		bool									GetRanging(int& x, int& y, int& w, int& h);

		D3DXVECTOR3								GetViewAt();
		D3DXVECTOR3								GetViewFront();
		D3DXVECTOR3								GetViewLeft();
		D3DXVECTOR3								GetViewUp();
		float									GetViewVertical();
		float									GetViewHorizontal();
		float									GetViewDistance();
	};
}

#endif
#ifndef VCZH_DIRECTX_MODELEDITORRENDERER
#define VCZH_DIRECTX_MODELEDITORRENDERER

#include "..\..\Shared\DirectXSetup.h"
#include "..\..\Shared\DirectXShader.h"
#include "..\..\..\..\..\..\Library\Pointer.h"

using namespace vl;
using namespace vl::directx;
using namespace vl::collections;

namespace modeleditor
{
	struct VertexAxis
	{
		D3DXVECTOR3								position;
		D3DXCOLOR								color;
	};
	
	struct VertexObject
	{
		D3DXVECTOR3								position;
		D3DXVECTOR3								normal;
		D3DXCOLOR								color;
		unsigned __int32						id;

		bool operator==(const VertexObject& vertex){return false;}
		bool operator!=(const VertexObject& vertex){return true;}
	};

	struct ConstantBuffer
	{
		D3DXMATRIX								worldMatrix;
		D3DXMATRIX								viewMatrix;
		D3DXMATRIX								projectionMatrix;
	};

	class Model
	{
	protected:
		DirectXEnvironment*						env;
		DirectXVertexBuffer<VertexObject>*		geometry;
	public:
		Array<VertexObject>						vertices;
		Array<unsigned int>						indices;
		D3DXMATRIX								worldMatrix;
		bool									selected;

		Model(DirectXEnvironment* _env);
		~Model();

		void									Update();
		void									Rebuild(DirectXEnvironment* _env);
		DirectXVertexBuffer<VertexObject>*		Geometry();
	};

	namespace ModelEditorOperation
	{
		enum Enum
		{
			None,
			Rotation,
			Zooming,
			Moving,
		};
	};

	struct ModelEditorData
	{
		int										originX, originY;
		ModelEditorOperation::Enum				modelEditorOperation;
		bool									modelEditorOperationActivated;

		ModelEditorData();
		~ModelEditorData();
	};

	class ModelEditorWindow
	{
	public:
		HWND									editorControl;
		UINT									subclass;
		SIZE									clientSize;
		ModelEditorData							modelEditorData;

	protected:
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
		DirectXVertexBuffer<VertexAxis>*		geometryAxisLine;
		Model*									geometryAxisObject;
		DirectXShader<VertexAxis>*				shaderAxis;
		DirectXShader<VertexObject>*			shaderObject;
		DirectXShader<VertexObject>*			shaderSelectedObject;
		DirectXShader<VertexObject>*			shaderSelector;

		D3DXVECTOR3								viewAt, viewFront, viewUp, viewLeft;
		float									viewAngleVertical;
		float									viewAngleHorizontal;
		float									viewDistance;

	protected:
		List<Ptr<Model>>						models;

	protected:

		void									ViewCalculateDirection();
		void									UpdateConstantBuffer(const D3DXMATRIX& worldMatrix);
		void									UpdateGeometryAxis();

		void									Initialize();
		void									Finalize();
		void									RebuildModels();

		void									RenderSelector();
		void									RenderSelectorModelIndexIncremented();
		void									RenderSelectorSelected();
		unsigned __int32						GetSelectorResult(int x, int y);
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		DirectXEnvironment*						Env();

		void									AddModel(Model* model);
		void									RemoveModel(Model* model);
		int										ModelCount();
		Model*									GetModel(int index);

		void									Resize();
		void									Render();
		int										QueryModel(int x, int y);
		void									SelectModel(int index);

		void									ViewReset();
		void									ViewRotateVertical(float angle);
		void									ViewRotateHorizontal(float angle);
		void									ViewMove(float left, float up, float front);
	};
}

#endif
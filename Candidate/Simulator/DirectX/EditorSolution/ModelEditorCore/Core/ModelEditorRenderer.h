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
	};
	
	struct VertexObject
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXCOLOR Color;

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

		Model(DirectXEnvironment* _env);
		~Model();

		void									Update();
		void									Rebuild(DirectXEnvironment* _env);
		DirectXVertexBuffer<VertexObject>*		Geometry();
	};

	namespace ViewOperation
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
		ViewOperation::Enum						viewOperation;
		bool									viewOperationActivated;

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

		DirectXConstantBuffer<ConstantBuffer>*	constantBuffer;
		DirectXVertexBuffer<VertexAxis>*		geometryAxis;
		DirectXShader<VertexAxis>*				shaderAxis;
		DirectXShader<VertexObject>*			shaderObject;

		D3DXVECTOR3								viewCenterPosition, viewAt, viewUp, viewLeft;
		float									viewAngleVertical;
		float									viewAngleHorizontal;
		float									viewDistance;

	protected:
		List<Ptr<Model>>						models;

	protected:

		void									ViewCalculateUpAt();
		void									UpdateConstantBuffer(const D3DXMATRIX& worldMatrix);
		void									UpdateGeometryAxis();

		void									Initialize();
		void									Finalize();
		void									RebuildModels();
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

		void									ViewReset();
		void									ViewRotateVertical(float angle);
		void									ViewRotateHorizontal(float angle);
		void									ViewWalk(float distance);
		void									ViewMove(float left, float up);
	};
}

#endif
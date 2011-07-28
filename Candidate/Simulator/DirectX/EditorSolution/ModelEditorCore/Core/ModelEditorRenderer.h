#ifndef VCZH_DIRECTX_MODELEDITORRENDERER
#define VCZH_DIRECTX_MODELEDITORRENDERER

#include "ModelDataStructure.h"
#include "ModelObject.h"
#include "ModelSceneRenderer.h"

namespace modeleditor
{
	namespace ModelEditorOperation
	{
		enum Enum
		{
			None,
			ViewRotation,
			ViewZooming,
			ViewMoving,
			ObjectEditing,
		};
	};

	namespace ModelEditorMode
	{
		enum Enum
		{
			ObjectSelection,
			ObjectTranslation,
			ObjectRotation,
			ObjectScaling,
		};
	};

	namespace ModelEditorAxis
	{
		enum Enum
		{
			AxisGlobal,
			AxisLocal,
		};
	};

	namespace ModelEditorAxisDirection
	{
		enum Enum
		{
			None,
			X,
			Y,
			Z,
		};
	};

	struct ModelEditorData
	{
		int										originX, originY;
		ModelEditorOperation::Enum				modelEditorOperation;
		bool									modelEditorOperationActivated;
		ModelEditorMode::Enum					modelEditorMode;
		ModelEditorAxis::Enum					modelEditorAxis;
		ModelEditorAxisDirection::Enum			modelEditorAxisDirection;

		ModelEditorData();
		~ModelEditorData();
	};

	class ModelEditorWindow : public ModelSceneRenderer
	{
	public:
		ModelEditorData							modelEditorData;

	protected:
		List<Ptr<Model>>						models;
		Model*									mainSelectedModel;

	protected:
		void									CallbackRebuildModels();
		void									CallbackDrawEditorMode(WinDC* dc);
		void									CallbackRenderModels(bool onlySelected, DirectXShader<VertexObject>* normalObjectShader, DirectXShader<VertexObject>* selectedObjectShader);
		void									CallbackRenderSelectorSelected();
		bool									CallbackRenderLocalAxis(D3DXMATRIX& worldMatrix);

		void									RenderSelectorModelIndexIncremented();
		void									RenderSelectorSelected();
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		void									AddModel(Model* model);
		void									RemoveModel(Model* model);
		int										ModelCount();
		Model*									GetModel(int index);
		int										QueryModel(int x, int y);
		void									SelectModel(int index);
		Model*									GetMainSelectedModel();

		void									SetEditorMode(ModelEditorMode::Enum value);
		void									SetEditorAxis(ModelEditorAxis::Enum value);
		void									SetEditorAxisDirection(ModelEditorAxisDirection::Enum value);
	};
}

#endif
#ifndef VCZH_DIRECTX_MODELEDITORRENDERER
#define VCZH_DIRECTX_MODELEDITORRENDERER

#include "ModelDataStructure.h"
#include "ModelObject.h"
#include "ModelSceneRenderer.h"

namespace modeleditor
{
	class ModelEditorRenderer : public ModelSceneRenderer
	{
	protected:
		List<Ptr<Model>>						models;
		Model*									mainSelectedModel;

	protected:
		void									CallbackRebuildModels();
		void									CallbackRenderModels(bool onlySelected, DirectXShader<VertexObject>* normalObjectShader, DirectXShader<VertexObject>* selectedObjectShader);
		void									CallbackRenderVertexHighlights();
		void									CallbackRenderSelectorSelected();
		bool									CallbackRenderLocalAxis(D3DXMATRIX& worldMatrix);

		void									RenderSelectorModelIndexIncremented();
		void									RenderSelectorFaceIndexIncremented();
		void									RenderSelectorSelected();
	public:
		ModelEditorRenderer(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorRenderer();

		void									AddModel(Model* model);
		void									RemoveModel(Model* model);
		int										ModelCount();
		Model*									GetModel(int index);

		int										QueryModel(int x, int y);
		bool									QueryFace(int x, int y, int& modelIndex, int& faceIndex);
		bool									QueryVertex(int x, int y, int& modelIndex, int& vertexIndex);
		void									SelectModel(int index);
		void									SelectFace(int index, int faceIndex);
		void									SelectFaceFromSelectedModels(int x, int y, int w, int h);
		void									SelectVertex(int index, int vertexIndex);
		void									SelectVertexFromSelectedModels(int x, int y, int w, int h);
		Model*									GetMainSelectedModel();

		void									DeleteSelectedFaces(Model* model);
		void									DeleteSelection();
	};
}

#endif
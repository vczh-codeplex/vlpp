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
		struct PushDataDistanceVertex
		{
			Model*								model;
			int									vertexIndex;
			D3DXVECTOR3							originalPosition;
			D3DXVECTOR3							normal;

			bool operator==(const PushDataDistanceVertex& value){return false;}
			bool operator!=(const PushDataDistanceVertex& value){return true;}
		};

		struct PushDataPercentVertex
		{
			Model*								model;
			int									vertexIndex;
			D3DXVECTOR3							p1;
			D3DXVECTOR3							p2;
			float								originalPercent;
			float								percent;

			bool operator==(const PushDataPercentVertex& value){return false;}
			bool operator!=(const PushDataPercentVertex& value){return true;}
		};

		friend class ReadonlyListEnumerator<PushDataDistanceVertex>;
		friend class ReadonlyListEnumerator<PushDataPercentVertex>;

		struct PushData
		{
			List<PushDataDistanceVertex>		distanceVertices;
			List<PushDataPercentVertex>			percentVertices;
			List<Model*>						affectedModels;

			void								Clear();
			bool								Available();
		};
	protected:
		List<Ptr<Model>>						models;
		Model*									mainSelectedModel;
		PushData								pushData;

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
		void									SelectModel(int index, bool append);
		void									SelectFace(int index, int faceIndex, bool append);
		void									SelectFaceFromSelectedModels(int x, int y, int w, int h, bool append, bool includeBackFaces);
		void									SelectVertex(int index, int vertexIndex, bool append);
		void									SelectVertexFromSelectedModels(int x, int y, int w, int h, bool append, bool includeBackFaces);
		Model*									GetMainSelectedModel();

		void									DeleteSelectedFaces(Model* model);
		void									DeleteSelection();
		void									DeleteSelectedLineBetweenSelectionPoints();
		void									AddLineBetweenSelectionPoints();
		void									AddPointBetweenSelectionPoints(int count);

		bool									PushSelectedFaces();
		bool									PushSelectedLines();
		bool									PushSelectedPoints();
		void									PushModify(float distance, float percent);
		void									PushStopModify();
	};
}

#endif
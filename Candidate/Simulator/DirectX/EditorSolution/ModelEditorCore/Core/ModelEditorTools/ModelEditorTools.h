#ifndef VCZH_DIRECTX_MODELEDITORTOOLS
#define VCZH_DIRECTX_MODELEDITORTOOLS

#include "..\ModelEditorWindow.h"

namespace modeleditor
{
	class MetObjectSelection : public ModelEditorTool
	{
	public:
		MetObjectSelection(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Select\t\t{LBUTTON(click) + CTRL?}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetFaceSelection : public ModelEditorTool
	{
	public:
		MetFaceSelection(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Select Face\t{LBUTTON(click|drag + SHIFT?) + CTRL?}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetVertexSelection : public ModelEditorTool
	{
	public:
		MetVertexSelection(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Select Vertex\t{LBUTTON(click|drag + SHIFT?) + CTRL?}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetTranslation : public ModelEditorTool
	{
	public:
		MetTranslation(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Move\t\t{LBUTTON + [XYZ]}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetRotation : public ModelEditorTool
	{
	public:
		MetRotation(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Rotate\t\t{LBUTTON + [XYZ]}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetScaling : public ModelEditorTool
	{
	public:
		MetScaling(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Scale\t\t{LBUTTON + [X|Y|Z|SHIFT]*}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
	
	class MetPushing : public ModelEditorTool
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

		PushData								pushData;
	public:
		MetPushing(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Pushing\t\t{LBUTTON + SHIFT?}";}

		void									Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);

		bool									PushSelectedFaces();
		bool									PushSelectedLines();
		bool									PushSelectedPoints();
		void									PushModify(float distance, float percent);
		void									PushStopModify();
	};
}

#endif
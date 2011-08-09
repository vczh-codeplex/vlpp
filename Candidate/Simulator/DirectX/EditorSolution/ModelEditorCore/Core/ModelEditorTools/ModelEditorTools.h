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
	public:
		MetPushing(ModelEditorWindow* _editorWindow)
			:ModelEditorTool(_editorWindow)
		{
		}

		WString Name(){return L"Pushing\t\t{LBUTTON + SHIFT?}";}

		void Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass);
	};
}

#endif
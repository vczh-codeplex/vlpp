#ifndef VCZH_DIRECTX_MODELEDITORWINDOWSUBCLASS
#define VCZH_DIRECTX_MODELEDITORWINDOWSUBCLASS

#include "ModelEditorWindow.h"

namespace modeleditor
{
	extern bool					ToolObjectEditingInfo(D3DXVECTOR3& axis, Model*& selectedLocalModel, ModelEditorWindow* editorWindow, bool enableAxisCombination);
	extern void					ToolObjectTransformSelectedVertices(Model* model, const D3DXMATRIX& transformation);
	extern D3DXVECTOR3			ToolObjectTransform(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& inverseMatrix, const D3DXMATRIX& transformation, D3DXVECTOR3 modelVertex);

	extern LRESULT CALLBACK		EditorWindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
}

#endif
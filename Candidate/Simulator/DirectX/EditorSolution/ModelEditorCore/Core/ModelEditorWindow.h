#ifndef VCZH_DIRECTX_MODELEDITORWINDOW
#define VCZH_DIRECTX_MODELEDITORWINDOW

#include "ModelEditorRenderer.h"

namespace modeleditor
{
	namespace ModelEditorMode
	{
		enum Enum
		{
			ObjectSelection,
			ObjectFaceSelection,
			ObjectVertexSelection,
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
		ModelEditorMode::Enum					modelEditorMode;
		ModelEditorAxis::Enum					modelEditorAxis;
		ModelEditorAxisDirection::Enum			modelEditorAxisDirection;

		ModelEditorData();
		~ModelEditorData();
	};

	class ModelEditorWindow : public ModelEditorRenderer
	{
	public:
		typedef void(*ToolMessageProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	protected:
		void									CallbackDrawEditorMode(WinDC* dc);
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		ModelEditorData							modelEditorData;
		ToolMessageProc							currentToolMessageProc;

		void									SetEditorMode(ModelEditorMode::Enum value);
		void									SetEditorAxis(ModelEditorAxis::Enum value);
		void									SetEditorAxisDirection(ModelEditorAxisDirection::Enum value);
	};
}

#endif
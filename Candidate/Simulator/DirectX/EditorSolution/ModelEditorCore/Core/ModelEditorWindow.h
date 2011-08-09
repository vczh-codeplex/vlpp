#ifndef VCZH_DIRECTX_MODELEDITORWINDOW
#define VCZH_DIRECTX_MODELEDITORWINDOW

#include "ModelEditorRenderer.h"

namespace modeleditor
{
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
		ModelEditorAxis::Enum					modelEditorAxis;
		ModelEditorAxisDirection::Enum			modelEditorAxisDirection;
		bool									rotatingView;
		bool									executingEditorTool;

		ModelEditorData();
		~ModelEditorData();
	};

	class ModelEditorWindow;

	class ModelEditorTool : public Object
	{
	protected:
		ModelEditorWindow*						editorWindow;
	public:
		ModelEditorTool(ModelEditorWindow* _editorWindow);
		~ModelEditorTool();

		virtual void							Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)=0;
		virtual WString							Name()=0;
	};

	class ModelEditorWindow : public ModelEditorRenderer
	{
	protected:
		void									CallbackDrawEditorMode(WinDC* dc);
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		ModelEditorData							modelEditorData;
		Ptr<ModelEditorTool>					currentEditorTool;

		void									SetEditorTool(ModelEditorTool* editorTool);
		void									StopTemporaryEditorTool();
		void									SetEditorAxis(ModelEditorAxis::Enum value);
		void									SetEditorAxisDirection(ModelEditorAxisDirection::Enum value);
	};
}

#endif
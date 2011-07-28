#ifndef VCZH_DIRECTX_MODELEDITORWINDOW
#define VCZH_DIRECTX_MODELEDITORWINDOW

#include "ModelEditorRenderer.h"

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

	class ModelEditorWindow : public ModelEditorRenderer
	{
	protected:
		void									CallbackDrawEditorMode(WinDC* dc);
	public:
		ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory);
		~ModelEditorWindow();

		ModelEditorData							modelEditorData;

		void									SetEditorMode(ModelEditorMode::Enum value);
		void									SetEditorAxis(ModelEditorAxis::Enum value);
		void									SetEditorAxisDirection(ModelEditorAxisDirection::Enum value);
	};
}

#endif
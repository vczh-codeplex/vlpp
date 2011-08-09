#include "ModelEditorWindow.h"
#include "..\ModelEditorCore.h"

namespace modeleditor
{

/***********************************************************************
ModelEditorData
***********************************************************************/

	ModelEditorData::ModelEditorData()
		:originX(0)
		,originY(0)
		,modelEditorAxis(ModelEditorAxis::AxisGlobal)
		,modelEditorAxisDirection(ModelEditorAxisDirection::None)
		,rotatingView(false)
		,executingEditorTool(false)
	{
	}

	ModelEditorData::~ModelEditorData()
	{
	}

/***********************************************************************
ModelEditorTool
***********************************************************************/

	ModelEditorTool::ModelEditorTool(ModelEditorWindow* _editorWindow)
		:editorWindow(_editorWindow)
	{
	}

	ModelEditorTool::~ModelEditorTool()
	{
	}

/***********************************************************************
ModelEditorWindow
***********************************************************************/

	void ModelEditorWindow::CallbackDrawEditorMode(WinDC* dc)
	{
		WString axisDirection;
		switch(modelEditorData.modelEditorAxisDirection)
		{
		case ModelEditorAxisDirection::X:
			axisDirection=L"(X)";
			break;
		case ModelEditorAxisDirection::Y:
			axisDirection=L"(Y)";
			break;
		case ModelEditorAxisDirection::Z:
			axisDirection=L"(Z)";
			break;
		}

		dc->DrawString(10, 10, L"Change View\t\t\t\t\t{RBUTTON|MBUTTON|WHEEL}", 32, 10);
		if(currentEditorTool)
		{
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t:"+currentEditorTool->Name(), 32, 10);
		}
		switch(modelEditorData.modelEditorAxis)
		{
		case ModelEditorAxis::AxisGlobal:
			dc->DrawString(10, 50, L"Relative Axis[GL]\t\t: Global", 32, 10);
			break;
		case ModelEditorAxis::AxisLocal:
			dc->DrawString(10, 50, L"Relative Axis[GL]\t\t: Local", 32, 10);
			break;
		}
	}

	ModelEditorWindow::ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory)
		:ModelEditorRenderer(_editorControl, _workingDirectory)
	{
		Constructor();
		EditorModeSelection(this);
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		Destructor();
	}

	void ModelEditorWindow::SetEditorTool(ModelEditorTool* editorTool)
	{
		currentEditorTool=editorTool;
		ToolDrawEditorMode();
	}

	void ModelEditorWindow::StopTemporaryEditorTool()
	{
		EditorModeSelection(this);
	}

	void ModelEditorWindow::SetEditorAxis(ModelEditorAxis::Enum value)
	{
		modelEditorData.modelEditorAxis=value;
		ToolDrawEditorMode();
	}

	void ModelEditorWindow::SetEditorAxisDirection(ModelEditorAxisDirection::Enum value)
	{
		modelEditorData.modelEditorAxisDirection=value;
		ToolDrawEditorMode();
	}
}
#include "ModelEditorWindow.h"

namespace modeleditor
{

/***********************************************************************
ModelEditorData
***********************************************************************/

	ModelEditorData::ModelEditorData()
		:originX(0)
		,originY(0)
		,modelEditorMode(ModelEditorMode::ObjectSelection)
		,modelEditorAxis(ModelEditorAxis::AxisGlobal)
		,modelEditorAxisDirection(ModelEditorAxisDirection::None)
	{
	}

	ModelEditorData::~ModelEditorData()
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
		switch(modelEditorData.modelEditorMode)
		{
		case ModelEditorMode::ObjectSelection:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Select\t\t{LBUTTON}", 32, 10);
			break;
		case ModelEditorMode::ObjectFaceSelection:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Select Face\t{LBUTTON}", 32, 10);
			break;
		case ModelEditorMode::ObjectVertexSelection:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Select Vertex\t{LBUTTON}", 32, 10);
			break;
		case ModelEditorMode::ObjectTranslation:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Move"+axisDirection+L"\t\t{LBUTTON + [XYZ]}", 32, 10);
			break;
		case ModelEditorMode::ObjectRotation:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Rotate"+axisDirection+L"\t\t{LBUTTON + [XYZ]}", 32, 10);
			break;
		case ModelEditorMode::ObjectScaling:
			dc->DrawString(10, 30, L"Editor Mode[CFVTRS]\t: Scale\t\t{LBUTTON + [X|Y|Z|SHIFT]*}", 32, 10);
			break;
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
		,currentToolMessageProc(0)
	{
		Constructor();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		Destructor();
	}

	void ModelEditorWindow::SetEditorMode(ModelEditorMode::Enum value)
	{
		modelEditorData.modelEditorMode=value;
		ToolDrawEditorMode();
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
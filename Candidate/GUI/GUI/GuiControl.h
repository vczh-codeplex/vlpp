/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUICONTROL
#define VCZH_PRESENTATION_GUICONTROL

#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\String.h"
#include "..\..\..\Library\Event.h"
#include "..\..\..\Library\Collections\List.h"
#include "..\..\..\Library\Collections\Dictionary.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
������ʩ
***********************************************************************/

		class GuiBase : public Object
		{
		protected:
		public:
		};

		class GuiControl : public GuiBase
		{
		public:
			class Grid : public Object
			{
			};
		protected:
		public:
		};

		class GuiWindowBase : public GuiBase
		{
		protected:
		public:
		};

/***********************************************************************
�Ű�
***********************************************************************/

		class GuiNormalGrid : public GuiControl::Grid
		{
		};

/***********************************************************************
���㴰��
***********************************************************************/

		class GuiWindow : public GuiWindowBase
		{
		protected:
		public:
		};

/***********************************************************************
�ؼ���
***********************************************************************/

		class GuiLabel : public GuiControl
		{
		protected:
		public:
		};

		class GuiButtonBase : public GuiControl
		{
		protected:
		public:
		};

		class GuiButton : public GuiButtonBase
		{
		protected:
		public:
		};
	}
}

#endif
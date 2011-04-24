/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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
»ù´¡ÉèÊ©
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
ÅÅ°æ
***********************************************************************/

		class GuiNormalGrid : public GuiControl::Grid
		{
		};

/***********************************************************************
¶¥²ã´°¿Ú
***********************************************************************/

		class GuiWindow : public GuiWindowBase
		{
		protected:
		public:
		};

/***********************************************************************
¿Ø¼þ¿â
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
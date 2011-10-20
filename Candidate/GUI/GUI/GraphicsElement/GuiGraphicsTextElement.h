/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSTEXTELEMENT
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSTEXTELEMENT

#include "GuiGraphicsElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
ColorizedText
***********************************************************************/

			namespace text
			{
				struct CharAtt
				{
					unsigned __int16	rightOffset;
					unsigned __int16	colorIndex;
				};

				struct TextLine
				{
					static const int	BlockSize=32;
					static const int	MaxWidth=0xFFFF;

					wchar_t*			text;
					CharAtt*			att;
					int					availableOffsetCount;
					int					bufferLength;
					int					dataLength;

					TextLine();
					~TextLine();

					static int			CalculateBufferLength(int dataLength);
					bool				operator==(const TextLine& value)const{return false;}
					bool				operator!=(const TextLine& value)const{return true;}

					void				Initialize();
					void				Finalize();
					bool				IsReady();
					bool				Modify(int start, int count, const wchar_t* input, int inputCount);
					TextLine			Split(int index);
					void				AppendAndFinalize(TextLine& line);
				};

				class ICharMeasurer : public Interface
				{
				public:
					virtual void		SetFont(const FontProperties& font)=0;
					virtual Size		Measure(wchar_t character)=0;
					virtual int			GetRowHeight()=0;
				};

				struct TextLines
				{
					typedef collections::List<TextLine>		TextLineList;
				protected:
					TextLineList		lines;
					ICharMeasurer*		charMeasurer;
				public:
					TextLines();
					~TextLines();

					int					GetCount();
					TextLine&			GetLine(int row);
					ICharMeasurer*		GetCharMeasurer();
					void				SetCharMeasurer(ICharMeasurer* value);
					WString				GetText(TextPos start, TextPos end);
					WString				GetText();

					bool				RemoveLines(int start, int count);
					bool				IsAvailable(TextPos pos);
					bool				Modify(TextPos start, TextPos end, const wchar_t** inputs, int* inputCounts, int rows);
					bool				Modify(TextPos start, TextPos end, const wchar_t* input, int inputCount);
					bool				Modify(TextPos start, TextPos end, const wchar_t* input);
					bool				Modify(TextPos start, TextPos end, const WString& input);
					void				Clear();

					void				MeasureRow(int row);
					int					GetRowWidth(int row);
					int					GetRowHeight();
					int					GetMaxWidth();
					int					GetMaxHeight();
					TextPos				GetTextPosFromPoint(Point point);
					Point				GetPointFromTextPos(TextPos pos);
					Rect				GetRectFromTextPos(TextPos pos);
				};
			}

			class GuiColorizedTextElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiColorizedTextElement, L"ColorizedText");
			protected:

				GuiColorizedTextElement();
			public:
				~GuiColorizedTextElement();
			};
		}
	}
}

extern void GuiMain();

#endif
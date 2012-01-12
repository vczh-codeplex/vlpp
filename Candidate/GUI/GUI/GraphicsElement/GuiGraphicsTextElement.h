/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Element System

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
					unsigned __int32				rightOffset;
					unsigned __int32				colorIndex;
				};

				struct TextLine
				{
					static const int				BlockSize=32;
					static const int				MaxWidth=0xFFFF;

					wchar_t*						text;
					CharAtt*						att;
					int								availableOffsetCount;
					int								bufferLength;
					int								dataLength;

					TextLine();
					~TextLine();

					static int						CalculateBufferLength(int dataLength);
					bool							operator==(const TextLine& value)const{return false;}
					bool							operator!=(const TextLine& value)const{return true;}

					void							Initialize();
					void							Finalize();
					bool							IsReady();
					bool							Modify(int start, int count, const wchar_t* input, int inputCount);
					TextLine						Split(int index);
					void							AppendAndFinalize(TextLine& line);
				};

				class CharMeasurer : public virtual IDescriptable
				{
				protected:
					IGuiGraphicsRenderTarget*		oldRenderTarget;
					int								rowHeight;
					int								widths[65536];
					
					virtual int						MeasureWidthInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)=0;
					virtual int						GetRowHeightInternal(IGuiGraphicsRenderTarget* renderTarget)=0;
				public:
					CharMeasurer(int _rowHeight);
					~CharMeasurer();

					void							SetRenderTarget(IGuiGraphicsRenderTarget* value);
					int								MeasureWidth(wchar_t character);
					int								GetRowHeight();
				};

				struct TextLines
				{
					typedef collections::List<TextLine>		TextLineList;
				protected:
					TextLineList					lines;
					CharMeasurer*					charMeasurer;
					IGuiGraphicsRenderTarget*		renderTarget;
					int								tabWidth;
					int								tabSpaceCount;
				public:
					TextLines();
					~TextLines();

					int								GetCount();
					TextLine&						GetLine(int row);
					CharMeasurer*					GetCharMeasurer();
					void							SetCharMeasurer(CharMeasurer* value);
					IGuiGraphicsRenderTarget*		GetRenderTarget();
					void							SetRenderTarget(IGuiGraphicsRenderTarget* value);
					WString							GetText(TextPos start, TextPos end);
					WString							GetText();
					void							SetText(const WString& value);

					bool							RemoveLines(int start, int count);
					bool							IsAvailable(TextPos pos);
					TextPos							Normalize(TextPos pos);
					TextPos							Modify(TextPos start, TextPos end, const wchar_t** inputs, int* inputCounts, int rows);
					TextPos							Modify(TextPos start, TextPos end, const wchar_t* input, int inputCount);
					TextPos							Modify(TextPos start, TextPos end, const wchar_t* input);
					TextPos							Modify(TextPos start, TextPos end, const WString& input);
					void							Clear();

					void							ClearMeasurement();
					int								GetTabSpaceCount();
					void							SetTabSpaceCount(int value);
					void							MeasureRow(int row);
					int								GetRowWidth(int row);
					int								GetRowHeight();
					int								GetMaxWidth();
					int								GetMaxHeight();
					TextPos							GetTextPosFromPoint(Point point);
					Point							GetPointFromTextPos(TextPos pos);
					Rect							GetRectFromTextPos(TextPos pos);
				};

				struct ColorItem
				{
					Color							text;
					Color							background;
				};

				struct ColorEntry
				{
					ColorItem						normal;
					ColorItem						selectedFocused;
					ColorItem						selectedUnfocused;

					bool							operator==(const ColorEntry& value){return false;}
					bool							operator!=(const ColorEntry& value){return true;}
				};
			}

			class GuiColorizedTextElement : public Object, public IGuiGraphicsElement, public Description<GuiColorizedTextElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiColorizedTextElement, L"ColorizedText");

				typedef collections::Array<text::ColorEntry>			ColorArray;
				typedef collections::IReadonlyList<text::ColorEntry>	IColorArray;
			public:
				class ICallback : public virtual IDescriptable
				{
				public:
					virtual void					ColorChanged()=0;
					virtual void					FontChanged()=0;
				};
			protected:
				ICallback*							callback;
				ColorArray							colors;
				FontProperties						font;
				Point								viewPosition;
				bool								isVisuallyEnabled;
				bool								isFocused;

				TextPos								caretBegin;
				TextPos								caretEnd;
				bool								caretVisible;
				Color								caretColor;

				GuiColorizedTextElement();
			public:
				~GuiColorizedTextElement();

				text::TextLines						lines;

				ICallback*							GetCallback();
				void								SetCallback(ICallback* value);

				const IColorArray&					GetColors();
				void								SetColors(const ColorArray& value);
				const FontProperties&				GetFont();
				void								SetFont(const FontProperties& value);
				Point								GetViewPosition();
				void								SetViewPosition(Point value);
				bool								GetVisuallyEnabled();
				void								SetVisuallyEnabled(bool value);
				bool								GetFocused();
				void								SetFocused(bool value);

				TextPos								GetCaretBegin();
				void								SetCaretBegin(TextPos value);
				TextPos								GetCaretEnd();
				void								SetCaretEnd(TextPos value);
				bool								GetCaretVisible();
				void								SetCaretVisible(bool value);
				Color								GetCaretColor();
				void								SetCaretColor(Color value);
			};
		}
	}
}

#endif
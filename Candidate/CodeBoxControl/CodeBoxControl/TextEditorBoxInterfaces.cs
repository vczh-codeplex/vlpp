using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace CodeBoxControl
{
    public class TextEditorColorItem
    {
        public Color Text { get; set; }
        public Color HighlightText { get; set; }
        public Color Highlight { get; set; }
    }

    public interface ITextEditorColorizer
    {
        TextEditorColorItem[] ColorItems { get; }
        int ColorizeLine(char[] items, int length, int initialState, int[] colors);
    }

    public interface ITextEditorControlPanel
    {
        int Width { get; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Drawing;
using CodeBoxControl.Core;
using System.Windows.Forms;
using CodeBoxControl;

namespace CodeForm
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "TextEditorService" in both code and config file together.
    public class TextEditorService : ITextEditorService
    {
        public static TextEditorBox EditorControl { get; set; }

        private Point ToPoint(TextPosition p)
        {
            return new Point(p.col, p.row);
        }

        private TextPosition ToPosition(Point p)
        {
            return new TextPosition(p.Y, p.X);
        }

        public Point GetSelectionAnchor()
        {
            return ToPoint(EditorControl.SelectionAnchor);
        }

        public Point GetSelectionCaret()
        {
            return ToPoint(EditorControl.SelectionCaret);
        }

        public Point GetSelectionStart()
        {
            return ToPoint(EditorControl.SelectionStart);
        }

        public Point GetSelectionEnd()
        {
            return ToPoint(EditorControl.SelectionEnd);
        }

        public int GetLineCount()
        {
            return EditorControl.TextProvider.Count;
        }

        public int GetLineLength(int index)
        {
            return EditorControl.TextProvider[index].CharCount;
        }

        public string GetString(Point p1, Point p2)
        {
            return EditorControl.TextProvider.GetString(ToPosition(p1), ToPosition(p2));
        }

        public Point TextPositionToViewPoint(Point p)
        {
            return EditorControl.TextPositionToViewPoint(ToPosition(p));
        }

        public int[] GetColorsOfLine(int index)
        {
            return EditorControl.GetColorsOfLine(index);
        }
    }
}

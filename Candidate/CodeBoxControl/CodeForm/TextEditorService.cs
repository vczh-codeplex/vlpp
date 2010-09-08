using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Drawing;
using CodeBoxControl.Core;
using System.Windows.Forms;

namespace CodeForm
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "TextEditorService" in both code and config file together.
    public class TextEditorService : ITextEditorService
    {
        public static CodeForm CodeForm { get; set; }

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
            return ToPoint(CodeForm.textEditorBox.SelectionAnchor);
        }

        public Point GetSelectionCaret()
        {
            return ToPoint(CodeForm.textEditorBox.SelectionCaret);
        }

        public Point GetSelectionStart()
        {
            return ToPoint(CodeForm.textEditorBox.SelectionStart);
        }

        public Point GetSelectionEnd()
        {
            return ToPoint(CodeForm.textEditorBox.SelectionEnd);
        }

        public int GetLineCount()
        {
            return CodeForm.textEditorBox.TextProvider.Count;
        }

        public int GetLineLength(int index)
        {
            return CodeForm.textEditorBox.TextProvider[index].CharCount;
        }

        public string GetString(Point p1, Point p2)
        {
            return CodeForm.textEditorBox.TextProvider.GetString(ToPosition(p1), ToPosition(p2));
        }

        public Point TextPositionToViewPoint(Point p)
        {
            return CodeForm.textEditorBox.TextPositionToViewPoint(ToPosition(p));
        }
    }
}

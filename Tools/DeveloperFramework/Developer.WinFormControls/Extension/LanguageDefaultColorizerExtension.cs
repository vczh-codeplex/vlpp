using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Core;
using System.Drawing;
using Developer.LanguageProvider;

namespace Developer.WinFormControls.Extension
{
    public class LanguageDefaultColorizerExtension
        : ITextEditorBoxColorizerExtension
        , ITextEditorBoxLineExtension
    {
        public ITextEditorControlPanelCallBack Callback { get; private set; }
        public ILanguageDefaultColorizerProvider Provider { get; private set; }

        public LanguageDefaultColorizerExtension(ILanguageDefaultColorizerProvider provider)
        {
            this.Provider = provider;
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            TextEditorBox textEditorBox = this.Callback.TextEditorBox;
            if (textEditorBox.Controller.LimitedMode)
            {
                if (textEditorBox.Controller.LimitedStart.row == lineIndex)
                {
                    return true;
                }
            }
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            return line.BlockCount > 0;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            TextEditorBox textEditorBox = this.Callback.TextEditorBox;
            TextLine<TextEditorBox.LineInfo> line = textEditorBox.TextProvider[lineIndex];
            foreach (var block in line.Blocks)
            {
                for (int i = block.Item1; i < block.Item2; i++)
                {
                    colors[i] = this.Provider.BlockColorId;
                }
            }
            if (textEditorBox.Controller.LimitedMode && textEditorBox.Controller.LimitedStart.row == lineIndex)
            {
                int start = Math.Min(line.CharCount, textEditorBox.Controller.LimitedStart.col);
                int end = Math.Min(line.CharCount, textEditorBox.Controller.LimitedEnd.col);
                for (int i = start; i < end; i++)
                {
                    colors[i] = this.Provider.SnippetColorId;
                }
            }
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            TextEditorBox textEditorBox = this.Callback.TextEditorBox;
            if (textEditorBox.Controller.LimitedMode && textEditorBox.Controller.LimitedStart.row == lineIndex)
            {
                Point p1 = textEditorBox.TextPositionToViewPoint(textEditorBox.Controller.LimitedStart);
                Point p2 = textEditorBox.TextPositionToViewPoint(textEditorBox.Controller.LimitedEnd);
                Rectangle r = new Rectangle(p1.X, p1.Y - textEditorBox.TextTopOffset, p2.X - p1.X, textEditorBox.LineHeight);
                Color color = this.Provider.SnippetBackColor;
                using (SolidBrush brush = new SolidBrush(color))
                {
                    g.FillRectangle(brush, r);
                }
            }
        }

        public void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            TextEditorBox textEditorBox = this.Callback.TextEditorBox;
            TextLine<TextEditorBox.LineInfo> line = textEditorBox.TextProvider[lineIndex];
            foreach (var block in
                !textEditorBox.UIExtensions.EditingSnippet
                    ? line.Blocks
                    : textEditorBox.UIExtensions.CurrentAffectedSegments
                        .Select(s =>
                            {
                                TextPosition start = textEditorBox.UIExtensions.GetSegmentStart(s);
                                string text = textEditorBox.UIExtensions.GetSegmentText(s);
                                TextPosition end = new TextPosition(start.row, start.col + text.Length);
                                return Tuple.Create(start, end);
                            })
                        .Where(t => t.Item1.row == lineIndex)
                        .Select(t => Tuple.Create(t.Item1.col, t.Item2.col))
                        .Concat(line.Blocks)
                )
            {
                Point p1 = textEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, block.Item1));
                Point p2 = textEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, block.Item2));
                Rectangle r = new Rectangle(p1.X, p1.Y - textEditorBox.TextTopOffset, p2.X - p1.X, textEditorBox.LineHeight);
                Color color = textEditorBox.Colorizer.ColorItems[this.Provider.BlockColorId].Text;
                using (Pen pen = new Pen(color))
                {
                    g.DrawRectangle(pen, r);
                }
            }
        }

        public void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }

    public interface ILanguageDefaultColorizerProvider
    {
        int BlockColorId { get; }
        int SnippetColorId { get; }
        Color SnippetBackColor { get; }
    }
}

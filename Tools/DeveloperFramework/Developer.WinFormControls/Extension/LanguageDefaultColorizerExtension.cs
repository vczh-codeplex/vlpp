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
        public int BlockColorId { get; private set; }

        public LanguageDefaultColorizerExtension(int blockColorId)
        {
            this.BlockColorId = blockColorId;
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            return line.BlockCount > 0;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            foreach (var block in line.Blocks)
            {
                for (int i = block.Item1; i < block.Item2; i++)
                {
                    colors[i] = this.BlockColorId;
                }
            }
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            TextEditorBox textEditorBox = this.Callback.TextEditorBox;
            TextLine<TextEditorBox.LineInfo> line = textEditorBox.TextProvider[lineIndex];
            foreach (var block in line.Blocks)
            {
                Point p1 = textEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, block.Item1));
                Point p2 = textEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, block.Item2));
                Rectangle r = new Rectangle(p1.X, p1.Y - 1, p2.X - p1.X, textEditorBox.LineHeight);
                Color color = textEditorBox.Colorizer.ColorItems[this.BlockColorId].Text;
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
}

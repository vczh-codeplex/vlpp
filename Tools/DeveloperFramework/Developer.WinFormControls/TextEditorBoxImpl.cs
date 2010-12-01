using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Windows.Forms;
using System.Xml.Linq;
using Developer.WinFormControls.Extension;

namespace Developer.WinFormControls
{
    public class TextEditorPlanTextColorizer : ITextEditorColorizer
    {
        public const int BlockColorId = 0;
        public const int NormalColorId = 1;

        private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        private readonly Color BlockColor = Color.Gray;
        private readonly Color NormalColor = Color.FromArgb(0, 0, 0);

        private TextEditorBox textEditorBox = null;
        private TextEditorColorItem[] colorItems = null;

        public TextEditorPlanTextColorizer(TextEditorBox textEditorBox)
        {
            this.textEditorBox = textEditorBox;
            this.colorItems = new TextEditorColorItem[NormalColorId + 1];
            this.colorItems[BlockColorId] = new TextEditorColorItem()
            {
                Text = BlockColor,
                HighlightText = BlockColor,
                Highlight = HighlightColor
            };
            this.colorItems[NormalColorId] = new TextEditorColorItem()
            {
                Text = NormalColor,
                HighlightText = NormalColor,
                Highlight = HighlightColor
            };
        }

        public TextEditorColorItem[] ColorItems
        {
            get
            {
                return this.colorItems;
            }
        }

        public int ColorizeLine(char[] items, int length, int initialState, int[] colors)
        {
            int count = items.Length;
            for (int i = 0; i < length; i++)
            {
                colors[i] = NormalColorId;
            }
            return initialState;
        }
    }

    public class TextEditorPlanTextControlPanel : TextEditorBoxExtensibleControlPanel
    {
        public TextEditorPlanTextControlPanel(int blockColorId)
        {
            ExtendBeforeInstall(new LanguageDefaultColorizerExtension(blockColorId));
        }
    }
}

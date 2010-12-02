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
        public const int SnippetColorId = 0;
        public const int BlockColorId = 1;
        public const int NormalColorId = 2;

        private static readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        private static readonly Color NormalColor = Color.FromArgb(0, 0, 0);

        private static readonly Color SnippetColor = Color.FromArgb(0, 0, 0);
        private static readonly Color SnippetHighlightColor = Color.FromArgb(173, 200, 198);
        private static readonly Color BlockColor = Color.Gray;

        public static readonly Color SnippetBackgroundColor = Color.FromArgb(255, 231, 160);

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
            this.colorItems[SnippetColorId] = new TextEditorColorItem()
            {
                Text = SnippetColor,
                HighlightText = SnippetColor,
                Highlight = SnippetHighlightColor
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
    }

    public class TextEditorPlanTextColorizedControlPanel : TextEditorBoxExtensibleControlPanel
    {
        public TextEditorPlanTextColorizedControlPanel(ILanguageDefaultColorizerProvider provider)
        {
            ExtendBeforeInstall(new LanguageDefaultColorizerExtension(provider));
        }
    }
}

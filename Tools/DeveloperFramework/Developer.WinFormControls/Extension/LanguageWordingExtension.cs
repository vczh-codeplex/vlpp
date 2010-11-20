using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;
using Developer.WinFormControls.Core;

namespace Developer.WinFormControls.Extension
{
    public class LanguageWordingExtension : ITextEditorBoxWordingExtension
    {
        protected ILanguageWordingProvider Provider { get; private set; }

        public ITextEditorControlPanelCallBack Callback { get; private set; }

        public LanguageWordingExtension(ILanguageWordingProvider provider)
        {
            this.Provider = provider;
        }

        public TextPosition GetLeftWord(TextPosition caret)
        {
            return GetWord(Backward(caret), Backward);
        }

        public TextPosition GetRightWord(TextPosition caret)
        {
            return Forward(GetWord(caret, Forward));
        }

        public void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }

        private TextPosition Backward(TextPosition caret)
        {
            if (caret.col == 0)
            {
                if (caret.row == 0)
                {
                    return caret;
                }
                else
                {
                    return new TextPosition(caret.row - 1, this.Callback.TextEditorBox.TextProvider[caret.row - 1].CharCount);
                }
            }
            else
            {
                return new TextPosition(caret.row, caret.col - 1);
            }
        }

        private TextPosition Forward(TextPosition caret)
        {
            if (caret.col == this.Callback.TextEditorBox.TextProvider[caret.row].CharCount)
            {
                if (caret.row == this.Callback.TextEditorBox.TextProvider.Count - 1)
                {
                    return caret;
                }
                else
                {
                    return new TextPosition(caret.row + 1, 0);
                }
            }
            else
            {
                return new TextPosition(caret.row, caret.col + 1);
            }
        }

        private TextPosition GetWord(TextPosition caret, Func<TextPosition, TextPosition> step)
        {
            TextPosition end = caret;
            int charClass = -1;
            while (true)
            {
                int currentCharClass = -1;
                TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[caret.row];
                if (caret.col < line.CharCount)
                {
                    currentCharClass = this.Provider.GetNonNegativeCharClass(line.CharArray[caret.col]);
                }
                else
                {
                    currentCharClass = this.Provider.GetNonNegativeCharClass('\n');
                }
                if (charClass == -1 || charClass == currentCharClass)
                {
                    charClass = currentCharClass;
                    end = caret;
                    caret = step(caret);
                    if (caret == end)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            return end;
        }
    }

    public interface ILanguageWordingProvider
    {
        int GetNonNegativeCharClass(char c);
    }
}

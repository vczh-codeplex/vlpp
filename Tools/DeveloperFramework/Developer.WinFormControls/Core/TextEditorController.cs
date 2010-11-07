using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.WinFormControls.Core
{
    public interface ITextContentProvider
    {
        int GetLineCount();
        int GetPageLineCount();
        int GetLineLength(int index);
        void OnSelectionAreaChanged();
        bool OnEdit(TextPosition start, TextPosition end, string[] lines);
        void OnFinishEdit();
        void OnRefreshSuggestion();

        TextPosition GetLeftWord(TextPosition caret);
        TextPosition GetRightWord(TextPosition caret);
        int GetLeftBlock(TextPosition caret);
        int GetRightBlock(TextPosition caret);
        Tuple<int, int> GetBlock(TextPosition caret);
    }

    public class TextEditorController
    {
        private TextPosition selectionAnchor = new TextPosition(0, 0);
        private TextPosition selectionCaret = new TextPosition(0, 0);
        private ITextContentProvider provider = null;
        private int refreshDisableCounter = 0;

        public TextEditorController(ITextContentProvider provider)
        {
            this.provider = provider;
        }

        public TextPosition SelectionAnchor
        {
            get
            {
                return this.selectionAnchor;
            }
        }

        public TextPosition SelectionCaret
        {
            get
            {
                return this.selectionCaret;
            }
        }

        // SHIFT
        public void PressUp(bool control, bool shift)
        {
            int newRow = Math.Max(0, this.selectionCaret.row - 1);
            Move(new TextPosition(newRow, this.selectionCaret.col), false, shift);
        }

        // SHIFT
        public void PressDown(bool control, bool shift)
        {
            int newRow = Math.Min(this.provider.GetLineCount() - 1, this.selectionCaret.row + 1);
            Move(new TextPosition(newRow, this.selectionCaret.col), false, shift);
        }

        // CONTROL + SHIFT
        public void PressLeft(bool control, bool shift)
        {
            if (this.selectionAnchor != this.selectionCaret && !control && !shift)
            {
                Move(this.selectionAnchor < this.selectionCaret ? this.selectionAnchor : this.selectionCaret, false, false);
            }
            else
            {
                int block = this.provider.GetLeftBlock(this.selectionCaret);
                if (block != this.selectionCaret.col)
                {
                    Move(new TextPosition(this.selectionCaret.row, block), false, shift);
                }
                else
                {
                    if (control)
                    {
                        TextPosition newCaret = this.provider.GetLeftWord(this.selectionCaret);
                        if (newCaret == this.selectionCaret)
                        {
                            control = false;
                        }
                        else
                        {
                            Move(newCaret, false, shift);
                        }
                    }
                    if (!control)
                    {
                        if (this.selectionCaret.col > 0)
                        {
                            Move(new TextPosition(this.selectionCaret.row, this.selectionCaret.col - 1), false, shift);
                        }
                        else if (this.selectionCaret.row > 0)
                        {
                            Move(new TextPosition(this.selectionCaret.row - 1, this.provider.GetLineLength(this.selectionCaret.row - 1)), false, shift);
                        }
                    }
                }
            }
        }

        // CONTROL + SHIFT
        public void PressRight(bool control, bool shift)
        {
            if (this.selectionAnchor != this.selectionCaret && !control && !shift)
            {
                Move(this.selectionAnchor > this.selectionCaret ? this.selectionAnchor : this.selectionCaret, false, false);
            }
            else
            {
                int block = this.provider.GetRightBlock(this.selectionCaret);
                if (block != this.selectionCaret.col)
                {
                    Move(new TextPosition(this.selectionCaret.row, block), false, shift);
                }
                else
                {
                    if (control)
                    {
                        TextPosition newCaret = this.provider.GetRightWord(this.selectionCaret);
                        if (newCaret == this.selectionCaret)
                        {
                            control = false;
                        }
                        else
                        {
                            Move(newCaret, false, shift);
                        }
                    }
                    if (!control)
                    {
                        if (this.selectionCaret.col < this.provider.GetLineLength(this.selectionCaret.row))
                        {
                            Move(new TextPosition(this.selectionCaret.row, this.selectionCaret.col + 1), false, shift);
                        }
                        else if (this.selectionCaret.row < this.provider.GetLineCount() - 1)
                        {
                            Move(new TextPosition(this.selectionCaret.row + 1, 0), false, shift);
                        }
                    }
                }
            }
        }

        // CONTROL + SHIFT
        public void PressHome(bool control, bool shift)
        {
            if (control)
            {
                Move(new TextPosition(0, 0), false, shift);
            }
            else
            {
                Move(new TextPosition(this.selectionCaret.row, 0), false, shift);
            }
        }

        // CONTROL + SHIFT
        public void PressEnd(bool control, bool shift)
        {
            if (control)
            {
                Move(new TextPosition(this.provider.GetLineCount() - 1, this.provider.GetLineLength(this.provider.GetLineCount() - 1)), false, shift);
            }
            else
            {
                Move(new TextPosition(this.selectionCaret.row, this.provider.GetLineLength(this.selectionCaret.row)), false, shift);
            }
        }

        // SHIFT
        public void PressPageUp(bool control, bool shift)
        {
            int newRow = Math.Max(0, this.selectionCaret.row - this.provider.GetPageLineCount());
            Move(new TextPosition(newRow, this.selectionCaret.col), false, shift);
        }

        // SHIFT
        public void PressPageDown(bool control, bool shift)
        {
            int newRow = Math.Min(this.provider.GetLineCount() - 1, this.selectionCaret.row + this.provider.GetPageLineCount());
            Move(new TextPosition(newRow, this.selectionCaret.col), false, shift);
        }

        // CONTROL + SHIFT
        public void PressDelete(bool control, bool shift)
        {
            StartTask();
            if (shift)
            {
                Input("", true);
            }
            else
            {
                if (this.selectionAnchor == this.selectionCaret)
                {
                    PressRight(control, true);
                }
                Input("", false);
            }
            FinishTask();
        }

        // CONTROL + SHIFT
        public void PressBackspace(bool control, bool shift)
        {
            StartTask();
            if (shift)
            {
                Input("", true);
            }
            else
            {
                if (this.selectionAnchor == this.selectionCaret)
                {
                    PressLeft(control, true);
                }
                Input("", false);
            }
            FinishTask();
        }

        // CONTROL + SHIFT
        public void PressEnter(bool control, bool shift)
        {
            StartTask();
            if (control)
            {
                if (shift)
                {
                    PressEnd(false, false);
                    Input("\r\n", false);
                }
                else
                {
                    PressHome(false, false);
                    Input("\r\n", false);
                    PressUp(false, false);
                }
            }
            else
            {
                Input("\r\n", false);
            }
            FinishTask();
        }

        public bool Input(string text, bool extendToWholeRow)
        {
            TextPosition start = this.selectionAnchor;
            TextPosition end = this.selectionCaret;
            if (start > end)
            {
                start = this.selectionCaret;
                end = this.selectionAnchor;
            }

            if (extendToWholeRow)
            {
                start.col = 0;
                if (end.row == this.provider.GetLineCount() - 1)
                {
                    end.col = this.provider.GetLineLength(end.row);
                }
                else
                {
                    end.row++;
                    end.col = 0;
                }
            }

            string[] lines = text.Split(new string[] { "\r\n" }, StringSplitOptions.None);
            if (this.provider.OnEdit(start, end, lines))
            {
                if (lines.Length == 1)
                {
                    Move(new TextPosition(start.row, start.col + lines[0].Length), false, false);
                }
                else
                {
                    Move(new TextPosition(start.row + lines.Length - 1, lines[lines.Length - 1].Length), false, false);
                }
                this.provider.OnFinishEdit();
                return true;
            }
            else
            {
                return false;
            }
        }

        public TextPosition Normalize(TextPosition position)
        {
            if (position.row < 0)
            {
                position = new TextPosition(0, 0);
            }
            else if (position.row >= this.provider.GetLineCount())
            {
                int row = this.provider.GetLineCount() - 1;
                position = new TextPosition(row, this.provider.GetLineLength(row));
            }
            else if (position.col < 0)
            {
                position = new TextPosition(position.row, 0);
            }
            else if (position.col > this.provider.GetLineLength(position.row))
            {
                position = new TextPosition(position.row, this.provider.GetLineLength(position.row));
            }
            Tuple<int, int> block = this.provider.GetBlock(position);
            position.col = Math.Abs(block.Item1 - position.col) < Math.Abs(block.Item2 - position.col) ? block.Item1 : block.Item2;
            return position;
        }

        // SHIFT
        public void Move(TextPosition position, bool control, bool shift)
        {
            StartTask();
            position = Normalize(position);
            this.selectionCaret = position;
            if (!shift)
            {
                this.selectionAnchor = position;
            }
            this.provider.OnSelectionAreaChanged();
            FinishTask();
        }

        public void Select(TextPosition anchor, TextPosition caret)
        {
            StartTask();
            this.selectionAnchor = Normalize(anchor);
            this.selectionCaret = Normalize(caret);
            this.provider.OnSelectionAreaChanged();
            FinishTask();
        }

        public void StartTask()
        {
            this.refreshDisableCounter++;
        }

        public void FinishTask()
        {
            this.refreshDisableCounter--;
            if (this.refreshDisableCounter == 0)
            {
                this.provider.OnRefreshSuggestion();
            }
        }
    }
}
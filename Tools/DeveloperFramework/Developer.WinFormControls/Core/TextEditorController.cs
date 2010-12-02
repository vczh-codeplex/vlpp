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
        private ITextContentProvider provider = null;
        private int refreshDisableCounter = 0;

        public TextPosition SelectionAnchor { get; private set; }
        public TextPosition SelectionCaret { get; private set; }
        public TextPosition LimitedStart { get; private set; }
        public TextPosition LimitedEnd { get; private set; }
        public bool LimitedMode { get; private set; }

        public bool InTask
        {
            get
            {
                return refreshDisableCounter > 0;
            }
        }

        public TextEditorController(ITextContentProvider provider)
        {
            this.provider = provider;
        }

        // SHIFT
        public void PressUp(bool control, bool shift)
        {
            int newRow = Math.Max(0, this.SelectionCaret.row - 1);
            Move(new TextPosition(newRow, this.SelectionCaret.col), false, shift);
        }

        // SHIFT
        public void PressDown(bool control, bool shift)
        {
            int newRow = Math.Min(this.provider.GetLineCount() - 1, this.SelectionCaret.row + 1);
            Move(new TextPosition(newRow, this.SelectionCaret.col), false, shift);
        }

        // CONTROL + SHIFT
        public void PressLeft(bool control, bool shift)
        {
            if (this.SelectionAnchor != this.SelectionCaret && !control && !shift)
            {
                Move(this.SelectionAnchor < this.SelectionCaret ? this.SelectionAnchor : this.SelectionCaret, false, false);
            }
            else
            {
                int block = this.provider.GetLeftBlock(this.SelectionCaret);
                if (block != this.SelectionCaret.col)
                {
                    Move(new TextPosition(this.SelectionCaret.row, block), false, shift);
                }
                else
                {
                    if (control)
                    {
                        TextPosition newCaret = this.provider.GetLeftWord(this.SelectionCaret);
                        if (newCaret == this.SelectionCaret)
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
                        if (this.SelectionCaret.col > 0)
                        {
                            Move(new TextPosition(this.SelectionCaret.row, this.SelectionCaret.col - 1), false, shift);
                        }
                        else if (this.SelectionCaret.row > 0)
                        {
                            Move(new TextPosition(this.SelectionCaret.row - 1, this.provider.GetLineLength(this.SelectionCaret.row - 1)), false, shift);
                        }
                    }
                }
            }
        }

        // CONTROL + SHIFT
        public void PressRight(bool control, bool shift)
        {
            if (this.SelectionAnchor != this.SelectionCaret && !control && !shift)
            {
                Move(this.SelectionAnchor > this.SelectionCaret ? this.SelectionAnchor : this.SelectionCaret, false, false);
            }
            else
            {
                int block = this.provider.GetRightBlock(this.SelectionCaret);
                if (block != this.SelectionCaret.col)
                {
                    Move(new TextPosition(this.SelectionCaret.row, block), false, shift);
                }
                else
                {
                    if (control)
                    {
                        TextPosition newCaret = this.provider.GetRightWord(this.SelectionCaret);
                        if (newCaret == this.SelectionCaret)
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
                        if (this.SelectionCaret.col < this.provider.GetLineLength(this.SelectionCaret.row))
                        {
                            Move(new TextPosition(this.SelectionCaret.row, this.SelectionCaret.col + 1), false, shift);
                        }
                        else if (this.SelectionCaret.row < this.provider.GetLineCount() - 1)
                        {
                            Move(new TextPosition(this.SelectionCaret.row + 1, 0), false, shift);
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
                Move(new TextPosition(this.SelectionCaret.row, 0), false, shift);
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
                Move(new TextPosition(this.SelectionCaret.row, this.provider.GetLineLength(this.SelectionCaret.row)), false, shift);
            }
        }

        // SHIFT
        public void PressPageUp(bool control, bool shift)
        {
            int newRow = Math.Max(0, this.SelectionCaret.row - this.provider.GetPageLineCount());
            Move(new TextPosition(newRow, this.SelectionCaret.col), false, shift);
        }

        // SHIFT
        public void PressPageDown(bool control, bool shift)
        {
            int newRow = Math.Min(this.provider.GetLineCount() - 1, this.SelectionCaret.row + this.provider.GetPageLineCount());
            Move(new TextPosition(newRow, this.SelectionCaret.col), false, shift);
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
                if (this.SelectionAnchor == this.SelectionCaret)
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
                if (this.SelectionAnchor == this.SelectionCaret)
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
            TextPosition start = this.SelectionAnchor;
            TextPosition end = this.SelectionCaret;
            if (start > end)
            {
                start = this.SelectionCaret;
                end = this.SelectionAnchor;
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

            string[] lines = text
                .Split(new string[] { "\r\n" }, StringSplitOptions.None)
                .SelectMany(s => s.Split(new string[] { "\n", "\r" }, StringSplitOptions.None))
                .ToArray();
            if (this.LimitedMode && lines.Length > 1)
            {
                lines = new string[] { lines[0] };
            }
            if (this.provider.OnEdit(start, end, lines))
            {
                if (lines.Length == 1)
                {
                    this.LimitedEnd = new TextPosition(this.LimitedEnd.row, this.LimitedEnd.col + lines[0].Length - (end.col - start.col));
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

        public TextPosition Normalize(TextPosition position, bool considerLimitedMode = false)
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

            if (considerLimitedMode && this.LimitedMode)
            {
                if (position < this.LimitedStart)
                {
                    position = this.LimitedStart;
                }
                else if (position > this.LimitedEnd)
                {
                    position = this.LimitedEnd;
                }
            }

            Tuple<int, int> block = this.provider.GetBlock(position);
            position.col = Math.Abs(block.Item1 - position.col) < Math.Abs(block.Item2 - position.col) ? block.Item1 : block.Item2;
            return position;
        }

        // SHIFT
        public void Move(TextPosition position, bool control, bool shift)
        {
            StartTask();
            position = Normalize(position, true);
            this.SelectionCaret = position;
            if (!shift)
            {
                this.SelectionAnchor = position;
            }
            this.provider.OnSelectionAreaChanged();
            FinishTask();
        }

        public void Select(TextPosition anchor, TextPosition caret)
        {
            StartTask();
            this.SelectionAnchor = Normalize(anchor, true);
            this.SelectionCaret = Normalize(caret, true);
            this.provider.OnSelectionAreaChanged();
            FinishTask();
        }

        public bool EnterLimitedMode(int row, int start, int end)
        {
            if (!this.LimitedMode)
            {
                if (row >= 0 && row < this.provider.GetLineCount())
                {
                    int count = this.provider.GetLineLength(row);
                    if (start <= end && 0 <= start && start <= end)
                    {
                        this.LimitedStart = new TextPosition(row, start);
                        this.LimitedEnd = new TextPosition(row, end);
                        this.LimitedMode = true;
                        return true;
                    }
                }
            }
            return false;
        }

        public bool LeaveLimitedMode()
        {
            if (this.LimitedMode)
            {
                this.LimitedMode = false;
                return true;
            }
            else
            {
                return false;
            }
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
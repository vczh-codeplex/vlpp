using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CodeBoxControl.Core;

namespace CodeBoxControl
{
    public partial class TextEditorBox : CodeBoxControl.ScrollableContentControl, ITextContentProvider
    {
        private const int EditorMargin = 4;

        class LineInfo
        {
            public int lineWidth = 0;
            public bool lineWidthAvailable = false;
            public int colorizerFinalState = 0;
        }

        #region Editing Processor Fields

        private TextProvider<LineInfo> textProvider = null;
        private TextEditorController controller = null;
        private TextEditorBoxKeyCommands keyCommands = new TextEditorBoxKeyCommands();

        #endregion

        #region View Calculation Fields

        private int lineHeight = 0;
        private int textHeight = 0;
        private int textTopOffset = 0;
        private int cachedWholeWidth = 0;
        private bool cachedWholeWidthAvailable = false;
        private bool needToForceUpdateViewSize = false;

        #endregion

        #region Colorization Fields

        private ITextEditorColorizer colorizer = null;
        private int colorizedLines = 0;

        #endregion

        #region Rendering Fields

        private bool caretVisible = true;
        private Control host = null;
        private Graphics temporaryGraphics = null;

        #endregion

        public TextEditorBox()
            : base(new TextContent())
        {
            this.textProvider = new TextProvider<LineInfo>();
            this.controller = new TextEditorController(this);
            this.ImeEnabled = true;
            this.EnableDefaultCommands = true;
            this.colorizer = new TextEditorPlanTextColorizer(this);

            InitializeComponent();
            UpdateLineHeight();

            this.keyCommands.RegisterCommand(Keys.C, true, false, DoCopy);
            this.keyCommands.RegisterCommand(Keys.X, true, false, DoCut);
            this.keyCommands.RegisterCommand(Keys.V, true, false, DoPaste);
            this.keyCommands.RegisterCommand(Keys.A, true, false, DoSelectAll);
        }

        public TextEditorBoxKeyCommands KeyCommands
        {
            get
            {
                return this.keyCommands;
            }
        }

        public bool EnableDefaultCommands { get; set; }

        #region Selection API

        public TextPosition SelectionAnchor
        {
            get
            {
                return this.controller.SelectionAnchor;
            }
        }

        public TextPosition SelectionCaret
        {
            get
            {
                return this.controller.SelectionCaret;
            }
        }

        public TextPosition SelectionStart
        {
            get
            {
                return this.SelectionAnchor < this.SelectionCaret ? this.SelectionAnchor : this.SelectionCaret;
            }
        }

        public TextPosition SelectionEnd
        {
            get
            {
                return this.SelectionAnchor > this.SelectionCaret ? this.SelectionAnchor : this.SelectionCaret;
            }
        }

        public void SelectText(TextPosition anchor, TextPosition caret)
        {
            this.controller.Select(anchor, caret);
        }

        public void SetCaret(TextPosition caret)
        {
            SelectText(caret, caret);
        }

        public void SelectAllText()
        {
            SelectText(new TextPosition(0, 0), new TextPosition(this.textProvider.Count - 1, this.textProvider[this.textProvider.Count - 1].CharCount));
        }

        public override string Text
        {
            get
            {
                return this.textProvider.Text;
            }
            set
            {
                SelectAllText();
                this.SelectedText = value;
                SetCaret(new TextPosition(0, 0));
            }
        }

        public string SelectedText
        {
            get
            {
                return this.textProvider.GetString(this.SelectionStart, this.SelectionEnd);
            }
            set
            {
                this.controller.Input(value, false);
            }
        }

        public void ForceUpdateViewSize()
        {
            this.cachedWholeWidth = 0;
            for (int i = 0; i < this.textProvider.Count; i++)
            {
                this.cachedWholeWidth = Math.Max(this.cachedWholeWidth, GetLineWidth(i));
            }
            this.cachedWholeWidthAvailable = true;
            UpdateViewSize();
        }

        #endregion

        #region Language Service API

        public ITextEditorColorizer Colorizer
        {
            get
            {
                return this.colorizer;
            }
            set
            {
                this.colorizer = value;
                this.colorizedLines = 0;
                this.host.Refresh();
            }
        }

        #endregion

        #region View API

        public Point ViewPointToDocumentPoint(Point point)
        {
            return Point.Add(point, new Size(this.ViewPosition));
        }

        public Point DocumentPointToViewPoint(Point point)
        {
            return Point.Subtract(point, new Size(this.ViewPosition));
        }

        public Point TextPositionToDocumentPoint(TextPosition position)
        {
            return Point.Add(CalculateOffset(position), new Size(EditorMargin, EditorMargin));
        }

        public TextPosition DocumentPointToTextPosition(Point point)
        {
            return CalculatePosition(Point.Subtract(point, new Size(EditorMargin, EditorMargin)));
        }

        public Point TextPositionToViewPoint(TextPosition position)
        {
            return DocumentPointToViewPoint(TextPositionToDocumentPoint(position));
        }

        public TextPosition ViewPointToTextPosition(Point point)
        {
            return DocumentPointToTextPosition(ViewPointToDocumentPoint(point));
        }

        #endregion

        #region Clipboard API

        public bool CanCopy
        {
            get
            {
                return this.SelectionAnchor != this.SelectionCaret;
            }
        }

        public bool CanCut
        {
            get
            {
                return this.SelectionAnchor != this.SelectionCaret;
            }
        }

        public bool CanPaste
        {
            get
            {
                return Clipboard.ContainsText();
            }
        }

        public bool Copy()
        {
            if (this.CanCopy)
            {
                Clipboard.SetText(this.SelectedText);
                return true;
            }
            return this.CanCopy;
        }

        public bool Cut()
        {
            if (this.CanCut)
            {
                Clipboard.SetText(this.SelectedText);
                this.SelectedText = "";
                return true;
            }
            return this.CanCut;
        }

        public bool Paste()
        {
            if (this.CanPaste)
            {
                this.SelectedText = Clipboard.GetText();
                return true;
            }
            return this.CanPaste;
        }

        #endregion

        #region Event Handlers

        protected override void OnFontChanged(EventArgs e)
        {
            UpdateLineHeight();
            base.OnFontChanged(e);
        }

        protected override void OnBackColorChanged(EventArgs e)
        {
            base.OnBackColorChanged(e);
            this.host.Refresh();
        }

        protected override void OnForeColorChanged(EventArgs e)
        {
            base.OnForeColorChanged(e);
            this.host.Refresh();
        }

        private void timerCaret_Tick(object sender, EventArgs e)
        {
            this.caretVisible = !this.caretVisible;
            this.host.Refresh();
        }

        #endregion

        #region Implementation

        private void UpdateViewSize()
        {
            this.ViewSize = new Size(this.cachedWholeWidth + EditorMargin * 2, this.textProvider.Count * this.lineHeight + EditorMargin * 2);
        }

        private void UpdateLineHeight()
        {
            using (Bitmap bitmap = new Bitmap(1, 1))
            using (Graphics g = Graphics.FromImage(bitmap))
            {
                this.textHeight = (int)(this.Font.SizeInPoints * g.DpiY / 75);
                this.lineHeight = (int)(1.5 * this.textHeight);
                this.textTopOffset = (this.lineHeight - this.textHeight) / 2;
            }
            UpdateViewSize();
            this.VerticalSmallChange = this.lineHeight;
            this.HorizontalSmallChange = CalculateOffset(" ");
        }

        private int CalculateOffset(string text)
        {
            if (this.temporaryGraphics == null)
            {
                this.temporaryGraphics = Graphics.FromHwnd(this.host.Handle);
            }
            Size size = TextRenderer.MeasureText(this.temporaryGraphics, text, this.Font, new Size(0, 0), TextFormatFlags.NoPadding);
            return size.Width;
        }

        private Point CalculateOffset(TextPosition position)
        {
            string text = this.textProvider[position.row].GetString(0, position.col);
            return new Point(CalculateOffset(text), position.row * this.lineHeight + this.textTopOffset);
        }

        private TextPosition CalculatePosition(Point point)
        {
            int row = point.Y / this.lineHeight;
            if (row < 0)
            {
                row = 0;
            }
            else if (row >= this.textProvider.Count)
            {
                row = this.textProvider.Count - 1;
            }

            int col1 = 0;
            int col2 = this.textProvider[row].CharCount;
            int x1 = 0;
            int x2 = CalculateOffset(new TextPosition(row, col2)).X;

            while (col2 - col1 > 1)
            {
                int col = (col2 + col1) / 2;
                int x = CalculateOffset(new TextPosition(row, col)).X;
                if (x < point.X)
                {
                    col1 = col;
                    x1 = x;
                }
                else if (x > point.X)
                {
                    col2 = col;
                    x2 = x;
                }
                else
                {
                    return new TextPosition(row, col);
                }
            }
            return new TextPosition(row, Math.Abs(point.X - x1) < Math.Abs(point.X - x2) ? col1 : col2);
        }

        private int GetCachedLineWidth(int index)
        {
            TextLine<LineInfo> line = this.textProvider[index];
            if (!line.Tag.lineWidthAvailable)
            {
                return -1;
            }
            else
            {
                return line.Tag.lineWidth;
            }
        }

        private int GetLineWidth(int index)
        {
            TextLine<LineInfo> line = this.textProvider[index];
            if (!line.Tag.lineWidthAvailable)
            {
                line.Tag.lineWidth = CalculateOffset(new TextPosition(index, line.CharCount)).X;
                line.Tag.lineWidthAvailable = true;
            }
            return line.Tag.lineWidth;
        }

        private void EnsureLineColorized(int index)
        {
            if (index >= this.colorizedLines)
            {
                for (int i = this.colorizedLines; i <= index; i++)
                {
                    int initialState = i == 0 ? 0 : this.textProvider[i - 1].Tag.colorizerFinalState;
                    TextLine<LineInfo> line = this.textProvider[i];
                    int finalState = this.colorizer.ColorizeLine(line.CharArray, line.CharCount, initialState, line.ColorArray);
                    line.Tag.colorizerFinalState = finalState;
                }
                this.colorizedLines = index + 1;
            }
        }

        #endregion

        #region Commands

        private bool DoCopy(TextEditorBox editor, KeyEventArgs e)
        {
            if (this.EnableDefaultCommands)
            {
                return Copy();
            }
            return this.EnableDefaultCommands;
        }

        private bool DoCut(TextEditorBox editor, KeyEventArgs e)
        {
            if (this.EnableDefaultCommands)
            {
                return Cut();
            }
            return this.EnableDefaultCommands;
        }

        private bool DoPaste(TextEditorBox editor, KeyEventArgs e)
        {
            if (this.EnableDefaultCommands)
            {
                return Paste();
            }
            return this.EnableDefaultCommands;
        }

        private bool DoSelectAll(TextEditorBox editor, KeyEventArgs e)
        {
            if (this.EnableDefaultCommands)
            {
                SelectAllText();
            }
            return this.EnableDefaultCommands;
        }

        #endregion

        #region ITextContentProvider Members

        int ITextContentProvider.GetLineCount()
        {
            return this.textProvider.Count;
        }

        int ITextContentProvider.GetPageLineCount()
        {
            return this.ViewVisibleSize.Height / this.lineHeight;
        }

        int ITextContentProvider.GetLineLength(int index)
        {
            return this.textProvider[index].CharCount;
        }

        void ITextContentProvider.OnSelectionAreaChanged()
        {
            TextPosition caret = this.controller.SelectionCaret;
            Point caretPosition = Point.Add(CalculateOffset(caret), new Size(EditorMargin, EditorMargin));
            if (!new Rectangle(this.ViewPosition, this.ViewVisibleSize).Contains(caretPosition))
            {
                Point oldViewPosition = this.ViewPosition;
                Point newViewPosition = new Point(caretPosition.X - this.ViewVisibleSize.Width / 2, caretPosition.Y - this.ViewVisibleSize.Height / 2);
                if (this.needToForceUpdateViewSize || newViewPosition.X < oldViewPosition.X && !this.cachedWholeWidthAvailable)
                {
                    ForceUpdateViewSize();
                    this.needToForceUpdateViewSize = false;
                }
                this.ViewPosition = newViewPosition;
            }
            if (this.needToForceUpdateViewSize)
            {
                this.needToForceUpdateViewSize = false;
                ForceUpdateViewSize();
            }
            this.caretVisible = true;
        }

        bool ITextContentProvider.OnEdit(TextPosition start, TextPosition end, string[] lines)
        {
            bool optimizable = this.colorizedLines + 1 >= end.row;
            int lastFinalState = this.textProvider[end.row].Tag.colorizerFinalState;
            int lastColorizedLines = this.colorizedLines;

            for (int i = start.row; i <= end.row; i++)
            {
                if (GetCachedLineWidth(i) >= this.cachedWholeWidth)
                {
                    this.cachedWholeWidthAvailable = false;
                }
                this.textProvider[i].Tag.lineWidthAvailable = false;
            }
            this.textProvider.Edit(start, end, lines);

            if (optimizable)
            {
                int newEndRow = start.row + lines.Length - 1;
                this.colorizedLines = start.row;
                EnsureLineColorized(newEndRow);
                if (this.textProvider[newEndRow].Tag.colorizerFinalState == lastFinalState)
                {
                    this.colorizedLines = lastColorizedLines + newEndRow - end.row;
                    return true;
                }
            }
            this.colorizedLines = Math.Min(this.colorizedLines, start.row);
            return true;
        }

        void ITextContentProvider.OnFinishEdit()
        {
            this.caretVisible = true;
            if (this.ViewSize.Height != this.textProvider.Count * this.lineHeight)
            {
                UpdateLineHeight();
            }
        }

        void ITextContentProvider.OnRefreshSuggestion()
        {
            this.host.Refresh();
        }

        TextPosition ITextContentProvider.GetLeftWord(TextPosition caret)
        {
            if (caret.col > 0)
            {
                return new TextPosition(caret.row, caret.col - 1);
            }
            else if (caret.row > 0)
            {
                return new TextPosition(caret.row - 1, this.textProvider[caret.row - 1].CharCount);
            }
            else
            {
                return caret;
            }
        }

        TextPosition ITextContentProvider.GetRightWord(TextPosition caret)
        {
            if (caret.col < this.textProvider[caret.row].CharCount)
            {
                return new TextPosition(caret.row, caret.col + 1);
            }
            else if (caret.row < this.textProvider.Count - 1)
            {
                return new TextPosition(caret.row + 1, 0);
            }
            else
            {
                return caret;
            }
        }

        #endregion

        class TextContent : IScrollableContent
        {
            enum MouseMode
            {
                Normal,
                Selecting
            }

            private Control host = null;
            private TextEditorBox textEditorBox = null;
            private MouseMode mouseMode = MouseMode.Normal;

            #region Mouse Handlers

            private void host_MouseDown(object sender, MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    this.mouseMode = MouseMode.Selecting;
                    TextPosition position = this.textEditorBox.ViewPointToTextPosition(e.Location);
                    this.textEditorBox.caretVisible = true;
                    if (Control.ModifierKeys == Keys.Shift)
                    {
                        this.textEditorBox.controller.Move(position, false, true);
                    }
                    else
                    {
                        this.textEditorBox.controller.Select(position, position);
                    }
                }
            }

            private void host_MouseMove(object sender, MouseEventArgs e)
            {
                switch (this.mouseMode)
                {
                    case MouseMode.Selecting:
                        {
                            TextPosition position = this.textEditorBox.ViewPointToTextPosition(e.Location);
                            this.textEditorBox.controller.Move(position, false, true);
                        }
                        break;
                }
            }

            private void host_MouseUp(object sender, MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    switch (this.mouseMode)
                    {
                        case MouseMode.Selecting:
                            {
                                this.mouseMode = MouseMode.Normal;
                            }
                            break;
                    }
                }
            }

            #endregion

            #region Key Handlers

            private void host_KeyPress(object sender, KeyPressEventArgs e)
            {
                e.Handled = true;
                this.textEditorBox.controller.Input(e.KeyChar.ToString(), false);
            }

            private void host_KeyDown(object sender, KeyEventArgs e)
            {
                Func<TextEditorBox, KeyEventArgs, bool> action = this.textEditorBox.keyCommands.GetAction(e);
                bool handled = false;
                if (action != null)
                {
                    handled = action(this.textEditorBox, e);
                }
                if (!handled)
                {
                    switch (e.KeyCode)
                    {
                        case Keys.Up:
                            this.textEditorBox.controller.PressUp(e.Control, e.Shift);
                            break;
                        case Keys.Down:
                            this.textEditorBox.controller.PressDown(e.Control, e.Shift);
                            break;
                        case Keys.Left:
                            this.textEditorBox.controller.PressLeft(e.Control, e.Shift);
                            break;
                        case Keys.Right:
                            this.textEditorBox.controller.PressRight(e.Control, e.Shift);
                            break;
                        case Keys.Home:
                            this.textEditorBox.controller.PressHome(e.Control, e.Shift);
                            break;
                        case Keys.End:
                            this.textEditorBox.controller.PressEnd(e.Control, e.Shift);
                            break;
                        case Keys.PageUp:
                            this.textEditorBox.controller.PressPageUp(e.Control, e.Shift);
                            break;
                        case Keys.PageDown:
                            this.textEditorBox.controller.PressPageDown(e.Control, e.Shift);
                            break;
                        case Keys.Back:
                            this.textEditorBox.controller.PressBackspace(e.Control, e.Shift);
                            break;
                        case Keys.Delete:
                            this.textEditorBox.controller.PressDelete(e.Control, e.Shift);
                            break;
                        case Keys.Enter:
                            this.textEditorBox.controller.PressEnter(e.Control, e.Shift);
                            break;
                        case Keys.Escape:
                            break;
                        default:
                            if (!e.Control)
                            {
                                return;
                            }
                            break;
                    }
                }
                e.SuppressKeyPress = true;
            }

            #endregion

            #region Focus Handlers

            private void host_LostFocus(object sender, EventArgs e)
            {
                this.host.Refresh();
            }

            private void host_GotFocus(object sender, EventArgs e)
            {
                this.textEditorBox.caretVisible = true;
                this.host.Refresh();
            }

            #endregion

            #region IScrollableContent Members

            public void Initialize(Control host, ScrollableContentControl control)
            {
                this.host = host;
                this.host.Cursor = Cursors.IBeam;
                this.host.KeyDown += new KeyEventHandler(host_KeyDown);
                this.host.KeyPress += new KeyPressEventHandler(host_KeyPress);
                this.host.GotFocus += new EventHandler(host_GotFocus);
                this.host.LostFocus += new EventHandler(host_LostFocus);
                this.host.MouseDown += new MouseEventHandler(host_MouseDown);
                this.host.MouseMove += new MouseEventHandler(host_MouseMove);
                this.host.MouseUp += new MouseEventHandler(host_MouseUp);

                this.textEditorBox = (TextEditorBox)control;
                this.textEditorBox.host = this.host;
            }

            public void RenderContent(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds)
            {
                int startLine = Math.Min(this.textEditorBox.textProvider.Count - 1, viewVisibleBounds.Top / this.textEditorBox.lineHeight);
                int endLine = Math.Min(this.textEditorBox.textProvider.Count - 1, viewVisibleBounds.Bottom / this.textEditorBox.lineHeight);
                bool widthChanged = false;

                int caretX = 0;
                int caretY1 = 0;
                int caretY2 = 0;

                for (int i = startLine; i <= endLine; i++)
                {
                    int width = this.textEditorBox.GetLineWidth(i);
                    if (width > this.textEditorBox.cachedWholeWidth)
                    {
                        this.textEditorBox.cachedWholeWidth = width;
                        this.textEditorBox.cachedWholeWidthAvailable = true;
                        widthChanged = true;
                    }
                }

                if (widthChanged)
                {
                    this.textEditorBox.UpdateViewSize();
                }
                else
                {

                    using (Brush backBrush = new SolidBrush(this.textEditorBox.BackColor))
                    using (Brush textBrush = new SolidBrush(this.textEditorBox.ForeColor))
                    {
                        g.FillRectangle(backBrush, viewAreaBounds);
                        TextPosition selectionStart = this.textEditorBox.controller.SelectionAnchor;
                        TextPosition selectionEnd = this.textEditorBox.controller.SelectionCaret;
                        if (selectionStart > selectionEnd)
                        {
                            selectionStart = this.textEditorBox.controller.SelectionCaret;
                            selectionEnd = this.textEditorBox.controller.SelectionAnchor;
                        }

                        for (int i = startLine; i <= endLine; i++)
                        {
                            this.textEditorBox.EnsureLineColorized(i);
                            TextLine<LineInfo> line = this.textEditorBox.textProvider[i];
                            int x = EditorMargin - viewVisibleBounds.Left;
                            int y = EditorMargin - viewVisibleBounds.Top + i * this.textEditorBox.lineHeight + this.textEditorBox.textTopOffset;
                            string text = line.Text;

                            int coffset = this.textEditorBox.lineHeight * 2;
                            int c0 = 0;
                            int c3 = text.Length;
                            int x0 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c0)).X;
                            int x3 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c3)).X;

                            if (x3 - x0 >= 32768)
                            {
                                c0 = this.textEditorBox.ViewPointToTextPosition(new Point(-coffset, y)).col;
                                x0 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c0)).X;

                                c3 = this.textEditorBox.ViewPointToTextPosition(new Point(viewAreaBounds.Width + coffset, y)).col;
                                x3 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c3)).X;
                            }

                            if (selectionStart.row <= i && i <= selectionEnd.row)
                            {
                                int c1 = selectionStart.row == i ? selectionStart.col : 0;
                                int c2 = selectionEnd.row == i ? selectionEnd.col : text.Length;

                                c1 = c1 < c0 ? c0 : c1 > c3 ? c3 : c1;
                                c2 = c2 < c0 ? c0 : c2 > c3 ? c3 : c2;

                                int x1 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c1)).X;
                                int x2 = x + this.textEditorBox.CalculateOffset(new TextPosition(i, c2)).X;

                                if (c0 < c1)
                                {
                                    RenderLine(g, i, c0, c1, new Point(x0, y), false, viewAreaBounds.Width);
                                }
                                if (c1 < c2)
                                {
                                    RenderLine(g, i, c1, c2, new Point(x1, y), true, viewAreaBounds.Width);
                                }
                                if (c2 < c3)
                                {
                                    RenderLine(g, i, c2, c3, new Point(x2, y), false, viewAreaBounds.Width);
                                }
                                if (c2 == text.Length && selectionEnd.row > i)
                                {
                                    RenderSelectedCrLf(g, new Point(x2, y));
                                }
                            }
                            else
                            {
                                RenderLine(g, i, c0, c3, new Point(x0, y), false, viewAreaBounds.Width);
                            }
                        }

                        if (this.textEditorBox.caretVisible && this.host.Focused)
                        {
                            Point caret = this.textEditorBox.CalculateOffset(this.textEditorBox.controller.SelectionCaret);
                            caretX = EditorMargin + caret.X;
                            caretY1 = EditorMargin + caret.Y;
                            caretY2 = caretY1 + this.textEditorBox.textHeight;

                            using (Pen caretPen = new Pen(textBrush))
                            {
                                g.DrawLine(caretPen,
                                    caretX - viewVisibleBounds.Left,
                                    caretY1 - viewVisibleBounds.Top,
                                    caretX - viewVisibleBounds.Left,
                                    caretY2 - viewVisibleBounds.Top
                                    );
                            }

                            Point caretPosition = new Point(caretX, caretY1);
                            if (this.textEditorBox.CaretPosition != caretPosition)
                            {
                                this.textEditorBox.CaretPosition = caretPosition;
                            }
                        }
                    }
                }
            }

            #endregion

            #region Rendering

            private void RenderLine(Graphics g, int row, int colStart, int colEnd, Point position, bool selected, int visibleWidth)
            {
                if (colEnd > colStart)
                {
                    TextLine<LineInfo> line = this.textEditorBox.textProvider[row];
                    TextEditorColorItem[] colorItems = this.textEditorBox.colorizer.ColorItems;
                    int[] colors = line.ColorArray;
                    int itemStart = colStart;
                    int itemColor = colors[colStart];
                    int xStart = position.X;

                    for (int i = colStart; i <= colEnd; i++)
                    {
                        if (i == colEnd || itemColor != colors[i])
                        {
                            string text = line.GetString(itemStart, i - itemStart);
                            TextEditorColorItem colorItem = colorItems[itemColor];
                            int xEnd = xStart + this.textEditorBox.CalculateOffset(text);
                            if (xStart < visibleWidth && xEnd >= 0)
                            {
                                if (selected)
                                {
                                    RenderString(g, text, new Point(xStart, position.Y), colorItem.HighlightText, colorItem.Highlight);
                                }
                                else
                                {
                                    RenderString(g, text, new Point(xStart, position.Y), colorItem.Text, this.textEditorBox.BackColor);
                                }
                            }
                            if (xEnd >= visibleWidth)
                            {
                                break;
                            }
                            if (i != colEnd)
                            {
                                itemStart = i;
                                itemColor = colors[i];
                                xStart = xEnd;
                            }
                        }
                    }
                }
            }

            private void RenderString(Graphics g, string text, Point position, Color foreColor, Color backColor)
            {
                TextRenderer.DrawText(g, text, this.textEditorBox.Font, position, foreColor, backColor, TextFormatFlags.NoPadding);
            }

            private void RenderSelectedCrLf(Graphics g, Point position)
            {
                RenderString(g, " ", position, SystemColors.HighlightText, SystemColors.Highlight);
            }

            #endregion
        }
    }

    public class TextEditorBoxKeyCommands
    {
        struct KeyCommand : IEquatable<KeyCommand>
        {
            public Keys Key { get; set; }
            public bool Control { get; set; }
            public bool Shift { get; set; }

            public bool Equals(KeyCommand other)
            {
                return this == other;
            }

            public override bool Equals(object obj)
            {
                if (obj is KeyCommand)
                {
                    return false;
                }
                else
                {
                    return Equals((KeyCommand)obj);
                }
            }

            public override int GetHashCode()
            {
                return this.Key.GetHashCode() + this.Control.GetHashCode() + this.Shift.GetHashCode();
            }

            public static bool operator ==(KeyCommand c1, KeyCommand c2)
            {
                return c2 != null && c1.Key == c2.Key && c1.Control == c2.Control && c1.Shift == c2.Shift;
            }

            public static bool operator !=(KeyCommand c1, KeyCommand c2)
            {
                return !(c1 != c2);
            }
        }

        private Dictionary<KeyCommand, Func<TextEditorBox, KeyEventArgs, bool>> actions = new Dictionary<KeyCommand, Func<TextEditorBox, KeyEventArgs, bool>>();

        public TextEditorBoxKeyCommands()
        {
        }

        public void RegisterCommand(Keys key, bool control, bool shift, Func<TextEditorBox, KeyEventArgs, bool> action)
        {
            this.actions.Add(new KeyCommand() { Key = key, Control = control, Shift = shift }, action);
        }

        public Func<TextEditorBox, KeyEventArgs, bool> GetAction(KeyEventArgs e)
        {
            KeyCommand key = new KeyCommand() { Key = e.KeyCode, Control = e.Control, Shift = e.Shift };
            Func<TextEditorBox, KeyEventArgs, bool> action = null;
            if (this.actions.TryGetValue(key, out action))
            {
                return action;
            }
            else
            {
                return null;
            }
        }
    }
}

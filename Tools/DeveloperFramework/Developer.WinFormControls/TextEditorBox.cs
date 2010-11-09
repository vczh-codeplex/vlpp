﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;

namespace Developer.WinFormControls
{
    public partial class TextEditorBox
        : ScrollableContentControl
        , ITextContentProvider
        , ITextEditorControlPanelCallBack
    {
        private const int EditorMargin = 4;

        public class LineInfo
        {
            public int lineWidth = 0;
            public bool lineWidthAvailable = false;
            public int colorizerFinalState = 0;
            public object controlPanelData = null;
        }

        #region Events

        public event EventHandler SelectionChanged;

        protected virtual void OnSelectionChanged(EventArgs e)
        {
            if (this.SelectionChanged != null)
            {
                this.SelectionChanged(this, e);
            }
        }

        #endregion

        #region Editing Processor Fields

        private TextProvider<LineInfo> textProvider = null;
        private TextEditorController controller = null;
        private TextEditorBoxKeyCommands keyCommands = null;
        private TextEditorPopupList popupList = null;

        #endregion

        #region View Calculation Fields

        private int lineHeight = 0;
        private int textHeight = 0;
        private int textTopOffset = 0;
        private int cachedWholeWidth = 0;
        private bool cachedWholeWidthAvailable = false;

        #endregion

        #region Colorization Fields

        private ITextEditorColorizer colorizer = null;
        private int colorizedLines = 0;

        #endregion

        #region Control Panel Fields

        private ITextEditorControlPanel controlPanel = null;

        #endregion

        #region Rendering Fields

        private bool caretVisible = true;
        private Control host = null;
        private Graphics temporaryGraphics = null;

        private TextPosition oldAnchor;
        private TextPosition oldCaret;
        private Point oldPoint;
        private Size oldSize;
        private int oldLineHeight = -1;

        #endregion

        public TextEditorBox()
            : base(new TextContent())
        {
            this.ImeEnabled = true;
            this.EnableDefaultCommands = true;

            this.textProvider = new TextProvider<LineInfo>();
            this.controlPanel = new TextEditorControlPanel();
            this.keyCommands = new TextEditorBoxKeyCommands();
            this.popupList = new TextEditorPopupList(this, this.host);

            this.controller = new TextEditorController(this);
            this.colorizer = new TextEditorPlanTextColorizer(this);

            InitializeComponent();
            this.components.Add(new DisposableComponent(this.textProvider));
            this.components.Add(this.popupList);
            UpdateLineHeight();

            this.keyCommands.RegisterCommand(Keys.C, true, false, DoCopy);
            this.keyCommands.RegisterCommand(Keys.X, true, false, DoCut);
            this.keyCommands.RegisterCommand(Keys.V, true, false, DoPaste);
            this.keyCommands.RegisterCommand(Keys.A, true, false, DoSelectAll);
        }

        public bool EnableDefaultCommands { get; set; }

        #region Rendering Area Calculation

        public override void RedrawContent(bool totalRefresh, bool refreshImmediately)
        {
            Point newPoint = this.ViewPosition;
            Size newSize = this.ViewAreaSize;
            TextPosition newAnchor = this.SelectionAnchor;
            TextPosition newCaret = this.SelectionCaret;
            if (refreshImmediately)
            {
                this.host.Refresh();
            }
            else if (!totalRefresh
                && this.oldPoint == newPoint
                && this.oldSize == newSize
                && this.oldAnchor == this.oldCaret
                && newAnchor == newCaret
                && this.oldAnchor.row == newAnchor.row
                && this.oldCaret.row == newCaret.row
                && this.oldLineHeight == this.lineHeight
                )
            {
                int row = newCaret.row;
                int x = 0;
                int y = this.TextPositionToViewPoint(new TextPosition(row, 0)).Y;
                int w = newSize.Width;
                int h = this.lineHeight;
                this.host.Invalidate(new Rectangle(x, y, w, h));
            }
            else
            {
                this.host.Invalidate();
            }
            this.oldPoint = newPoint;
            this.oldSize = newSize;
            this.oldAnchor = newAnchor;
            this.oldCaret = newCaret;
            this.oldLineHeight = this.lineHeight;
        }

        #endregion

        #region Service API

        public TextProvider<LineInfo> TextProvider
        {
            get
            {
                return this.textProvider;
            }
        }

        public TextEditorController Controller
        {
            get
            {
                return this.controller;
            }
        }

        public TextEditorBoxKeyCommands KeyCommands
        {
            get
            {
                return this.keyCommands;
            }
        }

        #endregion

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

        #region Callback Extension API

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
                RedrawContent(true, false);
            }
        }

        public ITextEditorControlPanel ControlPanel
        {
            get
            {
                return this.controlPanel;
            }
            set
            {
                this.controlPanel = value;
                if (this.controlPanel != null)
                {
                    this.controlPanel.InstallCallBack(this);
                }
                UpdateViewSize();
            }
        }

        public int EditorControlPanel
        {
            get
            {
                return this.controlPanel.Width;
            }
        }

        public int[] GetColorsOfLine(int index)
        {
            EnsureLineColorized(index);
            TextLine<LineInfo> line = this.textProvider[index];
            int[] colors = line.ColorArray;
            int[] copy = new int[line.CharCount];
            Array.Copy(colors, copy, copy.Length);
            return copy;
        }

        public bool AddBlock(int row, int start, int end)
        {
            if (this.textProvider[row].AddBlock(start, end))
            {
                RedrawContent(true, false);
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool RemoveBlock(int row, int start, int end)
        {
            if (this.textProvider[row].RemoveBlock(start, end))
            {
                RedrawContent(true, false);
                return true;
            }
            else
            {
                return false;
            }
        }

        #endregion

        #region UI Extension API

        public void PopupItems(IEnumerable<TextEditorPopupItem> items, bool forceClosingPrevious = false, string searchingKey = "", bool needToDisposeImages = true, int maxItems = 10)
        {
            if (forceClosingPrevious)
            {
                if (this.popupList.PopupVisible)
                {
                    this.popupList.Close();
                }
            }
            else
            {
                if (this.popupList.PopupVisible)
                {
                    return;
                }
            }
            Point position = TextPositionToViewPoint(this.SelectionCaret);
            if (position.X <= 0) position.X = 0; else if (position.X > this.host.Width) position.X = this.host.Width;
            if (position.Y <= 0) position.Y = 0; else if (position.Y > this.host.Height) position.Y = this.host.Height;
            position.Y += this.lineHeight;
            this.popupList.Open(this.host, position, items, searchingKey, needToDisposeImages, maxItems);
        }

        #endregion

        #region View API

        public int LineHeight
        {
            get
            {
                return this.lineHeight;
            }
        }

        public int TextHeight
        {
            get
            {
                return this.textHeight;
            }
        }

        public int TextTopOffset
        {
            get
            {
                return this.textTopOffset;
            }
        }

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
            return Point.Add(CalculateOffset(position), new Size(this.EditorControlPanel + EditorMargin, EditorMargin));
        }

        public TextPosition DocumentPointToTextPosition(Point point)
        {
            return CalculatePosition(Point.Subtract(point, new Size(this.EditorControlPanel + EditorMargin, EditorMargin)));
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
            }
            return this.CanCopy;
        }

        public bool Cut()
        {
            if (this.CanCut)
            {
                Clipboard.SetText(this.SelectedText);
                this.SelectedText = "";
            }
            return this.CanCut;
        }

        public bool Paste()
        {
            if (this.CanPaste)
            {
                this.SelectedText = Clipboard.GetText();
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
            RedrawContent(true, false);
        }

        protected override void OnForeColorChanged(EventArgs e)
        {
            base.OnForeColorChanged(e);
            RedrawContent(true, false);
        }

        private void timerCaret_Tick(object sender, EventArgs e)
        {
            this.caretVisible = !this.caretVisible;
            RedrawContent(false, false);
        }

        #endregion

        #region Sizing Core Implementation

        private void SetFormat(StringFormat format, int tabStart)
        {
            format.Alignment = StringAlignment.Near;
            format.FormatFlags = StringFormatFlags.NoClip | StringFormatFlags.FitBlackBox | StringFormatFlags.MeasureTrailingSpaces;
            format.HotkeyPrefix = System.Drawing.Text.HotkeyPrefix.None;
            format.LineAlignment = StringAlignment.Near;
            format.Trimming = StringTrimming.None;
            format.SetTabStops((tabStart % 48 + 48) % 48, new float[] { 48 });
        }

        private void RenderString(Graphics g, TextLine<LineInfo> line, int start, int count, int tabStart, Point position, SolidBrush foreColor)
        {
            using (StringFormat format = new StringFormat())
            {
                SetFormat(format, tabStart);
                string text = line.GetString(start, count);
                g.DrawString(text, this.Font, foreColor, position, format);
            }
            //TextRenderer.DrawText(g, line.GetString(start, count), this.Font, position, foreColor.Color, TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
        }

        private int CalculateOffset(string text)
        {
            if (this.temporaryGraphics == null)
            {
                this.temporaryGraphics = Graphics.FromHwnd(this.host.Handle);
            }
            using (StringFormat format = new StringFormat())
            {
                SetFormat(format, 0);
                format.SetMeasurableCharacterRanges(new CharacterRange[] { new CharacterRange(0, text.Length) });
                Region[] regions = this.temporaryGraphics.MeasureCharacterRanges(text, this.Font, RectangleF.Empty, format);
                int result = (int)regions[0].GetBounds(this.temporaryGraphics).Width;
                regions[0].Dispose();
                return result;
            }
            //Size size = TextRenderer.MeasureText(this.temporaryGraphics, text, this.Font, new Size(0, 0), TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
            //return size.Width;
        }

        #endregion

        #region Implementation

        private void UpdateViewSize()
        {
            this.ViewSize = new Size(this.cachedWholeWidth + this.EditorControlPanel + EditorMargin * 2, this.textProvider.Count * this.lineHeight + EditorMargin * 2);
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

        private int CalculateOffset(TextLine<LineInfo> line, int start)
        {
            int startOffset = 0;
            if (start > 0)
            {
                startOffset = line.OffsetArray[start - 1];
                if (startOffset == 0)
                {
                    startOffset = CalculateOffset("[" + line.GetString(0, start) + "]") - CalculateOffset("[]");
                    line.OffsetArray[start - 1] = startOffset;
                }
            }
            return startOffset;
        }

        private int CalculateOffset(TextLine<LineInfo> line, int start, int end)
        {
            return CalculateOffset(line, end) - CalculateOffset(line, start);
        }

        private Point CalculateOffset(TextPosition position)
        {
            position = this.controller.Normalize(position);
            TextLine<LineInfo> line = this.textProvider[position.row];
            return new Point(CalculateOffset(line, 0, position.col), position.row * this.lineHeight + this.textTopOffset);
        }

        private TextPosition CalculatePosition(Point point)
        {
            int row = point.Y / this.lineHeight;
            if (row < 0)
            {
                return new TextPosition(0, 0);
            }
            else if (row >= this.textProvider.Count)
            {
                row = this.textProvider.Count - 1;
                return new TextPosition(row, this.textProvider[row].CharCount);
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
            Point caretPositionTop = Point.Add(CalculateOffset(caret), new Size(this.EditorControlPanel + EditorMargin, EditorMargin));
            Point caretPositionBottom = new Point(caretPositionTop.X, caretPositionTop.Y + this.lineHeight);
            Rectangle visibleBounds = new Rectangle(this.ViewPosition, this.ViewAreaSize);
            if (!visibleBounds.Contains(caretPositionTop) || !visibleBounds.Contains(caretPositionBottom))
            {
                Point oldViewPosition = this.ViewPosition;
                Point newViewPosition = new Point(caretPositionTop.X - this.ViewVisibleSize.Width / 2, caretPositionTop.Y - this.ViewVisibleSize.Height / 2 + this.lineHeight / 2);
                if (newViewPosition.X < oldViewPosition.X && !this.cachedWholeWidthAvailable)
                {
                    ForceUpdateViewSize();
                }
                this.ViewPosition = newViewPosition;
            }
            this.caretVisible = true;
            OnSelectionChanged(new EventArgs());
        }

        bool ITextContentProvider.OnEdit(TextPosition start, TextPosition end, string[] lines)
        {
            this.controlPanel.OnBeforeEdit(start, end, ref lines);
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
                if (i > start.row)
                {
                    this.textProvider[i].Tag.controlPanelData = null;
                }
            }
            TextPosition newEnd = this.textProvider.Edit(start, end, lines);

            if (optimizable)
            {
                int newEndRow = start.row + lines.Length - 1;
                this.colorizedLines = start.row;
                EnsureLineColorized(newEndRow);
                if (this.textProvider[newEndRow].Tag.colorizerFinalState == lastFinalState)
                {
                    this.colorizedLines = lastColorizedLines + newEndRow - end.row;
                    goto FINISH_COLORIZING;
                }
            }
            this.colorizedLines = Math.Min(this.colorizedLines, start.row);

        FINISH_COLORIZING:
            this.controlPanel.OnAfterEdit(start, end, newEnd);
            OnTextChanged(new EventArgs());
            return true;
        }

        void ITextContentProvider.OnFinishEdit()
        {
            if (this.ViewSize.Height != this.textProvider.Count * this.lineHeight + EditorMargin * 2)
            {
                UpdateLineHeight();
            }
            (this as ITextContentProvider).OnSelectionAreaChanged();
        }

        void ITextContentProvider.OnRefreshSuggestion()
        {
            RedrawContent(false, false);
        }

        TextPosition ITextContentProvider.GetLeftWord(TextPosition caret)
        {
            return caret;
        }

        TextPosition ITextContentProvider.GetRightWord(TextPosition caret)
        {
            return caret;
        }

        int ITextContentProvider.GetLeftBlock(TextPosition caret)
        {
            return this.textProvider[caret.row].GetLeftBlock(caret.col);
        }

        int ITextContentProvider.GetRightBlock(TextPosition caret)
        {
            return this.textProvider[caret.row].GetRightBlock(caret.col);
        }

        Tuple<int, int> ITextContentProvider.GetBlock(TextPosition caret)
        {
            return this.textProvider[caret.row].GetBlock(caret.col);
        }

        #endregion

        #region ITextEditorControlPanelCallBack Members

        object ITextEditorControlPanelCallBack.GetLineData(int index)
        {
            return this.textProvider[index].Tag.controlPanelData;
        }

        void ITextEditorControlPanelCallBack.BindLineData(int index, object data)
        {
            this.textProvider[index].Tag.controlPanelData = data;
        }

        TextEditorBox ITextEditorControlPanelCallBack.TextEditorBox
        {
            get
            {
                return this;
            }
        }

        #endregion

        class TextContent : IScrollableContent
        {
            enum MouseMode
            {
                Normal,
                Selecting,
                ControlPanelDragging
            }

            enum MouseAction
            {
                Down,
                Move,
                Up
            }

            private Control host = null;
            private TextEditorBox textEditorBox = null;
            private MouseMode mouseMode = MouseMode.Normal;

            #region Mouse Handlers

            private bool InvokeControlPanel(MouseAction action, MouseEventArgs e, bool ensureInvoked)
            {
                int line = (e.Y + this.textEditorBox.ViewPosition.Y - EditorMargin) / this.textEditorBox.lineHeight;
                if (line >= 0 && line < this.textEditorBox.textProvider.Count)
                {
                    Rectangle area = new Rectangle(
                        0,
                        EditorMargin - this.textEditorBox.ViewPosition.Y + line * this.textEditorBox.lineHeight,
                        this.textEditorBox.EditorControlPanel, this.textEditorBox.lineHeight
                        );
                    Point position = new Point(e.X, e.Y - area.Top);
                    switch (action)
                    {
                        case MouseAction.Down:
                            this.textEditorBox.controlPanel.OnMouseDown(line, area, position, e.Button);
                            break;
                        case MouseAction.Move:
                            this.textEditorBox.controlPanel.OnMouseMove(line, area, position, e.Button);
                            break;
                        case MouseAction.Up:
                            this.textEditorBox.controlPanel.OnMouseUp(line, area, position, e.Button);
                            break;
                    }
                    return true;
                }
                else if (ensureInvoked)
                {
                    switch (action)
                    {
                        case MouseAction.Down:
                            this.textEditorBox.controlPanel.OnMouseDown(-1, Rectangle.Empty, Point.Empty, e.Button);
                            break;
                        case MouseAction.Move:
                            this.textEditorBox.controlPanel.OnMouseMove(-1, Rectangle.Empty, Point.Empty, e.Button);
                            break;
                        case MouseAction.Up:
                            this.textEditorBox.controlPanel.OnMouseUp(-1, Rectangle.Empty, Point.Empty, e.Button);
                            break;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }

            private void host_MouseDown(object sender, MouseEventArgs e)
            {
                if (e.X >= this.textEditorBox.EditorControlPanel)
                {
                    if (e.Button == MouseButtons.Left)
                    {
                        this.mouseMode = MouseMode.Selecting;
                        TextPosition position = this.textEditorBox.ViewPointToTextPosition(e.Location);
                        Tuple<int, int> block = this.textEditorBox.textProvider[position.row].GetBlock(position.col);
                        this.textEditorBox.caretVisible = true;
                        if (Control.ModifierKeys == Keys.Shift)
                        {
                            this.textEditorBox.controller.Move(new TextPosition(position.row, block.Item1), false, true);
                        }
                        else
                        {
                            this.textEditorBox.controller.Select(new TextPosition(position.row, block.Item1), new TextPosition(position.row, block.Item2));
                        }
                    }
                }
                else
                {
                    if (InvokeControlPanel(MouseAction.Down, e, false))
                    {
                        this.mouseMode = MouseMode.ControlPanelDragging;
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
                    case MouseMode.Normal:
                    case MouseMode.ControlPanelDragging:
                        {
                            if (e.X < this.textEditorBox.EditorControlPanel)
                            {
                                InvokeControlPanel(MouseAction.Move, e, this.mouseMode == MouseMode.ControlPanelDragging);
                            }
                        }
                        break;
                }
                if (e.X < this.textEditorBox.EditorControlPanel)
                {
                    this.host.Cursor = Cursors.Default;
                }
                else if (this.mouseMode == MouseMode.Selecting)
                {
                    this.host.Cursor = Cursors.IBeam;
                }
                else
                {
                    TextPosition position = this.textEditorBox.ViewPointToTextPosition(e.Location);
                    Tuple<int, int> block = this.textEditorBox.textProvider[position.row].GetBlock(position.col);
                    if (block.Item1 != block.Item2)
                    {
                        this.host.Cursor = Cursors.Default;
                    }
                    else
                    {
                        this.host.Cursor = Cursors.IBeam;
                    }
                }
            }

            private void host_MouseUp(object sender, MouseEventArgs e)
            {
                switch (this.mouseMode)
                {
                    case MouseMode.Selecting:
                        {
                            if (e.Button == MouseButtons.Left)
                            {
                                this.mouseMode = MouseMode.Normal;
                            }
                        }
                        break;
                    case MouseMode.ControlPanelDragging:
                        InvokeControlPanel(MouseAction.Up, e, true);
                        this.mouseMode = MouseMode.Normal;
                        break;
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
                this.textEditorBox.RedrawContent(false, false);
            }

            private void host_GotFocus(object sender, EventArgs e)
            {
                this.textEditorBox.caretVisible = true;
                this.textEditorBox.RedrawContent(false, false);
            }

            #endregion

            #region IScrollableContent Members

            public void Dispose()
            {
            }

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
                int startLine = Math.Min(this.textEditorBox.textProvider.Count - 1, (viewVisibleBounds.Top - EditorMargin) / this.textEditorBox.lineHeight);
                int endLine = Math.Min(this.textEditorBox.textProvider.Count - 1, (viewVisibleBounds.Bottom - EditorMargin) / this.textEditorBox.lineHeight);
                RenderContentRange(g, viewVisibleBounds, viewAreaBounds, startLine, endLine);
            }

            #endregion

            #region Rendering

            private void RenderContentRange(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds, int startLine, int endLine)
            {
                bool widthChanged = false;
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

                        int backgroundLeft = this.textEditorBox.EditorControlPanel;
                        int backgroundWidth = viewAreaBounds.Width - backgroundLeft;
                        for (int i = startLine; i <= endLine; i++)
                        {
                            RenderWholeLine(g, viewVisibleBounds, viewAreaBounds, selectionStart, selectionEnd, backgroundLeft, backgroundWidth, i);
                        }

                        if (this.textEditorBox.caretVisible && (this.host.Focused || this.textEditorBox.popupList.PopupVisible))
                        {
                            RenderCaret(g, viewVisibleBounds, viewAreaBounds, textBrush);
                        }

                        if (this.textEditorBox.EditorControlPanel > 0)
                        {
                            RenderControlPanel(g, viewVisibleBounds, viewAreaBounds, startLine, endLine, backBrush);
                        }
                    }
                }
            }

            private void RenderControlPanel(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds, int startLine, int endLine, Brush backBrush)
            {
                Rectangle controlPanelArea = new Rectangle(viewAreaBounds.Left, viewAreaBounds.Top, this.textEditorBox.EditorControlPanel, viewAreaBounds.Height);
                g.FillRectangle(backBrush, controlPanelArea);
                this.textEditorBox.controlPanel.DrawControlPanelBackground(g, controlPanelArea);
                int width = this.textEditorBox.EditorControlPanel;
                int height = this.textEditorBox.lineHeight;
                int y = EditorMargin - viewVisibleBounds.Top + startLine * height;
                for (int i = startLine; i <= endLine; i++)
                {
                    Rectangle area = new Rectangle(0, y, width, height);
                    y += height;
                    this.textEditorBox.controlPanel.DrawControlPanel(g, i, area);
                }
            }

            private void RenderCaret(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds, Brush textBrush)
            {
                Point caret = this.textEditorBox.CalculateOffset(this.textEditorBox.controller.SelectionCaret);
                int caretX = this.textEditorBox.EditorControlPanel + EditorMargin + caret.X + viewAreaBounds.Left;
                int caretY1 = EditorMargin + caret.Y + viewAreaBounds.Top;
                int caretY2 = caretY1 + this.textEditorBox.textHeight;

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

            private void RenderWholeLine(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds, TextPosition selectionStart, TextPosition selectionEnd, int backgroundLeft, int backgroundWidth, int lineIndex)
            {
                int backgroundTop = EditorMargin - viewVisibleBounds.Top + lineIndex * this.textEditorBox.lineHeight;
                Rectangle lineRectangle = new Rectangle(backgroundLeft, backgroundTop, backgroundWidth, this.textEditorBox.lineHeight);
                this.textEditorBox.controlPanel.DrawLineBackground(g, lineIndex, lineRectangle);

                this.textEditorBox.EnsureLineColorized(lineIndex);
                TextLine<LineInfo> line = this.textEditorBox.textProvider[lineIndex];
                int x = this.textEditorBox.EditorControlPanel + EditorMargin - viewVisibleBounds.Left + viewAreaBounds.Left;
                int y = backgroundTop + this.textEditorBox.textTopOffset + viewAreaBounds.Top;
                string text = line.Text;

                int coffset = this.textEditorBox.lineHeight * 2;
                int tabStart = EditorMargin - viewVisibleBounds.Left;
                int c0 = 0;
                int c3 = text.Length;
                int x0 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c0)).X;
                int x3 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c3)).X;

                if (x3 - x0 >= 32768)
                {
                    c0 = this.textEditorBox.ViewPointToTextPosition(new Point(-coffset, y)).col;
                    x0 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c0)).X;

                    c3 = this.textEditorBox.ViewPointToTextPosition(new Point(viewAreaBounds.Width + coffset, y)).col;
                    x3 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c3)).X;
                }

                int[] colors = null;
                if (this.textEditorBox.controlPanel.NeedColorLineForDisplay(lineIndex))
                {
                    colors = new int[line.CharCount];
                    Array.Copy(line.ColorArray, colors, line.CharCount);
                    this.textEditorBox.controlPanel.ColorLineForDisplay(lineIndex, colors);
                }
                else
                {
                    colors = line.ColorArray;
                }
                if (selectionStart.row <= lineIndex && lineIndex <= selectionEnd.row)
                {
                    int c1 = selectionStart.row == lineIndex ? selectionStart.col : 0;
                    int c2 = selectionEnd.row == lineIndex ? selectionEnd.col : text.Length;

                    c1 = c1 < c0 ? c0 : c1 > c3 ? c3 : c1;
                    c2 = c2 < c0 ? c0 : c2 > c3 ? c3 : c2;

                    int x1 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c1)).X;
                    int x2 = x + this.textEditorBox.CalculateOffset(new TextPosition(lineIndex, c2)).X;

                    if (c0 < c1)
                    {
                        RenderLine(g, lineIndex, c0, c1, new Point(x0, y), false, viewAreaBounds.Width, colors, tabStart);
                    }
                    if (c1 < c2)
                    {
                        RenderLine(g, lineIndex, c1, c2, new Point(x1, y), true, viewAreaBounds.Width, colors, tabStart);
                    }
                    if (c2 < c3)
                    {
                        RenderLine(g, lineIndex, c2, c3, new Point(x2, y), false, viewAreaBounds.Width, colors, tabStart);
                    }
                    if (c2 == text.Length && selectionEnd.row > lineIndex)
                    {
                        RenderSelectedCrLf(g, new Point(x2, y));
                    }
                }
                else
                {
                    RenderLine(g, lineIndex, c0, c3, new Point(x0, y), false, viewAreaBounds.Width, colors, tabStart);
                }

                this.textEditorBox.controlPanel.DrawLineForeground(g, lineIndex, lineRectangle);
            }

            private void RenderLine(Graphics g, int row, int colStart, int colEnd, Point position, bool selected, int visibleWidth, int[] colors, int tabStart)
            {
                if (colEnd > colStart)
                {
                    TextLine<LineInfo> line = this.textEditorBox.textProvider[row];
                    TextEditorColorItem[] colorItems = this.textEditorBox.colorizer.ColorItems;
                    int itemStart = colStart;
                    int itemColor = colors[colStart];
                    int xStart = position.X;

                    for (int i = colStart; i <= colEnd; i++)
                    {
                        if (i == colEnd || itemColor != colors[i])
                        {
                            TextEditorColorItem colorItem = colorItems[itemColor];
                            int xEnd = xStart + this.textEditorBox.CalculateOffset(line, itemStart, i);
                            if (xStart < visibleWidth && xEnd >= 0)
                            {
                                Point p = new Point(xStart, position.Y);
                                if (selected)
                                {
                                    RenderBackground(g, p, xEnd - xStart, colorItem.HighlightBrush);
                                    this.textEditorBox.RenderString(g, line, itemStart, i - itemStart, tabStart, p, colorItem.HighlightTextBrush);
                                }
                                else
                                {
                                    this.textEditorBox.RenderString(g, line, itemStart, i - itemStart, tabStart, p, colorItem.TextBrush);
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

            private void RenderBackground(Graphics g, Point position, int width, Brush brush)
            {
                int y = position.Y - this.textEditorBox.textTopOffset;
                g.FillRectangle(brush, position.X, y, width, this.textEditorBox.lineHeight);
            }

            private void RenderSelectedCrLf(Graphics g, Point position)
            {
                RenderBackground(g, position, 10, SystemBrushes.Highlight);
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

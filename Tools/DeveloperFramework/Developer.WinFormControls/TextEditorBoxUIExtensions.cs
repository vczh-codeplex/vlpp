using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Developer.LanguageProvider;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public class TextEditorBoxUIExtensions : IDisposable
    {
        private TextEditorBox textEditorBox = null;
        private Control host = null;
        private TextEditorPopup popupList = null;
        private TextEditorTooltip tooltip = null;
        private string lastTooltip = null;
        private bool needToClosePopupList = false;

        public TextEditorBoxUIExtensions(TextEditorBox textEditorBox, Control host)
        {
            this.textEditorBox = textEditorBox;
            this.host = host;
            this.popupList = new TextEditorPopup(this.textEditorBox, this.host);
            this.tooltip = new TextEditorTooltip();
            this.tooltip.VisibleChanged += new EventHandler(tooltip_VisibleChanged);
        }

        public void Dispose()
        {
            this.popupList.Dispose();
            this.tooltip.Dispose();
        }

        private void tooltip_VisibleChanged(object sender, EventArgs e)
        {
            if (!this.tooltip.Visible)
            {
                CloseQuickInfoTooltip();
            }
        }

        #region UI Extensions API

        public bool QuickInfoTooltipOpening { get; private set; }

        public void PopupItems(IEnumerable<TextEditorPopupItem> items, bool forceClosingPrevious = false, string searchingKey = "", bool needToDisposeImages = true, int maxItems = 10)
        {
            if (forceClosingPrevious)
            {
                if (this.popupList.Visible)
                {
                    this.popupList.Close();
                }
            }
            else
            {
                if (this.popupList.Visible)
                {
                    return;
                }
            }
            Point locationTop = this.host.PointToScreen(this.textEditorBox.TextPositionToViewPoint(this.textEditorBox.SelectionCaret));
            Point locationBottom = new Point(locationTop.X, locationTop.Y + this.textEditorBox.LineHeight);
            this.popupList.Show(this.host, locationTop, locationBottom, items, searchingKey, needToDisposeImages, maxItems);
        }

        public void ClosePopupItems()
        {
            this.popupList.Hide();
        }

        public TextEditorPopupItem[] PopupedItems
        {
            get
            {
                return this.popupList.Items;
            }
        }

        public TextEditorPopupItem SelectedPopupedItem
        {
            get
            {
                return this.popupList.SelectedItem;
            }
        }

        public void PopupTooltip(TextPosition pos, string text)
        {
            if (!this.QuickInfoTooltipOpening)
            {
                if (string.IsNullOrEmpty(text))
                {
                    this.tooltip.Hide();
                }
                else
                {
                    Point locationTop = this.textEditorBox.PointToScreen(this.textEditorBox.TextPositionToViewPoint(pos));
                    Point locationBottom = locationTop;
                    locationBottom.Y += this.textEditorBox.LineHeight;
                    text = text.TrimEnd(' ', '\t', '\r', '\n');
                    this.tooltip.Show(this.host, locationTop, locationBottom, new RichContent.Content(new RichContent.Text(text)));
                }
            }
        }

        public void PopupQuickInfoTooltip(TextPosition pos, XDocument content)
        {
            if (this.QuickInfoTooltipOpening)
            {
                if (content == null)
                {
                    CloseQuickInfoTooltip();
                }
                else
                {
                    Point aTop = this.textEditorBox.PointToScreen(this.textEditorBox.TextPositionToViewPoint(pos));
                    Point aBottom = new Point(aTop.X, aTop.Y + this.textEditorBox.LineHeight);
                    Point bTop = this.textEditorBox.PointToScreen(this.textEditorBox.TextPositionToViewPoint(this.textEditorBox.SelectionCaret));
                    Point bBottom = new Point(aTop.X, aTop.Y + this.textEditorBox.LineHeight);

                    int x = Math.Min(aTop.X, bTop.X);
                    int y1 = Math.Min(aTop.Y, bTop.Y);
                    int y2 = Math.Max(aBottom.Y, bBottom.Y);
                    this.tooltip.Show(this.host, new Point(x, y1), new Point(x, y2), content);
                }
            }
        }

        public void OpenQuickInfoTooltip()
        {
            if (!this.QuickInfoTooltipOpening)
            {
                PopupTooltip(new TextPosition(0, 0), null);
                this.QuickInfoTooltipOpening = true;
                var content = this.textEditorBox.ControlPanel.OnGetQuickInfoTooltip();
                if (content != null)
                {
                    PopupQuickInfoTooltip(content.Item2, content.Item1);
                }
            }
        }

        public void CloseQuickInfoTooltip()
        {
            if (this.QuickInfoTooltipOpening)
            {
                this.tooltip.Hide();
                this.QuickInfoTooltipOpening = false;
            }
        }

        #endregion

        #region UI Extensions Callbacks

        internal bool NeedToShowCaret
        {
            get
            {
                return this.tooltip.Visible || this.popupList.Visible;
            }
        }

        internal void HostHandleKey(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Back:
                    if (this.popupList.Visible)
                    {
                        if (!this.popupList.ProcessBackspace())
                        {
                            ClosePopupItems();
                        }
                    }
                    break;
            }
        }

        internal void HostHandleFinishEdit()
        {
            if (this.QuickInfoTooltipOpening)
            {
                var content = this.textEditorBox.ControlPanel.OnGetQuickInfoTooltip();
                if (content == null)
                {
                    PopupQuickInfoTooltip(new TextPosition(0, 0), null);
                }
                else
                {
                    PopupQuickInfoTooltip(content.Item2, content.Item1);
                }
            }
        }

        internal bool HostKeyPress(object sender, KeyPressEventArgs e)
        {
            if (this.popupList.Visible)
            {
                if (this.needToClosePopupList || !this.textEditorBox.ControlPanel.IsPopupListCharAcceptable(e.KeyChar))
                {
                    this.popupList.SelectItem();
                    ClosePopupItems();
                    this.needToClosePopupList = false;
                }
                else if (!this.popupList.ProcessChar(e.KeyChar))
                {
                    ClosePopupItems();
                }
            }
            return true;
        }

        internal bool HostKeyUp(object sender, KeyEventArgs e)
        {
            if (this.needToClosePopupList)
            {
                ClosePopupItems();
                this.needToClosePopupList = false;
            }
            return true;
        }

        internal bool HostKeyDown(object sender, KeyEventArgs e)
        {
            this.needToClosePopupList = false;
            if (this.popupList.Visible)
            {
                if (!this.textEditorBox.ControlPanel.IsPopupListKeyAcceptable(e))
                {
                    if (this.popupList.ProcessKey(e))
                    {
                        e.Handled = true;
                        e.SuppressKeyPress = true;
                        return false;
                    }
                    else if (e.KeyCode != Keys.Packet)
                    {
                        this.needToClosePopupList = true;
                    }
                }
            }
            return true;
        }

        internal bool HostBeforeMouseDoubleClick(object sender, MouseEventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseDoubleClick(object sender, MouseEventArgs e)
        {
        }

        internal bool HostBeforeMouseDown(object sender, MouseEventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseDown(object sender, MouseEventArgs e)
        {
        }

        internal bool HostBeforeMouseMove(object sender, MouseEventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseMove(object sender, MouseEventArgs e)
        {
            TextPosition tp1 = this.textEditorBox.ViewPointToTextPosition(e.Location);
            string tooltip = null;
            if (!this.QuickInfoTooltipOpening)
            {
                TextPosition tp0 = this.textEditorBox.Controller.Normalize(new TextPosition(tp1.row, tp1.col - 1));
                TextPosition tp2 = this.textEditorBox.Controller.Normalize(new TextPosition(tp1.row, tp1.col + 1));
                Point p0 = this.textEditorBox.TextPositionToViewPoint(tp0);
                Point p2 = this.textEditorBox.TextPositionToViewPoint(tp2);
                Rectangle r = new Rectangle(p0.X, p0.Y, p2.X - p0.X, this.textEditorBox.LineHeight);
                if (r.Contains(e.Location))
                {
                    tooltip = this.textEditorBox.ControlPanel.OnGetSimpleTooltip(tp1);
                }
            }
            if (this.lastTooltip != tooltip)
            {
                PopupTooltip(tp1, tooltip);
            }
            this.lastTooltip = tooltip;
        }

        internal bool HostBeforeMouseUp(object sender, MouseEventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseUp(object sender, MouseEventArgs e)
        {
        }

        internal bool HostBeforeMouseWheel(object sender, MouseEventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseWheel(object sender, MouseEventArgs e)
        {
            ClosePopupItems();
            CloseQuickInfoTooltip();
            PopupTooltip(new TextPosition(), null);
        }

        internal bool HostBeforeMouseLeave(object sender, EventArgs e)
        {
            return true;
        }

        internal void HostAfterMouseLeave(object sender, EventArgs e)
        {
            if (this.tooltip.Visible &&
                !new Rectangle(this.tooltip.Location, this.tooltip.Size).Contains(Control.MousePosition)
                )
            {
                this.lastTooltip = null;
                PopupTooltip(new TextPosition(0, 0), null);
            }
        }

        #endregion
    }
}

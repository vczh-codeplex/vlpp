using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Reflection;

namespace Developer.WinFormControls
{
    class TextEditorPopupList : ToolStripDropDown
    {
        class PopupList : Panel
        {
            private VScrollBar scrollBar = null;
            private ImageList imageList = null;

            private List<TextEditorPopupItem> items = null;
            private List<Bitmap> images = null;
            private TextEditorPopupItem selectedItem = null;
            private bool needToDisposeImages = false;
            private int maxItems = 0;

            private int itemWidth = 0;
            private int itemHeight = 0;
            private int itemTextOffset = 0;

            public PopupList()
            {
                this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
                this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

                this.scrollBar = new VScrollBar();
                this.scrollBar.Width = 16;
                this.scrollBar.ValueChanged += new EventHandler(scrollBar_ValueChanged);
                this.Controls.Add(this.scrollBar);

                this.imageList = new ImageList();
                this.imageList.ColorDepth = ColorDepth.Depth32Bit;
            }

            protected override void Dispose(bool disposing)
            {
                if (disposing)
                {
                    this.imageList.Dispose();
                }
                base.Dispose(disposing);
            }

            public List<TextEditorPopupItem> Items
            {
                get
                {
                    return this.items;
                }
            }

            public TextEditorPopupItem SelectedItem
            {
                get
                {
                    return this.selectedItem;
                }
                set
                {
                    int index = this.items.IndexOf(value);
                    if (index == -1)
                    {
                        this.selectedItem = null;
                    }
                    else
                    {
                        this.selectedItem = value;
                        if (index < this.scrollBar.Value)
                        {
                            this.scrollBar.Value = index;
                        }
                        else if (index - this.maxItems >= this.scrollBar.Value)
                        {
                            this.scrollBar.Value = index - this.maxItems + 1;
                        }
                    }
                    this.Refresh();
                }
            }

            public void FillList(List<TextEditorPopupItem> items, bool needToDisposeImages, int maxItems)
            {
                this.items = items;
                this.images = items.Select(i => i.Image).Distinct().ToList();
                this.selectedItem = null;
                this.needToDisposeImages = needToDisposeImages;
                this.maxItems = maxItems;

                SizeF[] sizes = null;
                using (Graphics g = Graphics.FromHwnd(this.Handle))
                {
                    sizes = this.items.Select(i => g.MeasureString(i.Text, this.Font)).ToArray();
                }

                this.itemHeight = 2 + (int)Math.Max(sizes.Max(s => s.Height), this.images.Max(i => i.Height));
                this.itemTextOffset = 2 + this.images.Max(i => i.Width);
                this.itemWidth = this.itemTextOffset + 2 + (int)sizes.Max(s => s.Width);
                if (items.Count <= maxItems)
                {
                    this.scrollBar.Visible = false;
                    this.scrollBar.Value = 0;
                    this.ClientSize = new Size(this.itemWidth, this.itemHeight * items.Count);
                }
                else
                {
                    this.scrollBar.Visible = true;
                    this.scrollBar.Value = 0;
                    this.scrollBar.Minimum = 0;
                    this.scrollBar.Maximum = items.Count - 1;
                    this.scrollBar.LargeChange = maxItems;
                    this.ClientSize = new Size(this.itemWidth + this.scrollBar.Width, this.itemHeight * maxItems);
                }

                this.imageList.Images.Clear();
                this.imageList.ImageSize = new Size(items.Max(i => i.Image.Width), items.Max(i => i.Image.Height));
                this.imageList.TransparentColor = items[0].Image.GetPixel(0, 0);
                this.imageList.Images.AddRange(this.images.ToArray());

                this.selectedItem = items[0];
            }

            public void UnfillList()
            {
                if (this.needToDisposeImages)
                {
                    foreach (Bitmap image in this.images)
                    {
                        image.Dispose();
                    }
                }
            }

            protected override void OnSizeChanged(EventArgs e)
            {
                base.OnSizeChanged(e);
                this.scrollBar.SetBounds(this.ClientSize.Width - this.scrollBar.Width, 0, this.scrollBar.Width, this.ClientSize.Height);
            }

            protected override void OnKeyDown(KeyEventArgs e)
            {
                base.OnKeyDown(e);
                switch (e.KeyCode)
                {
                    case Keys.Up:
                        this.SelectedItem = this.items[Math.Max(0, this.items.IndexOf(this.selectedItem) - 1)];
                        break;
                    case Keys.Down:
                        this.SelectedItem = this.items[Math.Min(this.items.Count - 1, this.items.IndexOf(this.selectedItem) + 1)];
                        break;
                    case Keys.Home:
                        this.SelectedItem = this.items[0];
                        break;
                    case Keys.End:
                        this.SelectedItem = this.items[this.items.Count - 1];
                        break;
                    default:
                        return;
                }
                this.Refresh();
            }

            protected override bool IsInputKey(Keys keyData)
            {
                return true;
            }

            protected override void OnMouseDown(MouseEventArgs e)
            {
                base.OnMouseDown(e);
                this.selectedItem = this.items[e.Y / this.itemHeight + this.scrollBar.Value];
                this.Refresh();
            }

            protected override void OnPaint(PaintEventArgs e)
            {
                base.OnPaint(e);
                int min = this.scrollBar.Value;
                int max = Math.Min(this.items.Count - 1, min + this.maxItems - 1);
                e.Graphics.FillRectangle(SystemBrushes.Window, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
                for (int i = min; i <= max; i++)
                {
                    int y = (i - min) * this.itemHeight;
                    TextEditorPopupItem item = this.items[i];
                    if (item == this.selectedItem)
                    {
                        e.Graphics.FillRectangle(SystemBrushes.Highlight, 0, y, this.itemWidth, this.itemHeight);
                    }

                    int ix = (this.itemTextOffset - this.imageList.ImageSize.Width) / 2;
                    int iy = (this.itemHeight - this.imageList.ImageSize.Height) / 2;
                    this.imageList.Draw(e.Graphics, ix, y + iy, this.images.IndexOf(item.Image));

                    int tx = this.itemTextOffset + 1;
                    int ty = (int)(this.itemHeight - e.Graphics.MeasureString(item.Text, this.Font).Height) / 2;
                    if (item == this.selectedItem)
                    {
                        e.Graphics.DrawString(item.Text, this.Font, SystemBrushes.HighlightText, tx, y + ty);
                    }
                    else
                    {
                        e.Graphics.DrawString(item.Text, this.Font, SystemBrushes.ControlText, tx, y + ty);
                    }
                }
            }

            private void scrollBar_ValueChanged(object sender, EventArgs e)
            {
                Refresh();
            }
        }

        private PopupList popupList = null;
        private ToolStripControlHost host = null;
        private TextEditorBox textEditorBox = null;
        private Control keyboardReceiver = null;

        private bool needToClose = false;
        private bool needToComplete = true;
        private string searchingKey = null;
        private string postfixKey = null;

        public bool PopupVisible { get; set; }

        public TextEditorPopupList(TextEditorBox textEditorBox, Control keyboardReceiver)
        {
            this.textEditorBox = textEditorBox;
            this.keyboardReceiver = keyboardReceiver;

            this.popupList = new PopupList();
            this.popupList.KeyDown += new KeyEventHandler(popupList_KeyDown);
            this.popupList.KeyPress += new KeyPressEventHandler(popupList_KeyPress);
            this.popupList.KeyUp += new KeyEventHandler(popupList_KeyUp);
            this.popupList.DoubleClick += new EventHandler(popupList_DoubleClick);

            this.host = new ToolStripControlHost(this.popupList);
            this.host.AutoSize = false;
            this.host.Margin = new Padding(0);
            this.Items.Add(host);
            this.Padding = new Padding(0);
        }

        public void Open(Control control, Point location, IEnumerable<TextEditorPopupItem> items, string searchingKey, bool needToDisposeImages, int maxItems)
        {
            if (items.Count() > 0)
            {
                this.popupList.FillList(items.OrderBy(i => i.Text.ToUpper()).ToList(), needToDisposeImages, maxItems);
                this.needToClose = false;
                this.needToComplete = true;
                this.searchingKey = searchingKey;
                this.postfixKey = "";
                LocateSearching();
                Show(control, location, ToolStripDropDownDirection.BelowRight);
                this.popupList.Focus();
            }
        }

        private void LocateSearching()
        {
            if (this.searchingKey == "")
            {
                this.popupList.SelectedItem = null;
            }
            else
            {
                TextEditorPopupItem item = this.popupList.Items.Where(i => i.Text.ToUpper().StartsWith(this.searchingKey.ToUpper())).FirstOrDefault();
                this.popupList.SelectedItem = item;
            }
        }

        private void SelectItem()
        {
            if (this.popupList.SelectedItem != null)
            {
                this.textEditorBox.ControlPanel.PopupListItemSelected(this.searchingKey, this.popupList.SelectedItem.Text, this.postfixKey);
            }
            else
            {
                this.textEditorBox.ControlPanel.PopupListItemSelected(this.searchingKey, this.searchingKey, this.postfixKey);
            }
        }

        protected override void OnOpened(EventArgs e)
        {
            base.OnOpened(e);
            this.PopupVisible = true;
        }

        protected override void OnClosed(ToolStripDropDownClosedEventArgs e)
        {
            base.OnClosed(e);
            this.PopupVisible = false;
            this.popupList.UnfillList();
        }

        private void popupList_KeyDown(object sender, KeyEventArgs e)
        {
            bool sendKeyDownToTextBox = false;
            if (this.textEditorBox.ControlPanel.IsPopupListKeyAcceptable(e))
            {
                sendKeyDownToTextBox = true;
                switch (e.KeyCode)
                {
                    case Keys.Back:
                        {
                            if (this.searchingKey == "")
                            {
                                this.needToClose = true;
                            }
                            else
                            {
                                this.searchingKey = this.searchingKey.Substring(0, this.searchingKey.Length - 1);
                            }
                            LocateSearching();
                            e.SuppressKeyPress = true;
                        }
                        break;
                }
            }
            else
            {
                if (e.Control || e.KeyCode == Keys.Back)
                {
                    e.SuppressKeyPress = true;
                }
                switch (e.KeyCode)
                {
                    case Keys.Up:
                    case Keys.Down:
                    case Keys.Home:
                    case Keys.End:
                        break;

                    case Keys.Enter:
                        e.SuppressKeyPress = true;
                        this.needToClose = true;
                        break;

                    case Keys.Left:
                    case Keys.Right:
                    case Keys.Escape:
                        e.SuppressKeyPress = true;
                        this.needToComplete = false;
                        this.needToClose = true;
                        sendKeyDownToTextBox = true;
                        break;

                    default:
                        sendKeyDownToTextBox = true;
                        this.needToClose = true;
                        break;
                }
            }
            if (sendKeyDownToTextBox)
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyDown", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
            }
        }

        private void popupList_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (this.needToClose)
            {
                this.postfixKey = e.KeyChar.ToString();
                Close();
                SelectItem();
            }
            else
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyPress", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
                this.searchingKey += e.KeyChar;
                LocateSearching();
                e.Handled = true;
            }
        }

        private void popupList_KeyUp(object sender, KeyEventArgs e)
        {
            if (this.textEditorBox.ControlPanel.IsPopupListKeyAcceptable(e))
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyUp", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
            }
            if (this.needToClose)
            {
                Close();
                if (this.needToComplete)
                {
                    SelectItem();
                }
            }
        }

        private void popupList_DoubleClick(object sender, EventArgs e)
        {
            Close();
            SelectItem();
        }
    }

    public class TextEditorPopupItem
    {
        public Bitmap Image { get; set; }
        public string Text { get; set; }
    }
}

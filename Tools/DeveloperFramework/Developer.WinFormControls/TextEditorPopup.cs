using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Reflection;

namespace Developer.WinFormControls
{
    public partial class TextEditorPopup : Form
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
                    Invalidate();
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

            public bool ProcessKey(KeyEventArgs e)
            {
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
                        return false;
                }
                Invalidate();
                return true;
            }

            protected override void OnSizeChanged(EventArgs e)
            {
                base.OnSizeChanged(e);
                this.scrollBar.SetBounds(this.ClientSize.Width - this.scrollBar.Width, 0, this.scrollBar.Width, this.ClientSize.Height);
            }

            protected override bool IsInputKey(Keys keyData)
            {
                return true;
            }

            protected override void OnMouseDown(MouseEventArgs e)
            {
                base.OnMouseDown(e);
                this.selectedItem = this.items[e.Y / this.itemHeight + this.scrollBar.Value];
                Invalidate();
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
                Invalidate();
            }
        }

        private PopupList popupList = null;
        private TextEditorBox textEditorBox = null;
        private Control keyboardReceiver = null;

        private string searchingKey = null;

        public TextEditorPopup(TextEditorBox textEditorBox, Control keyboardReceiver)
        {
            InitializeComponent();
            this.textEditorBox = textEditorBox;
            this.keyboardReceiver = keyboardReceiver;

            this.popupList = new PopupList();
            this.popupList.Location = new Point(0, 0);
            this.Controls.Add(this.popupList);
            this.popupList.DoubleClick += new EventHandler(popupList_DoubleClick);
        }

        public void Show(Control control, Point locationTop, Point locationBottom, IEnumerable<TextEditorPopupItem> items, string searchingKey, bool needToDisposeImages, int maxItems)
        {
            if (items.Count() > 0)
            {
                this.popupList.FillList(items.OrderBy(i => i.Text.ToUpper()).ToList(), needToDisposeImages, maxItems);
                this.searchingKey = searchingKey;
                LocateSearching();
                this.ClientSize = this.popupList.Size;

                Point location;
                Size textSize = this.Size;
                Screen screen = Screen.FromControl(control);
                if (locationBottom.Y + textSize.Height < screen.WorkingArea.Bottom)
                {
                    location = locationBottom;
                }
                else if (locationTop.Y - textSize.Height >= screen.WorkingArea.Top)
                {
                    location = new Point(locationTop.X, locationTop.Y - textSize.Height);
                }
                else
                {
                    location = new Point(locationTop.X, 0);
                }
                if (location.X + textSize.Width > screen.WorkingArea.Right)
                {
                    if (location.X - textSize.Width >= screen.WorkingArea.Left)
                    {
                        location.X -= textSize.Width;
                    }
                    else
                    {
                        location.X = 0;
                    }
                }
                this.SetBounds(location.X, location.Y, textSize.Width, textSize.Height);
                if (this.Visible)
                {
                    Refresh();
                }
                else
                {
                    ShowWindow(this.Handle, 4);//SW_SHOWNOACTIVATE
                }
                SetWindowPos(this.Handle, -1, 0, 0, 0, 0, 19);//HWND_TOPMOST, SWP_NOMOVE | SWP_MOSIZE | SWP_NOACTIVATE
                this.SetBounds(location.X, location.Y, textSize.Width, textSize.Height);
            }
        }

        public bool ProcessKey(KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (this.popupList.SelectedItem != null)
                {
                    SelectItem();
                    Hide();
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return this.popupList.ProcessKey(e);
            }
        }

        public bool ProcessChar(char c)
        {
            this.searchingKey += c;
            LocateSearching();
            return true;
        }

        public bool ProcessBackspace()
        {
            if (this.searchingKey != "")
            {
                this.searchingKey = this.searchingKey.Substring(0, this.searchingKey.Length - 1);
                return true;
            }
            else
            {
                return false;
            }
        }

        public void SelectItem()
        {
            if (this.popupList.SelectedItem != null)
            {
                this.textEditorBox.ControlPanel.PopupListItemSelected(this.searchingKey, this.popupList.SelectedItem.Text);
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

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            e.Cancel = true;
            Hide();
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            this.keyboardReceiver.GetType().GetMethod("OnKeyDown", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);
            this.keyboardReceiver.GetType().GetMethod("OnKeyPress", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);
            this.keyboardReceiver.GetType().GetMethod("OnKeyUp", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
        }

        private void popupList_DoubleClick(object sender, EventArgs e)
        {
            SelectItem();
            Hide();
        }

        [DllImport("User32.dll")]
        private static extern int ShowWindow(IntPtr hwnd, int flag);

        [DllImport("User32.dll")]
        private static extern int SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
    }
}

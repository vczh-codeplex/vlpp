﻿using System;
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
    public partial class TextEditorPopup : TextEditorPopupBase
    {
        class PopupList : Panel
        {
            private const TextFormatFlags Flags = TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix;
            private const string SnippetTail = " [TAB]";

            private VScrollBar scrollBar = null;
            private ImageList imageList = null;
            private Size[] cachedLetterSizes = null;

            private List<TextEditorPopupItem> items = null;
            private List<Bitmap> images = null;
            private TextEditorPopupItem previousSelectedItem = null;
            private TextEditorPopupItem selectedItem = null;
            private bool needToDisposeImages = false;
            private int maxItems = 0;

            private int itemWidth = 0;
            private int itemHeight = 0;
            private int itemTextOffset = 0;

            public PopupList()
            {
                this.DoubleBuffered = true;
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
                    if (value != null)
                    {
                        this.previousSelectedItem = value;
                    }
                    Invalidate();
                }
            }

            public void FillList(List<TextEditorPopupItem> items, bool needToDisposeImages, int maxItems)
            {
                UnfillList();
                this.items = items;
                this.images = items.Select(i => i.Image).Distinct().ToList();
                this.selectedItem = null;
                this.previousSelectedItem = null;
                this.needToDisposeImages = needToDisposeImages;
                this.maxItems = maxItems;

                {
                    bool nonLetter = false;
                    foreach (var item in items)
                    {
                        foreach (var c in item.Text)
                        {
                            if (!('A' <= c && c <= 'Z' || 'a' <= c || c <= 'z' || '0' <= c || c <= '9' || c == '_'))
                            {
                                nonLetter = true;
                                break;
                            }
                        }
                        if (nonLetter)
                        {
                            break;
                        }
                    }
                    string[] itemTexts = this.items.Select(i => i.Text + (i.Snippet == null ? "" : SnippetTail)).ToArray();

                    int maxHeight = 0;
                    int maxWidth = 0;
                    if (nonLetter)
                    {
                        string longestText = itemTexts.Aggregate("", (a, b) => (a.Length > b.Length ? a : b));
                        Size sizeChar = TextRenderer.MeasureText("我", this.Font);
                        Size longestSize = TextRenderer.MeasureText(longestText, this.Font);
                        maxHeight = Math.Max(sizeChar.Height, longestSize.Height);
                        maxWidth = longestText.Length * sizeChar.Width;
                    }
                    else
                    {
                        if (this.cachedLetterSizes == null)
                        {
                            string letters = "abcdefghijklmnopqrstuvwxyz";
                            letters = letters + letters.ToUpper() + "_0123456789";
                            int count = (int)letters.Max();
                            this.cachedLetterSizes = new Size[count + 1];
                            using (Graphics g = Graphics.FromHwnd(this.Handle))
                            {
                                foreach (var c in letters)
                                {
                                    this.cachedLetterSizes[(int)c] = TextRenderer.MeasureText(g, c.ToString(), this.Font, new Size(0, 0), TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
                                }
                            }
                        }
                        Size[] sizes = itemTexts.Select(i =>
                            i.Select(c => this.cachedLetterSizes[(int)c])
                            .Aggregate(new Size(0, 0), (a, b) => new Size(a.Width + b.Width, Math.Max(a.Height, b.Height)))
                            ).ToArray();
                        maxHeight = sizes.Select(s => s.Height).Max();
                        maxWidth = (int)(sizes.Select(s => s.Width).Max() * 1.2);
                    }

                    this.itemHeight = 2 + Math.Max(maxHeight, this.images.Max(i => i.Height));
                    this.itemTextOffset = 2 + this.images.Max(i => i.Width);
                    this.itemWidth = this.itemTextOffset + 2 + maxWidth;
                }

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
                if (this.images != null)
                {
                    if (this.needToDisposeImages)
                    {
                        foreach (Bitmap image in this.images)
                        {
                            image.Dispose();
                        }
                    }
                    this.images.Clear();
                }
            }

            public bool ProcessKey(KeyEventArgs e)
            {
                if (this.selectedItem == null)
                {
                    switch (e.KeyCode)
                    {
                        case Keys.Up:
                        case Keys.Down:
                            this.SelectedItem = this.previousSelectedItem ?? this.items[0];
                            Invalidate();
                            return true;
                    }
                }
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

                    string tail = item.Snippet == null ? "" : SnippetTail;
                    if (item == this.selectedItem)
                    {
                        TextRenderer.DrawText(e.Graphics, item.Text + tail, this.Font, new Point(tx, y + ty), SystemColors.HighlightText, Flags);
                    }
                    else
                    {
                        if (item.PureSnippet)
                        {
                            TextRenderer.DrawText(e.Graphics, item.Text + tail, this.Font, new Point(tx, y + ty), SystemColors.GrayText, Flags);
                        }
                        else
                        {
                            TextRenderer.DrawText(e.Graphics, item.Text, this.Font, new Point(tx, y + ty), SystemColors.ControlText, Flags);
                            int offset = TextRenderer.MeasureText(e.Graphics, item.Text, this.Font, new Size(0, 0), Flags).Width;
                            TextRenderer.DrawText(e.Graphics, tail, this.Font, new Point(tx + offset, y + ty), SystemColors.GrayText, Flags);
                        }
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

        public void Show(Control control, Point locationTop, Point locationBottom, IEnumerable<TextEditorPopupItem> items, IEnumerable<SnippetContent> snippets, Bitmap snippetImage, string searchingKey, bool needToDisposeImages, int maxItems)
        {
            Dictionary<string, TextEditorPopupItem> itemMap = new Dictionary<string, TextEditorPopupItem>();
            foreach (var item in items)
            {
                item.Snippet = null;
                item.PureSnippet = false;
                itemMap[item.Text] = item;
            }
            foreach (var snippet in snippets)
            {
                TextEditorPopupItem item = null;
                if (!itemMap.TryGetValue(snippet.Name, out item))
                {
                    item = new TextEditorPopupItem()
                    {
                        Image = snippetImage,
                        Text = snippet.Name,
                        PureSnippet = true
                    };
                    itemMap[item.Text] = item;
                }
                item.Snippet = snippet;
            }
            if (snippets.Count() == 0 && snippetImage != null && needToDisposeImages)
            {
                snippetImage.Dispose();
            }
            if (itemMap.Count > 0)
            {
                this.popupList.FillList(itemMap.Values.OrderBy(i => i.Text.ToUpper()).ToList(), needToDisposeImages, maxItems);
                this.searchingKey = searchingKey;
                LocateSearching();
                this.ClientSize = this.popupList.Size;
                Show(control, locationTop, locationBottom);
            }
        }

        public bool ProcessKey(KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Enter:
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
                case Keys.Tab:
                    {
                        TextEditorPopupItem selectedItem = this.popupList.SelectedItem;
                        if (selectedItem != null && selectedItem.Snippet != null)
                        {
                            SelectItem("");
                            Hide();
                            this.textEditorBox.UIExtensions.InsertSnippet(selectedItem.Snippet);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                default:
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

        public TextEditorPopupItem[] Items
        {
            get
            {
                return this.popupList.Items.ToArray();
            }
        }

        public TextEditorPopupItem SelectedItem
        {
            get
            {
                return this.popupList.SelectedItem;
            }
        }

        public void SelectItem(string text = null)
        {
            if (this.popupList.SelectedItem != null)
            {
                this.textEditorBox.ControlPanel.PopupListItemSelected(this.searchingKey, text ?? this.popupList.SelectedItem.Text);
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
                List<TextEditorPopupItem> items = this.popupList.Items;
                int first = 0;
                int last = items.Count - 1;
                TextEditorPopupItem item = null;
                while (first <= last)
                {
                    int mid = (first + last) / 2;
                    string text = items[mid].Text;
                    if (text.StartsWith(this.searchingKey, StringComparison.InvariantCultureIgnoreCase))
                    {
                        item = items[mid];
                    }
                    if (string.Compare(this.searchingKey, text, StringComparison.InvariantCultureIgnoreCase) <= 0)
                    {
                        last = mid - 1;
                    }
                    else
                    {
                        first = mid + 1;
                    }
                }
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

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            if (!this.Visible)
            {
                this.popupList.UnfillList();
            }
        }

        private void popupList_DoubleClick(object sender, EventArgs e)
        {
            SelectItem();
            Hide();
        }
    }

    public class TextEditorPopupItem
    {
        public Bitmap Image { get; set; }
        public string Text { get; set; }

        public SnippetContent Snippet { get; set; }
        public bool PureSnippet { get; set; }
    }
}

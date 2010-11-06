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
        private ImageList imageList = null;
        private ListView listView = null;
        private ToolStripControlHost host = null;
        private TextEditorBox textEditorBox = null;
        private Control keyboardReceiver = null;

        private bool needToClose = false;
        private string searchingKey = null;
        private string postfixKey = null;

        public bool PopupVisible { get; set; }

        public TextEditorPopupList(TextEditorBox textEditorBox, Control keyboardReceiver)
        {
            this.textEditorBox = textEditorBox;
            this.keyboardReceiver = keyboardReceiver;

            this.imageList = new ImageList();
            this.imageList.ColorDepth = ColorDepth.Depth32Bit;
            this.imageList.ImageSize = new Size(16, 16);
            this.listView = new ListView();
            this.listView.FullRowSelect = true;
            this.listView.SmallImageList = this.imageList;
            this.listView.View = View.List;
            this.listView.MultiSelect = false;
            this.listView.KeyDown += new KeyEventHandler(listView_KeyDown);
            this.listView.KeyPress += new KeyPressEventHandler(listView_KeyPress);
            this.listView.KeyUp += new KeyEventHandler(listView_KeyUp);
            this.listView.DoubleClick += new EventHandler(listView_DoubleClick);

            this.host = new ToolStripControlHost(this.listView);
            this.host.AutoSize = false;
            this.host.Margin = new Padding(0);
            this.Items.Add(host);
            this.Padding = new Padding(0);
        }

        public void Open(Control control, Point location, IEnumerable<TextEditorPopupItem> items, int maxItems)
        {
            if (items.Count() > 0)
            {
                this.listView.Items.Clear();
                this.imageList.Images.Clear();
                List<Bitmap> images = items.Select(i => i.Image).Where(i => i != null).Distinct().ToList();
                if (images.Count > 0)
                {
                    this.imageList.TransparentColor = images[0].GetPixel(0, 0);
                }
                this.imageList.Images.AddRange(images.ToArray());
                foreach (var item in items.OrderBy(i => i.Text.ToUpper()))
                {
                    ListViewItem listViewItem = new ListViewItem();
                    listViewItem.ImageIndex = images.IndexOf(item.Image);
                    listViewItem.Text = item.Text;
                    this.listView.Items.Add(listViewItem);
                }

                this.listView.Size = new System.Drawing.Size(100, 100);
                Show(control, location, ToolStripDropDownDirection.BelowRight);
                this.needToClose = false;
                this.searchingKey = "";
                this.postfixKey = "";
                this.listView.Focus();
                this.listView.SelectedIndices.Add(0);
            }
        }

        private void LocateSearching()
        {
            ListViewItem item = this.listView.Items.Cast<ListViewItem>().Where(i => i.Text.ToUpper().StartsWith(this.searchingKey.ToUpper())).FirstOrDefault();
            this.listView.SelectedIndices.Clear();
            if (item != null)
            {
                this.listView.SelectedIndices.Add(item.Index);
                this.listView.EnsureVisible(item.Index);
            }
        }

        private void SelectItem()
        {
            if (this.listView.SelectedItems.Count == 1)
            {
                this.textEditorBox.ControlPanel.PopupListItemSelected(this.searchingKey, this.listView.SelectedItems[0].Text + this.postfixKey);
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.listView.SmallImageList = null;
                this.imageList.Dispose();
            }
            base.Dispose(disposing);
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
        }

        private void listView_KeyDown(object sender, KeyEventArgs e)
        {
            if (this.textEditorBox.ControlPanel.IsPopupListKeyAcceptable(e))
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyDown", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
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
                                LocateSearching();
                            }
                            e.SuppressKeyPress = true;
                        }
                        break;
                }
            }
            else
            {
                switch (e.KeyCode)
                {
                    case Keys.Up:
                    case Keys.Down:
                    case Keys.Home:
                    case Keys.End:
                        break;

                    case Keys.Delete:
                    case Keys.Enter:
                    case Keys.Escape:
                        e.SuppressKeyPress = true;
                        this.needToClose = true;
                        break;

                    default:
                        this.needToClose = true;
                        break;
                }
            }
        }

        private void listView_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (this.needToClose)
            {
                this.postfixKey = e.KeyChar.ToString();
                SelectItem();
                Close();
            }
            else
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyPress", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
                this.searchingKey += e.KeyChar;
                LocateSearching();
                e.Handled = true;
            }
        }

        private void listView_KeyUp(object sender, KeyEventArgs e)
        {
            if (this.textEditorBox.ControlPanel.IsPopupListKeyAcceptable(e))
            {
                this.keyboardReceiver.GetType().GetMethod("OnKeyUp", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(this.keyboardReceiver, new object[] { e });
            }
            if (this.needToClose)
            {
                SelectItem();
                Close();
            }
        }

        private void listView_DoubleClick(object sender, EventArgs e)
        {
            SelectItem();
            Close();
        }
    }

    public class TextEditorPopupItem
    {
        public Bitmap Image { get; set; }
        public string Text { get; set; }
    }
}

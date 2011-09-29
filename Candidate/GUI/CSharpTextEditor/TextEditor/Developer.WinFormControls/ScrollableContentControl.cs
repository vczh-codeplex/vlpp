using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Developer.WinFormControls
{
    public partial class ScrollableContentControl : UserControl
    {
        private IScrollableContent scrollableContent;
        private bool supressScrollBarEvents = false;

        public IScrollableContent ScrollableContent
        {
            get
            {
                return this.scrollableContent;
            }
            set
            {
                if (this.scrollableContent != null)
                {
                    this.scrollableContent.ContentSizeChanged -= new EventHandler(scrollableContent_ContentSizeChanged);
                    this.scrollableContent.ContentChanged -= new EventHandler(scrollableContent_ContentChanged);
                }
                this.scrollableContent = value;
                if (this.scrollableContent != null)
                {
                    this.scrollableContent.Initialize(imeEnabledPanel, this);
                    this.scrollableContent.ContentSizeChanged += new EventHandler(scrollableContent_ContentSizeChanged);
                    this.scrollableContent.ContentChanged += new EventHandler(scrollableContent_ContentChanged);
                    this.scrollableContent.ControlSize = imeEnabledPanel.Size;
                }
            }
        }

        #region Scroll Bar Rearrangement

        private int GetScrollBarValue(ScrollBar scrollBar)
        {
            return scrollBar.Enabled ? scrollBar.Value : 0;
        }

        private int CurrentXValue
        {
            get
            {
                return GetScrollBarValue(horizontalScrollBar);
            }
        }

        private int CurrentYValue
        {
            get
            {
                return GetScrollBarValue(verticalScrollBar);
            }
        }

        private void SetScrollBar(ScrollBar scrollBar, int contentSize, int visibleSize)
        {
            int visiblePosition = GetScrollBarValue(scrollBar);
            if (visibleSize >= contentSize)
            {
                scrollBar.Enabled = false;
            }
            else
            {
                int page = Math.Min(contentSize, visibleSize);
                int value =
                    visiblePosition < 0 ? 0 :
                    visiblePosition > contentSize - page ? contentSize - page :
                    visiblePosition;
                scrollBar.Maximum = Math.Max(1, contentSize - 1);
                scrollBar.LargeChange = Math.Max(1, page);
                scrollBar.SmallChange = Math.Max(1, scrollBar.LargeChange / 40);
                scrollBar.Value = value;
                scrollBar.Enabled = true;
            }
        }

        private void ResetScrollBar()
        {
            if (this.scrollableContent != null)
            {
                this.supressScrollBarEvents = true;
                Size s = this.scrollableContent.ContentSize;
                SetScrollBar(horizontalScrollBar, s.Width, imeEnabledPanel.Width);
                SetScrollBar(verticalScrollBar, s.Height, imeEnabledPanel.Height);
                this.supressScrollBarEvents = false;
                UpdateVisibleArea();
                imeEnabledPanel.Refresh();
            }
        }

        private void UpdateVisibleArea(int horizontalValue, int verticalValue)
        {
            if (this.scrollableContent != null && !this.supressScrollBarEvents)
            {
                Size s = this.scrollableContent.ContentSize;
                int x = horizontalValue;
                int y = verticalValue;
                int w = Math.Min(s.Width, x + imeEnabledPanel.Width) - x;
                int h = Math.Min(s.Height, y + imeEnabledPanel.Height) - y;
                this.scrollableContent.VisibleArea = new Rectangle(x, y, w, h);
            }
        }

        private void UpdateVisibleArea()
        {
            UpdateVisibleArea(CurrentXValue, CurrentYValue);
        }

        #endregion

        public ScrollableContentControl()
        {
            InitializeComponent();
            imeEnabledPanel.ParentControl = this;
        }

        public virtual Point ViewPosition
        {
            get
            {
                return new Point(CurrentXValue, CurrentYValue);
            }
        }

        public virtual Point CaretPosition
        {
            get
            {
                return this.ViewPosition;
            }
        }

        private void verticalScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            UpdateVisibleArea(CurrentXValue, e.NewValue);
        }

        private void horizontalScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            UpdateVisibleArea(e.NewValue, CurrentYValue);
        }

        private void scrollableContent_ContentSizeChanged(object sender, EventArgs e)
        {
            ResetScrollBar();
        }

        private void scrollableContent_ContentChanged(object sender, EventArgs e)
        {
            imeEnabledPanel.Refresh();
        }

        private void imeEnabledPanel_SizeChanged(object sender, EventArgs e)
        {
            if (this.scrollableContent != null)
            {
                this.scrollableContent.ControlSize = imeEnabledPanel.Size;
            }
            ResetScrollBar();
        }
    }

    public interface IScrollableContent
    {
        Size ContentSize { get; }
        Size ControlSize { get; set; }
        Rectangle VisibleArea { get; set; }
        void Initialize(Control host, ScrollableContentControl container);
        void RenderContent(Graphics g);

        event EventHandler ContentSizeChanged;
        event EventHandler ContentChanged;
    }
}

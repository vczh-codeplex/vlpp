using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using TokenizerBuilder.Shape;
using CodeBoxControl;

namespace TokenizerBuilder
{
    public partial class TokenizerEditorBox : ScrollableContentControl
    {
        public ShapeManager Manager { get; private set; }

        public TokenizerEditorBox()
            : base(new Content())
        {
            InitializeComponent();
            this.Manager = new ShapeManager();
        }

        private class Content : IScrollableContent
        {
            private TokenizerEditorBox editor;
            private Control host;

            public void Initialize(Control host, ScrollableContentControl control)
            {
                this.editor = (TokenizerEditorBox)control;
                this.host = host;
            }

            public void RenderContent(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds)
            {
                g.FillRectangle(Brushes.White, viewAreaBounds);
                this.editor.Manager.Draw(g, this.editor.Font, new Size(-viewVisibleBounds.X, -viewVisibleBounds.Y));
            }
        }
    }
}

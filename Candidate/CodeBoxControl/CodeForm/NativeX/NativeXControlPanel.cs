using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Windows.Forms;

namespace CodeForm
{
    class NativeXControlPanel
        : ITextEditorControlPanel
        , INativeXAnalyzingResultReceiver
        , IDisposable
    {
        private ITextEditorControlPanelCallBack callback = null;
        private NativeXAnalyzingResult analyzingResult = null;
        private NativeXCodeAnalyzer analyzer = null;

        public int Width
        {
            get
            {
                return 0;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.callback = callback;
            this.analyzer = new NativeXCodeAnalyzer(this);
        }

        public void OnEdit(CodeBoxControl.Core.TextPosition start, CodeBoxControl.Core.TextPosition oldEnd, CodeBoxControl.Core.TextPosition newEnd)
        {
            this.analyzer.Analyze(this.callback.TextEditorBox.Text);
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            return false;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
        }

        public void DrawLineBackground(System.Drawing.Graphics g, int lineIndex, System.Drawing.Rectangle backgroundArea)
        {
        }

        public void DrawLineForeground(System.Drawing.Graphics g, int lineIndex, System.Drawing.Rectangle backgroundArea)
        {
        }

        public void DrawControlPanel(System.Drawing.Graphics g, int lineIndex, System.Drawing.Rectangle controlPanelArea)
        {
        }

        public void DrawControlPanelBackground(System.Drawing.Graphics g, System.Drawing.Rectangle backgroundArea)
        {
        }

        public void OnMouseDown(int lineIndex, System.Drawing.Rectangle controlPanelArea, System.Drawing.Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public void OnMouseMove(int lineIndex, System.Drawing.Rectangle controlPanelArea, System.Drawing.Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public void OnMouseUp(int lineIndex, System.Drawing.Rectangle controlPanelArea, System.Drawing.Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public void Receive(NativeXAnalyzingResult result)
        {
            this.callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.analyzingResult = result;
            }));
        }

        public void Dispose()
        {
            this.analyzer.Dispose();
        }
    }
}

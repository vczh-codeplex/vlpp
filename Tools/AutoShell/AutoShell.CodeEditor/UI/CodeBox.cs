using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Reflection;

namespace AutoShell.UI
{
    public class CodeBox : RichTextBox
    {
        private bool updating = false;
        private CodeEditorWindowPane pane = null;
        private ICodeFormatter formatter = null;

        #region Formatting

        private void UpdateRtf(int startChar, int endChar)
        {
            this.Select(startChar, (endChar - startChar) + 1);
            this.SelectedRtf = TranslateToRtf(this.Text, startChar, endChar);
        }

        private string TranslateToRtf(string code, int startChar, int endChar)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append(this.formatter.Manager.Header);

            int start = 0;
            int length = 0;
            int format = 0;
            int codeLength = code.Length;
            this.formatter.BeginFormat(code);

            while (start < codeLength)
            {
                if (this.formatter.GetToken(out format, out length))
                {
                    int end = start + length - 1;
                    if (end >= startChar)
                    {
                        CodeFormat codeFormat = this.formatter.Manager[format];
                        int min = start > startChar ? start : startChar;
                        int max = end < endChar ? end : endChar;
                        code.Substring(min, max - min + 1).ToRtf(builder, codeFormat.Header, codeFormat.Tail);
                        if (end >= endChar)
                        {
                            break;
                        }
                    }
                    start = end + 1;
                }
                else
                {
                    break;
                }
            }

            this.formatter.EndFormat();
            builder.AppendLine(@"}");
            return builder.ToString();
        }

        #endregion

        public CodeBox(CodeEditorWindowPane pane, ICodeFormatter formatter)
        {
            this.AcceptsTab = true;
            this.AutoWordSelection = false;
            this.HideSelection = false;
            this.ShortcutsEnabled = false;
            this.ScrollBars = RichTextBoxScrollBars.ForcedBoth;
            this.WordWrap = false;
            this.pane = pane;
            this.formatter = formatter;
        }

        protected override bool ProcessCmdKey(ref Message m, Keys keyData)
        {
            if ((keyData & Keys.Control) == Keys.Control)
            {
                MethodInfo info = this.Parent.GetType().GetMethod("ProcessCmdKey", BindingFlags.Instance | BindingFlags.NonPublic);
                return (bool)info.Invoke(this.Parent, new object[] { m, keyData }) || true;
            }
            else
            {
                return base.ProcessCmdKey(ref m, keyData);
            }
        }

        #region Scrolling Helper

        protected override void OnHScroll(EventArgs e)
        {
            if (!this.updating)
            {
                this.pane.Formatting = true;
                base.OnHScroll(e);
                HighlightCode();
                this.pane.Formatting = false;
            }
        }

        protected override void OnVScroll(EventArgs e)
        {
            if (!this.updating)
            {
                this.pane.Formatting = true;
                base.OnVScroll(e);
                HighlightCode();
                this.pane.Formatting = false;
            }
        }

        protected override void OnTextChanged(EventArgs e)
        {
            if (!this.updating)
            {
                base.OnTextChanged(e);
                this.pane.Formatting = true;
                HighlightCode();
                this.pane.Formatting = false;
            }
        }

        protected override void OnResize(EventArgs e)
        {
            if (!this.updating)
            {
                this.pane.Formatting = true;
                base.OnResize(e);
                HighlightCode();
                this.pane.Formatting = false;
            }
        }

        private void HighlightCode()
        {
            this.updating = true;
            RtfNativeMethod.BeginPaint(this);
            int start = this.SelectionStart;
            Point pos = RtfNativeMethod.GetScrollPos(this);

            int startChar = this.GetCharIndexFromPosition(new Point(0, 0));
            int endChar = this.GetCharIndexFromPosition(new Point(this.ClientSize));
            UpdateRtf(startChar, endChar);

            this.SelectionLength = 0;
            this.SelectionStart = start;
            RtfNativeMethod.SetScrollPos(this, pos);
            RtfNativeMethod.EndPaint(this);
            Refresh();
            this.updating = false;
        }

        #endregion
    }

    internal class RtfNativeMethod
    {
        private struct ScrollInfo
        {
            public uint cbSize;
            public uint fMask;
            public int nMin;
            public int nMax;
            public uint nPage;
            public int nPos;
            public int nTrackPos;
        }

        [DllImport("user32")]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, IntPtr lParam);

        [DllImport("user32")]
        private static extern int GetScrollPos(IntPtr hwnd, int bar);

        [DllImport("user32")]
        private static extern int SetScrollPos(IntPtr hwnd, int bar, int pos, bool redraw);

        [DllImport("user32")]
        private static extern int GetScrollInfo(IntPtr hwnd, int bar, ref ScrollInfo info);

        private const int WM_SETREDRAW = 0xB;
        private const int WM_HSCROLL = 0x114;
        private const int WM_VSCROLL = 0x115;
        private const int SB_HORZ = 0;
        private const int SB_VERT = 1;
        private const int SB_THUMBPOSITION = 4;
        private const int SIF_RANGE = 0x0001;
        private const int SIF_PAGE = 0x0002;
        private const int SIF_POS = 0x0004;
        private const int SIF_DISABLENOSCROLL = 0x0008;
        private const int SIF_TRACKPOS = 0x0010;
        private const int SIF_ALL = (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS);

        public static void BeginPaint(Control control)
        {
            SendMessage(control.Handle, WM_SETREDRAW, 0, IntPtr.Zero);
        }

        public static void EndPaint(Control control)
        {
            SendMessage(control.Handle, WM_SETREDRAW, 1, IntPtr.Zero);
        }

        public static Point GetScrollPos(Control control)
        {
            Point point = new Point();
            ScrollInfo info = new ScrollInfo();
            info.cbSize = (uint)Marshal.SizeOf(typeof(ScrollInfo));
            info.fMask = (uint)SIF_ALL;
            GetScrollInfo(control.Handle, SB_HORZ, ref info);
            point.X = info.nTrackPos;
            GetScrollInfo(control.Handle, SB_VERT, ref info);
            point.Y = info.nTrackPos;
            return point;
        }

        public static void SetScrollPos(Control control, Point point)
        {
            SetScrollPos(control.Handle, SB_HORZ, point.X, true);
            SetScrollPos(control.Handle, SB_VERT, point.Y, true);
            SendMessage(control.Handle, WM_HSCROLL, SB_THUMBPOSITION + 0x10000 * point.X, IntPtr.Zero);
            SendMessage(control.Handle, WM_VSCROLL, SB_THUMBPOSITION + 0x10000 * point.Y, IntPtr.Zero);
        }
    }
}

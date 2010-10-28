using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

namespace Developer.RibbonFramework
{
    public class WindowsAPI
    {
        #region Window Styles

        public const int GWL_WNDPROC = (-4);
        public const int GWL_HINSTANCE = (-6);
        public const int GWL_HWNDPARENT = (-8);
        public const int GWL_STYLE = (-16);
        public const int GWL_EXSTYLE = (-20);
        public const int GWL_USERDATA = (-21);
        public const int GWL_ID = (-12);

        public const uint WS_OVERLAPPED = 0x00000000;
        public const uint WS_POPUP = 0x80000000;
        public const uint WS_CHILD = 0x40000000;
        public const uint WS_MINIMIZE = 0x20000000;
        public const uint WS_VISIBLE = 0x10000000;
        public const uint WS_DISABLED = 0x08000000;
        public const uint WS_CLIPSIBLINGS = 0x04000000;
        public const uint WS_CLIPCHILDREN = 0x02000000;
        public const uint WS_MAXIMIZE = 0x01000000;
        public const uint WS_CAPTION = 0x00C00000;     /* WS_BORDER | WS_DLGFRAME  */
        public const uint WS_BORDER = 0x00800000;
        public const uint WS_DLGFRAME = 0x00400000;
        public const uint WS_VSCROLL = 0x00200000;
        public const uint WS_HSCROLL = 0x00100000;
        public const uint WS_SYSMENU = 0x00080000;
        public const uint WS_THICKFRAME = 0x00040000;
        public const uint WS_GROUP = 0x00020000;
        public const uint WS_TABSTOP = 0x00010000;
        public const uint WS_MINIMIZEBOX = 0x00020000;
        public const uint WS_MAXIMIZEBOX = 0x00010000;

        #endregion

        #region Window Non Client Messages

        public const int WM_NCMOUSEMOVE = 0x00A0;
        public const int WM_NCLBUTTONDOWN = 0x00A1;
        public const int WM_NCLBUTTONUP = 0x00A2;
        public const int WM_NCLBUTTONDBLCLK = 0x00A3;
        public const int WM_NCRBUTTONDOWN = 0x00A4;
        public const int WM_NCRBUTTONUP = 0x00A5;
        public const int WM_NCRBUTTONDBLCLK = 0x00A6;
        public const int WM_NCMBUTTONDOWN = 0x00A7;
        public const int WM_NCMBUTTONUP = 0x00A8;
        public const int WM_NCMBUTTONDBLCLK = 0x00A9;

        public const int WM_NCCREATE = 0x0081;
        public const int WM_NCDESTROY = 0x0082;
        public const int WM_NCCALCSIZE = 0x0083;
        public const int WM_NCHITTEST = 0x0084;
        public const int WM_NCPAINT = 0x0085;
        public const int WM_NCACTIVATE = 0x0086;

        #endregion

        #region Window Client Messages

        public const int WM_SIZE = 0x0005;
        public const int WM_PAINT = 0x000F;
        public const int WM_ERASEBKGND = 0x0014;
        public const int WM_WINDOWPOSCHANGED = 0x0047;
        public const int WM_EXITSIZEMOVE = 0x0232;

        #endregion

        #region Hit Test

        public const int HTERROR = (-2);
        public const int HTTRANSPARENT = (-1);
        public const int HTNOWHERE = 0;
        public const int HTCLIENT = 1;
        public const int HTCAPTION = 2;
        public const int HTSYSMENU = 3;
        public const int HTGROWBOX = 4;
        public const int HTSIZE = HTGROWBOX;
        public const int HTMENU = 5;
        public const int HTHSCROLL = 6;
        public const int HTVSCROLL = 7;
        public const int HTMINBUTTON = 8;
        public const int HTMAXBUTTON = 9;
        public const int HTLEFT = 10;
        public const int HTRIGHT = 11;
        public const int HTTOP = 12;
        public const int HTTOPLEFT = 13;
        public const int HTTOPRIGHT = 14;
        public const int HTBOTTOM = 15;
        public const int HTBOTTOMLEFT = 16;
        public const int HTBOTTOMRIGHT = 17;
        public const int HTBORDER = 18;
        public const int HTREDUCE = HTMINBUTTON;
        public const int HTZOOM = HTMAXBUTTON;
        public const int HTSIZEFIRST = HTLEFT;
        public const int HTSIZELAST = HTBOTTOMRIGHT;

        #endregion

        #region WM_NCCALCSIZE Results

        public const int WVR_ALIGNTOP = 0x0010;
        public const int WVR_ALIGNLEFT = 0x0020;
        public const int WVR_ALIGNBOTTOM = 0x0040;
        public const int WVR_ALIGNRIGHT = 0x0080;
        public const int WVR_HREDRAW = 0x0100;
        public const int WVR_VREDRAW = 0x0200;
        public const int WVR_REDRAW = (WVR_HREDRAW | WVR_VREDRAW);
        public const int WVR_VALIDRECTS = 0x0400;

        #endregion

        #region APIs

        [DllImport("DwmApi.dll")]
        public static extern bool DwmDefWindowProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, out IntPtr plResult);

        [DllImport("DwmApi.dll")]
        public static extern int DwmExtendFrameIntoClientArea(IntPtr hwnd, ref MARGINS pMarInset);

        [DllImport("DwmApi.dll")]
        public static extern void DwmIsCompositionEnabled(ref bool pfEnabled);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern IntPtr CreateCompatibleDC(IntPtr hDC);

        [DllImport("gdi32.dll", ExactSpelling = true)]
        public static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern bool DeleteObject(IntPtr hObject);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern bool DeleteDC(IntPtr hdc);

        [DllImport("gdi32.dll")]
        public static extern bool BitBlt(IntPtr hdc, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, uint dwRop);

        [DllImport("UxTheme.dll", CharSet = CharSet.Unicode)]
        public static extern int DrawThemeTextEx(IntPtr hTheme, IntPtr hdc, int iPartId, int iStateId, string text, int iCharCount, int dwFlags, ref RECT pRect, ref DTTOPTS pOptions);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateDIBSection(IntPtr hdc, BITMAPINFO pbmi, uint iUsage, int ppvBits, IntPtr hSection, uint dwOffset);

        [DllImport("user32.dll")]
        public static extern uint GetWindowLong(IntPtr hwnd, int index);

        [DllImport("user32.dll")]
        public static extern uint SetWindowLong(IntPtr hwnd, int index, uint value);

        #endregion

        #region Structures

        [StructLayout(LayoutKind.Sequential)]
        public struct MARGINS
        {
            public int cxLeftWidth;      // width of left border that retains its size
            public int cxRightWidth;     // width of right border that retains its size
            public int cyTopHeight;      // height of top border that retains its size
            public int cyBottomHeight;   // height of bottom border that retains its size
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct DTTOPTS
        {
            public int dwSize;
            public int dwFlags;
            public int crText;
            public int crBorder;
            public int crShadow;
            public int iTextShadowType;
            public POINT ptShadowOffset;
            public int iBorderSize;
            public int iFontPropId;
            public int iColorPropId;
            public int iStateId;
            public bool fApplyOverlay;
            public int iGlowSize;
            public int pfnDrawTextCallback;
            public IntPtr lParam;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public POINT(int x, int y)
            {
                this.x = x;
                this.y = y;
            }

            public int x;
            public int y;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class BITMAPINFO
        {
            public int biSize;
            public int biWidth;
            public int biHeight;
            public short biPlanes;
            public short biBitCount;
            public int biCompression;
            public int biSizeImage;
            public int biXPelsPerMeter;
            public int biYPelsPerMeter;
            public int biClrUsed;
            public int biClrImportant;
            public byte bmiColors_rgbBlue;
            public byte bmiColors_rgbGreen;
            public byte bmiColors_rgbRed;
            public byte bmiColors_rgbReserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;

            public RECT(int left, int top, int right, int bottom)
            {
                Left = left;
                Top = top;
                Right = right;
                Bottom = bottom;
            }

            public RECT(Rectangle rectangle)
            {
                Left = rectangle.X;
                Top = rectangle.Y;
                Right = rectangle.Right;
                Bottom = rectangle.Bottom;
            }

            public Rectangle ToRectangle()
            {
                return new Rectangle(Left, Top, Right - Left, Bottom - Top);
            }

            public override string ToString()
            {
                return "Left: " + Left + ", " + "Top: " + Top + ", Right: " + Right + ", Bottom: " + Bottom;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct NCCALCSIZE_PARAMS
        {
            public RECT rgrc0, rgrc1, rgrc2;
            public IntPtr lppos;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct WINDOWPOS
        {
            public IntPtr hwnd;
            public IntPtr hwndInsertAfter;
            public int x, y;
            public int cx, cy;
            public int flags;
        }

        public enum TextStyle
        {
            Normal,
            Glowing
        }

        public static bool IsCompositionEnabled
        {
            get
            {
                if (Environment.OSVersion.Version.Major < 6)
                    return false;

                bool compositionEnabled = false;
                DwmIsCompositionEnabled(ref compositionEnabled);
                return compositionEnabled;
            }
        }

        public static void ExtendGlassIntoClientArea(Form form, int leftMargin, int topMargin, int rightMargin, int bottomMargin)
        {
            MARGINS m = new MARGINS();
            m.cxLeftWidth = leftMargin;
            m.cxRightWidth = rightMargin;
            m.cyTopHeight = topMargin;
            m.cyBottomHeight = bottomMargin;

            DwmExtendFrameIntoClientArea(form.Handle, ref m);
        }

        #endregion

        #region Helper Functions

        public const int DrawAeroTextPadding = 8;
        private const int DTT_COMPOSITED = 8192;
        private const int DTT_GLOWSIZE = 2048;
        private const int DTT_TEXTCOLOR = 1;

        public static void DrawAeroText(Graphics graphics, string text, Font font, Rectangle bounds, Color color)
        {
            DrawAeroText(graphics, text, font, bounds, color, TextFormatFlags.Default, TextStyle.Glowing);
        }

        public static void DrawAeroText(Graphics graphics, string text, Font font, Rectangle bounds, Color color, TextFormatFlags flags, TextStyle textStyle)
        {
            Rectangle bitmapBounds = bounds;
            bitmapBounds.Inflate(DrawAeroTextPadding, DrawAeroTextPadding);
            Rectangle textBounds = bounds;
            DrawAeroText(graphics, text, font, bitmapBounds, textBounds, color, flags, textStyle, null);
        }

        public static void DrawAeroText(Graphics graphics, string text, Font font, Rectangle bitmapBounds, Rectangle textBounds, Color color, TextFormatFlags flags, TextStyle textStyle, Action<Graphics> backgroundPainter)
        {
            IntPtr primaryHdc = graphics.GetHdc();

            // Create a memory DC so we can work offscreen 
            IntPtr memoryHdc = CreateCompatibleDC(primaryHdc);

            // Create a device-independent bitmap and select it into our DC 
            BITMAPINFO info = new BITMAPINFO();
            info.biSize = Marshal.SizeOf(info);
            info.biWidth = bitmapBounds.Width;
            info.biHeight = -bitmapBounds.Height;
            info.biPlanes = 1;
            info.biBitCount = 32;
            info.biCompression = 0; // BI_RGB 
            IntPtr dib = CreateDIBSection(primaryHdc, info, 0, 0, IntPtr.Zero, 0);
            SelectObject(memoryHdc, dib);

            // Create and select font 
            IntPtr fontHandle = font.ToHfont();
            SelectObject(memoryHdc, fontHandle);

            if (backgroundPainter != null)
            {
                using (Graphics g = Graphics.FromHdc(memoryHdc))
                {
                    backgroundPainter(g);
                }
            }

            // Draw glowing text 
            System.Windows.Forms.VisualStyles.VisualStyleRenderer renderer = new System.Windows.Forms.VisualStyles.VisualStyleRenderer(System.Windows.Forms.VisualStyles.VisualStyleElement.Window.Caption.Active);
            DTTOPTS dttOpts = new DTTOPTS();
            dttOpts.dwSize = Marshal.SizeOf(typeof(DTTOPTS));

            if (textStyle == TextStyle.Glowing)
            {
                dttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE | DTT_TEXTCOLOR;
            }
            else
            {
                dttOpts.dwFlags = DTT_COMPOSITED | DTT_TEXTCOLOR;
            }
            dttOpts.crText = ColorTranslator.ToWin32(color);
            dttOpts.iGlowSize = 8; // This is about the size Microsoft Word 2007 uses 
            textBounds.Offset(-bitmapBounds.Left, -bitmapBounds.Top);
            RECT textRect = new RECT(textBounds);
            DrawThemeTextEx(renderer.Handle, memoryHdc, 0, 0, text, -1, (int)flags, ref textRect, ref dttOpts);
            // Copy to foreground 
            const int SRCCOPY = 0x00CC0020;
            BitBlt(primaryHdc, bitmapBounds.Left, bitmapBounds.Top, bitmapBounds.Width, bitmapBounds.Height, memoryHdc, 0, 0, SRCCOPY);

            // Clean up 
            DeleteObject(fontHandle);
            DeleteObject(dib);
            DeleteDC(memoryHdc);

            graphics.ReleaseHdc(primaryHdc);
        }

        #endregion
    }
}

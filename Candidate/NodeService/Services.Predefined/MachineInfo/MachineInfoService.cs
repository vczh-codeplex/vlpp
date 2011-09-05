using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace MachineInfo
{
    [NodeEndpoint(Program.ServiceName)]
    public class MachineInfoService : StrongTypedNodeEndpoint
    {
        public MachineInfoService()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public int GetScreenCount()
        {
            return Screen.AllScreens.Length;
        }

        [NodeEndpointMethod]
        public ScreenInfo GetScreenInfo(int index)
        {
            Screen screen = Screen.AllScreens[index];
            return new ScreenInfo
            {
                DeviceName = screen.DeviceName,
                IsPrimary = screen.Primary,
                Bounds = new ScreenBounds
                {
                    Left = screen.Bounds.Left,
                    Top = screen.Bounds.Top,
                    Width = screen.Bounds.Width,
                    Height = screen.Bounds.Height,
                },
                WorkingArea = new ScreenBounds
                {
                    Left = screen.WorkingArea.Left,
                    Top = screen.WorkingArea.Top,
                    Width = screen.WorkingArea.Width,
                    Height = screen.WorkingArea.Height,
                },
            };
        }

        [NodeEndpointMethod]
        public Stream GetScreenImage(int index)
        {
            Screen screen = Screen.AllScreens[index];
            MemoryStream stream = new MemoryStream();
            using (Bitmap bitmap = new Bitmap(screen.Bounds.Width, screen.Bounds.Height))
            {
                using (Graphics g = Graphics.FromImage(bitmap))
                {
                    g.CopyFromScreen(screen.Bounds.Location, new Point(0, 0), screen.Bounds.Size);
                    bitmap.Save(stream, ImageFormat.Jpeg);
                }
            }
            return stream;
        }

        [NodeEndpointMethod]
        public string[] GetDrivers()
        {
            return Environment.GetLogicalDrives();
        }

        [NodeEndpointMethod]
        public bool IsDirectoryExists(string directory)
        {
            return Directory.Exists(directory);
        }

        [NodeEndpointMethod]
        public bool IsFileExists(string file)
        {
            return File.Exists(file);
        }

        [NodeEndpointMethod]
        public string[] GetDirectories(string directory)
        {
            if (Directory.Exists(directory))
            {
                return Directory.GetDirectories(directory);
            }
            else
            {
                return new string[] { };
            }
        }

        [NodeEndpointMethod]
        public string[] GetFiles(string directory)
        {
            if (Directory.Exists(directory))
            {
                return Directory.GetFiles(directory);
            }
            else
            {
                return new string[] { };
            }
        }

        [NodeEndpointMethod]
        public void MouseMove(int x, int y)
        {
            MouseController.SetCursorPos(x, y);
        }

        [NodeEndpointMethod]
        public void MouseEvent(string operation, int data)
        {
            int flags = MouseController.MOUSEEVENTF_ABSOLUTE;
            switch (operation)
            {
                case "leftdown": flags |= MouseController.MOUSEEVENTF_LEFTDOWN; break;
                case "leftup": flags |= MouseController.MOUSEEVENTF_LEFTUP; break;
                case "rightdown": flags |= MouseController.MOUSEEVENTF_RIGHTDOWN; break;
                case "rightup": flags |= MouseController.MOUSEEVENTF_RIGHTUP; break;
                case "middledown": flags |= MouseController.MOUSEEVENTF_MIDDLEDOWN; break;
                case "middleup": flags |= MouseController.MOUSEEVENTF_MIDDLEUP; break;
                case "wheel": flags |= MouseController.MOUSEEVENTF_WHEEL; break;
            }
            if (flags != MouseController.MOUSEEVENTF_ABSOLUTE)
            {
                MouseController.mouse_event(flags, 0, 0, data, IntPtr.Zero);
            }
        }

        [NodeEndpointMethod]
        public void KeyEvent(byte key, bool down)
        {
            KeyController.keybd_event(key, 0, (down ? 0 : 2), IntPtr.Zero);
        }

        private static class MouseController
        {
            public const int MOUSEEVENTF_ABSOLUTE = 0x8000;
            public const int MOUSEEVENTF_LEFTDOWN = 0x0002;
            public const int MOUSEEVENTF_LEFTUP = 0x0004;
            public const int MOUSEEVENTF_MIDDLEDOWN = 0x0020;
            public const int MOUSEEVENTF_MIDDLEUP = 0x0040;
            public const int MOUSEEVENTF_MOVE = 0x0001;
            public const int MOUSEEVENTF_RIGHTDOWN = 0x0008;
            public const int MOUSEEVENTF_RIGHTUP = 0x0010;
            public const int MOUSEEVENTF_WHEEL = 0x0800;

            [DllImport("user32.dll")]
            public extern static bool SetCursorPos(int x, int y);

            [DllImport("user32.dll")]
            public extern static int mouse_event(int dwFlags, int dx, int dy, int dwData, IntPtr dwExtraInfo);
        }

        private static class KeyController
        {
            [DllImport("user32.dll")]
            public static extern void keybd_event(byte bVk, byte bScan, int dwFlags, IntPtr dwExtraInfo);
        }
    }
}

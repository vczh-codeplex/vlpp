using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;

namespace MachineInfo
{
    [NodeEndpoint(Program.ServiceName)]
    public class MachineInfoService : StrongTypedNodeEndpoint
    {
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
                    bitmap.Save(stream, ImageFormat.Png);
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
    }
}

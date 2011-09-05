using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost.GuardService;
using NodeService.Protocols;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace MachineInfo
{
    class Program
    {
        public const string ServiceName = "MachineInfo";

        static void Main(string[] args)
        {
            Console.Title = ServiceName;
            GuardServiceStarter<MachineInfoService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                ServiceName,
                new HttpProtocolFactory(),
                "http://+:9010/",
                Program.ServiceName
                );
        }
    }

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
    }

    [NodeEndpointDataType]
    public class ScreenBounds
    {
        [NodeEndpointDataMember]
        public int Left { get; set; }

        [NodeEndpointDataMember]
        public int Top { get; set; }

        [NodeEndpointDataMember]
        public int Width { get; set; }

        [NodeEndpointDataMember]
        public int Height { get; set; }
    }

    [NodeEndpointDataType]
    public class ScreenInfo
    {
        [NodeEndpointDataMember]
        public string DeviceName { get; set; }

        [NodeEndpointDataMember]
        public bool IsPrimary { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds Bounds { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds WorkingArea { get; set; }
    }
}

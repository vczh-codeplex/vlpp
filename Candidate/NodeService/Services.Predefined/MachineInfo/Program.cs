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
}

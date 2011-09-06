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
using ServiceConfigurations;

namespace MachineInfo
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = MachineInfoServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<MachineInfoService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                MachineInfoServiceConfiguration.CreateFactory(),
                MachineInfoServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }
}

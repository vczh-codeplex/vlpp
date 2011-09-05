using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost;
using NodeService;
using NodeServiceHost.GuardService;
using NodeService.Protocols;
using System.IO;

namespace PingNamedPipe
{
    class Program
    {
        public const string ServiceName = "PingServiceNamedPipe";

        static void Main(string[] args)
        {
            Console.Title = ServiceName;
            GuardServiceStarter<PingService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                ServiceName,
                new NamedPipeProtocolFactory(),
                "PredefinedServices",
                Program.ServiceName
                );
        }
    }

    [NodeEndpoint(Program.ServiceName)]
    public class PingService : StrongTypedNodeEndpoint
    {
        public PingService()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public string Hello(string name)
        {
            return string.Format("Hi, {0}. I am {1}.", name, Program.ServiceName);
        }

        [NodeEndpointMethod]
        public Stream HelloStream(string name)
        {
            return new MemoryStream(Hello(name).NodeServiceEncode());
        }
    }
}

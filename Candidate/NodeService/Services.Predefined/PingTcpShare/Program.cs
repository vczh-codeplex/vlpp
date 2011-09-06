using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeService;
using NodeService.Endpoints;
using NodeServiceHost.GuardService;
using System.IO;

namespace PingTcpShare
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = PingServiceTcpShareConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<PingService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                PingServiceTcpShareConfiguration.CreateFactory(),
                PingServiceTcpShareConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(PingServiceTcpConfiguration.EndpointName)]
    public class PingService : StrongTypedNodeEndpoint, IPingService
    {
        public PingService()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public string Hello(string name)
        {
            return string.Format("Hi, {0}. I am {1}.", name, this.EndpointName);
        }

        [NodeEndpointMethod]
        public Stream HelloStream(string name)
        {
            return new MemoryStream(Hello(name).NodeServiceEncode());
        }
    }
}

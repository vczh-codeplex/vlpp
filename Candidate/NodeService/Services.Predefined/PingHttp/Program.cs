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
using ServiceConfigurations;

namespace PingHttp
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = PingServiceHttpConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<PingService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                PingServiceHttpConfiguration.CreateFactory(),
                PingServiceHttpConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(PingServiceHttpConfiguration.EndpointName)]
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

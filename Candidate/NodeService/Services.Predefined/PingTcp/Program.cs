using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost;
using NodeService;
using NodeServiceHost.GuardService;
using NodeService.Protocols;

namespace PingTcp
{
    class Program
    {
        public const string ServiceName = "PingServiceTcp";

        static void Main(string[] args)
        {
            Console.Title = ServiceName;
            GuardServiceStarter<PingService, PingServerCallback>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                ServiceName + " (127.0.0.1:9001)"
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
    }

    public class PingServerCallback : INodeEndpointServerCallback<PingService>
    {
        private INodeEndpointProtocolFactory protocolFactory;

        public PingServerCallback()
        {
            this.protocolFactory = new TcpProtocolFactory();
        }

        public INodeEndpointProtocolFactory ProtocolFactory
        {
            get
            {
                return this.protocolFactory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return "9001";
            }
        }

        public string EndpointName
        {
            get
            {
                return Program.ServiceName;
            }
        }

        public PingService CreateEndpoint()
        {
            return new PingService();
        }

        public void OnEndpointStart(PingService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }

        public void OnEndpointStopped(PingService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }
    }
}

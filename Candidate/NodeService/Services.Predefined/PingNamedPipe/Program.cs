using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost;
using NodeService;
using NodeServiceHost.GuardService;
using NodeService.Protocols;

namespace PingNamedPipe
{
    class Program
    {
        public const string ServiceName = "PingServiceNamedPipe";

        static void Main(string[] args)
        {
            Console.Title = ServiceName;
            GuardServiceStarter<PingService, PingServerCallback>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                ServiceName + " (pipe://./PredefinedServices/" + ServiceName + "/)"
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
            this.protocolFactory = new NamedPipeProtocolFactory();
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
                return "PredefinedServices";
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

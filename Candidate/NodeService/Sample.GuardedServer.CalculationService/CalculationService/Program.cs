using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost;
using NodeService;
using NodeService.Protocols;
using NodeServiceHost.GuardService;
using System.Threading;
using System.Diagnostics;

namespace CalculationService
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = "CalculationService";
            GuardServiceStarter<CalculationService, CalculationServiceEndpointServerCallback>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                "pipe://./GuardedServiceTest/CalculationService"
                );
        }
    }

    [NodeEndpoint("CalculationService")]
    public class CalculationService : StrongTypedNodeEndpoint
    {
        [NodeEndpointMethod]
        public int Add(int a, int b)
        {
            return a + b;
        }
    }

    public class CalculationServiceEndpointServerCallback : INodeEndpointServerCallback<CalculationService>
    {
        private INodeEndpointProtocolFactory protocolFactory = new NamedPipeProtocolFactory();

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
                return "GuardedServiceTest";
            }
        }

        public string EndpointName
        {
            get
            {
                return null;
            }
        }

        public CalculationService CreateEndpoint()
        {
            return new CalculationService();
        }

        public void OnEndpointStart(CalculationService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }

        public void OnEndpointStopped(CalculationService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }
    }
}

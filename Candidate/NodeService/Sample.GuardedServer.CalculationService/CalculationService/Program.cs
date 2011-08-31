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
        static ManualResetEvent exitEvent = null;

        static void StartEventHandler(INodeEndpointServer<CalculationService> server)
        {
        }

        static void StopEventHandler()
        {
            exitEvent.Set();
        }

        static void Main(string[] args)
        {
            exitEvent = new ManualResetEvent(false);

            var guardService = GuardServiceStarter<CalculationService, CalculationServiceEndpointServerCallback>.ConnectGuardServiceFacade(StartEventHandler, StopEventHandler);
            Guid token = guardService.Register(new GuardedServiceDescription()
            {
                ExecutablePath = typeof(Program).Assembly.CodeBase,
                Arguments = "",
                Name = "pipe://./GuardedServiceTest/CalculationService",
            });
            Console.WriteLine("Server started. To close this server, use NodeServiceGuard.exe.");
            exitEvent.WaitOne();
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using ServiceConfigurations;
using NodeServiceHost.GuardService;

namespace TcpShareProvider
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = TcpShareProviderServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<TcpShareProviderService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                TcpShareProviderServiceConfiguration.CreateFactory(),
                TcpShareProviderServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(TcpShareProviderServiceConfiguration.EndpointName)]
    public class TcpShareProviderService : StrongTypedNodeEndpoint, ITcpShareProviderService
    {
        [NodeEndpointMethod]
        public void StartListeningPort(int port)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public void StopListeningPort(int port)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public int[] GetListeningPorts()
        {
            throw new NotImplementedException();
        }
    }
}

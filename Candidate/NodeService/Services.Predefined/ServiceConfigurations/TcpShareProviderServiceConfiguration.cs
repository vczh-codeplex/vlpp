using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace ServiceConfigurations
{
    public static class TcpShareProviderServiceConfiguration
    {
        public const string EndpointName = "TcpShareService";
        public const string ServerAddress = "9004";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new TcpProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return ClientAddress.BuildTcpAddress(ServerAddress, machineAddress);
        }
    }

    public interface ITcpShareProviderService
    {
        void StartListeningPort(int port);
        void StopListeningPort(int port);
        void StopListeningAllPort();
        int[] GetListeningPorts();
    }

    public interface ITcpShareProviderServiceClient : ITcpShareProviderService, INodeEndpointClient
    {
    }
}

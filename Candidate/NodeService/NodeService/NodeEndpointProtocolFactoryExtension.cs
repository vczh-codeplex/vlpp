using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Providers;
using NodeService.Endpoints;

namespace NodeService
{
    public static class NodeEndpointProtocolFactoryExtension
    {
        public static INodeEndpointProtocolServer WaitForServer(this INodeEndpointProtocolServerListener serverListener, string address, INodeEndpoint endpoint)
        {
            serverListener.Connect(address, endpoint.EndpointName);
            INodeEndpointProtocolServer server = serverListener.Listen();

            INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(endpoint);
            server.ProtocolListener.Listener = endpointListener;
            server.ProtocolListener.BeginListen();
            return server;
        }

        public static T WaitForClient<T>(this INodeEndpointProtocolFactory protocolFactory, string address, string endpointName, int timeout = 5000)
            where T : INodeEndpointClient
        {
            INodeEndpointProtocolClient client = protocolFactory.CreateClient();
            if (client.Connect("localhost/CalculationService", "CalculationEndpoint", timeout))
            {
                INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                provider.Protocol = client;
                T endpointInterface = StrongTypedNodeEndpointClient.Create<T>(provider);
                return endpointInterface;
            }
            else
            {
                return default(T);
            }
        }
    }
}
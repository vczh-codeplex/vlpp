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
        private static INodeEndpointProtocolServer WaitForServerBase(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            INodeEndpoint endpoint
            )
        {
            serverListener.Connect(address, endpoint.EndpointName);
            INodeEndpointProtocolServer server = serverListener.Listen();

            INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(endpoint);
            server.ProtocolListener.Listener = endpointListener;
            return server;
        }

        public static INodeEndpointProtocolServer WaitForServer(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            INodeEndpoint endpoint
            )
        {
            INodeEndpointProtocolServer server = WaitForServerBase(serverListener, address, endpoint);
            server.ProtocolListener.BeginListen();
            return server;
        }

        public static INodeEndpointProtocolServer WaitForServer<T>(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            IDuplexNodeEndpoint<T> endpoint)
            where T : INodeEndpointClient
        {
            INodeEndpointProtocolServer server = WaitForServerBase(serverListener, address, endpoint);
            if (server != null)
            {
                INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                provider.Protocol = server;
                T endpointInterface = StrongTypedNodeEndpointClientBuilder.Create<T>(provider);
                endpoint.Callback = endpointInterface;
            }
            server.ProtocolListener.BeginListen();
            return server;
        }

        public static T WaitForClient<T>(
            this INodeEndpointProtocolFactory protocolFactory,
            string address,
            string endpointName,
            int timeout = 5000
            )
            where T : INodeEndpointClient
        {
            INodeEndpointProtocolClient client = protocolFactory.CreateClient();
            if (client.Connect(address, endpointName, timeout))
            {
                INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                provider.Protocol = client;
                T endpointInterface = StrongTypedNodeEndpointClientBuilder.Create<T>(provider);
                return endpointInterface;
            }
            else
            {
                return default(T);
            }
        }

        public static T WaitForClient<T, U>(
            this INodeEndpointProtocolFactory protocolFactory,
            string address,
            string endpointName,
            U callback,
            int timeout = 5000
            )
            where T : IDuplexNodeEndpointClient<U>
            where U : INodeEndpoint
        {
            T client = WaitForClient<T>(protocolFactory, address, endpointName, timeout);
            if (!object.ReferenceEquals(client, default(T)))
            {
                INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(callback);
                client.Provider.Protocol.ProtocolListener.Listener = endpointListener;
                client.Callback = callback;
            }
            return client;
        }
    }
}
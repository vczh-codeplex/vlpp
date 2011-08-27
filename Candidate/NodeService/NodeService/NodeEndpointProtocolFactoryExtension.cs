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

        public static INodeEndpointProtocolServer WaitForServerAndCallback<T, U>(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            T endpoint,
            Action<T, U> setupCallback)
            where T : INodeEndpoint
            where U : INodeEndpointClient
        {
            INodeEndpointProtocolServer server = WaitForServerBase(serverListener, address, endpoint);
            if (server != null)
            {
                INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                provider.Protocol = server;
                U endpointInterface = StrongTypedNodeEndpointClient.Create<U>(provider);
                setupCallback(endpoint, endpointInterface);
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
                T endpointInterface = StrongTypedNodeEndpointClient.Create<T>(provider);
                return endpointInterface;
            }
            else
            {
                return default(T);
            }
        }

        public static T WaitForClientAndCallback<T, U>(
            this INodeEndpointProtocolFactory protocolFactory,
            string address,
            string endpointName,
            U callback,
            Action<T, U> setupCallback = null,
            int timeout = 5000
            )
            where T : INodeEndpointClient
            where U : INodeEndpoint
        {
            T client = WaitForClient<T>(protocolFactory, address, endpointName, timeout);
            if (!object.ReferenceEquals(client, default(T)))
            {
                INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(callback);
                client.Provider.Protocol.ProtocolListener.Listener = endpointListener;
                if (setupCallback != null)
                {
                    setupCallback(client, callback);
                }
            }
            return client;
        }
    }
}
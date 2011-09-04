using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Providers;
using NodeService.Endpoints;
using System.IO;
using NodeService.Protocols;

namespace NodeService
{
    public static class NodeEndpointProtocolFactoryExtension
    {
        public const int DefaultTimeout = 1000;

        #region Server Utilities

        private static INodeEndpointProtocolServer WaitForServerBase(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            INodeEndpoint endpoint,
            int timeout
            )
        {
            if (!serverListener.Connected)
            {
                serverListener.Connect(address, endpoint.EndpointName);
            }
            INodeEndpointProtocolServer server = serverListener.Listen(timeout);
            if (server != null)
            {
                INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(endpoint);
                server.AddListener(endpointListener);
            }
            return server;
        }

        public static INodeEndpointProtocolServer WaitForServer(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            INodeEndpoint endpoint,
            int timeout = DefaultTimeout
            )
        {
            INodeEndpointProtocolServer server = WaitForServerBase(serverListener, address, endpoint, timeout);
            if (server != null)
            {
                server.BeginListen();
            }
            return server;
        }

        public static INodeEndpointProtocolServer WaitForServer<T>(
            this INodeEndpointProtocolServerListener serverListener,
            string address,
            IDuplexNodeEndpoint<T> endpoint,
            int timeout = DefaultTimeout
            )
            where T : INodeEndpointClient
        {
            INodeEndpointProtocolServer server = WaitForServerBase(serverListener, address, endpoint, timeout);
            if (server != null)
            {
                if (server.EnableDuplex)
                {
                    INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                    provider.Protocol = server;
                    T endpointInterface = StrongTypedNodeEndpointClientBuilder.Create<T>(provider);
                    endpoint.Callback = endpointInterface;
                }
                else
                {
                    server.Disconnect();
                    throw new InvalidOperationException("The protocol does not support duplex communication.");
                }
                server.BeginListen();
            }
            return server;
        }

        #endregion

        #region Client Utilities

        public static T WaitForClient<T>(
            this INodeEndpointProtocolFactory protocolFactory,
            string address,
            string endpointName,
            int timeout = DefaultTimeout
            )
            where T : INodeEndpointClient
        {
            INodeEndpointProtocolClient client = protocolFactory.CreateClient();
            if (client.Connect(address, endpointName, timeout))
            {
                INodeEndpointClientProvider provider = new ProtocolEnabledClientProvider();
                provider.Protocol = client;
                T endpointInterface = StrongTypedNodeEndpointClientBuilder.Create<T>(provider);
                client.BeginListen();
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
                if (client.Provider.Protocol.EnableDuplex)
                {
                    INodeEndpointProtocolRequestListener endpointListener = new ProtocolEnabledRequestListener(callback);
                    client.Provider.Protocol.AddListener(endpointListener);
                    client.Callback = callback;
                }
                else
                {
                    client.Dispose();
                    throw new InvalidOperationException("The protocol does not support duplex communication.");
                }
            }
            return client;
        }

        #endregion

        #region String Encoding

        public static void Send(this INodeEndpointProtocol protocol, string message)
        {
            protocol.Send(message.NodeServiceEncode());
        }

        public static string RequestMessage(this INodeEndpointProtocolRequest request)
        {
            return request.Message.NodeServiceDecode();
        }

        public static void Respond(this INodeEndpointProtocolRequest request, string message)
        {
            request.Respond(message.NodeServiceEncode());
        }

        public static byte[] NodeServiceEncode(this string message)
        {
            return Encoding.UTF8.GetBytes(message);
        }

        public static string NodeServiceDecode(this byte[] bytes)
        {
            return Encoding.UTF8.GetString(bytes);
        }

        public static byte[] ReadAllBytesAndClose(this Stream stream)
        {
            using (MemoryStream memoryStream = new MemoryStream())
            {
                byte[] buffer = new byte[65536];
                int read = -1;
                while ((read = stream.Read(buffer, 0, buffer.Length)) > 0)
                {
                    memoryStream.Write(buffer, 0, read);
                }

                memoryStream.Seek(0, SeekOrigin.Begin);
                buffer = new byte[memoryStream.Length];
                memoryStream.Read(buffer, 0, buffer.Length);
                return buffer;
            }
        }

        #endregion

        #region Factory Building

        public static INodeEndpointProtocolFactory With(this INodeEndpointProtocolFactory factory, ITranslatorProtocolHandlerFactory handlerFactory)
        {
            return new TranslatorProtocolFactory(factory, handlerFactory);
        }

        public static INodeEndpointProtocolFactory With(this INodeEndpointProtocolFactory factory, ITranslatorProtocolHandlerSimple handler)
        {
            return factory.With(new TranslatorProtocolHandlerFactorySimple(handler));
        }

        #endregion
    }
}
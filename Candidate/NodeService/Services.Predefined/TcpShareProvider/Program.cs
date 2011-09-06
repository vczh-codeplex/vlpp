using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using ServiceConfigurations;
using NodeServiceHost.GuardService;
using System.Threading;
using NodeService;
using NodeService.Protocols;

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

    class TcpShareProviderServiceRedirector : INodeEndpointProtocolRequestListener
    {
        private NamedPipeProtocolFactory protocolFactory = null;
        private INodeEndpointProtocolServer server = null;
        private INodeEndpointProtocolClient client = null;
        private int port;

        class ResponseListener : INodeEndpointProtocolRequestListener
        {
            private INodeEndpointProtocolServer server;

            public ResponseListener(INodeEndpointProtocolServer server)
            {
                this.server = server;
            }

            public void OnReceivedRequest(INodeEndpointProtocolRequest request)
            {
                this.server.Send(request.Message);
            }
        }

        public TcpShareProviderServiceRedirector(int port, NamedPipeProtocolFactory protocolFactory, INodeEndpointProtocolServer server)
        {
            this.protocolFactory = protocolFactory;
            this.server = server;
            this.port = port;
        }

        public void OnReceivedRequest(INodeEndpointProtocolRequest request)
        {
            if (!this.server.Connected)
            {
                if (this.client != null)
                {
                    this.client.Disconnect();
                }
            }
            else
            {
                if (this.client == null)
                {
                    string message = request.RequestMessage();
                    if (message.StartsWith("[CONNECTENDPOINT]"))
                    {
                        string endpointName = message.Substring(17);
                        INodeEndpointProtocolClient protocolClient = this.protocolFactory.CreateClient();
                        if (protocolClient.Connect(TcpShareProtocolFactory.GetInternalPipeAddress(port), endpointName, NodeEndpointProtocolFactoryExtension.DefaultTimeout))
                        {
                            this.client = protocolClient;
                            this.client.AddListener(new ResponseListener(this.server));
                            this.client.BeginListen();
                        }
                        else
                        {
                            request.Respond("[CANNOTFINDENDPOINT]");
                        }
                    }
                    else
                    {
                        request.Respond("[ENDPOINTNOTCONNECTED]");
                    }
                    this.server.Disconnect();
                }
                else if (this.client.Connected)
                {
                    this.client.Send(request.Message);
                }
                else
                {
                    this.server.Disconnect();
                }
            }
        }
    }

    [NodeEndpoint(TcpShareProviderServiceConfiguration.EndpointName)]
    public class TcpShareProviderService : StrongTypedNodeEndpoint, ITcpShareProviderService
    {
        private class ListeningThread
        {
            public Thread Thread { get; set; }
            public INodeEndpointProtocolServerListener ServerListener { get; set; }
        }

        private static Dictionary<int, ListeningThread> portListeningThreads = new Dictionary<int, ListeningThread>();
        private static TcpProtocolFactory inFactory = new TcpProtocolFactory();
        private static NamedPipeProtocolFactory outFactory = new NamedPipeProtocolFactory();

        private static void ClearServices(List<INodeEndpointProtocolServer> servers, bool disconnectedOnly)
        {
            lock (servers)
            {
                for (int i = servers.Count - 1; i >= 0; i--)
                {
                    INodeEndpointProtocolServer server = servers[i];

                    if (!disconnectedOnly || !server.Connected)
                    {
                        server.Disconnect();
                        servers.RemoveAt(i);
                    }
                }
            }
        }

        private static void PortListeningThreadProc(int port, INodeEndpointProtocolServerListener serverListener)
        {
            List<INodeEndpointProtocolServer> servers = new List<INodeEndpointProtocolServer>();
            try
            {
                while (true)
                {
                    if (!serverListener.Connected)
                    {
                        serverListener.Connect(port.ToString(), TcpShareProviderServiceConfiguration.EndpointName);
                    }
                    if (!serverListener.Connected)
                    {
                        lock (portListeningThreads)
                        {
                            serverListener.Disconnect();
                            portListeningThreads.Remove(port);
                            break;
                        }
                    }

                    INodeEndpointProtocolServer server = serverListener.Listen(NodeEndpointProtocolFactoryExtension.DefaultTimeout);
                    if (server != null)
                    {
                        TcpShareProviderServiceRedirector redirector = new TcpShareProviderServiceRedirector(port, outFactory, server);
                        server.AddListener(redirector);
                        server.BeginListen();
                        servers.Add(server);
                    }

                    ClearServices(servers, true);
                }
            }
            catch (ThreadAbortException)
            {
            }
            ClearServices(servers, false);
        }

        public TcpShareProviderService()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public void StartListeningPort(int port)
        {
            lock (portListeningThreads)
            {
                if (portListeningThreads.ContainsKey(port))
                {
                    throw new InvalidOperationException("Port " + port.ToString() + " has been listened.");
                }

                var serverListener = inFactory.CreateServerListener();
                serverListener.Connect(port.ToString(), TcpShareProviderServiceConfiguration.EndpointName);
                if (serverListener.Connected)
                {
                    Thread thread = new Thread(() => PortListeningThreadProc(port, serverListener));
                    ListeningThread listeningThread = new ListeningThread()
                    {
                        Thread = thread,
                        ServerListener = serverListener,
                    };
                    portListeningThreads.Add(port, listeningThread);
                    thread.Start();
                }

                throw new InvalidOperationException("Failed to listen to port " + port.ToString() + ".");
            }
        }

        [NodeEndpointMethod]
        public void StopListeningPort(int port)
        {
            lock (portListeningThreads)
            {
                ListeningThread listeningThread = null;
                if (!portListeningThreads.TryGetValue(port, out listeningThread))
                {
                    throw new InvalidOperationException("Port " + port.ToString() + " has not been listened.");
                }

                listeningThread.Thread.Abort();
                listeningThread.ServerListener.Disconnect();
                portListeningThreads.Remove(port);
            }
        }

        [NodeEndpointMethod]
        public int[] GetListeningPorts()
        {
            lock (portListeningThreads)
            {
                return portListeningThreads.Keys.ToArray();
            }
        }
    }
}

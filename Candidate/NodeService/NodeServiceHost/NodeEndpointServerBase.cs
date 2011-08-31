using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Threading;

namespace NodeServiceHost
{
    public abstract class NodeEndpointServerBase<T> : INodeEndpointServer<T>
        where T : INodeEndpoint
    {
        private NodeEndpointServerState serverState = NodeEndpointServerState.Ready;
        private INodeEndpointServerCallback<T> callback = null;

        public INodeEndpointProtocolFactory ProtocolFactory { get; private set; }
        public INodeEndpointProtocolServerListener ServerListener { get; private set; }

        private volatile bool needToStop = false;
        private Thread serverListenerThread = null;
        private List<Tuple<T, INodeEndpointProtocolServer>> runningServices = new List<Tuple<T, INodeEndpointProtocolServer>>();

        protected abstract INodeEndpointProtocolServer CreateProtocolServer(T endpoint, int timeout);

        private void ClearServices(bool disconnectedOnly)
        {
            lock (this.runningServices)
            {
                for (int i = this.runningServices.Count - 1; i >= 0; i--)
                {
                    var pair = this.runningServices[i];
                    T endpoint = pair.Item1;
                    INodeEndpointProtocolServer server = pair.Item2;

                    if (!disconnectedOnly || !server.Connected)
                    {
                        this.callback.OnEndpointStopped(endpoint, server);
                        server.Disconnect();
                        endpoint.Dispose();
                        this.runningServices.RemoveAt(i);
                    }
                }
            }
        }

        private void ServerListenerThreadProc()
        {
            while (!this.needToStop)
            {
                T endpoint = this.callback.CreateEndpoint();
                INodeEndpointProtocolServer server = CreateProtocolServer(endpoint, NodeEndpointProtocolFactoryExtension.DefaultTimeout);
                if (server == null)
                {
                    endpoint.Dispose();
                }
                else
                {
                    lock (this.runningServices)
                    {
                        this.runningServices.Add(Tuple.Create(endpoint, server));
                        this.callback.OnEndpointStart(endpoint, server);
                    }
                }
                ClearServices(true);
            }
        }

        public NodeEndpointServerState ServerState
        {
            get
            {
                return this.serverState;
            }
        }

        public INodeEndpointServerCallback<T> Callback
        {
            get
            {
                return this.callback;
            }
        }

        public virtual void Start(INodeEndpointServerCallback<T> callback)
        {
            if (this.serverState != NodeEndpointServerState.Ready)
            {
                throw new InvalidOperationException("The server cannot be started.");
            }

            this.callback = callback;
            this.ProtocolFactory = this.callback.ProtocolFactory;
            this.ServerListener = this.ProtocolFactory.CreateServerListener();

            this.serverListenerThread = new Thread(ServerListenerThreadProc);
            this.serverListenerThread.Start();

            this.serverState = NodeEndpointServerState.Running;
        }

        public virtual void Stop()
        {
            if (this.serverState != NodeEndpointServerState.Running)
            {
                throw new InvalidOperationException("The server cannot be stopped.");
            }

            this.needToStop = true;
            if (!this.serverListenerThread.Join(NodeEndpointProtocolFactoryExtension.DefaultTimeout))
            {
                this.serverListenerThread.Abort();
            }
            ClearServices(false);
            this.ServerListener.Disconnect();
            this.ServerListener = null;
            this.serverState = NodeEndpointServerState.Stopped;
        }
    }
}

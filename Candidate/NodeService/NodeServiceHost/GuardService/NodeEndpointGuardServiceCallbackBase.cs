using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Endpoints;

namespace NodeServiceHost.GuardService
{
    public abstract class NodeEndpointGuardServiceCallbackBase<T> : GuardServiceCallback
        where T : INodeEndpoint
    {
        private NodeEndpointServerBase<T> server = null;
        private INodeEndpointServerCallback<T> callback = null;

        protected abstract NodeEndpointServerBase<T> CreateNodeEndpointServer();

        private void StartServer()
        {
            if (this.server == null)
            {
                this.server = CreateNodeEndpointServer();
                this.server.Start(this.callback);
            }
        }

        private void StopServer()
        {
            if (this.server != null)
            {
                this.server.Stop();
                this.server = null;
            }
        }

        public NodeEndpointGuardServiceCallbackBase(INodeEndpointServerCallback<T> callback)
        {
            this.callback = callback;
        }

        public override void Start(string semaphoreName)
        {
            base.Start(semaphoreName);
            StartServer();
        }

        public override void Stop()
        {
            StopServer();
            base.Stop();
        }

        public override void Dispose()
        {
            StopServer();
            base.Dispose();
        }
    }

    [NodeEndpoint]
    public class NodeEndpointGuardServiceCallback<T> : NodeEndpointGuardServiceCallbackBase<T>
        where T : INodeEndpoint
    {
        protected override NodeEndpointServerBase<T> CreateNodeEndpointServer()
        {
            return new NodeEndpointServer<T>();
        }

        public NodeEndpointGuardServiceCallback(INodeEndpointServerCallback<T> callback)
            : base(callback)
        {
        }
    }

    [NodeEndpoint]
    public class DuplexNodeEndpointGuardServiceCallback<T, U> : NodeEndpointGuardServiceCallbackBase<T>
        where T : IDuplexNodeEndpoint<U>
        where U : INodeEndpointClient
    {
        protected override NodeEndpointServerBase<T> CreateNodeEndpointServer()
        {
            return new DuplexNodeEndpointServer<T, U>();
        }

        public DuplexNodeEndpointGuardServiceCallback(INodeEndpointServerCallback<T> callback)
            : base(callback)
        {
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Endpoints;
using System.Xml.Linq;

namespace NodeServiceHost.GuardService
{
    public abstract class NodeEndpointGuardServiceCallbackBase<T> : GuardServiceCallback
        where T : INodeEndpoint
    {
        private INodeEndpointServer<T> server = null;
        private INodeEndpointServerCallback<T> callback = null;

        protected abstract INodeEndpointServer<T> CreateNodeEndpointServer();

        public Action<INodeEndpointServer<T>> StartEventHandler { get; set; }
        public Action StopEventHandler { get; set; }

        private void StartServer()
        {
            if (this.server == null)
            {
                this.server = CreateNodeEndpointServer();
                this.server.Start(this.callback);
                if (this.StartEventHandler != null)
                {
                    this.StartEventHandler(this.server);
                }
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

        public override void Dispose()
        {
            StopServer();
            base.Dispose();
        }

        public override void Start(string semaphoreName)
        {
            base.Start(semaphoreName);
            StartServer();
        }

        public override XElement GetServiceDescription()
        {
            return this.server.GetServiceDescription();
        }

        public override void Stop()
        {
            StopServer();
            base.Stop();
            if (this.StopEventHandler != null)
            {
                this.StopEventHandler();
            }
        }

        public override void StartTracing()
        {
            INodeEndpointServerTracer tracer = this.server.Tracer;
            if (tracer == null)
            {
                throw new InvalidOperationException("This server does not support tracing.");
            }
            tracer.StartTracing();
        }

        public override void StopTracing()
        {
            INodeEndpointServerTracer tracer = this.server.Tracer;
            if (tracer == null)
            {
                throw new InvalidOperationException("This server does not support tracing.");
            }
            tracer.StopTracing();
        }

        public override bool IsTracing()
        {
            INodeEndpointServerTracer tracer = this.server.Tracer;
            if (tracer == null)
            {
                return false;
            }
            return tracer.IsTracing();
        }

        public override XElement GetTracingResult()
        {
            INodeEndpointServerTracer tracer = this.server.Tracer;
            if (tracer == null)
            {
                throw new InvalidOperationException("This server does not support tracing.");
            }
            return tracer.GetTracingResult();
        }
    }

    [NodeEndpoint]
    public class NodeEndpointGuardServiceCallback<T> : NodeEndpointGuardServiceCallbackBase<T>
        where T : INodeEndpoint
    {
        protected override INodeEndpointServer<T> CreateNodeEndpointServer()
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
        protected override INodeEndpointServer<T> CreateNodeEndpointServer()
        {
            return new DuplexNodeEndpointServer<T, U>();
        }

        public DuplexNodeEndpointGuardServiceCallback(INodeEndpointServerCallback<T> callback)
            : base(callback)
        {
        }
    }
}

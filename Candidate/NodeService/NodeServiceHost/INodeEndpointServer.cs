using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Xml.Linq;

namespace NodeServiceHost
{
    public interface INodeEndpointServer<T>
        where T : INodeEndpoint
    {
        NodeEndpointServerState ServerState { get; }
        INodeEndpointServerCallback<T> Callback { get; }
        INodeEndpointServerTracer Tracer { get; }

        void Start(INodeEndpointServerCallback<T> callback);
        void Stop();
        XElement GetServiceDescription();
    }

    public interface INodeEndpointServerTracer
    {
        void StartTracing();
        void StopTracing();
        bool IsTracing();
        XElement GetTracingResult();
    };

    public enum NodeEndpointServerState
    {
        Ready,
        Running,
        Stopped,
    }

    public interface INodeEndpointServerCallback<T>
        where T : INodeEndpoint
    {
        INodeEndpointProtocolFactory ProtocolFactory { get; }
        string ProtocolAddress { get; }
        string EndpointName { get; }

        T CreateEndpoint();
        void OnEndpointStart(T endpoint, INodeEndpointProtocolServer protocolServer);
        void OnEndpointStopped(T endpoint, INodeEndpointProtocolServer protocolServer);
    }
}

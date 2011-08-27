using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService
{
    public interface INodeEndpoint
    {
        string EndpointName { get; }

        void QueueRequest(INodeEndpointRequest request);
    }

    public interface IDuplexNodeEndpoint<T> : INodeEndpoint
        where T : INodeEndpointClient
    {
        T Callback { get; set; }
    }

    public interface INodeEndpointRequest
    {
        string EndpointName { get; }
        string Method { get; }
        string PeerAddress { get; }
        XElement Body { get; }
        bool WaitingForResponse { get; }

        void Respond(XNode response);
        void Respond(Exception exception);
        void Respond();
    }

    public class NodeEndpointRequestException : Exception
    {
        public NodeEndpointRequestException(string message)
            : base(message)
        {
        }
    }

    public class NodeEndpointMessageException : Exception
    {
        public NodeEndpointMessageException(string message, Exception innerException = null)
            : base(message, innerException)
        {
        }
    }
}

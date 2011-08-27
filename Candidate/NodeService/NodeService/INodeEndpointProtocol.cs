using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeService
{
    public interface INodeEndpointProtocol
    {
        bool EnableDuplex { get; }
        bool Connected { get; }
        INodeEndpointProtocol OuterProtocol { get; }
        INodeEndpointProtocol InnerProtocol { get; }

        void Disconnect();
        void BeginListen();
        void AddListener(INodeEndpointProtocolRequestListener listener);
        void RemoveListener(INodeEndpointProtocolRequestListener listener);
        void Send(string message);
    }

    public interface INodeEndpointProtocolRequestListener
    {
        void OnReceivedRequest(INodeEndpointProtocolRequest request);
    }

    public interface INodeEndpointProtocolRequest
    {
        bool CanRespond { get; }
        string Message { get; }

        void Respond(string response);
    }

    public interface INodeEndpointProtocolFactory
    {
        INodeEndpointProtocolServerListener CreateServerListener();
        INodeEndpointProtocolClient CreateClient();
    }

    public interface INodeEndpointProtocolServerListener
    {
        bool Connected { get; }

        void Connect(string address, string endpointName);
        void Disconnect();
        INodeEndpointProtocolServer Listen();
    }

    public interface INodeEndpointProtocolServer : INodeEndpointProtocol
    {
        void SetOuterProtocol(INodeEndpointProtocolServer protocol);

        void OnOuterProtocolListened();
        void OnInnerProtocolSet(INodeEndpointProtocolServer protocol);
    }

    public interface INodeEndpointProtocolClient : INodeEndpointProtocol
    {
        bool Connect(string address, string endpointName, int timeout);
        void SetOuterProtocol(INodeEndpointProtocolClient protocol);

        void OnOuterProtocolConnected();
        void OnInnerProtocolSet(INodeEndpointProtocolClient protocol);
    }
}
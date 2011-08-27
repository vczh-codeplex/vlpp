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
        INodeEndpointProtocolListener ProtocolListener { get; }
        INodeEndpointProtocolSender ProtocolSender { get; }

        void Disconnect();
    }

    public interface INodeEndpointProtocolListener : INodeEndpointProtocol
    {
        INodeEndpointProtocolRequestListener Listener { get; set; }
    }

    public interface INodeEndpointProtocolSender : INodeEndpointProtocol
    {
        INodeEndpointProtocolResponse Send(string method, string message);
    }

    public interface INodeEndpointProtocolRequestListener
    {
        void OnReceivedRequest(INodeEndpointProtocolRequest request);
    }

    public interface INodeEndpointProtocolRequest
    {
        string Session { get; }
        string PeerAddress { get; }
        string Method { get; }
        string Message { get; }

        void Respond(string response);
    }

    public interface INodeEndpointProtocolResponse : IDisposable
    {
        bool EnableAsynchronization { get; }
        bool ReceivedResponse { get; }
        string Response { get; }

        void WaitForResponse();
        void RegisterCallback(Action<INodeEndpointProtocolResponse> callback);
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
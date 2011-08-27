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

        void SetOuterProtocol(INodeEndpointProtocol protocol);
        void Disconnect();
    }

    public interface INodeEndpointProtocolListener : INodeEndpointProtocol
    {
        INodeEndpointProtocolRequestListener Listener { get; set; }
    }

    public interface INodeEndpointProtocolSender : INodeEndpointProtocol
    {
        INodeEndpointProtocolResponse Send(string message);
    }

    public interface INodeEndpointProtocolRequestListener
    {
        void OnReceivedRequest(INodeEndpointRequest request);
    }

    public interface INodeEndpointProtocolRequest
    {
        string Session { get; }
        string Message { get; }

        void Respond(string response);
    }

    public interface INodeEndpointProtocolResponse
    {
        bool ReceivedResponse { get; }
        string Response { get; }

        void WaitForResponse();
        void RegisterCallback(Action<INodeEndpointProtocolResponse> callback);
    }

    public interface INodeEndpointProtocolFactory
    {
        INodeEndpointProtocolServer CreateServer();
        INodeEndpointProtocolClient CreateClient();
    }

    public interface INodeEndpointProtocolServer : INodeEndpointProtocol
    {
        bool Listen(string address, string endpointName);

        void OnOuterProtocolListened();
    }

    public interface INodeEndpointProtocolClient : INodeEndpointProtocol
    {
        bool Connect(string address, string endpointName);

        void OnOuterProtocolConnected();
    }
}
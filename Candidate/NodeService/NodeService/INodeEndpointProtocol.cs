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

        void Disconnect();
        void BeginListen();
        void AddListener(INodeEndpointProtocolRequestListener listener);
        void RemoveListener(INodeEndpointProtocolRequestListener listener);
        void Send(byte[] message);
    }

    public interface INodeEndpointProtocolRequestListener
    {
        void OnReceivedRequest(INodeEndpointProtocolRequest request);
    }

    public interface INodeEndpointProtocolRequest
    {
        bool CanRespond { get; }
        byte[] Message { get; }

        void Respond(byte[] message);
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
        INodeEndpointProtocolServer Listen(int timeout);
    }

    public interface INodeEndpointProtocolServer : INodeEndpointProtocol
    {
        INodeEndpointProtocolServer OuterProtocol { get; }
        INodeEndpointProtocolServer InnerProtocol { get; }

        void SetOuterProtocol(INodeEndpointProtocolServer protocol);
        void OnInnerProtocolSet(INodeEndpointProtocolServer protocol);
    }

    public interface INodeEndpointProtocolClient : INodeEndpointProtocol
    {
        INodeEndpointProtocolClient OuterProtocol { get; }
        INodeEndpointProtocolClient InnerProtocol { get; }

        bool Connect(string address, string endpointName, int timeout);
        void SetOuterProtocol(INodeEndpointProtocolClient protocol);

        void OnOuterProtocolConnected();
        void OnInnerProtocolSet(INodeEndpointProtocolClient protocol);
    }
}
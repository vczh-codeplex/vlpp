using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.IO;

namespace NodeService
{
    public interface INodeEndpointClient : IDisposable
    {
        INodeEndpointClientProvider Provider { get; }
    }

    public interface IDuplexNodeEndpointClient<T> : INodeEndpointClient
        where T : INodeEndpoint
    {
        T Callback { get; set; }
    }

    public interface INodeEndpointClientProvider
    {
        INodeEndpointProtocol Protocol { get; set; }
        INodeEndpointResponse Send(string method, XElement body);
        void Disconnect();
    }

    public enum RequestState
    {
        WaitingForResponse,
        ReceivedResponse,
        ReceivedStream,
        RaisedException,
    }

    public interface INodeEndpointResponse : IDisposable
    {
        bool EnableAsynchronization { get; }
        RequestState RequestState { get; }
        XNode Response { get; }
        Stream Stream { get; }
        Exception Exception { get; }

        void WaitForResponse();
        void RegisterCallback(Action<INodeEndpointResponse> callback);
    }
}

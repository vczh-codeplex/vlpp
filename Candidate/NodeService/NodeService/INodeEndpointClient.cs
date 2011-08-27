using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

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
    }

    public enum RequestState
    {
        WaitingForResponse,
        ReceivedResponse,
        RaisedException,
    }

    public interface INodeEndpointResponse : IDisposable
    {
        bool EnableAsynchronization { get; }
        RequestState RequestState { get; }
        XNode Response { get; }
        Exception Exception { get; }

        void WaitForResponse();
        void RegisterCallback(Action<INodeEndpointResponse> callback);
    }
}

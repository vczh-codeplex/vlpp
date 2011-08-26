using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService
{
    public enum RequestState
    {
        WaitingForResponse,
        ReceivedResponse,
        RaisedException,
    }

    public interface INodeEndpointResponse
    {
        bool EnableAsynchronization { get; }
        RequestState RequestState { get; }
        XNode Response { get; }
        Exception Exception { get; }

        void WaitForResponse();
        void RegisterCallback(Action<INodeEndpointResponse> callback);
    }
}
